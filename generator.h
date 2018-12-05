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
#include "stack.h"

/*
Good ol' tutorial:

definice funkce:
  prisel def + seznam parametru, funkce se uklada to tabulky symbolu:
    - gen_def(tSymPtr func)
  prisel posledni identifikator funkce:
    - RETURN_VALUE(val) <- val = volani printf na hodnotu nebo promennou (vcetne typu/ramce)
  prisel end patrici k def:
    - gen_end_def(char *name) <- nazev funkce, ktera skoncila

while:
  prisel while a podminka je vyhodnocena v nejake pomocne promenne:
    - gen_while(unsigned num, char *varname) <- num = unikatni cislo patrici k tomuto while
                                                varname = nazev pomocne promenne obsahujici bool vysledku (vc. frame)
  prisel end patrici k while
    - gen_while_end(unsigned num) <- num = cislo prirazene k tomuto while pri gen_while

volani funkce:
  zavolat makro CREATEFRAME
  pro kazdy parametr:

    CALL_PARAM(x, num) <- x = hodnota jako volani print (nazvu promenne nebo konstanty)
                          num = cislo parametru, pro kazdy parametr se zvysi o 1, zacina pro kazde volani na 1.
  gen_call(tSymPtr func) <- func ukazatel na funkci do tabulky symbolu
  Pro ziskani navratove hodnoty:
    GET_RETVAL(val) <- val = printf promenne vcetne ramce, do ktere ulozit vysledek

if:
  prisel if a podminka je vyhodnocena v nejake pomocne promenne
    - gen_if(unsigned num, char *varname) <- num = unikatni cislo patrici k tomuto if
                                             varname = nazev pomocne promenne obsahujici bool vysledku (vc. frame)
  prislo else
    - gen_else(unsigned num) <- num = cislo prirazene k tomuto if pri gen_if

  prisel end patrici k tomuto if
    - gen_if_end(unsigned num) <- num = cislo prirazene k tomuto if pri gen_if

vyhodnoceni vyrazu:
  pruchod expression stromem post-order. Pro konstanty a promenne se vola PUSH_ONE(x).
  x = hodnota jako volani printf (nazvu promenne nebo konstanty)
  Hodnoty se pridaji na zasobnik.
  Pro operatory se vola jejich ekvivalentni operace ve verzi pro zasobnik (funkce koncici S)
  napr. makro ADDS pro +.
  DIV je pouze pro int, IDIV pouze pro float. Je nutne kontrolovat spravnost typu pred volanim,
  pripadne provest konverzi int -> float.

definice globalni promenne:
  gen_var_global(tSymPtr fun) <- fun = odkaz na promennou do tabulky symbolu

  prvni parametr je ukazatel do tabulky symbolu, druhy je hodnota. Pro jednotlive typy:
  gen_var_seti() pro int
  gen_var_setf() pro float
  gen_var_setn() pro nil (bez druheho parametru)
  gen_var_sets pro string

Vestavene funkce:
  strlen:
    STRLEN(printf("ramec@promenna"), printf("hodnota")); Prvni printf je promenna, do ktere bude ulozeny vysledek.
    Druhy pritnf je hodnota bud v promenne (vcetne ramce), nebo primo hodnota stringu jako string@text
  ord:
    STRLEN(printf("ramec@promenna"), printf("hodnota"), printf("hodnota")); Prvni printf je promenna, do ktere bude ulozeny vysledek.
    Druhy printf je hodnota bud v promenne (vcetne ramce), nebo primo hodnota stringu jako string@text (musi byt string).
    Treti printf je hodnota bud v promenne (vcetne ramce), nebo primo hodnota int jako int@hodnota (musi byt int).

  print:
    pro každý parametr zavolat PRINT(x) -> x = volani printf na hodnotu nebo promennou vcetne ramce. Muze byt libovolny datovy typ (TODO: TEST!!!)

*/


/*
  Macros
*/

