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
  printf("# Jakub Man, Adam Melichar, Jiri Tykva and Jan Martinak\n\n");
  printf("#main frame\nCREATEFRAME\nPUSHFRAME\n\n");
  char *tmp = malloc(sizeof(char) * 50);
  strcpy(tmp, "\"Toto je testovaci string\"");
  print_string(tmp);
}

/*************
  BASIC CONSTRUCTIONS
************ */

void gen_def(tSymPtr func) {
  printf("\n\n");
  printf("JUMP $$END-%s\n", func->name);
  printf("LABEL $%s\n", func->name);
  printf("PUSHFRAME\n");
  printf("DEFVAR LF@%%retval\n");
  printf("MOVE LF@%%retval nil@nil\n");
  if(func->type == FUNCTION) {
    if(func->data.funData.paramCount > 0 && func->data.funData.params != NULL) {
      tFuncParam *current = func->data.funData.params;
      for(int i = 1; i <= func->data.funData.paramCount; i++) {
        printf("DEFVAR LF@%s\nMOVE LF@%s  LF@%%%d\n", current->name, current->name, i);
        if(current->next != NULL) {
          current = current->next;
        }
      }
    }
  }
  else {
    fprintf(stderr, ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET "%s is not a function!\n", func->name);
  }
}


void gen_end_def(char *name) {
  printf("POPFRAME\nRETURN\n");
  printf("LABEL $$END-%s\n\n", name);
}

void gen_while(tStack *stack) {
  int stack_top = s_top(stack);

  printf("JUMPIFEQ $$WHILEEND%d LF@whilecond%d bool@false\n", stack_top, stack_top);

}

void gen_while_end(tStack *stack) {
  int stack_top = s_top(stack);
  printf("JUMP $$WHILE%d\n", stack_top);
  printf("LABEL $$WHILEEND%d\n", stack_top);
}

// Podminka byla vyhodnocena
void gen_if(tStack *stack) {
  int stack_top = s_top(stack);
  printf("JUMPIFEQ $$IFELSE%d LF@ifcond%d bool@false\n", stack_top, stack_top);
}
// Prisel token else
void gen_else(tStack *stack) {
  int stack_top = s_top(stack);
  printf("JUMP $$IFEND%d\n", stack_top);
  printf("LABEL $$IFELSE%d\n", stack_top);
}
// prisel token end patrici k if.
void gen_if_end(tStack *stack) {
  int stack_top = s_top(stack);
  printf("LABEL $$IFEND%d\n", stack_top);
}

void gen_call(tSymPtr func) {
  printf("CALL $%s\n", func->name);
}


void gen_var(tSymPtr var, bool global) {
  printf("DEFVAR %s@%s\n", global ? "GF": "LF", var->name);
}


void gen_var_seti(tSymPtr var, char *val, bool global) {
  printf("MOVE %s@%s ", global ? "GF": "LF", var->name);

  print_int(val);
  printf("\n");
}
void gen_var_setf(tSymPtr var, char *val, bool global) {
  printf("MOVE %s@%s ", global ? "GF": "LF", var->name);
  print_float(val);
  printf("\n");
}

void gen_var_sets(tSymPtr var, char *c, bool global) {
  printf("MOVE %s@%s ", global ? "GF": "LF", var->name);
  print_string(c);
  printf("\n");
}

void gen_var_setn(tSymPtr var, bool global) {
  printf("MOVE %s@%s ", global ? "GF": "LF", var->name);
  print_nil();
  printf("\n");
}

// Using top of the stack, returning it back there
void int2floats() {
  static int uniqid = 0;
  printf("DEFVAR LF@$itf%d\n", uniqid);
  printf("DEFVAR LF@$itftype%d\n", uniqid);
  printf("POPS LF@$itf%d\n", uniqid);
  printf("TYPE LF@$itftype%d LF@$itf%d\n", uniqid, uniqid);
  PUSH_ONE(printf("LF@$itf%d", uniqid));
  printf("JUMPIFEQ $isint%d LF@$itftype%d string@int\n", uniqid, uniqid);
  printf("JUMPIFEQ $isfloat%d LF@$itftype%d string@float\n", uniqid, uniqid);
  printf("EXIT int@4");
  printf("LABEL $isint%d\n", uniqid);
  printf("INT2FLOATS\n");
  printf("LABEL $isfloat%d\n", uniqid);
  // printf("PUSHS LF$itf%d", uniqid);

  uniqid++;
}

