/**
 * @file ass.c
 * @author Jakub Man
 * @date 22.11. 2018
 * @brief Abstract syntax tree library (A.S.S. -> abstraktni syntakticky strom)
 * Víme že se jedná o nekonzistentní pojmenování, ale název knihovny ass.h držel morálku našeho týmu nad vodou. ;)
 */
#include "ass.h"


void ass_init(tAssPtr *root)
{
  if(root != NULL) {
    *root = NULL;
  }
}

/*int ass_insert(tAssPtr *root, int type, char *text)
{
  if(root != NULL) {
    // Create a new element
    tAssPtr new = malloc(sizeof(struct assNode));
    if(new != NULL) {
      new->type = type;
      if(text != NULL) {
        new->text = malloc(sizeof(char) * (strlen(text) + 1));
        if(new->text != NULL) {
          strcpy(new->text, text);
        }
        else {
          free(new);
          return INTERNAL_ERR;
        }
      }
      else {
        new->text = NULL;
      }
      // We have a new element, lets insert it
      if(*root == NULL) {
        // The tree was empty
        new->lptr = NULL;
        new->rptr = NULL;
        *root = new;
      }
      // TODO: Insert cases when tree is not empty
    }
    else {
      return INTERNAL_ERR;
    }
  }

  return SUCCESS;
}*/

void ass_empty(tAssPtr *root)
{
  if(root != NULL && *root != NULL) {
    ass_empty(&((*root)->lptr)); // Remove left subtree
    ass_empty(&((*root)->rptr)); // Remove right subtree
    if((*root)->text != NULL) {
      free((*root)->text);
      (*root)->text = NULL;
    }
    free(*root); // Remove current node
    *root = NULL;
  }
}

tAssPtr ass_make_tree(int type, char *text, tSymPtr symtable, tAssPtr left, tAssPtr right)
{
  tAssPtr new = ass_make_leaf(type, text, symtable);
  if(new != NULL) {
    new->lptr = left;
    new->rptr = right;

  }
  return new;
}

tAssPtr ass_make_leaf(int type, char *text, tSymPtr symtable)
{
  tAssPtr new = malloc(sizeof(struct assNode));
  if(new != NULL) {
    new->type = type;
    new->symtable = symtable;
    if(text != NULL) {
      new->text = malloc(sizeof(char) * (strlen(text) + 1));
      if(new->text != NULL) {
        strcpy(new->text, text);
      }
      else {
        // Text memory allocation failed.
        free(new);
        return NULL;
      }
    }
    else {
      new->text = NULL;
    }
    // We have a new element
    new->lptr = NULL;
    new->rptr = NULL;
    return new;
  }
  else {
    return NULL;
  }
}
/*  mozna neco takove ukroot = root a na konci return ukroot*/
// co je ukroot?

void ass_find_father(tAssPtr root, tAssPtr current_node, tAssPtr *father_node)
{
  if (root != NULL) {
    // preorder search
    if (root->lptr == current_node || root->rptr == current_node || root == current_node) { // root == current_node --> when there is only 1 node in the tree
      *father_node = root;
      return;
    }
    ass_find_father(root->lptr, current_node, father_node);
    ass_find_father(root->rptr, current_node, father_node);
  }
}

void in_order(tAssPtr root)
{
  if (root != NULL) {
    in_order(root->lptr);
    in_order(root->rptr);
    fprintf(stderr, "%s, ", root->text);
  }
}

int ass_check_data_types(tAssPtr root, tSymPtr sym)
{
  int type_left = -1;
  int type_right = -1;
  if (root->lptr == NULL && root->rptr == NULL) { // single node in the tree
    return root->type;
  }
  if (root->lptr != NULL && root->lptr->type == OPERATOR) {
    type_left = ass_check_data_types(root->lptr, sym);
  }
  if (root->rptr != NULL && root->rptr->type == OPERATOR) {
    type_right = ass_check_data_types(root->rptr, sym);
  }

  // last operator
  if (type_left == -1) {
    type_left = root->lptr->type;
  }
  if (type_right == -1) {
    type_right = root->rptr->type;
  }

  root->lptr->type = type_left;
  root->rptr->type = type_right;
  return ass_evaluate_types(root->lptr, root->rptr, root->text, sym);
}

