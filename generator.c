/**
 * @file generator.c
 * @author Jakub Man (AKA BIG DICK IN TOWN)
 * @author Jan Martinák
 * @date 28.11. 2018
 * @brief Implementation of code generator
 */

#include "generator.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


void gen_test(int val) {

}

void gen_start() {
  printf("# Generated using ifj18 compiler by\n");
  printf("# Jakub Man, Adam Melichar, Jiri Tykva and Jan Martinak\n");
  printf(".IFJcode18\nJUMP $$MAIN\n");
}


/*************
  BASIC CONSTRUCTIONS
************ */

void gen_def(tSymPtr func) {
  printf("\n\n");
  printf("LABEL $%s\n", func->name);
  printf("PUSHFRAME\n");
  printf("DEFVAR LF@%%retval\n");
  printf("MOVE LF@%%retval nil@nil\n");
  if(func->type == FUNCTION) {
    if(func->data.funData.paramCount > 0 && func->data.funData.params != NULL) {
      for(int i = 0; i < func->data.funData.paramCount; i++) {
        printf("DEFVAR LF@%%param%d\nMOVE LF@%%param%d  LF@%%%d", i, i, i);
      }
    }
  }
  else {
    fprintf(stderr, ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET "%s is not a function!\n", func->name);
  }
}

void gen_end_def() {
  printf("POPFRAME\nRETURN\n\n");
}

void gen_call(tSymPtr func) {
  printf("CREATEFRAME");

}

void gen_var_global(tSymPtr var) {
  printf("DEFVAR GF@%s\n", var->name);
}

void gen_var_seti(tSymPtr var, int i) {
  printf("MOVE GF@%s ", var->name);
  print_int(i);
  printf("\n");
}
void gen_var_setf(tSymPtr var, double f) {
  printf("MOVE GF@%s ", var->name);
  print_float(f);
  printf("\n");
}

void gen_var_sets(tSymPtr var, char *c) {
  printf("MOVE GF@%s ", var->name);
  print_string(c);
  printf("\n");
}

void gen_var_setn(tSymPtr var) {
  printf("MOVE GF@%s ", var->name);
  print_nil();
  printf("\n");
}

/*************
  ARITHMETIC OPERATIONS
************ */

void gen_add_ii(char *varName, int i, int j)
{

}
void gen_add_ff(char *varName, double i, double j)
{

}
void gen_add_vv(char *result, char *op1, char *op2)
{

}

/*************
  CONSTANTS
************ */
void print_int(int i) {
  printf("int@%d", i);
}
void print_float(double f) {
  printf("float@%a", f);
}
void print_string(char *s) {
  rewrite_string(&s);
  printf("string@%s", s);
}
void print_bool(bool b) {
  if(b) {
    printf("bool@true");
  }
  else {
    printf("bool@false");
  }
}
void print_nil() {
  printf("nil@nil");
}


/*************
  HELPER FUNCTIONS
************ */
void rewrite_string(char **str) {
  for(int i = 0; (*str)[i] != '\0'; i++) {
    if((*str)[i] == 35 || (*str)[i] <= 32 || (*str)[i] == 92 ) {
      char buff[5] = {0,};
      // Convert letter to \xyz, where xyz is decimal value of the letter.
      sprintf(buff, "\\%3d", (*str)[i]);
      insert_string_to_string(str, buff, i);
    }
  }
}

void insert_string_to_string(char **toStr, char *inStr, int pos) {
  realloc(*toStr, (strlen(*toStr) + strlen(inStr) + 1) * sizeof(char));
  char *tmp = malloc((strlen(*toStr) + strlen(inStr) + 1) * sizeof(char));
  strncpy(tmp, *toStr, pos);
  tmp[pos] = '\0';
  strcat(tmp, inStr);
  strcat(tmp, *toStr + pos);
  strcpy(*toStr, tmp);
  free(tmp);
}

/*
 * INTEGRATED FUNCTIONS (KYS)
 */

void in_substr() {
  printf("# Integrated function substr\n"
         "\n");
}

void in_length() {
  printf("# Integrated function length\n"
         "LABEL @length\n"
         "PUSHFRAME\n"
         "DEFVAR LF@_ret\n"
         "STRLEN LF@_delka LF@0\n"
         "POPFRAME\n"
         "RETURN\n"
         "# End of function length\n");
}

void in_ord() {
  printf("# Integrated function ord\n"
         "LABEL @ord\n");
}

void in_chr() {
  printf("# Integrated fuction chr\n"
         "LABEL @chr\n"
         "PUSHFRAME\n"
         "DEFVAR\n"
         "MOVE LF@_ret string@\n"
         "DEFVAR LF@support\n"
         "MOVE LF@support string@\n"
         "INT2CHAR LF@ret LF@i\n"
         "POPFRAME\n"
         "RETURN\n"
         "# End f integrated function chr\n");
}
