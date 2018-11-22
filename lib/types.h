/**
 * @file types.h
 * @author Jakub Man
 * @author Jiří Tykva
 * @date 4.10. 2018
 * @brief Definition of types required for the project
 */

// TODO: Define required types
#ifndef TYPES_H
#define TYPES_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum {
  DEF, //keywords 0
  DO, // 1
  ELSE, // 2
  END, // 3
  IF, // 4
  NOT, // 5
  NIL, // 6
  THEN, // 7
  WHILE, // 8
  ID, // Identifier 9
  INTEGER, // Integer 10
  FLOATING_POINT, // Floating point number 11
  HEXA, // 12
  OCTA, // 13
  BINARY, //14
  OPERATOR, // Operator, eg +, -, /, (, )... 15
  STRING, //16
  ERROR, // Error type (unknown character) 17
  EOL, // 18
  COMMA
} token_type;

typedef enum {
  LL_PROG = 100,
  LL_PARAMS,
  LL_PARAMS_N,
  LL_STATEMENT,
  LL_DEF_ARGS,
  LL_DEFINE,
  LL_ARGS,
  LL_ARGS_N,
  LL_VALUE,
  LL_EXPRESSION,
  LL_DEF = 1000,
  LL_ID,
  LL_EOL,
  LL_END,
  LL_COMA,
  LL_IF,
  LL_THEN,
  LL_ELSE,
  LL_WHILE,
  LL_DO,
  LL_EQUAL,
  LL_PRINT,
  LL_INPUTS,
  LL_INPUTI,
  LL_INPUTF,
  LL_LENGHT,
  LL_SUBSTR,
  LL_ORD,
  LL_CHR,
  LL_BRACKET_LEFT,
  LL_BRACKET_RIGHT,
  LL_INT,
  LL_FLOAT,
  LL_STRING
} ll_table_type;

typedef struct token {
  char *text;
  token_type type;
} tToken;

#endif
