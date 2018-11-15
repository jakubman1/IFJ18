#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lib/errcodes.h"
#include "lib/types.h"
#include "lib/strbuff.h"

#define LEFT 0
#define RIGHT 8 //TODO: Comment magic constants

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
  FLOAT_ERR1,
  FLOAT_ERR2,
  FLOAT_ERR3,
  FLOAT_ERR4,
  ZERO, // Got 0 as a first character in a number
  BIN, // Got 0b
  HEX, // Got 0x
  OCT, // Got 0[0-7]
  EXCLAMATION_POINT, // Got !
  GREATER, // Got >
  LESS, // Got <
  SET, // Got =
  NEW_LINE,
  MULTILINE_COMMENT

} scanner_state;

/**
Finite state machine for scanner
@returns return code of the program
*/
int scanner();

/**
@brief Change buffer into token and send it to syntactic analysis. Buffer will be cleared
@param type Type of token from token_type enum defined in types.h
@param buffer Poiner to buffer containing token text
*/
void send_buffer(token_type type, tBuffer *buffer);

/**
@brief Send char as token to syntactic analysis. Used for one char long tokens (eg operator +, -...)
@param type Type of token from token_type enum defined in types.h
@param c Character to send as token text. Will be converted into string
 */
void send_char(token_type type, char c);

/**
Function distinguishes token type and if it matches a keyword it changes it's type.

@brief Changes type of token if keyword
@param token Token structure
*/
void correct_token (tToken *token);

#endif
