/**
 * @file generator.c
 * @author Jakub Man
 * @author Jan Martinák
 * @date 28.11. 2018
 * @brief Implementation of code generator
 */

#include "generator.h"

//#define STR_HELPER(x) #x
//#define STR(x) STR_HELPER(x)


void gen_start() {
  printf(".IFJcode18\n");
  printf("# Generated using ifj18 compiler by\n");
  printf("# Jakub Man, Adam Melichar, Jiri Tykva and Jan Martinak\n");
}

/*************
  BASIC CONSTRUCTIONS
************ */

void gen_def(tSymPtr func) {
  printf("\n\n");
  printf("JUMP $$END-%s", func->name);
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


void gen_end_def(char *name) {
  printf("POPFRAME\nRETURN\n\n");
  printf("LABEL $$END-%s", name);
}

void gen_while(unsigned int num, char *varname) {

  printf("LABEL $$WHILE%d\n", num);
  // podminka
  printf("JUMPIFEQ $$WHILEEND%d %s bool@false\n", num, varname);

}

void gen_while_end(unsigned int num) {
  printf("JUMP $$WHILE%d\n", num);
  printf("LABEL $$WHILEEND%d", num);
}

// Podminka byla vyhodnocena
void gen_if(unsigned int num, char *varname) {
  printf("JUMPIFEQ $$IFELSE%d %s bool@false", num, varname);
}
// Prisel token else
void gen_else(unsigned int num) {
  printf("JUMP $$IFEND%d", num);
  printf("LABEL $$IFELSE%d", num);
}
// prisel token end patrici k if.
void gen_if_end(unsigned int num) {
  printf("LABEL $$IFEND%d", num);
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
  INTEGRATED FUNCTIONS
************ */
void gen_inputi(char *varName, char *frame) {
  printf("READ %s@%s int", frame, varName);
}
void gen_inputf(char *varName, char *frame) {
  printf("READ %s@%s float", frame, varName);
}
void gen_inputs(char *varName, char *frame) {
  printf("READ %s@%s string", frame, varName);
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
 * INTEGRATED FUNCTIONS
 */

/*void in_substr() {
  printf("# Integrated function substr\n"
         "\n");
}

void in_length() {
  printf("# Integrated function length\n"
         "LABEL length\n"
         "PUSHFRAME\n"
         "DEFVAR LF@%retval\n"
         "STRLEN LF@%delka LF@%0\n"
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
         "MOVE LF@%retval string@\n"
         "DEFVAR LF@%support\n"
         "MOVE LF@%support string@\n"
         "INT2CHAR LF@%ret LF@%i\n"
         "POPFRAME\n"
         "RETURN\n"
         "# End f integrated function chr\n");
}*/
