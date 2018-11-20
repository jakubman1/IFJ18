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
  EOL // 18
} token_type;

typedef struct token {
  char *text;
  token_type type;
} tToken;

#endif