void gen_check_string() {
  static int uniqid = 0;

  printf("DEFVAR LF@$strcheck%d\n", uniqid);
  printf("DEFVAR LF@$strchecktype%d\n", uniqid);
  printf("POPS LF@$strcheck%d\n", uniqid);
  printf("TYPE LF@$strchecktype%d LF@$strcheck%d\n", uniqid, uniqid);
  PUSH_ONE(printf("LF@$strcheck%d", uniqid));
  printf("JUMPIFEQ $isstring%d LF@$strchecktype%d string@string\n", uniqid, uniqid);
  printf("EXIT int@4");
  printf("LABEL $isstring%d\n", uniqid);

  uniqid++;
}

void gen_check_nil() {
  static int uniqid = 0;

  printf("DEFVAR LF@$nilcheck%d\n", uniqid);
  printf("DEFVAR LF@$nilchecktype%d\n", uniqid);
  printf("POPS LF@$nilcheck%d\n", uniqid);
  printf("TYPE LF@$nilchecktype%d LF@$nilcheck%d\n", uniqid, uniqid);
  PUSH_ONE(printf("LF@$nilcheck%d", uniqid));
  printf("JUMPIFEQ $isnil%d LF@$strchecktype%d string@nil\n", uniqid, uniqid);
  printf("EXIT int@4");
  printf("LABEL $isnil%d\n", uniqid);

  uniqid++;
}

void gen_check_int() {
  static int uniqid = 0;

  printf("DEFVAR LF@$intcheck%d\n", uniqid);
  printf("DEFVAR LF@$intchecktype%d\n", uniqid);
  printf("POPS LF@$intcheck%d\n", uniqid);
  printf("TYPE LF@$intchecktype%d LF@$intcheck%d\n", uniqid, uniqid);
  PUSH_ONE(printf("LF@$intcheck%d", uniqid));
  printf("JUMPIFEQ $isintc%d LF@$intchecktype%d string@int\n", uniqid, uniqid);
  printf("EXIT int@4");
  printf("LABEL $isintc%d\n", uniqid);

  uniqid++;
}

void concat_strings() {
  static int uniqid = 0;

  printf("DEFVAR LF@$strcon%d\n", uniqid);
  printf("DEFVAR LF@$strcontwo%d\n", uniqid);
  printf("POPS LF@$strcon%d\n", uniqid);
  printf("POPS LF@$strcontwo%d\n", uniqid);
  printf("CONCAT LF@$strcon%d LF@$strcon%d LF@$strcontwo%d\n", uniqid, uniqid, uniqid);
  printf("PUSHS LF@$strcon%d\n", uniqid);
  uniqid++;
}

/*************
  INTEGRATED FUNCTIONS
************ */
void gen_inputi(char *varName, char *frame) {
  printf("READ %s@%s int\n", frame, varName);
}
void gen_inputf(char *varName, char *frame) {
  printf("READ %s@%s float\n", frame, varName);
}
void gen_inputs(char *varName, char *frame) {
  printf("READ %s@%s string\n", frame, varName);
}




/*************
  CONSTANTS
************ */
void print_int(char *val) {
  int i = 0;
  sscanf(val, "%d", &i);
  printf("int@%d", i);
}
void print_float(char *val) {
  double d = 0.0;
  sscanf(val, "%lf", &d);
  printf("float@%a", d);
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
  fprintf(stderr, "got rewrite string\n");
  for(int i = 0; (*str)[i] != '\0'; i++) {
    if(i == 0) {
      char *p = *str;
      p++;
      *str = p;
    }
    else if( i == (strlen(*str) - 1)) {
      str[i] = '\0';
    }
    if((*str)[i] == 35 || (*str)[i] <= 32 || (*str)[i] == 92 ) {
      char buff[5] = {0,};
      // Convert letter to \xyz, where xyz is decimal value of the letter.
      sprintf(buff, "\\%03d", (*str)[i]);
      insert_string_to_string(str, buff, i);
      //fprintf(stderr, "string is now %d\n", **str);
    }
  }
}

