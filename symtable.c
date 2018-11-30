/**
 * @file symtable.c
 * @author Jakub Man
 * @date 24.11. 2018
 * @brief Data types and functions for symbol table.
 */


#include "symtable.h"

void symtable_init(tSymPtr *root)
{
  if(root != NULL) {
    *root = NULL;
  }
}

int symtable_insert_unknown(tSymPtr *root, char *name)
{
  tSymPtr new = NULL;
  symtable_search(*root, name, &new);

  if(new == NULL)
  {
    //fprintf(stderr, "Allocating space for a new node. \n");
    new = malloc(sizeof(struct symNode));
    //fprintf(stderr, "Allocated. \n");
    if(new != NULL)
    {
      int len = strlen(name) + 1;
      new->name = malloc(sizeof(char) * len);
      //fprintf(stderr, "Allocating %d chars for string %s\n", len, name );
      if(new->name != NULL)
      {
        strcpy(new->name, name);
      }
      else
      {
        free(new);
        return INTERNAL_ERR;
      }
      new->type = UNKNOWN;
      new->lptr = NULL;
      new->rptr = NULL;
      symtable_insert(root, new);
    }
    else
    {
      return INTERNAL_ERR;
    }
  }
  return SUCCESS;
}

int symtable_insert_variable(tSymPtr *root, char *name, data_type type, bool define)
{

  tSymPtr new = NULL;
  symtable_search(*root, name, &new);
  if(new == NULL) {
    // Create a new item in the tree
    new = malloc(sizeof(struct symNode));
    if(new != NULL) {
      new->name = malloc(sizeof(char) * (strlen(name) + 1));
      if(new->name != NULL) {
        strcpy(new->name, name);
      }
      else {
        free(new);
        return INTERNAL_ERR;
      }
      new->type = VARIABLE;
      new->data.varData.defined = define;
      new->lptr = NULL;
      new->rptr = NULL;
      symtable_insert(root, new);
    }
    else {
      return INTERNAL_ERR;
    }

  }
  else {
    // Check if symbol is a variable, otherwise its a semantic error
    if(new->type == FUNCTION ) {
      return VARIABLE_ERR;
    }
    if(new->type == UNKNOWN) {
      new->type = VARIABLE;
    }
  }
  // Update existing (or newly created) item
  new->data.varData.type = type;
  if(define) {
    new->data.varData.defined = true;
  }

  return SUCCESS;
}

int symtable_insert_function(tSymPtr *root, char *name, data_type returnType, int paramCount, tFuncParam *params, bool define)
{
  tSymPtr new = NULL;
  symtable_search(*root, name, &new);
  if(new == NULL) {
    // Create a new item in the tree
    new = malloc(sizeof(struct symNode));
    if(new != NULL) {
      new->name = malloc(sizeof(char) * (strlen(name) + 1));
      if(new->name != NULL) {
        strcpy(new->name, name);
      }
      else {
        free(new);
        return INTERNAL_ERR;
      }
      new->type = FUNCTION;
      new->data.funData.defined = define;
      new->lptr = NULL;
      new->rptr = NULL;
      symtable_insert(root, new);
    }
    else {
      return INTERNAL_ERR;
    }
  }
  else {
    // Check if symbol is a function, otherwise its a semantic error
    if(new->type == VARIABLE) {
      return VARIABLE_ERR;
    }
    if(new->type == UNKNOWN) {
      new->type = FUNCTION;
    }
  }
  // Update existing (or newly created) item
  if(define) {
    new->data.funData.defined = true;
  }
  new->data.funData.returnType = returnType;
  new->data.funData.paramCount = paramCount;
  new->data.funData.params = params;

  return SUCCESS;
}

