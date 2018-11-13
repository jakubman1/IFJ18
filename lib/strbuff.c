/**
 * @file strbuff.c
 * @author Jakub Man
 * @date 13.11. 2018
 * @brief Dynamic string buffer
 */
#include "strbuff.h"

/** ADAM, JIRKA, HONZA VERZE
@Brief Adds char to the buffer
@param buffer Destination
@param buff_size Current size of the buffer
@param c Char to add
*/
void add_to_buffer(tBuffer *buffer, int *buff_size, int c)
{
  int len = strlen(*buffer);
  // Test print
  fprintf(stderr, "LEN = %d\n", len);

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
