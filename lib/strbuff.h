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

#define DEFAULT_BUFFER_SIZE 4 // Buffer size on init (default 4)
typedef char* tBuffer;

/**
@Brief adds character to dynamic char buffer. If lenght of a buffer is too small, size gets doubeled.
@param buffer Allocated memory for buffer
@param buff_size Pointer to int containing current buffer size. Gets updated automatically.
@param c Character to add to buffer.
 */
void add_to_buffer(tBuffer *buffer, int *buff_size, char c);


#endif
