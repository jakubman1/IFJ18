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
  if (root == current_node) {
    *father_node = root;
    return;
  }
 else if (root == NULL) {
    return;
  }
  ass_find_father(root->rptr, current_node, father_node);
  if (*father_node == current_node) {
    *father_node = root;
    return;
  }
  ass_find_father(root->lptr, current_node, father_node);
  if (*father_node == current_node) {
    *father_node = root;
    return;
  }
}

void in_order(tAssPtr root)
{
  if (root != NULL) {
    in_order(root->lptr);
    // fprintf(stderr, "%s, ", root->text);
    in_order(root->rptr);
  }
}

int ass_check_data_types(tAssPtr root)
{

  //fprintf(stderr, "opraveno mistrem rekurze\n");

  int type_left = -1;
  int type_right = -1;

  if (root->lptr == NULL && root->rptr == NULL) { // single node in the tree
    return root->type;
  }

  if (root->lptr != NULL && root->lptr->type == OPERATOR) {
    type_left = ass_check_data_types(root->lptr);
  }

  if (root->rptr != NULL && root->rptr->type == OPERATOR) {
    type_right = ass_check_data_types(root->rptr);
  }

  // last operator
  if (type_left == -1) {
    type_left = root->lptr->type;
  }
  if (type_right == -1) {
    type_right = root->rptr->type;
  }

  // for testing
  if (type_left != type_right) {
    return TYPE_ERR;
  }
  else {
    return INTEGER;
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
      int2float(int) , float --> true/false
      int == string/nil --> false
      int != string/nil --> true
      float == string/nil --> false
      float != string/nil --> true
      string == int/float/nil --> false
      string != int/float/nil --> true
      nil == int/float/string --> false
      nil != int/float/string --> true
  */
