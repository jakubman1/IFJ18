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

/* TODO PRACE SE SYMTABLE

  UKLADANI FUNKCE
1) kdykoliv narazime na DEF, mela by se inicializovat nova tabulka symbolu
    def id ( PARAMS ) eol STATEMENT_N end eol PROG
    ^   ^      ^                  ^
    0)  2)     3)                 4)

symtable_insert_function
0) define
1) root
2) name
3) paramCount, params
4) returnType1

*/

int parser()
{
  int result = SUCCESS;
  int scanner_out = SUCCESS;
  tToken currentToken = {"", ERROR};
  tStack stack;
  tSymPtr globalTree = NULL;

  char *idName = NULL;

  symtable_init(&globalTree);
  s_init(&stack);

  s_push(&stack, LL_BOTTOM);
  s_push(&stack, LL_PROG);


  while((scanner_out = scanner(&currentToken)) == SUCCESS && result == SUCCESS) {
    // create derivation tree
    // currentToken contains new token in every iteration

    /*SYMTABLE*/

    if ( currentToken.type == ID ) {
      /// tu to porovnani jestli je to funkca strecu
      if ( (strcmp(currentToken.text, "print") == 0) || (strcmp(currentToken.text, "inputs") == 0) || (strcmp(currentToken.text, "inputi") == 0) || (strcmp(currentToken.text, "inputf") == 0) || (strcmp(currentToken.text, "lenght") == 0) || (strcmp(currentToken.text, "subst") == 0) || (strcmp(currentToken.text, "ord") == 0) || (strcmp(currentToken.text, "chr") == 0)) {
        symtable_insert_function(&globalTree, currentToken.text, TYPE_NIL, -1, NULL, true);
      }
      idName = malloc(strlen(currentToken.text) * sizeof(char));
      if (idName == NULL) {
        result = INTERNAL_ERR;
      }
      else {
        strcpy(idName, currentToken.text);
        symtable_insert_unknown(&globalTree, idName);
      }
    }

    if (currentToken.type == OPERATOR && (strcmp(currentToken.text, "=") == 0)) {
      symtable_insert_variable(&globalTree, idName, TYPE_NIL, true);
    }

    /*SYMTABLE*/

    if(result == 0) {
      result = ll_predict(&currentToken, &stack, &globalTree);
    }

    //// MUSI BYT AZ NA KONCI CYKLU !!!!!!!!!!!!!!!
    if(currentToken.text != NULL) {
      // Free allocated text
      free(currentToken.text);
      currentToken.text = NULL;
    }
    if(result != 0) {
      // TODO: free allocated resources
    }
  } // end while

  if(scanner_out != -1) {
    // handle errors during lexical analysis

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
        if(result == INTERNAL_ERR) {
          fprintf(stderr, ANSI_COLOR_RED "Internal error: " ANSI_COLOR_RESET "memory allocation failed. Not enough memory?");
        }
      // Free allocated resources and quit (if there are any)
    }
  }
  // We no longer need the stack, remove it.
  else {
    // EOF
    int top = s_top(&stack);
    if (top == LL_PROG) {
      top = s_top(&stack);
      if (top == LL_BOTTOM) {
        result = SUCCESS;
      }
    }
    else {
      fprintf(stderr, ANSI_COLOR_RED "Syntax error: " ANSI_COLOR_RESET "Unexpected end of file.\n");
      result = SYNTAX_ERR;
    }

  }
  s_free(&stack);
  return result;
}


