/**
@file generator.h
@brief Code generator functions
*/
#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ass.h"
#include "symtable.h"

/**
Rewrite escaped string characters to format required by IFJcode18
*/
void rewrite_string(char **str);
void insert_string_to_string(char **toStr, char *inStr, int pos);
/**

*/
void insert_string_to_string(char **toStr, char *inStr, int pos);
void print_int(int i);
void print_float(double f);
void print_string(char *s);
void print_bool(bool b);
void print_nil();

#endif
