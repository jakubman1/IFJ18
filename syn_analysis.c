/**
 * @file syn_analysis.c
 * @author Jiri Tykva
 * @author Jakub Man
 * @author Adam Melichar
 * @author Jan Martinák
 * @date 19.11. 2018
 * @brief Syntactic analysis file
 */

#include "syn_analysis.h"

int parser()
{
  int result = SUCCESS;
  int scanner_out = SUCCESS;
  tToken currentToken = {"", ERROR};
  tStack stack;
  s_init(&stack);

  s_push(&stack, LL_BOTTOM);
  s_push(&stack, LL_PROG);
  while((scanner_out = scanner(&currentToken)) == SUCCESS) {
    // create derivation tree
    // currentToken contains new token in every iteration

    result = ll_predict(&currentToken, &stack);

    //// MUSI BYT AZ NA KONCI CYKLU !!!!!!!!!!!!!!!
    if(currentToken.text != NULL) {
      // Free allocated text
      free(currentToken.text);
      currentToken.text = NULL;
    }
    if(result != 0) {
      // neco je spatne, a ona to vi
      break;
    }
  }
  if(scanner_out != -1) {
    // handle errors during lexical analysis
    result = scanner_out;
    if(scanner_out == LEXICAL_ERR) {
      while((scanner_out = scanner(&currentToken)) == SUCCESS || scanner_out == LEXICAL_ERR) {
        // Print all lexicals errors (scanner takes care of printing)
        if(currentToken.text != NULL) {
          // Free allocated text
          free(currentToken.text);
        }
      }
    }
    else {
      // Free allocated resources and quit (if there are any)
    }
  }
  else {
    // EOF
    int top = s_top(&stack);
    if (top == LL_BOTTOM)
      return SUCCESS;
    else {
      fprintf(stderr, "Syntax error\n");
      return SYNTAX_ERR;
    }

  }

  return result;
}



