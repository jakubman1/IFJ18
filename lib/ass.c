/**
 * @file ass.c
 * @author Jakub Man
 * @date 22.11. 2018
 * @brief Abstract syntax tree library (A.S.S. -> abstraktni syntakticky strom)
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

tAssPtr ass_find_father(tAssPtr root, tAssPtr current_node)
{
  if (root == current_node) {
    return root;
  }

  if (root == NULL) {
    return NULL;
  }

  if (ass_find_father(root->rptr, current_node) == current_node) {
    return root;
  }

  if (ass_find_father(root->lptr, current_node) == current_node) {
    return root;
  }

  return NULL;
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
  if (root != NULL) {
    ass_check_data_types(root->lptr);
    ass_check_data_types(root->rptr);
    int ret_value = ass_check_data_types_aux(root);
    if (ret_value != SUCCESS) {
      return ret_value;
    }
  }
}

int ass_check_data_types_aux(tAssPtr root)
{

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
      int == float/string/nil --> false
      int != float/string/nil --> false
      float == int/string/nil --> false
      float != int/string/nil --> false
      string == int/float/nil --> false
      string != int/float/nil --> false
      nil == int/float/string --> false
      nil != int/float/string --> false
  */
  return SUCCESS;
}
