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



/*const int rule_table[NUMBER_OF_RULES][MAX_RULE_LENGHT] =
{
  {P_E, P_PLUS, P_E},                       // E → E + E    0
  {P_E, P_MINUS, P_E},                      // E → E - E    1
  {P_E, P_MULTIPLY, P_E},                   // E → E * E    2
  {P_E, P_DIVIDE, P_E},                     // E → E / E    3
  {P_E, P_LOWER, P_E},                      // E → E < E    4
  {P_E, P_LOWEREQ, P_E},                    // E → E <= E   5
  {P_E, P_GREATER, P_E},                    // E → E > E    6
  {P_E, P_GREATEREQ, P_E},                  // E → E >= E   7
  {P_E, P_EQ, P_E},                         // E → E == E   8
  {P_E, P_NOTEQ, P_E},                      // E → E != E   9
  {P_LEFT_BRACKET, P_E, P_RIGHT_BRACKET},   // E → (E)      10
  {P_ID, -1, -1}                            // E → i        11
};*/

/*
*     ASS se da sestrojit z praveho rozboru
*/

int stack_terminal_top(tStack *searched_stack, tStack *aux_stack)
{
  // pop from searched stack
  while (!s_empty(searched_stack) && s_top(searched_stack) == P_E) // while non terminal on top
  {
    s_push(aux_stack, s_pop(searched_stack));
  }

  int top = s_top(searched_stack);

  // fill back again the searched stack
  while (!s_empty(aux_stack))
  {
    s_push(searched_stack, s_pop(aux_stack));
  }

  return top;
}

int evaluate_rule (tStack *stack_temp, tStack *stack_pushdown, tStack *stack_rules)
{
  int tmp = s_pop(stack_temp);

  // rule No. 11
  if ( tmp == P_ID ) { // i
    if ( s_empty(stack_temp) ) {
      s_push(stack_rules, 11); // Right parse
      s_push(stack_pushdown, P_E);
      return 11;
    }
  }

  // rule No. 10
  else if (tmp == P_LEFT_BRACKET) {
    if (s_pop(stack_temp) == P_E) {
      if (s_pop(stack_temp) == P_RIGHT_BRACKET) {
        if (s_empty(stack_temp)) {
          s_push(stack_rules, 10); // Right parse
          s_push(stack_pushdown, P_E);
          return 10;
        }
      }
    }
  }

  // rule No. 0 - 9
  else if(tmp == P_E) {
    switch(s_pop(stack_temp)) {
      case P_PLUS:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 0); // Right parse
            s_push(stack_pushdown, P_E);
            return 0;
          }
        }
        break;
      case P_MINUS:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 1); // Right parse
            s_push(stack_pushdown, P_E);
            return 1;
          }
        }
        break;
      case P_MULTIPLY:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 2); // Right parse
            s_push(stack_pushdown, P_E);
            return 2;
          }
        }
        break;
      case P_DIVIDE:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 3); // Right parse
            s_push(stack_pushdown, P_E);
            return 3;
          }
        }
        break;
      case P_LOWER:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 4); // Right parse
            s_push(stack_pushdown, P_E);
            return 4;
          }
        }
        break;
      case P_LOWEREQ:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 5); // Right parse
            s_push(stack_pushdown, P_E);
            return 5;
          }
        }
        break;
      case P_GREATER:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 6); // Right parse
            s_push(stack_pushdown, P_E);
            return 6;
          }
        }
        break;
      case P_GREATEREQ:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 7); // Right parse
            s_push(stack_pushdown, P_E);
            return 7;
          }
        }
        break;
      case P_EQ:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 8); // Right parse
            s_push(stack_pushdown, P_E);
            return 8;
          }
        }
        break;
      case P_NOTEQ:
        if (s_pop(stack_temp) == P_E) {
          if (s_empty(stack_temp)) {
            s_push(stack_rules, 9); // Right parse
            s_push(stack_pushdown, P_E);
            return 9;
          }
        }
        break;
      default:
        return -2;
    }
  }
  return -2;
}

