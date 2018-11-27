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
#include <string.h>
#include "lib/errcodes.h"


typedef enum {
  VARIABLE,
  FUNCTION,
  UNKNOWN
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
  struct symNode *lptr;
  struct symNode *rptr;
} *tSymPtr;

/**
Initialize symtable
*/
void symtable_init(tSymPtr *root);

/**
Unse
*/
int symtable_insert_unknown(tSymPtr *root, char *name);

/**
Insert a new variable into symtable. If variable already exists, update it.
@param root Root of symtable tree.
@param name Name of a variable
@param type Type of variable (can be TYPE_INT, TYPE_STRING, TYPE_FLOAT or TYPE_NIL)
@param define Was variable defined or not? If yes, variable will be set as defined
@returns 0 on success, VARIABLE_ERR if name is a function, 99 on memory allocation errors.
*/
int symtable_insert_variable(tSymPtr *root, char *name, data_type type, bool define);

/**
Insert a new function into symtable. If function already exists, update it.
@param root Root of symtable tree.
@param name Name of a function
@param returnType Return type of the function (can be TYPE_INT, TYPE_STRING, TYPE_FLOAT or TYPE_NIL)
@param paramCount Number of parameters that the function has.
@param params Single-linked list of function parameters
@param define Was function defined (true) or just called (false)?
@returns 0 on success, VARIABLE_ERR if name is a variable, 99 on memory allocation errors.
*/
int symtable_insert_function(tSymPtr *root, char *name, data_type returnType, int paramCount, tFuncParam *params, bool define);

/**
Helper function for inserting into symtable.
Do not call this function directly !
*/
void symtable_insert(tSymPtr *root, tSymPtr node);

/**
Search in symtable by name
@param root Root of symtable tree
@param name Name of the symbol you want to find
@returns pointer to symbol or NULL if nothing was found
*/
tSymPtr symtable_search(tSymPtr root, char *name);

/**
Free all allocated resources of a symtable, including parameter lists.
@param root Root of a symtable
*/
void symtable_clear(tSymPtr *root);



#endif