void insert_string_to_string(char **toStr, char *inStr, int pos) {
  fprintf(stderr, "got insert to string. String is %s. InStr is %s Pos is %d\n", *toStr, inStr, pos);
  *toStr = realloc(*toStr, (strlen(*toStr) + strlen(inStr) + 2) * sizeof(char));
  fprintf(stderr, "Got after realloc\n");
  char *tmp = malloc((strlen(*toStr) + strlen(inStr) + 2) * sizeof(char));
  fprintf(stderr, "Got after tmp malloc\n");
  if(tmp == NULL || *toStr == NULL) {
    fprintf(stderr, "ERROR: No memory left!!!!\n");
    exit(99);
    return;
  }
  fprintf(stderr, "reallocated\n");
  strncpy(tmp, *toStr, pos);
  tmp[pos] = '\0';
  fprintf(stderr, "strncpy 1: %s\n", tmp);
  strcat(tmp, inStr);
  fprintf(stderr, "strcat 1: %s\n", tmp);
  strcat(tmp, *toStr + pos + 1);
  fprintf(stderr, "strcat 2: %s\n", tmp);
  strcpy(*toStr, tmp);
  fprintf(stderr, "strncpy 2: %s\n", *toStr);
  free(tmp);
}

void call_function (tSymPtr func, char *var_name, bool global_frame, tFuncParam *args)
{
  if (strcmp(func->name, "inputi") == 0) {
    gen_inputi(var_name, global_frame ? "GF" : "LF");
  }
  else if (strcmp(func->name, "inputf") == 0) {
    gen_inputf(var_name, global_frame ? "GF" : "LF");
  }
  else if (strcmp(func->name, "inputs") == 0) {
    gen_inputs(var_name, global_frame ? "GF" : "LF");
  }
  else if (strcmp(func->name, "print") == 0) {
    //fprintf(stderr, "PRINT ARGS:");
    for (tFuncParam *tmp = args; tmp != NULL; tmp = tmp->next) {
      PRINT(print_args(global_frame, tmp));
      //fprintf(stderr, "%s ", tmp->name);
    }
    fprintf(stderr, "\n");
    if (args != NULL) {
      free(args);
    }
  }
  else if (args != NULL) {
    if (strcmp(func->name, "length") == 0) {
      //STRLEN(printf("%s@%s", global_frame ? "GF" : "LF", var_name), args->name);
      STRLEN(printf("%s@%s", global_frame ? "GF" : "LF", var_name), print_args(global_frame, args));
      free(args);
    }
    else if (strcmp(func->name, "substr") == 0) {
      // TODO
      free(args);
    }
    else if (strcmp(func->name, "ord") == 0) {
      //ORD(printf("%s@%s", global_frame ? "GF" : "LF", var_name), args->name, args->next->name);
      ORD(printf("%s@%s", global_frame ? "GF" : "LF", var_name), print_args(global_frame, args), print_args(global_frame, args->next));
      free(args);
    }
    else if (strcmp(func->name, "chr") == 0) {
      CHR(printf("%s@%s", global_frame ? "GF" : "LF", var_name), print_args(global_frame, args));
      free(args);
    }
  }
  else {
    gen_call(func);
    GET_RETVAL(printf("GF@%s", var_name));
  }
}

void print_args (bool global_frame, tFuncParam *args) {
  if (args->type == TYPE_ID) {
    printf("%s@%s", global_frame ? "GF" : "LF", args->name);
  }
  else {
    if (args->type == TYPE_INT) {
      print_int(args->name);
    }
    else if (args->type == TYPE_FLOAT) {
      print_float(args->name);
    }
    else if (args->type == TYPE_STRING) {
      print_string(args->name);
    }
    else if (args->type == TYPE_NIL) {
      print_nil();
    }
  }
}

void set_variable (tSymPtr var, char *value, data_type new_type, bool global_frame) {
if (new_type == TYPE_INT) {
    gen_var_seti(var, value, global_frame);
  }
  else if (new_type == TYPE_FLOAT) {
    gen_var_setf(var, value, global_frame);
  }
  else if (new_type == TYPE_STRING) {
    gen_var_sets(var, value, global_frame);
  }
  else if (new_type == TYPE_NIL) {
    gen_var_setn(var, global_frame);
  }
}

int uniqueInt() {
  static unsigned int i = 0;
  return i++;
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