void symtable_insert(tSymPtr *root, tSymPtr node)
{
  int cmpval = 0;
  if(root != NULL) {
    if(*root == NULL) {
      // Insert into root
      *root = node;
    }
    else {
      // cmpval should never be 0, when strcmp returns 0, strings are equal.
      // This situation is handled by symtable_insert_function() or symtable_insert_variable()
      if(node->name != NULL) {
        cmpval = strcmp((*root)->name, node->name);
        if(cmpval > 0) {
          symtable_insert(&(((*root)->lptr)), node);
          fprintf(stderr, "davame doleva %s\n", node->name);
        }
        else if(cmpval < 0) {
          symtable_insert(&(((*root)->rptr)), node);
          fprintf(stderr, "davame doprava %s\n", node->name);
        }
      }
      else {
        fprintf(stderr, "Node name was NULL!\n");
      }
    }
  }
}

tSymPtr symtable_search(tSymPtr root, char *name, tSymPtr *result)
{
  int cmpval = 0;
  if(root != NULL && root->name != NULL) {
    cmpval = strcmp(root->name, name);
    if(cmpval == 0) {
      *result = root;
      //return root;
    }
    else if(cmpval < 0) {
      symtable_search(root->lptr, name, result);
    }
    else {
      symtable_search(root->rptr, name, result);
    }
  }
  // Not found
  return NULL;
}

void symtable_clear(tSymPtr *root)
{
  if(root != NULL && *root != NULL) {
    symtable_clear(&((*root)->lptr)); // Remove left subtree
    symtable_clear(&((*root)->rptr)); // Remove right subtree
    if((*root)->name != NULL) {
      free((*root)->name);
      (*root)->name = NULL;
    }
    if((*root)->type == FUNCTION) {
      // We need to free param list
      tFuncParam *tmp;
      for(tFuncParam *current = (*root)->data.funData.params; current != NULL; current = tmp) {
        tmp = current->next;
        free(current);
      }
    }
    free(*root); // Remove current node
    *root = NULL;
  }
}

int add_param(tFuncParam *head, data_type type)
{

  if(head != NULL) {
    if (head->type == NONE) { // first param in the list
      head->next = NULL;
      head->type = type;
    }
    else {
      tFuncParam *current = NULL;
      for(current = head; current != NULL; current = current->next) {
        // Go to the end of the list
        if(current->next == NULL) {
          // insert a new element
          current->next = malloc(sizeof(tFuncParam));
          if(current->next != NULL) {
            current->next->next = NULL;
            current->next->type = type;
            break;
          }
          else {
            return INTERNAL_ERR;
          }
        }
      }
    }
  }
  return SUCCESS;
}

int insert_built_in_functions (tSymPtr *root)
{
  tFuncParam *length_params = malloc(sizeof(tFuncParam));
  if(length_params != NULL) {
    length_params->type = TYPE_STRING;
    length_params->next = NULL;
  }
  else {
    fprintf(stderr, "Internal Error: No memory left.\n");
    return INTERNAL_ERR;
  }

  tFuncParam *substr_params = malloc(sizeof(tFuncParam));
  if(substr_params != NULL) {
    substr_params->type = TYPE_STRING;
    substr_params->next = NULL;
    if (add_param(substr_params, TYPE_INT) == INTERNAL_ERR) {
      return INTERNAL_ERR;
    }
    if (add_param(substr_params, TYPE_INT) == INTERNAL_ERR) {
      return INTERNAL_ERR;
    }
  }
  else {
    fprintf(stderr, "Internal Error: No memory left.\n");
    return INTERNAL_ERR;
  }

  tFuncParam *ord_params = malloc(sizeof(tFuncParam));
  if(ord_params != NULL) {
    ord_params->type = TYPE_STRING;
    ord_params->next = NULL;
    if (add_param(ord_params, TYPE_INT) == INTERNAL_ERR) {
      return INTERNAL_ERR;
    }
  }
  else {
    fprintf(stderr, "Internal Error: No memory left.\n");
    return INTERNAL_ERR;
  }

  tFuncParam *chr_params = malloc(sizeof(tFuncParam));
  if(chr_params != NULL) {
    chr_params->type = TYPE_INT;
    chr_params->next = NULL;
  }
  else {
    fprintf(stderr, "Internal Error: No memory left.\n");
    return INTERNAL_ERR;
  }

// Insert builtin functions
  symtable_insert_function(root, "print", TYPE_NIL, -1, NULL, true); // error returns NIL
  symtable_insert_function(root, "inputs", TYPE_STRING, 0, NULL, true);
  symtable_insert_function(root, "inputi", TYPE_INT, 0, NULL, true);
  symtable_insert_function(root, "inputf", TYPE_FLOAT, 0, NULL, true);
  symtable_insert_function(root, "length", TYPE_INT, 1, length_params, true);
  symtable_insert_function(root, "substr", TYPE_STRING, 3, substr_params, true); // error returns NIL
  symtable_insert_function(root, "ord", TYPE_INT, 2, ord_params, true); // error returns NIL
  symtable_insert_function(root, "chr", TYPE_STRING, 1, chr_params, true);

  return SUCCESS;
}

