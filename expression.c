/**
 * @file expression.c
 * @author Jiri Tykva
 * @author Jakub Man
 * @author Adam Melichar
 * @author Jan Martinák
 * @date 24.11. 2018
 * @brief Implementation of "expression"
 */

#include "expression.h"

/*
*     // TODO
*     POKUD V LL_PREDICT FUNKCI DOJDE then, do NEBO eol TAK MUSIME EXPRESSION ZAVOLAT JESTE JEDNOU S NEJAKYM
*     SPECIALNIM SYMBOLEM KTERY BUDE ZNACIT, ZE JE KONEC EXPRESSIONU
*
*/

int prec_table (tToken *token)
{
  int result = SUCCESS; // TODO
  int prec_table_out = SUCCESS;
  tToken currentToken = {"", ERROR};

  static tStack stack_rules; // stores postfix notation
  static tStack stack_pushdown; // auxiliary stack
  static bool init = false;

  if (!init)
  {
    s_init(&stack_rules);
    s_init(&stack_pushdown);

    s_push(&stack_pushdown, P_BOTTOM);

    init = true;
  }

  int token_input;
  if (token->type == OPERATOR && token->text[0] == '+') {
    token_input = 0;
  }
  else if (token->type == OPERATOR && token->text[0] == '-') {
    token_input = 1;
    //TODO
  }
  else if (token->type == OPERATOR && token->text[0] == '*') {
    token_input = 2;
  }
  else if (token->type == OPERATOR && token->text[0] == '/') {
    token_input = 3;
  }
  else if (token->type == OPERATOR && token->text[0] == '<') {
    token_input = 4;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "<=") == 0)) {
    token_input = 5;
  }
  else if (token->type == OPERATOR && token->text[0] == '>') {
    token_input = 6;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, ">=") == 0)) {
    token_input = 7;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "==") == 0)) {
    token_input = 8;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "!=") == 0)) {
    token_input = 9;
  }
  else if (token->type == OPERATOR && token->text[0] == '(') {
    token_input = 10;
  }
  else if (token->type == OPERATOR && token->text[0] == ')') {
    token_input = 11;
  }
  else if (token->type == ID || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING) { // i
    token_input = 12;
  }
  else { // end of expression (== $)
    token_input = 13;
  }



  switch (precedent_table[s_top(&stack_pushdown)][token_input])
  {
    case E: // =
      break;
    case R: // >
      break;
    case S: // <
      break;
  }
}
