#include "single_link_list.h"

void InitList (tList *L)
{
  L->Act = NULL;
  L->First = NULL;
}

// TODO call DisposeSymtable in every element
void DisposeList (tList *L)
{
  L->Act = L->First;

  while ( L->Act != NULL )
  {
    L->First = L->Act->ptr;
    free(L->Act);
    L->Act = L->First;
  }
}

int InsertFirst (tList *L, tSymPtr table_ptr, char *table_name)
{
  tElemPtr new_elem = malloc(sizeof( struct tElem ));

  if ( new_elem == NULL )
  {
    return INTERNAL_ERR;
  }

  new_elem->table_name = table_name;
  new_elem->table_ptr = table_ptr;
  new_elem->ptr = NULL;
  L->First = new_elem;
  L->Act = new_elem;

  return SUCCESS;
}

void Succ (tList *L)
{
  if ( L->Act == NULL )
  {
    return;
  }

  L->Act = L->Act->ptr;
}

int PostInsert (tList *L, tSymPtr table_ptr, char *table_name)
{
  if ( L->Act == NULL )
  {
    return -1;
  }

  tElemPtr new_elem = malloc(sizeof( struct tElem ));

  if ( new_elem == NULL )
  {
    return INTERNAL_ERR;
  }

  new_elem->table_name = table_name;
  new_elem->table_ptr = table_ptr;
  new_elem->ptr = L->Act->ptr;
  L->Act->ptr = new_elem;

  Succ(L);

  return SUCCESS;
}

int Active (tList *L)
{
  return L->Act != NULL;
}
