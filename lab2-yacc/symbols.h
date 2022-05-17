#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include <stdio.h>
#include <stdlib.h>

int getsym();
int yylex();
extern char* yytext;
extern const char* symname[];
const char* getsymname(int sym);

enum STATUS {
    GOING,
    STATUS_ACC,
    STATUS_ERR,
};

enum SYMBOL {
    ERR = -1,
    SYM_EOF = 0,
    NUM = 1,
    E_,
    E,
    T,
    T_,
    F
};

enum ACTION_TYPE {
ACTION_ERR,
S,
R,
G,
ACC,
};

struct action {
    int type;
    int go;
};

struct generator {
    int from;
    int to[10];
    int len;
};

#endif // SYMBOLS_H_
