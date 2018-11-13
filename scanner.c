#include "scanner.h"

int scanner()
{
  int c = '\0';

  scanner_state state = START;
  int retcode = SUCCESS;



  int buff_size = DEFAULT_BUFFER_SIZE;
  tBuffer buffer = calloc(buff_size, sizeof(char)); // orig buff_size + 1

  if(buffer == NULL) {
    fprintf(stderr, "Internal Error: Not enough memory\n");
    exit(INTERNAL_ERR);
  }

  while((c = getc(stdin)) != EOF) {
    switch(state) {
      case ERR:
        send_char(ERROR, c);
        state = START;
        break;
      case START:
        if (c == '\n') {
          state = NEW_LINE;
        }
        else if (c == '!') {
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
      case NEW_LINE:
        if (c == '=') {

          /* Jirka */
          /*const char *comment = "begin";
          char myString[6] = {0,}; // strlen(comment) + 1
          int i = 0;

          for (i; i < strlen(comment); i++) {
            c = getc(stdin);

            if (c == 'b' || c == 'e' || c == 'g' || c == 'i' || c == 'n') {
              myString[i] = c;
            }
            else {
              // i == position where it went wrong
              for (int j = 0; j < i; j++) { // correction
                ungetc(myString[j], stdin);
              }
              break;
            }
          }
          if (strcmp(myString, comment)) {
            state = MULTILINE_COMMENT;
          }
          else {
            state = SET;
          }
*/


          /* Adam */

          int i = 0;
          do {
            i++;
            c = getc(stdin);
          } while ((c == 'b' && i == 1) || (c == 'e' && i == 2) || (c == 'g' && i == 3) || (c == 'i' && i == 4));

          if (i == 5 && c == 'n') {
            state = MULTILINE_COMMENT;
          }
          else if (i == 1) {
            ungetc(c, stdin);
          }
          else if (i == 2) {
            ungetc('b', stdin);
            ungetc(c, stdin);
          }
          else if (i == 3) {
            ungetc('b', stdin);
            ungetc('e', stdin);
            ungetc(c, stdin);
          }
          else if (i == 4) {
            ungetc('b', stdin);
            ungetc('e', stdin);
            ungetc('g', stdin);
            ungetc(c, stdin);
          }
          else if (i == 5) {
            ungetc('b', stdin);
            ungetc('e', stdin);
            ungetc('g', stdin);
            ungetc('i', stdin);
            ungetc(c, stdin);
          }

        }
        else {
          ungetc(c, stdin);
          state = START;
        }
      break;
      case COMMENT:
        if(c == '\n') {
          // FIXME: Not working on all architectures
          ungetc(c, stdin);
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
          send_buffer(INTEGER, &buffer);
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
      case MULTILINE_COMMENT:
          //fprintf (stderr, "In Multiline Comment\n");
          if(c == '\n') {
            // detect =end
            if((c = getc(stdin)) == '=') {
              if((c = getc(stdin)) == 'e') {
                if((c = getc(stdin)) == 'n') {
                  if((c = getc(stdin)) == 'd') {
                    state = START;
                  }
                }
              }
            }
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

void correct_token (tToken *token)
{
  const char *key_words[] = {"def", "do", "else", "end", "if", "nil", "not", "then", "while"};
  int l = LEFT;
  int r = RIGHT;

  while (l <= r) {
    int i = (l + r) / 2; // MIDDLE

    if (strcmp(token->text, key_words[i]) < 0) { // str1 < str2
      r = i - 1;
    }
    else if (strcmp(token->text, key_words[i]) > 0) { // str1 > str2
      l = i + 1;
    }
    else { // str1 == str2
      token->type = i;
      return;
    }
  }
    // not a keyword
}

/**     ADAM, JIRKA VERZE

@brief Function sends token.
@param type Token type
@param buffer Token text
*/
void send_buffer(token_type type, tBuffer *buffer)
{
  int len = strlen(*buffer);
  char *text = malloc(sizeof(char) * len + 1);

  if(text == NULL) {
    fprintf(stderr, "Internal Error: Not enough memory\n");
    exit(INTERNAL_ERR);
  }

  strncpy(text, *buffer, len + 1);
  tToken token = {text, type};

  if (type == 9) { // ID
    correct_token(&token);
  }

  (*buffer)[0] = 0;
  #ifdef DEBUG
  printf("DEBUG: Added to buffer: %s, %d\n", token.text, token.type);
  #endif
  free(text);
}


void send_char(token_type type, char c) {
  char text[2] = {0,};
  text[0] = c;
  tToken token = {text, type};
  #ifdef DEBUG
  printf("DEBUG: Added to buffer: %s, %d\n", token.text, token.type);
  #endif
}