int ass_evaluate_types(tAssPtr left_node, tAssPtr right_node, char *operator, tSymPtr sym)
{
  static int error_code = -1;
  if ((strcmp(operator, "/") == 0) && (strcmp(right_node->text, "0") == 0)) {
    error_code = DIVISION_BY_ZERO;
    return DIVISION_BY_ZERO;
  }
  if ((strcmp(operator, "+") == 0) || (strcmp(operator, "-") == 0) || (strcmp(operator, "*") == 0) || (strcmp(operator, "/") == 0)) {
    if (left_node->type == INTEGER && right_node->type == INTEGER) {
      return INTEGER;
    }
    else if ((left_node->type == FLOATING_POINT && right_node->type == FLOATING_POINT) || (left_node->type == INTEGER && right_node->type == FLOATING_POINT) || (left_node->type == FLOATING_POINT && right_node->type == INTEGER)) {
      return FLOATING_POINT;
    }
    else if (left_node->type == STRING && right_node->type == STRING) {
      return STRING;
    }
    else if ((left_node->type == UNIVERSAL && right_node->type == INTEGER) || (left_node->type == INTEGER && right_node->type == UNIVERSAL)) {
      return INTEGER;
    }
    else if ((left_node->type == UNIVERSAL && right_node->type == FLOATING_POINT) || (left_node->type == FLOATING_POINT && right_node->type == UNIVERSAL)) {
      return FLOATING_POINT;
    }
    else if ((left_node->type == UNIVERSAL && right_node->type == STRING) || (left_node->type == STRING && right_node->type == UNIVERSAL)) {
      return STRING;
    }
    else if ((left_node->type == UNIVERSAL && right_node->type == NIL) || (left_node->type == NIL && right_node->type == UNIVERSAL)) {
      return NIL;
    }
    else {
      if (error_code == DIVISION_BY_ZERO) {
        return DIVISION_BY_ZERO;
      }
      else {
        return TYPE_ERR;
      }
    }
  }
  else if ((strcmp(operator, "<") == 0) || (strcmp(operator, "<=") == 0) || (strcmp(operator, ">") == 0) || (strcmp(operator, ">=") == 0)) {
    if (left_node->type == INTEGER && right_node->type == INTEGER) {
      return BOOL;
    }
    else if ((left_node->type == FLOATING_POINT && right_node->type == FLOATING_POINT) || (left_node->type == INTEGER && right_node->type == FLOATING_POINT) || (left_node->type == FLOATING_POINT && right_node->type == INTEGER)) {
      return BOOL;
    }
    else if (left_node->type == STRING && right_node->type == STRING) {
      return BOOL;
    }
    else if (left_node->type == NIL && right_node->type == NIL) {
      return BOOL;
    }
    else if ((left_node->type == UNIVERSAL && right_node->type == INTEGER) || (left_node->type == INTEGER && right_node->type == UNIVERSAL)) {
      return BOOL;
    }
    else if ((left_node->type == UNIVERSAL && right_node->type == FLOATING_POINT) || (left_node->type == FLOATING_POINT && right_node->type == UNIVERSAL)) {
      return BOOL;
    }
    else if ((left_node->type == UNIVERSAL && right_node->type == STRING) || (left_node->type == STRING && right_node->type == UNIVERSAL)) {
      return BOOL;
    }
    else {
      return TYPE_ERR;
    }
  }
  else {
    return BOOL;
  }
}

  /*  SCITANI, ODCITANI, NASOBENI, DELENI, KONKATENACE
      int , int --> int
      int , float --> float
      float , float --> float
      string , string --> string
      ------------------------
      int / 0 --> ERROR
      float / 0 --> ERROR
      true/false, int/float/string/nil --> ERROR

      POROVNAVANI (<, >, =<, >=, ==, !=)
      int , int --> true/false
      float , float --> true/false
      string , string --> true/false
      int2float(int) , float --> true/false // TODO
      ----------------------------
      int == string/nil --> false
      int != string/nil --> true
      float == string/nil --> false
      float != string/nil --> true
      string == int/float/nil --> false
      string != int/float/nil --> true
      nil == int/float/string --> false
      nil != int/float/string --> true
  */
