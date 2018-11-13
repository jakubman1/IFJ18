/**
 * @file errcodes.h
 * @author Jakub Man
 * @date 18.10. 2018
 * @brief Enum of error codes
 */
#ifndef ERRCODES_H
#define ERRCODES_H
 /* Error return codes */
 typedef enum {
   SUCCESS = 0, /* No error */
   LEXICAL_ERR = 1, /* Error in lexical analysis */
   SYNTAX_ERR = 2, /* Error in syntactic analysis */
   VARIABLE_ERR = 3, /* Undefined function/variable, attempt to redefine function/variable etc. */
   TYPE_ERR = 4, /* Attempt to use an operation on incompatible types */
   PARAM_ERR = 5, /* Wrong number of parameters when calling a funciton */
   SEMANTIC_OTHER_ERR = 6, /* Semantic error undefined by previous error codes */
   DIVISION_BY_ZERO = 9, /* Attempt to divide by zero */
   INTERNAL_ERR = 99 /* Internal compiler errors (memory allocation errors..) */
 } err_codes;

#endif