int fill_symtable (tSymPtr *symtable_ptr, tToken *token)
{
  tSymPtr searchID = NULL;
  if (token->type == ID) {
    symtable_search(*symtable_ptr, token->text, &searchID);
  }

  static char *nameID = NULL;
  static bool seenID = false;
  int return_value = SUCCESS;

  if (token->type == ID && searchID == NULL)
  {
    nameID = malloc((strlen(token->text) + 1) * sizeof(char));

    if (nameID == NULL)
    {
      return INTERNAL_ERR;
    }
    else
    {
      strcpy(nameID, token->text);
      return_value = symtable_insert_unknown(symtable_ptr, nameID);
      if (return_value == SUCCESS)
      {
        seenID = true;
      }
      else
      {
        free(nameID);
      }
      return return_value;
    }
  }
  else if ((token->type == OPERATOR && (strcmp(token->text, "=") == 0)) && seenID) {
    if (strchr(nameID, '!') != NULL || strchr(nameID, '?') != NULL) {
      return VARIABLE_ERR;
    }

    return_value = symtable_insert_variable(symtable_ptr, nameID, TYPE_NIL, true);

    seenID = false;
    free(nameID);

    return return_value;
  }
  else if (seenID) {
    seenID = false;
    free(nameID);
  }
  return SUCCESS;
}

// TODO ID funkce nesmi byt shodne s zadnou lokalni ani globalni promennou
/* TODO UKLADANI FUNKCE

1) kdykoliv narazime na DEF, mela by se inicializovat nova lokalni tabulka symbolu
    def id ( PARAMS ) eol STATEMENT_N end eol PROG
    ^   ^      ^                  ^   ^
    0)  2)     3)                 4)  5)

symtable_insert_function
0) define
1) root
2) name
3) paramCount, params
4) returnType1
5) konec ramce lokalni tabulky symbolu - pomoci pocitadla endu

3:
a) vytvorit bool seen_left_bracket
  // pozor na volani funkce v definici funkce, abychom neukladali i jeji parametry - meli bychom parametry ukladat jenom potom, co je aktivni seenDef
b) while (seen_left_bracket && token-type neni right_bracket)
  {
  kdyz prijde ID --> param_count++
                 --> ulozit id do seznamu parametru v te lokalni symtable
  }
c) az prijde right_bracket --> uloz param_count

*/

