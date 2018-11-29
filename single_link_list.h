/**
 * @file single_link_list.h
 * @author Adam Melichar
 * @date 29.11. 2018
 * @brief Single linked list for symtables.
 */

#ifndef SINGLE_LINK_LIST_H
#define SINGLE_LINK_LIST_H

#include<stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/errcodes.h"
#include "lib/types.h"
#include "lib/stack.h"
#include "scanner.h"
#include "symtable.h"
#include "expression.h"

#define TRUE 1
#define FALSE 0

typedef struct tElem {
    char *table_name;
    tSymPtr table_ptr;
    struct tElem *ptr; // next element
} *tElemPtr;

typedef struct {
    tElemPtr Act;
    tElemPtr First;
} tList;

void InitList (tList *L);
void DisposeList (tList *L);
void Succ (tList *L);
int Insert (tList *L, tSymPtr table_ptr, char *table_name);
int Active (tList *L); // returns true if active, false otherwise

#endif
