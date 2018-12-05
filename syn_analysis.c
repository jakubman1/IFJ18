/**
 * @file syn_analysis.c
 * @author Jiri Tykva
 * @author Jakub Man
 * @author Adam Melichar
 * @author ̶J̶a̶n̶ M̶a̶r̶t̶i̶n̶a̶k̶
 * @date 19.11. 2018
 * @brief Syntactic analysis file
 */

#include "syn_analysis.h"

int insert_built_in_functions (tSymPtr *root)
{
  tFuncParam *length_params = NULL;
  if (add_param(&length_params, STRING, "") == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }

  tFuncParam *substr_params = NULL;
  if (add_param(&substr_params, TYPE_STRING, "") == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }
  if (add_param(&substr_params, TYPE_INT, "") == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }
  if (add_param(&substr_params, TYPE_INT, "") == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }

  tFuncParam *ord_params = NULL;
  if (add_param(&ord_params, TYPE_STRING, "") == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }
  if (add_param(&ord_params, TYPE_INT, "") == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }

  tFuncParam *chr_params = NULL;
  if (add_param(&chr_params, TYPE_INT, "") == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }

// Insert built in functions
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

int fill_global_symtable (tList *symtable_list, tToken *token)
{
  static char *nameID = NULL;
  static bool seenID = false;
  int return_value = SUCCESS;

  tSymPtr ID_global = NULL;
  tSymPtr ID_local = NULL;
  tSymPtr searchNameID = NULL;
  tSymPtr ID_gen_var = NULL;

  if (token->type == ID) {
    symtable_search(symtable_list->First->table_ptr, token->text, &ID_global); // searches through global Tree
    symtable_search(symtable_list->Act->table_ptr, token->text, &ID_local); // searches through local Tree
    seenID = true;
    nameID = malloc((strlen(token->text) + 1) * sizeof(char));
    if (nameID == NULL) {
      return INTERNAL_ERR;
    }
    strcpy(nameID, token->text);
  }

  if (token->type == ID && (ID_global == NULL || (ID_global != NULL && ID_global->type == UNKNOWN))) {
    // save as unknown
    return_value = symtable_insert_unknown(&(symtable_list->First->table_ptr), nameID);
    if (return_value != SUCCESS) {
      free(nameID);
    }
    seenID = true;
    return return_value;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "=") == 0) && seenID) {
    symtable_search(symtable_list->First->table_ptr, nameID, &searchNameID);
    if (searchNameID == NULL) {
      free(nameID);
      return INTERNAL_ERR;
    }
    if (searchNameID->type != FUNCTION && strchr(nameID, '!') == NULL && strchr(nameID, '?') == NULL) {
      return_value = symtable_insert_variable(&(symtable_list->First->table_ptr), nameID, TYPE_NIL, true);
      // GENERATING IFJcode18
      symtable_search(symtable_list->First->table_ptr, nameID, &ID_gen_var);
      gen_var(ID_gen_var, true);
      free(nameID);
      seenID = false;
      return return_value;
    }
    else {
      free(nameID);
      seenID = false;
      return VARIABLE_ERR;
    }
  }
  else if (seenID == true && (ID_global != NULL && ID_global->type != FUNCTION)) {
    free(nameID);
    seenID = false;
  }
  return SUCCESS;
}

