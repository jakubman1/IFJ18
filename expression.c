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

int stack_terminal_top(tStack *searched_stack, tStack *aux_stack)
{
  while (!s_empty(searched_stack) && s_top(searched_stack) > P_BOTTOM) // while non terminal on top
  {
    s_push(aux_stack, s_pop(searched_stack));
  }

  return s_top(searched_stack);
}

void furfil_stack(tStack *searched_stack, tStack *aux_stack)
{
    while (!s_empty(aux_stack))
    {
      s_push(searched_stack, s_pop(aux_stack));
    }
}


int prec_table(tToken *token)
{
  int result = SUCCESS; // TODO
  //int prec_table_out = SUCCESS;
  //tToken currentToken = {"", ERROR};

  static tStack stack_rules; // stores postfix notation
  static tStack stack_pushdown; // auxiliary stack
  static tStack stack_temp; // auxiliary stack for searching for first terminal
  static tTStack token_stack; // stores text and type of the tokens until the end of the expression
  static bool init = false;

  if (!init)
  {
    s_init(&stack_rules);
    s_init(&stack_pushdown);
    s_init(&stack_temp);
    st_init(&token_stack);

    s_push(&stack_pushdown, P_BOTTOM);

    // s_push(&stack_tmp, s_pop(&druhy_stack));

    init = true;
  }

/*
  // $ vs $ || $E vs $ --> success
  if (token->type == LL_BOTTOM) {
    if (s_top(&stack_pushdown) == P_E) {
      s_pop(&stack_pushdown);
    }
    if (s_top(&stack_pushdown) == P_BOTTOM) {
      // TODO
      //if (!s_empty(&stack_rules) {
      //  send pravy rozbor do ASS
      //}
      return SUCCESS;
    }
    else {
      return SYNTAX_ERR;
    }
  }
*/

  int token_input;
  if (token->type == OPERATOR && token->text[0] == '+') {
    token_input = P_PLUS;
  }
  else if (token->type == OPERATOR && token->text[0] == '-') {
    token_input = P_MINUS;
    //TODO
  }
  else if (token->type == OPERATOR && token->text[0] == '*') {
    token_input = P_MULTIPLY;
  }
  else if (token->type == OPERATOR && token->text[0] == '/') {
    token_input = P_DIVIDE;
  }
  else if (token->type == OPERATOR && token->text[0] == '<') {
    token_input = P_LOWER;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "<=") == 0)) {
    token_input = P_LOWEREQ;
  }
  else if (token->type == OPERATOR && token->text[0] == '>') {
    token_input = P_GREATER;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, ">=") == 0)) {
    token_input = P_GREATEREQ;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "==") == 0)) {
    token_input = P_EQ;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "!=") == 0)) {
    token_input = P_NOTEQ;
  }
  else if (token->type == OPERATOR && token->text[0] == '(') {
    token_input = P_LEFT_BRACKET;
  }
  else if (token->type == OPERATOR && token->text[0] == ')') {
    token_input = P_RIGHT_BRACKET;
  }
  else if (token->type == ID || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING) { // i
    token_input = P_ID;
  }
  else { // end of expression (== $)
    token_input = P_BOTTOM;
  }

  int top_terminal = stack_terminal_top(&stack_pushdown, &stack_temp);
  furfil_stack (&stack_pushdown, &stack_temp);


  switch (precedent_table[top_terminal][token_input])
  {
    case E: // =
      s_push(&stack_pushdown, token_input);
      break;
    case S: // <
      top_terminal = stack_terminal_top(&stack_pushdown, &stack_temp);
      s_push(&stack_pushdown, S);
      furfil_stack(&stack_pushdown, &stack_temp);
      s_push(&stack_pushdown, token_input);
      break;
    case R: // >
      top_terminal = stack_terminal_top(&stack_pushdown, &stack_temp);
      int second_top_terminal = stack_terminal_top(&stack_pushdown, &stack_temp); // should be S
      if (second_top_terminal == S) {
        s_pop(&stack_pushdown); // get rid of < (== S)

        bool successful_rules [NUMBER_OF_RULES];
        for (int i = 0; i < NUMBER_OF_RULES; i++) {
          successful_rules[i] = true;
        }

        // compare rules
        for (int j = 0; !s_empty(&stack_temp); j++) { // j MAX 3
          int rule_element = s_pop(&stack_temp);
          for (int i = 0; i < NUMBER_OF_RULES; i++) { // i MAX 12
            if (successful_rules[i] == true) {
              if (rule_table[i][j] != rule_element) {
                successful_rules[i] = false;
              }
            }
          }
        }

        int rule = -1;
        for (int i = 0; i < NUMBER_OF_RULES; i++)
        {
          if (successful_rules[i] == true) {
            rule = i;
          }
        }

        if (rule == -1) {
          return SYNTAX_ERR;
        }
        else {
          s_push(&stack_rules, rule); // Right parse
          s_push(&stack_pushdown, P_E);
        }

      }
      else {
        return SYNTAX_ERR;
      }
      break; // end of Case R
    default:  // NONE
      return SYNTAX_ERR;
      break;
  }

  if (token_input == P_ID) {
    st_push(&token_stack, *token);
  }

  /*TODO: if (konec) {
  s_free(&stack_pushdown);
  s_free(&stack_temp);
  s_free(&stack_rules);
  st_free(&token_stack);
  }*/
  return result;
}
