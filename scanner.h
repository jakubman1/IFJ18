#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lib/strbuff.h"
#include "lib/errcodes.h"
#include "lib/types.h"


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
  ZERO, // Got 0 as a first character in a number
  BIN, // Got 0b
  HEX, // Got 0x
  OCT, // Got 0[0-7]
  EXCLAMATION_POINT, // Got !
  GREATER, // Got >
  LESS, // Got <
  SET, // Got =

} scanner_state;

int scanner();
void send_buffer(token_type type, char **buffer);
void send_char(token_type type, char c);
void correct_token (tToken *token);

#endif