int fill_local_symtable (tList *symtable_list, tToken *token, bool isParam)
{
  static char *nameID = NULL;
  static bool seenID = false;
  int return_value = SUCCESS;

  tSymPtr ID_global = NULL;
  tSymPtr ID_local = NULL;
  tSymPtr searchNameID = NULL;
  tSymPtr ID_gen_var = NULL;

  if (token->type == ID) {
    symtable_search(symtable_list->First->table_ptr, token->text, &ID_global); // searches through global Tree
    symtable_search(symtable_list->Act->table_ptr, token->text, &ID_local); // searches through local Tree
  }

  if (isParam == true && ID_local == NULL && (ID_global == NULL || (ID_global != NULL && ID_global->type != FUNCTION))) {
    return_value = symtable_insert_variable(&(symtable_list->Act->table_ptr), token->text, UNIVERSAL, true);
    return return_value;
  }
  else if (isParam == true && (ID_local != NULL || (ID_global == NULL && ID_global->type == FUNCTION))) {
    return VARIABLE_ERR;
  }
  else if (isParam == false ) {
    if (token->type == ID && (ID_local == NULL || (ID_local != NULL && ID_local->type == UNKNOWN))) {
      // save as unknown
      nameID = malloc((strlen(token->text) + 1) * sizeof(char));

      if (nameID == NULL) {
        return INTERNAL_ERR;
      }
      strcpy(nameID, token->text);
      return_value = symtable_insert_unknown(&(symtable_list->Act->table_ptr), nameID);
      if (return_value != SUCCESS) {
        free(nameID);
      }
      seenID = true;
      return return_value;
    }
    else if (token->type == OPERATOR && (strcmp(token->text, "=") == 0) && seenID) {
      symtable_search(symtable_list->First->table_ptr, nameID, &searchNameID);
      if (searchNameID != NULL && searchNameID->type == FUNCTION) {
        free(nameID);
        return VARIABLE_ERR;
      }
      if (strchr(nameID, '!') == NULL && strchr(nameID, '?') == NULL) {
        return_value = symtable_insert_variable(&(symtable_list->Act->table_ptr), nameID, TYPE_NIL, true);
        symtable_search(symtable_list->Act->table_ptr, nameID, &ID_gen_var);
        // GENERATING IFJcode18
        gen_var(ID_gen_var, false);
        free(nameID);
        seenID = false;
        return return_value;
      }
      else {
        free(nameID);
        seenID = false;
        return VARIABLE_ERR;
      }
    }
    else if (seenID == true) {
      free(nameID);
      seenID = false;
    }
  }
  return SUCCESS;
}

