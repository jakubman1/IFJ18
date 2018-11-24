#include "ass.h"


void ass_init(tAssPtr *root)
{
  if(*root != NULL) {
    *root = NULL;
  }
}

int ass_insert(tAssPtr *root, int type, char *text)
{
  if(root != NULL) {
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
    }
    else {
      return INTERNAL_ERR;
    }
  }

  return SUCCESS;
}

void ass_empty(tAssPtr *root)
{
  if(root != NULL && *root != NULL) {
    ass_empty(&((*root)->lptr)); // Remove left subtree
    ass_empty(&((*root)->rptr)); // Remove right subtree
    free(*root); // Remove current node
    *root = NULL;
  }
}
