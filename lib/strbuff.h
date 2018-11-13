/**
 * @file strbuff.h
 * @author Jakub Man
 * @date 13.11. 2018
 * @brief Dynamic string buffer
 */

#ifndef STRBUFF_H
#define STRBUFF_H

#define DEFAULT_BUFFER_SIZE 4 // Buffer size on init (default 4)
typedef char* tBuffer;

void add_to_buffer(tBuffer *buffer, int *buff_size, char c);


#endif