int add_to_symtable(tList *symtable_list, tToken *token, tStack *gen_stack)
{
  tSymPtr ID_global = NULL; // driv bylo searchID, tak to pak prepsat
  tSymPtr ID_local = NULL;

  if (token->type == ID) {
    symtable_search(symtable_list->First->table_ptr, token->text, &ID_global); // searches through global Tree
  }
  if (token->type == ID) {
    symtable_search(symtable_list->Act->table_ptr, token->text, &ID_local); // searches through local Tree
  }
  // TODO pokud searchID neni NULL, tak mozna predefinice, ted nevim, nechce se mi o tom premyslet


  static bool seenDEF = false;
  static bool seen_left_bracket = false;
  static bool local_symtable = false;
  static bool is_end_while = false;
  static int countEND = 0;
  static int count_param = 0;
  static char *nameID = NULL;
  static tFuncParam *params_list = NULL;
  int return_value = SUCCESS;

  if (token->type == DEF) {
    seenDEF = true;
    countEND++;
  }
  else if (token->type == ID && seenDEF && (ID_global == NULL || (ID_global != NULL && ID_global->type == UNKNOWN))) {
    local_symtable = true;
    // insert function to global symtable
    if (ID_global == NULL) {
      return_value = symtable_insert_function(&(symtable_list->First->table_ptr), token->text, UNIVERSAL, -2, params_list, true);
    }
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
    list_insert (symtable_list, localTree, nameID);
  }
  else if (token->type == ID && seenDEF && (ID_global != NULL && ID_global->type == VARIABLE)) {
    return VARIABLE_ERR;
  }
  else if (token->type == OPERATOR && token->text[0] == '(' && seenDEF) {
    seen_left_bracket = true;
    seenDEF = false; // setting false here prevents from storing params inside the definition if other function is called
  }
  else if (token->type == ID && seen_left_bracket && (ID_global == NULL || (ID_global != NULL && ID_global->type != FUNCTION)) && ID_local == NULL) {
    count_param++;
    return_value = add_param(&params_list, UNIVERSAL, token->text);
    if (return_value == INTERNAL_ERR) {
      return INTERNAL_ERR;
    }

    return_value = fill_local_symtable (symtable_list, token, true);
    if (return_value != SUCCESS) {
      free(nameID);
      return return_value;
    }

  }
  else if (token->type == ID && seen_left_bracket && ((ID_global != NULL && ID_global->type == FUNCTION) || ID_local != NULL)) {
    free(nameID);
    return VARIABLE_ERR;
  }
  else if (token->type == OPERATOR && token->text[0] == ')' && seen_left_bracket) {
    seen_left_bracket = false;

    tSymPtr rest = NULL;
    symtable_search(symtable_list->First->table_ptr, nameID, &rest);
    if (rest->data.funData.paramCount != -1) {
      if (rest->type == UNKNOWN && count_param != rest->data.funData.paramCount) {
        fprintf(stderr, ANSI_COLOR_RED "Param error: "ANSI_COLOR_RESET" wrong number of arguments.\n");
        return PARAM_ERR;
      }
    }

    return_value = symtable_insert_function(&(symtable_list->First->table_ptr), nameID, UNIVERSAL, count_param, params_list, true);
    if (return_value != SUCCESS) {
      return return_value;
    }

    // GENERATING IFJcode18
    tSymPtr gen_func = NULL;
    symtable_search(symtable_list->First->table_ptr, nameID, &gen_func);
    gen_def(gen_func);

    count_param = 0;
    free(nameID);
  }
  else if (local_symtable && (token->type == IF || token->type == WHILE)) {
    countEND++;
    if (token->type == IF) {
      is_end_while = false;
    }
    if (token->type == WHILE) {
      is_end_while = true;
    }
  }
  else if (local_symtable && token->type == END) {
    countEND--;
    if (countEND == 0) {
      local_symtable = false;
      // GENERATING IFJcode18
      gen_end_def(symtable_list->Act->table_name);
    }
    else {
      if (is_end_while) {
        gen_while_end(gen_stack);
      }
      else {
        gen_if_end(gen_stack);
      }
    }
  }
  //BODY OF THE FUNCTION
  else if (local_symtable == true) {
    return_value = fill_local_symtable (symtable_list, token, false);
    if (return_value != SUCCESS) {
      return return_value;
    }

  }
  // GLOBAL
  else if (local_symtable == false) {
    return_value = fill_global_symtable (symtable_list, token);
    if (return_value != SUCCESS) {
      return return_value;
    }
  }
  return SUCCESS;
}


