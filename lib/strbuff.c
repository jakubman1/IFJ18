/**
 * @file strbuff.c
 * @author Jakub Man
 * @date 13.11. 2018
 * @brief Dynamic string buffer
 */
#include "strbuff.h"

/*void add_to_buffer(tBuffer *buffer, int *buff_size, char c)
{
  int len = strlen(*buffer);
  fprintf(stderr, "LEN: %d\n", len);
  if((len + 2) >= *buff_size) {
    *buff_size = *buff_size * 2;
    fprintf(stderr, "Incresing buffer\n");
    realloc(*buffer, sizeof(char) * (*buff_size));
    if(*buffer == NULL) {
      fprintf(stderr, "NO MEMORY LEFT!");
      exit(99);
    }
    fprintf(stderr, "New buffer size: %d\n", *buff_size);
  }
  fprintf(stderr, "Adding to position %d\n", len);
  *((*buffer) + len) = c;
  *((*buffer) + len + 1) = '\0';
}*/

/*
void add_to_buffer(tBuffer *buffer, int *buff_size, char c)
{
  int len = strlen(*buffer);
  fprintf(stderr, "LEN: %d\n", len);
  if(len >= (*buff_size - 2))
  {
    // Change buffer size
    *buff_size *= 2;
    realloc(*buffer, sizeof(char) * (*buff_size));
    if(*buffer == NULL)
    {
      fprintf(stderr, "NO MEMORY LEFT!");
      exit(99);
    }
    fprintf(stderr, "New buffer size: %d\n", *buff_size);
  }
  // Add new character to the end of buffer
  fprintf(stderr, "Adding to position %d\n", len);
  *((*buffer) + len) = c;
  *((*buffer) + len + 1) = '\0';

}
*/

/** ADAM, JIRKA, HONZA VERZE
@Brief Adds char to the buffer
@param buffer Destination
@param buff_size Current size of the buffer
@param c Char to add
*/
void add_to_buffer(tBuffer *buffer, int *buff_size, char c)
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
  strcat(*buffer, &c);
}
