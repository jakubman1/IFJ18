/**
 * @file symtable.h
 * @author Jakub Man
 * @date 24.11. 2018
 * @brief Data types and functions for symbol table.
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lib/errcodes.h"


typedef enum {
  VARIABLE,
  FUNCTION
} symbol_type;
typedef enum {
  TYPE_INT,
  TYPE_STRING,
  TYPE_FLOAT,
  TYPE_NIL
} data_type;

/* typedef union {
  int ival;
  double fval;
  char *sval;
} tVarVal;*/

typedef struct funcParam {
  data_type type;
  struct funcParam *next;
} tFuncParam;

typedef struct variableData {
  // tVarVal value;
  bool defined;
  data_type type;
} tVariableData;

typedef struct functionData {
  bool defined;
  data_type returnType;
  int paramCount;
  tFuncParam *params;
} tFuncitonData;

typedef union {
  tVariableData varData;
  tFuncitonData funData;
} tDataType;

typedef struct symNode {
  char *name;
  symbol_type type;
  tDataType data; // Based on symbol type
  struct symNode *left;
  struct symNode *right;
} *tSymPtr;

void symtable_init(tSymPtr *root);
void symtable_insert_variable(tSymPtr *root, char *name, data_type type);
void symtable_insert_function(tSymPtr *root, char *name, data_type returnType, int paramCount, tFuncParam *params, bool define);
void symtable_insert(tSymPtr *root, tSymPtr node);
tSymPtr symtable_search(tSymPtr *root, char *name);
void symtable_clear(tSymPtr *root);



#endif