// x can be any valid symb print call, x should be int
#define CALL_PARAM(x, num) printf("DEFVAR TF@%%%d\n", num); printf("MOV TF@%%%d ", num); x; printf("\n")
#define CREATEFRAME printf("CREATEFRAME\n")
#define RETURN_VALUE(val) printf("MOVE LF@%%retval "); val; printf("\n")
#define GET_RETVAL(out) printf("MOVE "); out; printf(" TF@%%retval"); printf("\n")


// Arithmetic operations
// x and y must be either format@value (eg. int@5) or frame@name (eg. GF@a) and
// must be printf or function calls

#define ADD(out, x, y) printf("ADD %s ", out); x; printf(" "); y; printf("\n")
#define SUB(out, x, y) printf("SUB %s ", out); x; printf(" "); y; printf("\n")
#define MUL(out, x, y) printf("MUL %s ", out); x; printf(" "); y; printf("\n")
#define DIV(out, x, y) printf("DIV %s ", out); x; printf(" "); y; printf("\n")
#define IDIV(out, x, y) printf("IDIV %s ", out); x; printf(" "); y; printf("\n")

// Stack version of arithmetic functions. Result will be on top of the stack
// Expression tree should be browsed using post-order
// PUSH_ONE(printf("%s@%s", isGlobal ? "GF": "LF", token->text))
// POPS(printf(printf("%s@%s", isGlobal ? "GF": "LF", nazev_promenne_do_ktere_to_chces_strcit))
#define PUSH_ONE(x) printf("PUSHS "); x; printf("\n")
#define PUSH_TWO(x, y) printf("PUSHS "); y; printf("\nPUSHS "); x; printf("\n")
#define POPS(out) printf("POPS "); out; printf("\n")
#define ADDS printf("ADDS\n")
#define SUBS printf("SUBS\n")
#define MULS printf("MULS\n")
#define DIVS printf("DIVS\n")
#define IDIVS printf("IDIVS\n")

#define LTS printf("LTS\n")
#define GTS printf("GTS\n")
#define EQS printf("EQS\n")
#define NOTS printf("NOTS\n")

#define INT2FLOATS printf("INT2FLOATS\n")
#define FLOAT2INTS printf("FLOAT2INTS\n")


#define STRLEN(out, x) printf("STRLEN "); out; printf(" "); x; printf("\n")
#define ORD(out, x, i) printf("STR2INT "); out; printf(" "); x; printf(" "); i; printf("\n")
#define CHR(out, i) printf("INT2CHAR "); out; printf(" "); i; printf("\n")
#define PRINT(x) printf("WRITE "); x; printf("\n")
#define SUBSTR(s, i, n) pritnf("")

void gen_start();

void gen_def(tSymPtr func);
void gen_end_def(char *name);

void gen_while(tStack *stack);
void gen_while_end(tStack *stack);

void gen_if(tStack *stack);
void gen_else(tStack *stack);
void gen_if_end(tStack *stack);

void gen_call(tSymPtr func);

void gen_var(tSymPtr var, bool global);


/**
Rewrite escaped string characters to format required by IFJcode18
*/
void rewrite_string(char **str);
void insert_string_to_string(char **toStr, char *inStr, int pos);
/**

*/
void insert_string_to_string(char **toStr, char *inStr, int pos);
void print_int(char *val);
void print_float(char *val);
void print_string(char *s);
void print_bool(bool b);
void print_nil();
void concat_strings();

void int2floats();
void gen_check_string();
void gen_check_int();
void gen_check_nil();

void gen_inputi(char *varName, char *frame);
void gen_inputs(char *varName, char *frame);
void gen_inputf(char *varName, char *frame);

void gen_var_seti(tSymPtr var, char *val, bool global);
void gen_var_setf(tSymPtr var, char *val, bool global);
void gen_var_sets(tSymPtr var, char *c, bool global);
void gen_var_setn(tSymPtr var, bool global);

void call_function (tSymPtr func, char *var_name, bool global_frame, tFuncParam *args);
void print_args (bool global_frame, tFuncParam *args);
void set_variable (tSymPtr var, char *value, data_type new_type, bool global_frame);
int uniqueInt();

#endif
