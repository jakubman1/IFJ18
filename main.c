/**
 * @file main.c
 * @author Jakub Man
 * @date 4.10. 2018
 * @brief Main project file
 */
/* Import global libraries */
#include <stdio.h>

/* Import local libraries */
#include "scanner.h"

int main(int argc, char const *argv[]) {
  int retcode = SUCCESS; /* Return code of the compiler */
  /* Call lexical analysis */
  /* Call syntactic analysis */
  retcode = scanner();
  return retcode;
}
