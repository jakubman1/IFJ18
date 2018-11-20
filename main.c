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
  while(scanner(&t) == 0) {
    ;
  }
  #endif
  #ifdef DEBUG2
  // syntactic analysis test
    
  #endif
  return retcode;
}
