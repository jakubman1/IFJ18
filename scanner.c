#include "scanner.h"



int scanner()
{
  // ungetc(c, stdin);
  int c;
  scanner_state state = START;
  int retcode = SUCCESS;
  int buff_size = DEFAULT_BUFFER_SIZE;
  char *buffer = malloc(sizeof(char) * buff_size);
  if(buffer == NULL) {
    return INTERNAL_ERR;
  }

/*
  add_to_buffer(...);
  send_buffer(...);
  ungetc(...);
  state = ...;
*/

  while((c = getc(stdin)) != EOF) {
    switch(state) {
      case ERR:
        fprintf(stderr, "ACHTUNG, SAJRAJT!!!!\n");
        state = START;
        break;
      case START:
        if (c == '!') {
          add_to_buffer(&buffer, &buff_size, c);
          state = EXCLAMATION_POINT;
        }
        else if (c == '>') {
          add_to_buffer(&buffer, &buff_size, c);
          state = GREATER;
        }
        else if (c == '<') {
          add_to_buffer(&buffer, &buff_size, c);
          state = LESS;
        }
        else if (c == '=') {
          add_to_buffer(&buffer, &buff_size, c);
          state = SET;
        }
        else if (c == ')' || c == '(' || c == '/' || c == '*' || c == '-' || c == '+') {
          send_char(OPERATOR, c);
          state = START;
        }
        else if (c == '#') {
          state = COMMENT;
        }
        else if (c == '_' || (c >= 'a' && c <= 'z')) {
          add_to_buffer(&buffer, &buff_size, c);
          state = IDENT;
        }
        else if (c == '"') {
          add_to_buffer(&buffer, &buff_size, c);
          state = STRING_START;
        }
        else if (isdigit(c) && c != '0') { // c == 1 to 9
          add_to_buffer(&buffer, &buff_size, c);
          state = INT;
        }
        else if (c == '0') {
          add_to_buffer(&buffer, &buff_size, c);
          state = ZERO;
        }
        break;
      case COMMENT:
        if(c == '\n') {
          // FIXME: Not working on all architectures
          state = START;
        }
        break;
      case IDENT:
        if (isalnum(c) || c == '_') {
          add_to_buffer(&buffer, &buff_size, c);
          state = IDENT;
        }
        else if (c == '!' || c == '?') {
          add_to_buffer(&buffer, &buff_size, c);
          send_buffer(ID, &buffer);
          state = START;
        }
        else {
          send_buffer(ID, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case STRING_START:
        if (c != '"' && c != '\n') {
          add_to_buffer(&buffer, &buff_size, c);
          state = STRING_START;
        }
        else if (c == '\\') {
          add_to_buffer(&buffer, &buff_size, c);
          state = STRING_ESCAPING;
        }
        else { // c == '"'  ||  c == '\n'
          add_to_buffer(&buffer, &buff_size, c);
          send_buffer(STRING, &buffer);
          state = START;
        }
        break;
      case STRING_ESCAPING:
        if (c != '"' && c != '\n') {
          add_to_buffer(&buffer, &buff_size, c);
          state = STRING_START;
        }
        else { // c == '"'
          add_to_buffer(&buffer, &buff_size, c);
          send_buffer(STRING, &buffer);
          state = START;
        }
        break;
      case INT:
        if (c >= '0' && c <= '9') {
          add_to_buffer(&buffer, &buff_size, c);
          state = INT;
        }
        else if (c == '.') {
          add_to_buffer(&buffer, &buff_size, c);
          state = FLOAT;
        }
        else if (c == 'e' || c == 'E') {
          add_to_buffer(&buffer, &buff_size, c);
          state = FLOAT_E;
        }
        else {
          send_buffer(INTEGER, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case FLOAT:
        if ((c >= '0' && c <= '9') || c == '.') { //
          add_to_buffer(&buffer, &buff_size, c);
          state = FLOAT;
        }
        else if (c == 'e' || c == 'E') {
          add_to_buffer(&buffer, &buff_size, c);
          state = FLOAT_E;
        }
        else {
          send_buffer(FLOATING_POINT, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case FLOAT_E:
        if((c >= '0' && c <= '9') || c == '.' || c == '+' || c == '-') {
          add_to_buffer(&buffer, &buff_size, c);
          state = FLOAT;
        }
        else if(c == 'e' || c == 'E') {
          add_to_buffer(&buffer, &buff_size, c);
          state = FLOAT_E;
        }
        else {
          send_buffer(FLOATING_POINT, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case ZERO:
        if (c == 'b') {
          add_to_buffer(&buffer, &buff_size, c);
          state = BIN;
        }
        else if (c == 'x') {
          add_to_buffer(&buffer, &buff_size, c);
          state = HEX;
        }
        else if (c >= '0' && c <= '7') {
          add_to_buffer(&buffer, &buff_size, c);
          state = OCT;
        }
        else { // for 09 sends two INT tokens 0 and 9
          send_buffer(INT, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case BIN:
        if (c == '1' || c == '0') {
          add_to_buffer(&buffer, &buff_size, c);
          state = BIN;
        }
        else {
          send_buffer(BINARY, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case HEX:
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) { // c == 0 to 9, A to F
          add_to_buffer(&buffer, &buff_size, c);
          state = HEX;
        }
        else {
          send_buffer(HEX, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case OCT:
        if (c >= '0' && c <= '7') {
          add_to_buffer(&buffer, &buff_size, c);
          state = OCT;
        }
        else {
          send_buffer(OCT, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case EXCLAMATION_POINT:
        if (c == '=') { // !=
          add_to_buffer(&buffer, &buff_size, c);
          send_buffer(OPERATOR, &buffer);
          state = START;
        }
        else { // !
          send_buffer(ERROR, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case GREATER:
        if (c == '=') { // >=
          add_to_buffer(&buffer, &buff_size, c);
          send_buffer(OPERATOR, &buffer);
          state = START;
        }
        else { // >
          send_buffer(OPERATOR, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case LESS:
        if (c == '=') { // <=
          add_to_buffer(&buffer, &buff_size, c);
          send_buffer(OPERATOR, &buffer);
          state = START;
        }
        else { // <
          send_buffer(OPERATOR, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      case SET:
        if (c == '=') // ==
        {
          add_to_buffer(&buffer, &buff_size, c);
          send_buffer(OPERATOR, &buffer);
          state = START;
        }
        else { // =
          send_buffer(OPERATOR, &buffer);
          ungetc(c, stdin);
          state = START;
        }
        break;
      default:
        state = ERR;
        break;
    }
  }
  free(buffer);
  return retcode;
}

void add_to_buffer(char **buffer, int *buff_size, char c)
{
  int len = strlen(*buffer);
  //fprintf(stderr, "LEN: %d\n", len);
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
  *(*buffer + len) = c;
  *(*buffer + len + 1) = '\0';

  // fprintf(stderr, "Adding char %c to buffer\n", c);
}

void send_buffer(token_type type, char **buffer)
{
  int buffer_size = strlen(*buffer);
  char *text = malloc(sizeof(char) * buffer_size + 1);
  strcpy(text, *buffer);
  token_t token = {text, type};
  // TODO send_to_syntactic_analysis(token);
  for(int i = 0; i < buffer_size; i++) {
    *(*buffer + i) = 0;
  }
  printf("DEBUG: Added to buffer: %s, %d\n", token.text, token.type);
  free(token.text); //TODO: REMOVE THIS!!!!
}

void send_char(token_type type, char c) {
  char text[2] = {0,};
  text[0] = c;
  token_t token = {text, type};
  printf("DEBUG: Added to buffer: %s, %d\n", token.text, token.type);
}

token_type replace_keyword(char *token_text) {
  // TODO: Vyhledavani ve stromu;
  return ID;
}
