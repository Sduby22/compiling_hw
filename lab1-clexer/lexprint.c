/******************************************************************************
* File:             lexprint.c
*
* Author:           李子豪  
* Created:          10/01/21 
* Description:      提供打印符号结构体的函数
*****************************************************************************/

#include <stdio.h>
#include "symbol.h"

// SYMTYPE	SYMATTR		SYMVALUE
void symprint(struct symbol* sym) {
	const char *attr = "---";
        const char *type = sym->value;

	if (sym->attr < MAXATTR && attrname[sym->attr])
		attr = attrname[sym->attr];
	if (sym->type < MAXSYM && symname[sym->type])
		type = symname[sym->type];

	printf("%s\t%s\t%s\n", type, attr, sym->value);
}

