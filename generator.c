/**
 * @file generator.c
 * @author Jan Martinák
 * @date 28.11. 2018
 * @brief Implementation of code generator
 */

#include "generator.h"





void start() {
  printf(".IFJcode18\n");

}

/*void opetator {
  switch () {
    case :
  }
}*/


/*
 * INTEGRATED FUNCTIONS
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
