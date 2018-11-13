void add_to_buffer(tBuffer *buffer, int *buff_size, char c)
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

  // fprintf(stderr, "Adding char %c to buffer\n", c);
}
