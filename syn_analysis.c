/**
 * @file syn_analysis.c
 * @author Jan Martinák
 * @date 19.11. 2018
 * @brief Syntactic analysis file
 */

#include "syn_analysis.h"


int value()
{
  return 0;
}

int args_n()
{
  return 0;
}

int args()
{
  return 0;
}

int define()
{
  return 0;
}

int def_args()
{
  return 0;
}

int statement()
{
  return 0;
}

int params_n()
{
  return 0;
}

int params()
{
  return 0;
}

int prog()
{
  return 0;
}

int parser()
{
  int result = SUCCESS;
  int scanner_out = SUCCESS;
  tToken currentToken = {"", ERROR};

  while((scanner_out = scanner(&currentToken)) == SUCCESS) {
    // create derivation tree
    // currentToken contains new token in every iteration

    printf("Current token: %s, %d\n", currentToken.text, currentToken.type);
    //// MUSI BYT AZ NA KONCI CYKLU !!!!!!!!!!!!!!!
    if(currentToken.text != NULL) {
      // Free allocated text
      free(currentToken.text);
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
    if(scanner_out == INTERNAL_ERR) {
      // Free allocated resources and quit (if there are any)
    }
  }

  return result;
}