int ll_predict(tToken *token, tStack *stack, tSymPtr *globalTree)
{
  int top = s_top(stack);

  // NON TERMINALS
while ((top = s_top(stack)) >= LL_PROG && top < LL_BOTTOM) {
  //fprintf(stderr, "TOP IS : %d\n", top);
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
      else if(token->type == END || token->type == ELSE ) {
        PUSH_RULE_3; // $ handled before ll_predict
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_PARAMS:
      if(token->type == ID) {
        PUSH_RULE_4;
      }
      else if(token->type == OPERATOR && token->text[0] == ')') {
        PUSH_RULE_5;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s. Expected EOL, ( or )\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_STATEMENT_N:
      if(token->type == ID || token->type == EOL || token->type == IF || token->type == WHILE) {
        PUSH_RULE_12;
      }
      else if (token->type == END || token->type == ELSE) {
        PUSH_RULE_13;
      }
      break;
    case LL_STATEMENT:
      if(token->type == ID) {
        PUSH_RULE_10;
      }
      else if(token->type == EOL) {
        PUSH_RULE_11;
      }
      else if(token->type == IF) {
        PUSH_RULE_8;
      }
      else if(token->type == WHILE) {
        PUSH_RULE_9;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_PARAMS_N:
      if(token->type == OPERATOR && token->text[0] == ')') {
        PUSH_RULE_7;
      }
      else if(token->type == COMMA) {
        PUSH_RULE_6;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s. Expected EOL, ) or ,\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_DEF_ARGS:
      if(token->type == ID || token->type == EOL || (token->type == OPERATOR && token->text[0] == '(') || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING ) {
        PUSH_RULE_15;
      }
      else if(token->type == OPERATOR && (strcmp(token->text, "=") == 0)) {
        PUSH_RULE_14;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_DEFINE:
      if(token->type == EOL) {
        PUSH_RULE_18;
      }
      else if(token->type == ID) { // TODO must be ID of a function
        tSymPtr sym = symtable_search(globalTree, token->text);
        // Muze byt UNKNOWN,FUNCTION nebo VARIABLE
        if(sym->type == UNKNOWN || sym->type == FUNCTION) {
          PUSH_RULE_16;
        }
        else {
          PUSH_RULE_17;
        }
      }
      else if (token->type == INTEGER || token->type == STRING || token->type == FLOATING_POINT || token->type == OPERATOR) { // everything that fits in expression,
        PUSH_RULE_17;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ARGS:
      if(token->type == ID || token->type == INTEGER || token->type == FLOATING_POINT || token->type == STRING) {
        PUSH_RULE_21;
      }
      else if(token->type == EOL || (token->type == OPERATOR && token->text[0] == ')')) {
        PUSH_RULE_19;
      }
      else if(token->type == OPERATOR && token->text[0] == '(') {
        PUSH_RULE_20;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_ARGS_N:
      if (token->type == EOL || (token->type == OPERATOR && token->text[0] == ')')) {
        PUSH_RULE_23;
      }
      else if (token->type == COMMA) {
        PUSH_RULE_22;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
      }
      break;
    case LL_VALUE:
      if (token->type == ID) {
        PUSH_RULE_26;
      }
      else if (token->type == INTEGER) {
        PUSH_RULE_24;
      }
      else if (token->type == FLOATING_POINT) {
        PUSH_RULE_25;
      }
      else if (token->type == STRING) {
        PUSH_RULE_27;
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" unexpected identifier %s.\n", token->text);
        return SYNTAX_ERR;
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
    case LL_EXPRESSION:
      if (token->type == INTEGER || token->type == STRING || token->type == FLOATING_POINT || token->type == OPERATOR || token->type == ID) { // everything that fits in expression,  TODO must be ID of a variable
          if(prec_table(token) == SYNTAX_ERR) {
            fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET"Unexpected token in expression.\n");
            return SYNTAX_ERR;
          }
      }
      else { // everything else or ID of a function
        tToken endExpression = {"", LL_BOTTOM}; // finish expression
        fprintf(stderr, "PRISEL EOL, UKONCUJE SE PREC\n");
        if (prec_table(&endExpression) == SYNTAX_ERR) {
          fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET"Unexpected token in expression.\n");
          return SYNTAX_ERR;
        }
        s_pop(stack);
        ll_predict(token, stack, globalTree); // recall the function in order not to lose a token
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
    case LL_SET:
      if (token->type == OPERATOR && (strcmp(token->text, "=") == 0)) {
        s_pop(stack);
      }
      else {
        fprintf(stderr, ANSI_COLOR_RED "Syntax error: "ANSI_COLOR_RESET" = unexpected.\n");
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

int add_to_symtable(tToken *cur_token, tToken *prev_token, tSymPtr *globalTree)
{
  if(cur_token->type == OPERATOR && (strcmp(cur_token->text, "=") == 0) && prev_token->type == ID) {
    // nastav predchozi token na promennou
    symtable_insert_variable(globalTree, prev_token->text, TYPE_NIL, true);
  }
  /*
  if(token->type == ID) {
    // pridej do symtable, ale prvni sezen jeho typ.
    // Prislo ID, pockej na dalsi token.
    // Jesli dalsi token je =, nastav ID jako promenou, jinak nechej unknown
    return symtable_insert_unknown(globalTree, token->text);
  }
  */
  return 0;
}
