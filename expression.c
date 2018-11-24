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
*
*     ASS se da sestrojit z praveho rozboru
*
*
*/
// find first terminal closest to the top of the stack
int stack_terminal_top (tStack *searched_stack, tStack *aux_stack)
{
  while (!s_empty(searched_stack) && s_top(searched_stack) > BOTTOM) // while non terminal on top
  {
    s_push(aux_stack, s_pop(searched_stack));
  }

  int top = s_top(searched_stack);

  while (!s_empty(aux_stack))
  {
    s_push(searched_stack, s_pop(aux_stack));
  }

  return top;
}

int prec_table (tToken *token)
{
  int result = SUCCESS; // TODO
  int prec_table_out = SUCCESS;
  tToken currentToken = {"", ERROR};

  static tStack stack_rules; // stores postfix notation
  static tStack stack_pushdown; // auxiliary stack
  static tStack stack_temp; // auxiliary stack for searching for first terminal
  static tPStack token_stack; // stores text and type of the tokens until the end of the expression
  static bool init = false;

  if (!init)
  {
    s_init(&stack_rules);
    s_init(&stack_pushdown);
    s_init(&stack_temp);
    sp_init(&token_stack);

    s_push(&stack_pushdown, P_BOTTOM);

    // s_push(&stack_tmp, s_pop(&druhy_stack));

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

  int top_terminal = stack_terminal_top(&stack_pushdown, &stack_temp);

  switch (precedent_table[top_terminal][token_input])
  {
    case E: // =
      s_push(&stack_pushdown, token_input);
      sp_push(&token_stack, (void *) token);
      break;
    case S: // <

      break;
    case R: // >
      break;
  }

  /*TODO: if (konec) {
  s_free(&stack_pushdown);
  s_free(&stack_temp);
  s_free(&stack_rules);
  sp_free(&token_stack);
  }*/
}
