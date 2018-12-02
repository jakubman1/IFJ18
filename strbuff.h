/**
 * @file strbuff.h
 * @author Jakub Man
 * @date 13.11. 2018
 * @brief Dynamic string buffer
 */

#ifndef STRBUFF_H
#define STRBUFF_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "errcodes.h"

#define DEFAULT_BUFFER_SIZE 4 // Buffer size on init (default 4)

typedef char* tBuffer;

/**
Init a new buffer with default buffer size of DEFAULT_BUFFER_SIZE.
If param buffer is NULL, a new buffer is created. Otherwise length of string in buffer is returned.
@param buffer Buffer variable to insert new buffer into. Should be NULL when calling this function
@returns int Length of a new buffer.
*/
int init_buffer(tBuffer *buffer);

/**
@Brief adds character to dynamic char buffer. If lenght of a buffer is too small, size gets doubeled.
@param buffer Allocated memory for buffer
@param buff_size Pointer to int containing current buffer size. Gets updated automatically.
@param c Character to add to buffer.
 */
void add_to_buffer(tBuffer *buffer, int *buff_size, int c);


#endif
