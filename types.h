/**
 * @file types.h
 * @author Jiri Tykva
 * @author Jakub Man
 * @author Adam Melichar
 * @author Jan Martinák
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
  COMMA,
  BOOL
} token_type;

typedef enum {
  LL_PROG = 100, // non-terminals
  LL_PARAMS, // 101
  LL_PARAMS_N, // 102
  LL_STATEMENT, // 103
  LL_STATEMENT_N, // 104
  LL_DEF_ARGS, // 105
  LL_DEFINE, // 106
  LL_ARGS, // 107
  LL_ARGS_N, // 108
  LL_VALUE, // 109
  LL_BOTTOM = 1000, // $  terminals
  LL_DEF, //1001
  LL_ID,
  LL_EOL,
  LL_END,
  LL_COMMA,
  LL_IF,
  LL_THEN,
  LL_ELSE, // 1008
  LL_WHILE,
  LL_DO,
  LL_SET,
  LL_BRACKET_LEFT,
  LL_BRACKET_RIGHT,
  LL_EXPRESSION,
  LL_INT,
  LL_FLOAT,
  LL_STRING,
  LL_NIL
} ll_table_type;

typedef struct token {
  char *text;
  int type;
} tToken;

#endif