int create_local_symtable(tList *symtable_list, tToken *token)
{
  tSymPtr searchID = NULL;

  if (token->type == ID) {
    symtable_search(symtable_list->First->table_ptr, token->text, &searchID); // searches through global Tree
  }
  // TODO pokud searchID neni NULL, tak mozna predefinice, ted nevim, nechce se mi o tom premyslet

  static bool seenDEF = false;
  static bool seen_left_bracket = false;
  static bool fill_local_symtable = false;
  static int countEND = 0;
  static int count_param = 0;
  static char *nameID = NULL;
  static tFuncParam *params_list = NULL;
  int return_value = SUCCESS;

  if (token->type == DEF) {
    seenDEF = true;
    countEND++;
  }
  else if (token->type == ID && searchID == NULL && seenDEF) {
    // insert function to global symtable
    return_value = symtable_insert_function(&(symtable_list->First->table_ptr), token->text, TYPE_NIL, -2, NULL, true);
    if (return_value != SUCCESS) {
      return return_value;
    }
    // remember the name of the function for later
    nameID = malloc((strlen(token->text) + 1) * sizeof(char));
    if (nameID == NULL) {
      return INTERNAL_ERR;
    }
    strcpy(nameID, token->text);
    // init new symtable
    tSymPtr localTree = NULL;
    symtable_init(&localTree);
    // insert new element in the list
    list_insert (symtable_list, localTree, token->text);
  }
  else if (fill_local_symtable && (token->type == IF || token->type == WHILE)) {
    countEND++;
  }
  else if (fill_local_symtable && token->type == END) {
    countEND--;
    if (countEND == 0) {
      fill_local_symtable = false;
    }
  }
  else if (token->type == OPERATOR && token->text[0] == '(') {
    seen_left_bracket = true;
    seenDEF = false; // setting false here prevents from storing params inside the definition if other function is called
    params_list = malloc(sizeof(tFuncParam);
    if (params_list == NULL) {
      free(nameID);
      fprintf(stderr, "Internal Error: No memory left.\n");
      return INTERNAL_ERR;
    }
    else {
      param_list->type = NONE;
      param_list->next = NULL;
    }
  }
  else if (token->type == OPERATOR && token->text[0] == ')') {
    seen_left_bracket = false;
    return_value = symtable_insert_function(&(symtable_list->First->table_ptr), nameID, TYPE_NIL, count_param, params_list, true);
    if (return_value != SUCCESS) {
      return return_value;
    }
    count_param = 0;
    fill_local_symtable = true;
    free(nameID);
    //free (params_list); // FIX ME - copy pointer in the symtable_insert_function so it can be freed here
  }
  else if (token->type == ID && searchID == NULL && seen_left_bracket) { // param is an unknown variable thus we don't know it's type
    free(nameID);
    // free (params_list); // FIX ME - copy pointer in the symtable_insert_function so it can be freed here
    return VARIABLE_ERR;
  }
  else if (token->type == ID && searchID != NULL && seen_left_bracket) {
    count_param++;
    // TODO get to know the ID type in Global symtable
    if (add_param(params_list, TYPE_INT) == INTERNAL_ERR) { // zatim jen nafejkovanej INT, protoze nemame ASS
      free(nameID);
      //free (params_list);  // FIX ME - copy pointer in the symtable_insert_function so it can be freed here
      return INTERNAL_ERR;
    }
  }
  else if (fill_local_symtable == true) {
    return_value = fill_symtable (&(symtable_list->Act->table_ptr), token);
    if (return_value != SUCCESS) {
      return return_value;
    }
  }
  else if (fill_local_symtable == false) {
    return_value = fill_symtable (&(symtable_list->First->table_ptr), token);
    if (return_value != SUCCESS) {
      return return_value;
    }
  }
  return SUCCESS;
}

int add_to_symtable(tToken *cur_token, tToken *prev_token, tSymPtr *globalTree)
{
  if(cur_token->type == OPERATOR && (strcmp(cur_token->text, "=") == 0) && prev_token->type == ID) {
    // nastav predchozi token na promennou
    int result = SUCCESS;
    result = symtable_insert_variable(globalTree, prev_token->text, TYPE_NIL, true);
    return result;
  }
  /*
  if(token->type == ID) {
    // pridej do symtable, ale prvni sezen jeho typ.
    // Prislo ID, pockej na dalsi token.
    // Jesli dalsi token je =, nastav ID jako promenou, jinak nechej unknown
    return symtable_insert_unknown(globalTree, token->text);
  }
  */
  return SUCCESS;
}