int ll_predict(tToken *token, tStack *stack)
{
  // following ifj pres:
  // X == top
  // a == token->type

  int top = s_top(stack);
  fprintf(stderr, "TOP IS : %d\n", top);

  // NON TERMINALS
while ((top = s_top(stack)) >= LL_PROG && top < LL_BOTTOM) {
  switch (s_top(stack)) {
    case LL_BOTTOM: // input cannot be EOF here
      return SYNTAX_ERR;
      break;
    case LL_PROG:
      if(token->type == DEF) {
        PUSH_RULE_1;
      }
      else if(token->type == ID || token->type == IF || token->type == WHILE || token->type == EOL) {
        PUSH_RULE_2;
      }
      // PUSH_RULE_3 handled before ll_predict
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_PARAMS:
      if(token->type == ID) {
        PUSH_RULE_5;
      }
      else if(token->type == EOL || (token->type == OPERATOR && token->text[0] == ')')) {
        PUSH_RULE_6;
      }
      else if(token->type == OPERATOR && token->text[0] == '(') {
        PUSH_RULE_4;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s. Expected EOL, ( or )\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_PARAMS_N:
      if(token->type == EOL || (token->type == OPERATOR && token->text[0] == ')')) {
        PUSH_RULE_8;
      }
      else if(token->type == COMMA) {
        PUSH_RULE_7;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s. Expected EOL, ) or ,\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_STATEMENT:
      if(token->type == ID) {
        PUSH_RULE_11;
      }
      else if(token->type == EOL || token->type == END || token->type == ELSE) {
        PUSH_RULE_12;
      }
      else if(token->type == IF) {
        PUSH_RULE_9;
      }
      else if(token->type == WHILE) {
        PUSH_RULE_10;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_DEF_ARGS:
      if(token->type == ID || token->type == EOL || token->type == END || (token->type == OPERATOR && token->text[0] == '(') || token->type == ELSE || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING ) {
        PUSH_RULE_14;
      }
      else if(token->type == OPERATOR && token->text[0] == '=') {
        PUSH_RULE_13;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_DEFINE:
      if(token->type == ID) {
        PUSH_RULE_15;
      }
      else if(token->type == EOL || token->type == END || token->type == ELSE) {
        PUSH_RULE_25;
      }
      else if(token->type == ID && (strcmp(token->text, "print") == 0)) {
        PUSH_RULE_16;
      }
      else if(token->type == ID && (strcmp(token->text, "inputs") == 0)) {
        PUSH_RULE_17;
      }
      else if(token->type == ID && (strcmp(token->text, "inputi") == 0)) {
        PUSH_RULE_18;
      }
      else if(token->type == ID && (strcmp(token->text, "inputf") == 0)) {
        PUSH_RULE_19;
      }
      else if(token->type == ID && (strcmp(token->text, "length") == 0)) {
        PUSH_RULE_20;
      }
      else if(token->type == ID && (strcmp(token->text, "substr") == 0)) {
        PUSH_RULE_21;
      }
      else if(token->type == ID && (strcmp(token->text, "ord") == 0)) {
        PUSH_RULE_22;
      }
      else if(token->type == ID && (strcmp(token->text, "chr") == 0)) {
        PUSH_RULE_23;
      }
      else if (token->type == INTEGER) {
        PUSH_RULE_24;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ARGS:
      if(token->type == ID || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING) {
        PUSH_RULE_28;
      }
      else if(token->type == EOL || token->type == END || token->type == ELSE || (token->type == OPERATOR && token->text[0] == ')')) {
        PUSH_RULE_26;
      }
      else if(token->type == OPERATOR && token->text[0] == '(') {
        PUSH_RULE_27;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ARGS_N:
      if (token->type == EOL || token->type == END || (token->type == OPERATOR && token->text[0] == ')') || token->type == ELSE ) {
        PUSH_RULE_30;
      }
      else if (token->type == COMMA) {
        PUSH_RULE_29;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_VALUE:
      if (token->type == ID) {
        PUSH_RULE_33;
      }
      else if (token->type == INTEGER) {
        PUSH_RULE_31;
      }
      else if (token->type == FLOATING_POINT) {
        PUSH_RULE_32;
      }
      else if (token->type == STRING) {
        PUSH_RULE_34;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_EXPRESSION: // FOR TESTING: EXPRESSION CAN ONLY BE SINGLE INT
      // TODO call precedent table
      // for testing purposes only
      if(token->type == INTEGER)
        if(!s_empty(stack)) {
          s_pop(stack); // discards EXPRESSION non terminal from stack
          s_push(stack, LL_INT); // ads INT terminal on top of the stack
        }
      break;
    default:
      // Handle syntax error
      fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" Total RIP :(\n");
      return SYNTAX_ERR;
      break;
    } // end switch non-terminals
  } // end while non-terminals

  // TERMINALS
  switch(s_top(stack)) {
    case LL_DEF:
      if (token->type == DEF) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s, expected def.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ID:
      if (token->type == ID) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_EOL:
      if (token->type == EOL) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s, expected EOL.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_END:
      if (token->type == END) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s, expected end.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_COMMA:
      if (token->type == COMMA) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected comma.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_IF:
      if (token->type == IF) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected if.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_THEN:
      if (token->type == THEN) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected then with no if.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_ELSE:
      if (token->type == ELSE) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected else with no if.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_WHILE:
      if (token->type == WHILE) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected while.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_DO:
      if (token->type == DO) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected do.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_EQUAL:
      if (token->type == OPERATOR && (token->text[0] == '=')) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" = unexpected.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_PRINT:
      if (token->type == ID && (strcmp(token->text, "print") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected print.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_INPUTS:
      if (token->type == ID && (strcmp(token->text, "inputs") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected inputs.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_INPUTI:
      if (token->type == ID && (strcmp(token->text, "inputi") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected inputi.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_INPUTF:
      if (token->type == ID && (strcmp(token->text, "inputf") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected inputf.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_LENGHT:
      if (token->type == ID && (strcmp(token->text, "lenght") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected length.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_SUBSTR:
      if (token->type == ID && (strcmp(token->text, "substr") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected substr.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_ORD:
      if (token->type == ID && strcmp(token->text, "ord") == 0) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected ord.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_CHR:
      if (token->type == ID && strcmp(token->text, "chr") == 0) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected chr.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_BRACKET_LEFT:
      if (token->type == OPERATOR && token->text[0] == '(') {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected (.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_BRACKET_RIGHT:
      if (token->type == OPERATOR && token->text[0] == ')') {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" imbalanced brackets.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_INT:
      if (token->type == INTEGER) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected integer.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_FLOAT:
      if (token->type == FLOATING_POINT) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected float.\n");
        return SYNTAX_ERR;
      }
      break;
    case LL_STRING:
      if (token->type == STRING) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected string.\n");
        return SYNTAX_ERR;
      }
      break;
    default:
      // Handle syntax error
      fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" Total RIP :(\n");
      return SYNTAX_ERR;
      break;
  } // end switch terminals

  // TODO insert into tree, derivation tree

  // Why can you drink a drink, but not food a food?
  return SUCCESS;
}
