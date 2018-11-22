/**
 * @file syn_analysis.h
 * @author Jan Martinák
 * @date 19.11. 2018
 * @brief Syntactic analysis header file
 */

#ifndef SYN_ANALYSYS_H
#define SYN_ANALYSYS_H

#include <stdio.h>
#include <stdlib.h>
#include "lib/errcodes.h"
#include "lib/types.h"
#include "scanner.h"

 /**
  * @brief Implementation of <value> rules
  */
 int value();

 /**
  * @brief Implementation of <args_n> rules
  */
 int args_n();

 /**
  * @brief Implementation of <args> rules
  */
 int args();

 /**
  * @brief Implementation of <define> rules
  */
 int define();

 /**
  * @brief Implementation of <def_args> rules
  */
 int def_args();

 /**
  * @brief Implementation of <statement> rules
  */
 int statement();

 /**
  * @brief Implementation of <params_n> rules
  */
 int params_n();

 /**
  * @brief Implementation of <params> rules
  */
 int params();

 /**
  * @brief Implementation of <prog> rules
  */
 int prog();

 /**
  * @brief Main function of parser
  */
 int parser();

 #endif
