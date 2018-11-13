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
  ERROR // Error type (unknown character) 17
} token_type;

typedef enum {
  ERR, // Error state
  START, // Default state
  COMMENT, // Comment
  IDENT, // Identifier
  STRING_START, // Got first "
  STRING_ESCAPING, // Got \ in string
  INT, // Got number not starting with 0
  FLOAT, // Current number is float
  FLOAT_E, // Number is float with letter 'e' in it
  ZERO, // Got 0 as a first character in a number
  BIN, // Got 0b
  HEX, // Got 0x
  OCT, // Got 0[0-7]
  EXCLAMATION_POINT, // Got !
  GREATER, // Got >
  LESS, // Got <
  SET, // Got =

} scanner_state;

typedef struct token {
  char *text;
  token_type type;
} tToken;

#endif
