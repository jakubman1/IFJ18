#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "errcodes.h"
#include "types.h"

#define DEFAULT_BUFFER_SIZE 4
#define LEFT 0
#define RIGHT 8



int scanner();
void add_to_buffer(char **buffer, int *buff_size, char c);
void send_buffer(token_type type, char **buffer);
void send_char(token_type type, char c);
void correct_token (token_t *token);

#endif