int parser()
{
  int result = SUCCESS;
  int scanner_out = SUCCESS;
  tToken currentToken = {"", ERROR};
  tStack stack;
  tList symtable_list;
  tSymPtr globalTree = NULL;
  tStack gen_stack;

  int countEND = 0;
  bool isGlobal = true;

  symtable_init(&globalTree);

  if (insert_built_in_functions(&globalTree) == INTERNAL_ERR) {
    return INTERNAL_ERR;
  }

  list_init(&symtable_list);
  list_insert(&symtable_list, globalTree, NULL);

  s_init(&stack);
  s_init(&gen_stack);

  s_push(&stack, LL_BOTTOM);
  s_push(&stack, LL_PROG);

  char *id_name = malloc(sizeof(char) * 4);
  if(id_name == NULL) {
    return INTERNAL_ERR;
  }

  char *id_func_name = malloc(sizeof(char) * 4);
  if(id_func_name == NULL) {
    return INTERNAL_ERR;
  }

  while((scanner_out = scanner(&currentToken)) == SUCCESS && result == SUCCESS) {
    // create abstract syntax tree
    // currentToken contains new token in every iteration

    result = add_to_symtable(&symtable_list, &currentToken, &gen_stack);

    if(result == 0) {
      if (currentToken.type == DEF) {
        countEND++;
      }
      else if ((currentToken.type == IF || currentToken.type == WHILE) && isGlobal == false) {
        countEND++;
      }
      else if (currentToken.type == END && isGlobal == false) {
        countEND--;
      }

      if (countEND == 0) {
        isGlobal = true;
      }
      else {
        isGlobal = false;
      }

      result = ll_predict(&currentToken, &stack, &symtable_list, isGlobal, id_name, id_func_name, &gen_stack);
    }

    //// MUSI BYT AZ NA KONCI CYKLU !!!!!!!!!!!!!!!
    if(currentToken.text != NULL) {
      // Free allocated text
      free(currentToken.text);
      currentToken.text = NULL;
    }
    if(result == VARIABLE_ERR) {
      fprintf(stderr, ANSI_COLOR_RED "Variable error: " ANSI_COLOR_RESET "Can not assign value into a function\n");
      // nechybi tu free stack?
      free(id_name);
      free(id_func_name);
      return result;
    }
    // TODO: Free allocated resouorces on

  } // end while

  if(scanner_out != -1) {
    // handle errors during lexical analysis

    if(scanner_out == LEXICAL_ERR) {
      while((scanner_out = scanner(&currentToken)) == SUCCESS || scanner_out == LEXICAL_ERR) {
        // Print all lexicals errors (scanner takes care of printing)
        if(currentToken.text != NULL) {
          // Free allocated text
          free(currentToken.text);
        }
      }
      result = LEXICAL_ERR;
    }
    else {
        if(result == INTERNAL_ERR) {
          fprintf(stderr, ANSI_COLOR_RED "Internal error: " ANSI_COLOR_RESET "memory allocation failed. Not enough memory?\n");
        }
      // Free allocated resources and quit (if there are any)
    }
  }
  // We no longer need the stack, remove it.
  else {
    // EOF
    int top = s_top(&stack);
    if (top == LL_PROG) {
      while(top == LL_PROG) {
        top = s_pop(&stack);
      }
      if (top == LL_BOTTOM) {
        result = SUCCESS;
      }
      else if(result == SUCCESS){
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: " ANSI_COLOR_RESET "Unexpected end of file.\n");
        result = SYNTAX_ERR;
      }
    }
    else if(result == SUCCESS){
      fprintf(stderr, ANSI_COLOR_RED "Syntax error: " ANSI_COLOR_RESET "Unexpected end of file.\n");
      result = SYNTAX_ERR;
    }

  }
  s_free(&stack);

  /*fprintf(stderr, "VYPIS TABULEK SYMBOLU:\n");
  //fprintf(stderr, "Globalni 1. ID (foo): %s\n", symtable_list.First->table_ptr->name);
  //fprintf(stderr, "Globalni 1. ID (foo) param_count: %d\n", symtable_list.First->table_ptr->data.funData.paramCount);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 1. Element: %p\n", symtable_list.First->table_ptr->data.funData.params);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 1. Element Typ: %d\n", symtable_list.First->table_ptr->data.funData.params->type);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 1. (a) Element head: %p\n", symtable_list.First->table_ptr->data.funData.params);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 2. (b) Element next: %p\n", symtable_list.First->table_ptr->data.funData.params->next);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 3. (c) Element next->next: %p\n", symtable_list.First->table_ptr->data.funData.params->next->next);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 1. Element rptr: %p\n", symtable_list.First->table_ptr->rptr);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 2. Element: %p\n", symtable_list.First->table_ptr->rptr->data.funData.params);
  //fprintf(stderr, "Globalni 1. ID (foo) param_list 2. Element Typ: %d\n", symtable_list.First->table_ptr->rptr->data.funData.params->type);
  //fprintf(stderr, "Globalni 2. ID (ref): %s\n", symtable_list.First->table_ptr->rptr->name);
  //fprintf(stderr, "Lokalni jmeno: %s\n", symtable_list.Act->table_name);
  //fprintf(stderr, "Lokalni 1. ID (a): %s\n", symtable_list.Act->table_ptr->name);
  //fprintf(stderr, "Lokalni 2. ID (b): %s\n", symtable_list.Act->table_ptr->rptr->name);
  //fprintf(stderr, "Lokalni 2. ID (b): %s\n", symtable_list.Act->table_ptr->rptr->name);
  fprintf(stderr, "%s type: %d\n", symtable_list.First->table_ptr->name, symtable_list.First->table_ptr->data.varData.type);
  fprintf(stderr, "%s type: %d\n", symtable_list.First->table_ptr->rptr->name, symtable_list.First->table_ptr->rptr->data.varData.type);
  fprintf(stderr, "KONEC VYPISU:\n");*/

  free(id_name);
  free(id_func_name);
  return result;
}


