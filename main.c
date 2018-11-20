/**
 * @file main.c
 * @author Jakub Man
 * @date 4.10. 2018
 * @brief Main project file
 */
/* Import system libraries */
#include <stdio.h>

/* Import local libraries */
#include "scanner.h"

int main(int argc, char const *argv[]) {
  int retcode = SUCCESS; /* Return code of the compiler */

  #ifdef DEBUG
  // lexical analysis test
  tToken t = {"", 0};
  int scannerout = 0;
  while((scannerout = scanner(&t)) == 0 || scannerout == 1) {
    ;
  }
  #endif  
  #ifdef DEBUG2
  // syntactic analysis test

  #endif
  return retcode;
}