int prec_table(tToken *token, tSymPtr sym)
{
  static tStack stack_rules; // stores postfix notation
  static tStack stack_pushdown; // auxiliary stack
  static tStack stack_temp; // auxiliary stack for searching for first terminal
  static tTStack token_stack; // stores text and type of the tokens until the end of the expression
  static bool init = false;

  const int precedent_table[PRECEDENT_TABLE_SIZE][PRECEDENT_TABLE_SIZE] =
  {
    //+ |- |* |/ |< |<=|> |>=|==|!=|( |) |i |$ |          token_input -->
     {R, R, S, S, R, R, R, R, R, R, S, R, S, R}, // +     stack_pushdown
     {R, R, S, S, R, R, R, R, R, R, S, R, S, R}, // -      |
     {R, R, R, R, R, R, R, R, R, R, S, R, S, R}, // *      V
     {R, R, R, R, R, R, R, R, R, R, S, R, S, R}, // /
     {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // <
     {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // <=
     {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // >
     {S, S, S, S, N, N, N, N, R, R, S, R, S, R}, // >=
     {S, S, S, S, S, S, S, S, N, N, S, R, S, R}, // ==
     {S, S, S, S, S, S, S, S, N, N, S, R, S, R}, // !=
     {S, S, S, S, S, S, S, S, S, S, S, E, S, N}, // (
     {R, R, R, R, R, R, R, R, R, R, N, R, N, R}, // )
     {R, R, R, R, R, R, R, R, R, R, N, R, N, R}, // i
     {S, S, S, S, S, S, S, S, S, S, S, N, S, N}  // $
  };

  if (!init)
  {
    s_init(&stack_rules);
    s_init(&stack_pushdown);
    s_init(&stack_temp);
    st_init(&token_stack);

    s_push(&stack_pushdown, P_BOTTOM);

    init = true;
  }

  int token_input;

  if (token->type == OPERATOR && token->text[0] == '+') {
    token_input = P_PLUS;
  }
  else if (token->type == OPERATOR && token->text[0] == '-') {
    token_input = P_MINUS;
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
  else if (token->type == ID || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING || token->type == NIL) { // i
    token_input = P_ID;
  }
  else if (token->type == OPERATOR && (strcmp(token->text, "=") == 0)) {
    // TODO free resources
    return SYNTAX_ERR;
  }
  else { // end of expression (== $)
    token_input = P_BOTTOM;
  }

  int top_terminal = stack_terminal_top(&stack_pushdown, &stack_temp);

  switch (precedent_table[top_terminal][token_input])
  {
    case E: // =
      s_push(&stack_pushdown, token_input);
      break;
    case S: // <
      // find first top terminal
      while ( !s_empty(&stack_pushdown) && s_top(&stack_pushdown) == P_E ) {
        s_push(&stack_temp, s_pop(&stack_pushdown));
      }
      s_push(&stack_pushdown, S); // change terminal to terminal<
      while ( !s_empty(&stack_temp)) { // fill back again stack_pushdown
        s_push(&stack_pushdown, s_pop(&stack_temp));
      }
      s_push(&stack_pushdown, token_input); // put token on top
      break;
    case R: // >
      while ( s_top(&stack_pushdown) != S && s_top(&stack_pushdown) != P_BOTTOM ) {
        s_push(&stack_temp, s_pop(&stack_pushdown));
      }

      if (s_top(&stack_pushdown) == S) {
        s_pop(&stack_pushdown);

        if ( evaluate_rule(&stack_temp, &stack_pushdown, &stack_rules) == -2 ) {
          return SYNTAX_ERR;
        }
        else {
          // po vyhodnoceni pravidla se vola se stejnym tokenem
          if (token_input != P_BOTTOM || (stack_terminal_top(&stack_pushdown, &stack_temp) != P_BOTTOM)) {
            if (prec_table(token, sym) == SYNTAX_ERR) {
              return SYNTAX_ERR;
            }
          }
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
    char *copyText = malloc(sizeof(char) * (strlen(token->text) + 1));
    if (copyText == NULL) {
      return INTERNAL_ERR;
    }
    strcpy(copyText, token->text);

    tToken copyToken = {copyText, token->type};
    st_push(&token_stack, copyToken);
  }

  // END OF EXPRESSION
  if (token_input == P_BOTTOM && s_top(&stack_pushdown) != P_BOTTOM) {
    s_pop(&stack_pushdown); // get rid of E, stack is now ready for new expression


    /**********************
         GENERATING ASS
    **********************/
    // right parse in STACK_RULES
    // data in TOKEN_STACK
    // we know the types of every token here in expression

    //
    tAssPtr top_ass = NULL;
    tAssPtr current_ass = NULL; // ass_make_tree()
    tAssPtr new_ass = NULL; // ass_make_leaf()

    while (!s_empty(&stack_rules)) {
      int top_rule = s_pop(&stack_rules);
      tToken top_token = {"",ERROR};
      if (top_rule == 11) {
        top_token = st_pop(&token_stack);
      }

      switch (top_rule) {
        case 0: // E → E + E
        case 1: // E → E - E
        case 2: // E → E * E
        case 3: // E → E / E
        case 4: // E → E < E
        case 5: // E → E <= E
        case 6: // E → E > E
        case 7: // E → E >= E
        case 8: // E → E == E
        case 9: // E → E != E

          switch (top_rule) {
            case 0: // E → E + E
              new_ass = ass_make_tree(OPERATOR, "+", NULL, NULL, NULL);
              break;
            case 1: // E → E - E
              new_ass = ass_make_tree(OPERATOR, "-", NULL, NULL, NULL);
              break;
            case 2: // E → E * E
              new_ass = ass_make_tree(OPERATOR, "*", NULL, NULL, NULL);
              break;
            case 3: // E → E / E
              new_ass = ass_make_tree(OPERATOR, "/", NULL, NULL, NULL);
              break;
            case 4: // E → E < E
              new_ass = ass_make_tree(OPERATOR, "<", NULL, NULL, NULL);
              break;
            case 5: // E → E <= E
              new_ass = ass_make_tree(OPERATOR, "<=", NULL, NULL, NULL);
              break;
            case 6: // E → E > E
              new_ass = ass_make_tree(OPERATOR, ">", NULL, NULL, NULL);
              break;
            case 7: // E → E >= E
              new_ass = ass_make_tree(OPERATOR, ">=", NULL, NULL, NULL);
              break;
            case 8: // E → E == E
              new_ass = ass_make_tree(OPERATOR, "==", NULL, NULL, NULL);
              break;
            case 9: // E → E != E
              new_ass = ass_make_tree(OPERATOR, "!=", NULL, NULL, NULL);
              break;
          } // end of 2. switch

          if (new_ass == NULL) {
            // TODO uvolnit stacky
            return INTERNAL_ERR;
          }

          if (top_ass == NULL) { // very first node inserted
            top_ass = new_ass;
            current_ass = new_ass;
          }
          else if (current_ass->rptr == NULL) {
              current_ass->rptr = new_ass;
          }
          else if (current_ass->lptr == NULL) {
            current_ass->lptr = new_ass;
          }
          current_ass = new_ass;
          break;

        case 10: // E → (E)
          break;
        case 11: // E → i
          if (top_token.type == ID) {
            new_ass = ass_make_leaf(top_token.type, top_token.text, sym);
          }
          else { // token->type == INTEGER || FLOATING_POINT || STRING
            new_ass = ass_make_leaf(top_token.type, top_token.text, NULL);
          }
          if (new_ass == NULL) {
            // TODO uvolnit stacky
            return INTERNAL_ERR;
          }

          if (top_ass == NULL) { // very first node inserted
            top_ass = new_ass;
            current_ass = new_ass;
          }
          else if (current_ass->rptr == NULL) {
            current_ass->rptr = new_ass;
          }
          else if (current_ass->lptr == NULL) {
            current_ass->lptr = new_ass;
            // move to upper node where lptr is NULL
            current_ass = ass_find_father(top_ass, current_ass);
          }
          break;
        default:

          break;
      } // end of 1. switch
    } // end of while

    /*  SCITANI, ODCITANI, NASOBENI, DELENI, KONKATENACE
        int , int --> int
        int , float --> float
        float , float --> float
        string , string --> string
        ------------------------
        int / 0 --> ERROR
        float / 0 --> ERROR

        POROVNAVANI (<, >, =<, >=, ==, !=)
        int , int --> true/false
        float , float --> true/false
        string , string --> true/false
        int2float(int) , float --> true/false
        int == float/string/nil --> false
        int != float/string/nil --> false
        float == int/string/nil --> false
        float != int/string/nil --> false
        string == int/float/nil --> false
        string != int/float/nil --> false
        nil == int/float/string --> false
        nil != int/float/string --> false
    */

    // VYPIS ASS
  /*  fprintf(stderr, "ASS: ");
    in_order(top_ass);
    fprintf(stderr, "\n");*/

    // VYPIS PRAVY ROZBOR
    /*fprintf(stderr, "PRAVY ROZBOR\n");
    while (!s_empty(&stack_rules)) {
      fprintf(stderr, "%d, ", s_pop(&stack_rules));
          fprintf(stderr, "\n");
    }*/
  } // end of if

  /*TODO: if (konec) {
  s_free(&stack_pushdown);
  s_free(&stack_temp);
  s_free(&stack_rules);
  st_free(&token_stack);
  }*/
  return SUCCESS;
}
