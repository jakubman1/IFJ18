/**
 * @file strbuff.c
 * @author Jakub Man
 * @date 13.11. 2018
 * @brief Dynamic string buffer
 */
#include "strbuff.h"

int init_buffer(tBuffer *buffer) {
  if(*buffer == NULL) {
    *buffer = calloc(DEFAULT_BUFFER_SIZE, sizeof(char)); // orig buff_size + 1

    if(*buffer == NULL) {
      fprintf(stderr, ANSI_COLOR_RED "Internal Error:" ANSI_COLOR_RESET " Not enough memory\n");
      exit(INTERNAL_ERR);
    }
    return DEFAULT_BUFFER_SIZE;
  }
  else {
    return strlen(*buffer);
  }
}

void add_to_buffer(tBuffer *buffer, int *buff_size, int c)
{
  int len = strlen(*buffer);

  if (len + 1 >= *buff_size) { // extend buffer
    *buff_size *= 2;
    *buffer = (char*) realloc(*buffer, *buff_size);

    if(*buffer == NULL) {
      fprintf(stderr, "NO MEMORY LEFT!");
      exit(99);
    }
  }
  if(c != EOF) {
    char new[2] = {(char)c, 0};
    strcat(*buffer, new);
  }

}
