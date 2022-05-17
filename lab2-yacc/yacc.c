#include "symbols.h"
#include "stdio.h"

int main() {
    int lex;
    while((lex = yylex()) != SYM_EOF) {
        printf("%d ", lex);
    }
}
