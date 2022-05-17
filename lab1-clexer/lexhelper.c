#include "symbol.h"

struct wc yywc = {.lc=1};
char stringbuf[MAXSTRING];
static int pos = 0;

struct symbol lexhelper() {
	symtype type = yylex();
	int len = strlen(yytext);
	yywc.bc+=len;
	
	struct symbol sym = {0, 0, 0};

	sym.type = type;
	sym.value = yytext;
	
	switch(type) {

	case 0:
		if (strlen(yytext) == 0) {
			sym.type = T_EOF;
			yywc.lc--;
		} else {
			yywc.null++;
		}
		break;
	
	case T_ID:
		yywc.id++;
		break;

	case T_IF: case T_ELSE: case T_WHILE: case T_FOR: case T_SWITCH:
	case T_CASE: case T_GOTO: case T_BREAK: case T_CONTINUE: case T_DO:
	case T_RETURN: case T_DEFAULT: case T_TYPEDEF: case T_INT: case T_LONG:
	case T_SHORT: case T_FLOAT: case T_DOUBLE: case T_CHAR: case T_UNSIGNED:
	case T_SIGNED: case T_CONST: case T_VOID: case T_VOLATILE: case T_ENUM:
	case T_STRUCT: case T_UNION: case T_REGISTER: case T_EXTERN:
	case T_STATIC: case T_SIZEOF:
		yywc.keyword++;
		break;

	case T_RELOP_GE: case T_RELOP_LE: case T_RELOP_EQ: case T_RELOP_NEQ:
	case T_ARROW: case T_BOOLOP_OR: case T_BOOLOP_AND: case T_CALOP_RSHIFT:
	case T_CALOP_LSHIFT: case T_CALOP_INC: case T_CALOP_DEC: case T_PLUSEQUAL:
	case T_MINUSEQUAL: case T_MULEQUAL: case T_DIVEQUAL: case T_MODEQUAL:
	case T_ANDEQUAL: case T_OREQUAL: case T_XOREQUAL: case T_RSEQUAL:
	case T_LSEQUAL: 
	case '(': case ')': case '{': case '}': case '[': case ']': case '<':
	case '>': case '"': case '\'': case ';': case ':': case '.': case ',':
	case '#': case '+': case '-': case '*': case '/': case '|': case '^':
	case '&': case '!': case '%': case '=': case '?': case '~':
		yywc.punc++;
		break;

	case T_NUM_FLOAT:
		yywc.num++;
		sym.type = T_NUM;
		sym.attr = ATTR_NUM_FLOAT;
		break;

	case T_NUM_HEX:
		yywc.num++;
		sym.type = T_NUM;
		sym.attr = ATTR_NUM_HEX;
		break;

	case T_NUM_BIN:
		yywc.num++;
		sym.type = T_NUM;
		sym.attr = ATTR_NUM_BIN;
		break;

	case T_NUM_OCT:
		yywc.num++;
		sym.type = T_NUM;
		sym.attr = ATTR_NUM_OCT;
		break;

	case T_NUM:
		yywc.num++;
		sym.attr = ATTR_NUM_DECIMAL;
		break;

	case T_LITERAL:
		yywc.literal++;
		yywc.bc-=len;
		sym.value = stringbuf;
		break;
	} 

	return sym;
}

void yystrinit() {
	pos = 0;
}

int yyappend(const char* yytext) {
	int len = strlen(yytext);
	yywc.bc+=len;

	// 必须给双引号和最后的0预留位置, 没位置则失败
	if (len+pos > MAXSTRING - 2) {
		return -1;
	}
	memcpy(stringbuf+pos, yytext, len+1);
	pos += len;

	return 0;
}
