/******************************************************************************
* File:             symbol.c
*
* Author:           李子豪  
* Created:          10/01/21 
* Description:      提供从整数类型到符号名称字符串的映射
*****************************************************************************/

#include "symbol.h"

const char* errorname[ERR_OFFSET+1] = {
	[ERR_UNTERMINATED_STRING_LITERAL+ERR_OFFSET] = "ERR_UNTERMINATED_STRING_LITERAL",
	[ERR_UNTERMINATED_CHAR_LITERAL+ERR_OFFSET] = "ERR_UNTERMINATED_CHAR_LITERAL",
	[ERR_UNTERMINATED_BLOCK_COMMENT+ERR_OFFSET] = "ERR_UNTERMINATED_BLOCK_COMMENT",
	[ERR_UNEXPECTED_IDENTIFIER+ERR_OFFSET] = "ERR_UNEXPECTED_IDENTIFIER",
	[ERR_NULL_CHARACTER_LITERAL+ERR_OFFSET] = "ERR_NULL_CHARACTER_LITERAL",
	[T_EOF+ERR_OFFSET] = "T_EOF",
	[T_NULL+ERR_OFFSET] = "ERR_UNEXPECTED_CHARACTER",
	[ERR_MAX_STRING_LENGTH_EXCEEDED+ERR_OFFSET] 
		= "ERR_MAX_STRING_LENGTH_EXCEEDED"
};

const char* symname[MAXSYM] = {
	[T_IF]           = "T_IF",
	[T_ELSE]         = "T_ELSE",
	[T_WHILE]        = "T_WHILE",
	[T_DO]           = "T_DO",
	[T_FOR]          = "T_FOR",
	[T_SWITCH]       = "T_SWITCH",
	[T_CASE]         = "T_CASE",
	[T_GOTO]         = "T_GOTO",
	[T_BREAK]        = "T_BREAK",
	[T_CONTINUE]     = "T_CONTINUE",
	[T_RETURN]       = "T_RETURN",
	[T_DEFAULT]      = "T_DEFAULT",
	[T_TYPEDEF]      = "T_TYPEDEF",
	[T_INT]          = "T_INT",
	[T_LONG]         = "T_LONG",
	[T_SHORT]        = "T_SHORT",
	[T_FLOAT]        = "T_FLOAT",
	[T_DOUBLE]       = "T_DOUBLE",
	[T_CHAR]         = "T_CHAR",
	[T_UNSIGNED]     = "T_UNSIGNED",
	[T_SIGNED]       = "T_SIGNED",
	[T_CONST]        = "T_CONST",
	[T_VOID]         = "T_VOID",
	[T_VOLATILE]     = "T_VOLATILE",
	[T_ENUM]         = "T_ENUM",
	[T_STRUCT]       = "T_STRUCT",
	[T_UNION]        = "T_UNION",
	[T_REGISTER]     = "T_REGISTER",
	[T_EXTERN]       = "T_EXTERN",
	[T_STATIC]       = "T_STATIC",
	[T_SIZEOF]       = "T_SIZEOF",
	[T_ID]           = "T_ID",
	[T_NUM]		 = "T_NUM",
	[T_LITERAL]      = "T_LITERAL",
	[T_ARROW]        = "T_ARROW",
	[T_CALOP_INC]    = "T_CALOP_INC",
	[T_CALOP_DEC]    = "T_CALOP_DEC",
	[T_CALOP_LSHIFT] = "T_CALOP_LSHIFT",
	[T_CALOP_RSHIFT] = "T_CALOP_RSHIFT",
	[T_BOOLOP_AND]   = "T_BOOLOP_AND",
	[T_BOOLOP_OR]    = "T_BOOLOP_OR",
	[T_RELOP_LE]     = "T_RELOP_LE",
	[T_RELOP_GE]     = "T_RELOP_GE",
	[T_RELOP_EQ]     = "T_RELOP_EQ",
	[T_RELOP_NEQ]    = "T_RELOP_NEQ",
	[T_PLUSEQUAL]  = "T_PLUSEQUAL",
	[T_MINUSEQUAL] = "T_MINUSEQUAL",
	[T_MULEQUAL]   = "T_MULEQUAL",
	[T_DIVEQUAL]   = "T_DIVEQUAL",
	[T_MODEQUAL]   = "T_MODEQUAL",
	[T_ANDEQUAL]   = "T_ANDEQUAL",
	[T_OREQUAL]    = "T_OREQUAL",
	[T_XOREQUAL]   = "T_XOREQUAL",
	[T_LSEQUAL]    = "T_LSEQUAL",
	[T_RSEQUAL]    = "T_RSEQUAL",
};

const char* attrname[MAXATTR] = {
	[0]                   = "---",

	[ATTR_NUM_FLOAT] = "ATTR_NUM_FLOAT",
	[ATTR_NUM_DECIMAL] = "ATTR_NUM_DECIMAL",
	[ATTR_NUM_OCT] = "ATTR_NUM_OCT",
	[ATTR_NUM_HEX] = "ATTR_NUM_HEX",
	[ATTR_NUM_BIN] = "ATTR_NUM_BIN",
};

const char* keywords[] = {
	"if",      
	"else",    
	"while",   
	"for",     
	"switch",  
	"case",    
	"goto",    
	"break",   
	"continue",
	"do",      
	"return",  
	"default", 
	"typedef", 
	"int",     
	"long",    
	"short",   
	"float",   
	"double",  
	"char",    
	"unsigned",
	"signed",  
	"const",   
	"void",    
	"volatile",
	"enum",    
	"struct",  
	"union",   
	"register",
	"extern",  
	"static",  
	"sizeof",  
};

const symtype keyword_type[] = {
	T_IF,
	T_ELSE,
	T_WHILE,
	T_FOR,
	T_SWITCH,
	T_CASE,
	T_GOTO,
	T_BREAK,
	T_CONTINUE,
	T_DO,
	T_RETURN,
	T_DEFAULT,
	T_TYPEDEF,
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
};