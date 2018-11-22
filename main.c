/**
 * @file main.c
 * @author Jakub Man
 * @date 4.10. 2018
 * @brief Main project file
 */
/* Import system libraries */
#include <stdio.h>

/* Import local libraries */
#include "syn_analysis.h"

int main(int argc, char const *argv[]) {
  int retcode = SUCCESS; /* Return code of the compiler */

  retcode = parser();

  #ifdef DEBUG
  // lexical analysis test
  tToken t = {"", 0};
  int scanner_out = 0;
  while((scanner_out = scanner(&t)) == 0 || scanner_out == LEXICAL_ERR) {
    ;
  }
  #endif
  #ifdef DEBUG2
  // syntactic analysis test

  #endif
  return retcode;
}