int ll_predict(tToken *token, tStack *stack, tList *symtable_list, bool isGlobal, char *id_name, char *id_func_name, tStack *gen_stack)
{
  int top = s_top(stack);
  static int paramCount = 0;
  static tFuncParam *args = NULL;
  static int gen_uniq_num = 0;
  static bool is_end_while = false;

  // NON TERMINALS
while ((top = s_top(stack)) >= LL_PROG && top < LL_BOTTOM) {
  switch (s_top(stack)) {
    case LL_BOTTOM: // input cannot be EOF here
      return SYNTAX_ERR;
      break;
    case LL_PROG:
      if(token->type == DEF) {
        PUSH_RULE_1;
      }
      else if(token->type == ID || token->type == IF || token->type == WHILE || token->type == EOL) {
        if (token->type == WHILE) {
        //  s_push(gen_stack, gen_uniq_num);
          printf("DEFVAR LF@whilecond%d\n", gen_uniq_num);
          printf("LABEL $$WHILE%d\n", gen_uniq_num);

        }
        PUSH_RULE_2;
      }
      else if(token->type == END || token->type == ELSE ) {
        PUSH_RULE_3; // $ handled before ll_predict
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_PARAMS:
      if(token->type == ID) {
        PUSH_RULE_4;
      }
      else if(token->type == OPERATOR && token->text[0] == ')') {
        PUSH_RULE_5;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s. Expected EOL, ( or )\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_STATEMENT_N:
      if(token->type == ID || token->type == EOL || token->type == IF || token->type == WHILE) {
        PUSH_RULE_12;
      }
      else if (token->type == END || token->type == ELSE) {
        PUSH_RULE_13;
      }
      break;
    case LL_STATEMENT:
      if(token->type == ID) {
        PUSH_RULE_10;
        id_name = (char *)realloc(id_name, strlen(token->text) + 1);
        if (id_name == NULL) {
          return INTERNAL_ERR;
        }
        strcpy(id_name, token->text);

      //  fprintf(stderr, "ukladame %s\n", token->text);
        id_func_name = (char *) realloc(id_func_name, sizeof(char) * (strlen(token->text) + 1));
        if (id_func_name == NULL) {
          return INTERNAL_ERR;
        }
        strcpy(id_func_name, token->text);

      }
      else if(token->type == EOL) {
        PUSH_RULE_11;
      }
      else if(token->type == IF) {
        PUSH_RULE_8;
      }
      else if(token->type == WHILE) {
        PUSH_RULE_9;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_PARAMS_N:
      if(token->type == OPERATOR && token->text[0] == ')') {
        PUSH_RULE_7;
      }
      else if(token->type == COMMA) {
        PUSH_RULE_6;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s. Expected EOL, ) or ,\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_DEF_ARGS:
      if(token->type == ID || token->type == EOL || (token->type == OPERATOR && token->text[0] == '(') || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING || token->type == NIL) {
        PUSH_RULE_15;
      }
      else if(token->type == OPERATOR && (strcmp(token->text, "=") == 0)) {
        PUSH_RULE_14;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_DEFINE:
      if(token->type == EOL) {
        PUSH_RULE_18;
      }
      else if(token->type == ID) { // must be ID of a function

        tSymPtr sym = NULL;

        if (isGlobal) {
          symtable_search(symtable_list->First->table_ptr, token->text, &sym);
        }
        else {
          symtable_search(symtable_list->Act->table_ptr, token->text, &sym);
        }

        if (sym == NULL) {
          return VARIABLE_ERR;
        }

        if (sym->type == UNKNOWN || sym->type == FUNCTION) {
          id_func_name = (char *) realloc(id_func_name, sizeof(char) * (strlen(sym->name) + 1));
          if (id_func_name == NULL) {
            return INTERNAL_ERR;
          }
          strcpy(id_func_name, sym->name);
        }

        if(sym->type == UNKNOWN || sym->type == FUNCTION) {
          PUSH_RULE_16;
        }
        else {
          PUSH_RULE_17;
        }
      }
      else if (token->type == INTEGER || token->type == STRING || token->type == FLOATING_POINT || token->type == NIL || token->type == OPERATOR) { // everything that fits in expression,
        PUSH_RULE_17;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ARGS:
      if(token->type == ID || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING || token->type == NIL) {
        PUSH_RULE_21;
        // GENERATING IFJcode18
        // start of call func
        if (!BUILT_IN_FUNCTION) {
         CREATEFRAME;
        }
      }
      else if(token->type == EOL || (token->type == OPERATOR && token->text[0] == ')')) {
        PUSH_RULE_19;
        // insert data type - ID_variable = ID_function
        tSymPtr result = NULL;
        if (isGlobal) {
          symtable_search(symtable_list->First->table_ptr, id_name, &result);
        }
        else {
          symtable_search(symtable_list->Act->table_ptr, id_name, &result);
        }

        if (result->type != FUNCTION) {
          symtable_search(symtable_list->First->table_ptr, id_func_name, &result);
          if (result->type == UNKNOWN) {
            result->data.funData.returnType = UNIVERSAL;
          }
          if (result->type == FUNCTION || result->type == UNKNOWN) {
            symtable_insert_variable(&(symtable_list->First->table_ptr), id_name, result->data.funData.returnType, true);
            // GENERATING IFJcode18
            fprintf(stderr, "tady 1\n");
            //call_function(result, id_name, isGlobal, args);
            tSymPtr temp = NULL;
            symtable_search(symtable_list->First->table_ptr, id_name, &temp);
            set_variable(temp, id_name, result->data.funData.returnType, isGlobal);
          }
        }
        if (result->type == VARIABLE) {
          break;
        }
        // check param count
        if (result->type == UNKNOWN) {
          result->data.funData.paramCount = paramCount;
        }
        else if (paramCount != result->data.funData.paramCount) {
          if (result->data.funData.paramCount != -1) {
            fprintf(stderr, ANSI_COLOR_RED "Param error: "ANSI_COLOR_RESET" wrong number of arguments.\n");
            return PARAM_ERR;
          }
        }
      }
      else if(token->type == OPERATOR && token->text[0] == '(') {
        PUSH_RULE_20;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ARGS_N:
      if (token->type == EOL || (token->type == OPERATOR && token->text[0] == ')')) {
        PUSH_RULE_23;
        // insert data type - ID_variable = ID_function
        tSymPtr result = NULL;
        if (isGlobal) {
          symtable_search(symtable_list->First->table_ptr, id_name, &result);
        }
        else {
          symtable_search(symtable_list->Act->table_ptr, id_name, &result);
        }
        if (result->type != FUNCTION) {
          fprintf(stderr, "asdasfdasfdasf\n");
          symtable_search(symtable_list->First->table_ptr, id_func_name, &result);
          if (result->type == UNKNOWN) {
            result->data.funData.returnType = UNIVERSAL;
          }
          if (result->type == FUNCTION || result->type == UNKNOWN) {
            symtable_insert_variable(&(symtable_list->First->table_ptr), id_name, result->data.funData.returnType, true);
            // GENERATING IFJcode18
            fprintf(stderr, "tady 2\n");
            call_function(result, id_name, isGlobal, args);
            tSymPtr temp = NULL;
            symtable_search(symtable_list->First->table_ptr, id_name, &temp);
            set_variable(temp, id_name, result->data.funData.returnType, isGlobal);
          }
        }

        if (result->type == UNKNOWN) {
          result->data.funData.paramCount = paramCount;
        }
        else if (paramCount != result->data.funData.paramCount) {
          if (result->data.funData.paramCount != -1) {
            fprintf(stderr, ANSI_COLOR_RED "Param error: "ANSI_COLOR_RESET" wrong number of arguments.\n");
            return PARAM_ERR;
          }
        }
      }
      else if (token->type == COMMA) {
        PUSH_RULE_22;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_VALUE:
      paramCount++; // start of func param check
      // save params for these functions
      if (BUILT_IN_FUNCTION) {
      //  fprintf(stderr, "strkam tam %s\n", token->text);
        if (add_param(&args, token->type, token->text) == INTERNAL_ERR) {
          // TODO free
          return INTERNAL_ERR;
        }
      }
      // GENERATING IFJcode18
      if (token->type == ID) {
        PUSH_RULE_26;
        // GENERATING IFJcode18
        if (!BUILT_IN_FUNCTION)
        {
          if (isGlobal) {
            CALL_PARAM(printf("GF@%s",token->text), paramCount);
          }
          else {
            CALL_PARAM(printf("LF@%s",token->text), paramCount);
          }
        }
      }
      else if (token->type == INTEGER) {
        PUSH_RULE_24;
        // GENERATING IFJcode18
        if (!BUILT_IN_FUNCTION) {
        CALL_PARAM(print_int(token->text), paramCount);
        }
      }
      else if (token->type == FLOATING_POINT) {
        PUSH_RULE_25;
        // GENERATING IFJcode18
        if (!BUILT_IN_FUNCTION) {
          CALL_PARAM(print_float(token->text), paramCount);
        }
      }
      else if (token->type == STRING) {
        PUSH_RULE_27;
        // GENERATING IFJcode18
        if (!BUILT_IN_FUNCTION) {
          CALL_PARAM(print_string(token->text), paramCount);
        }
      }
      else if (token->type == NIL) {
        PUSH_RULE_28;
        // GENERATING IFJcode18
        if (!BUILT_IN_FUNCTION) {
          CALL_PARAM(print_nil(), paramCount);
        }
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    default:
      // Handle syntax error
      fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" Total RIP :(\n");
      return SYNTAX_ERR;
      break;
    } // end switch non-terminals
  } // end while non-terminals

  // TERMINALS
  tSymPtr sym = NULL;
  switch(s_top(stack)) {
    case LL_DEF:
      if (token->type == DEF) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s, expected def.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_EXPRESSION: // "Skoc na mna" -Adam Melichar 2018
      if (isGlobal) {
        sym = symtable_list->First->table_ptr;
      }
      else {
        sym = symtable_list->Act->table_ptr;
      }
      int return_value = 0;
      if (token->type == INTEGER || token->type == STRING || token->type == FLOATING_POINT || token->type == NIL || token->type == OPERATOR || token->type == ID) { // everything that fits in expression,  TODO must be ID of a variable
        return_value = prec_table(token, sym, isGlobal);
        if(return_value == SYNTAX_ERR) {
          fprintf(stderr, ANSI_COLOR_RED "Syntax error: " ANSI_COLOR_RESET "Unexpected token \"%s\" in expression.\n", token->text);
          return SYNTAX_ERR;
        }
        else if (return_value == DIVISION_BY_ZERO) {
          fprintf(stderr, ANSI_COLOR_RED "Fatal error: " ANSI_COLOR_RESET "Division by zero.\n");
          return DIVISION_BY_ZERO;
        }
        else if(return_value == TYPE_ERR) {
          fprintf(stderr, ANSI_COLOR_RED "Type error: " ANSI_COLOR_RESET "Operation on incompatible types.\n");
          return TYPE_ERR;
        }
      }
      else { // everything else or ID of a function
        tToken endExpression = {"", LL_BOTTOM}; // finish expression
        return_value = prec_table(&endExpression, sym, isGlobal);
        if (return_value == SYNTAX_ERR) {
          fprintf(stderr, ANSI_COLOR_RED "Syntax error: " ANSI_COLOR_RESET "Unexpected token \"%s\" in expression.\n", token->text);
          return SYNTAX_ERR;
        }
        else if (return_value == TYPE_ERR) {
          fprintf(stderr, ANSI_COLOR_RED "Type error: " ANSI_COLOR_RESET "Operation on incompatible types.\n");
          return TYPE_ERR;
        }
        else if (return_value == DIVISION_BY_ZERO) {
          fprintf(stderr, ANSI_COLOR_RED "Fatal error: " ANSI_COLOR_RESET "Division by zero.\n");
          return DIVISION_BY_ZERO;
        }
        else if (return_value == NIL || return_value == INTEGER || return_value == FLOATING_POINT || return_value == STRING || return_value == BOOL) {
          if (token->type == EOL) {
            int retval = symtable_insert_variable(&sym, id_name, return_value, true);
            printf("POPS %s@%s\n", isGlobal ? "GF" : "LF", id_name);
            if (retval == INTERNAL_ERR) {
              return INTERNAL_ERR;
            }
          }
          else if (token->type == DO) { // end of while expression

            // Tady musi prijit kod expression
            printf("POPS LF@whilecond%d\n", gen_uniq_num);
            gen_while(gen_stack);
            gen_uniq_num++;
          }
          else if (token->type == THEN) { // end of if expression
            s_push(gen_stack, gen_uniq_num);
            printf("DEFVAR LF@ifcond%d\n", gen_uniq_num);
            printf("POPS LF@ifcond%d\n", gen_uniq_num);
            gen_if(gen_stack);
            gen_uniq_num++;
          }
          s_pop(stack);
          ll_predict(token, stack, symtable_list, isGlobal, id_name, id_func_name, gen_stack); // recall the function in order not to lose a token
        }
      }
      break;
    case LL_ID:
      if (token->type == ID) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_EOL:
      paramCount = 0;
      if (token->type == EOL) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s, expected EOL.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_END:
      if (token->type == END) {
        s_pop(stack);
        if (isGlobal) {
          if (is_end_while) {
            gen_while_end(gen_stack);
          }
          else { // of of if
            gen_if_end(gen_stack);
          }
          //fprintf(stderr, "POPUJEM\n");
          s_pop(gen_stack);
        }
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s, expected end.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_COMMA:
      if (token->type == COMMA) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected comma, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_IF:
      if (token->type == IF) {
        s_pop(stack);
        is_end_while = false;
        //gen_uniq_num++;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected if, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_THEN:
      if (token->type == THEN) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected then, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ELSE:
      if (token->type == ELSE) {
        s_pop(stack);
        gen_else(gen_stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected else, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_WHILE:
      if (token->type == WHILE) {
        s_push(gen_stack, gen_uniq_num);
        s_pop(stack);
        is_end_while = true;
        //gen_uniq_num++;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected while, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_DO:
      if (token->type == DO) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected do, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_SET:
      if (token->type == OPERATOR && (strcmp(token->text, "=") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected =, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_BRACKET_LEFT:
      if (token->type == OPERATOR && token->text[0] == '(') {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected (, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_BRACKET_RIGHT:
      if (token->type == OPERATOR && token->text[0] == ')') {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" imbalanced brackets.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_INT:
      if (token->type == INTEGER) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected integer, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_FLOAT:
      if (token->type == FLOATING_POINT) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected float, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_STRING:
      if (token->type == STRING) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected string, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_NIL:
      if (token->type == NIL) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" expected string, got \"%s\".\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    default:
      // Handle syntax error
      fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" Total RIP :(\n");
      return SYNTAX_ERR;
      break;
  } // end switch terminals

  // TODO  into tree, derivation tree

  // Why can you drink a drink, but not food a food?
  return SUCCESS;
}
