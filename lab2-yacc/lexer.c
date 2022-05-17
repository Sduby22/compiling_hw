#include "symbols.h"

int getsym() {
    int lex = yylex();
    while(lex == ERR) {
	printf("lexer: 输入非法字符%s, 跳过\n", yytext);
	lex = yylex();
    }
    return lex;
}
