/******************************************************************************
* File:             symbol.h
*
* Author:           Li Zihao  
* Created:          10/01/21 
* Description:      提供符号类型 / 符号属性枚举类型定义
*		    符号结构类型定义
*		    全局变量外部声明
*		    函数声明
*****************************************************************************/
	
#ifndef SYMBOL_H_Q2QNGONU
#define SYMBOL_H_Q2QNGONU

#include <stdint.h>
#include <string.h>

#define MAXSYM 512
#define MAXATTR 256
#define MAXSTRING 4098 // Max string literal length is limited to 4095 bytes.

#define ERR_OFFSET 100

//SYMBOL TYPE
enum {
	ERR_UNTERMINATED_STRING_LITERAL = -ERR_OFFSET,
	ERR_UNTERMINATED_CHAR_LITERAL,
	ERR_UNTERMINATED_BLOCK_COMMENT,
	ERR_UNEXPECTED_IDENTIFIER,
	ERR_NULL_CHARACTER_LITERAL,
	ERR_MAX_STRING_LENGTH_EXCEEDED,

	T_EOF = -1,
	T_NULL,

	// keywords
	T_IF = 256,
	T_ELSE,
	T_WHILE,
	T_DO,
	T_FOR,
	T_SWITCH,
	T_CASE,
	T_GOTO,
	T_BREAK,
	T_CONTINUE,
	T_RETURN,
	T_DEFAULT,
	T_TYPEDEF,

	//types
	T_INT,
	T_LONG,
	T_SHORT,
	T_FLOAT,
	T_DOUBLE,
	T_CHAR,
	T_UNSIGNED,
	T_SIGNED,
	T_CONST,
	T_VOID,
	T_VOLATILE,
	T_ENUM,
	T_STRUCT,
	T_UNION,

	T_REGISTER,
	T_EXTERN,
	T_STATIC,
	T_SIZEOF,

	T_ID,
	T_NUM,

	// interim, eliminate later
	T_NUM_FLOAT,
	T_NUM_OCT,
	T_NUM_BIN,
	T_NUM_HEX,
	T_LITERAL,

	T_ARROW,

	T_CALOP_INC,
	T_CALOP_DEC,
	T_CALOP_LSHIFT,
	T_CALOP_RSHIFT,

	//BOOLOP ATTR
	T_BOOLOP_AND,
	T_BOOLOP_OR,

	T_PLUSEQUAL,
	T_MINUSEQUAL,
	T_MULEQUAL,
	T_DIVEQUAL,
	T_MODEQUAL,
	T_ANDEQUAL,
	T_OREQUAL,
	T_XOREQUAL,
	T_LSEQUAL,
	T_RSEQUAL,

	//RELOP ATTR
	T_RELOP_LE,
	T_RELOP_GE,
	T_RELOP_EQ,
	T_RELOP_NEQ,
};

//SYMBOL ATTRIBUTES
enum {
	ATTR_NULL = 0,

	ATTR_NUM_FLOAT,
	ATTR_NUM_DECIMAL,
	ATTR_NUM_OCT,
	ATTR_NUM_HEX,
	ATTR_NUM_BIN,
};

typedef int64_t symtype;
typedef int64_t symattr;

struct symbol {
	symtype type;
	symattr attr;
	const char* value;
};

struct wc {
	int lc;
	int bc;
	int comment;
	int keyword;
	int punc;
	int num;
	int literal;
	int id;
	int null;
};

extern const char* symname[]; //defined in symbol.c
extern const char* attrname[]; //defined in symbol.c
extern const symtype keyword_type[31];
extern const char* keywords[31];
extern const char* errorname[ERR_OFFSET+1];

// defined in lexer
extern char* yytext;
extern char stringbuf[MAXSTRING];
extern struct wc yywc;

// in lexer.c or lex.yy.c
int yylex();

// in symprint.c
void symprint(struct symbol *sym);

// in lexhelper.c
struct symbol lexhelper();
int yyappend(const char*);
void yystrinit();

#endif /* end of include guard: SYMBOL_H_Q2QNGONU */
