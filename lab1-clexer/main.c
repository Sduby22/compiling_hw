/******************************************************************************
* File:             main.c
*
* Author:           Li Zihao  
* Created:          10/01/21 
* Description:      从标准输入流中读取整个文件,循环调用分析函数并返回词法分析的结果
*****************************************************************************/

#include "symbol.h"
#include "stdio.h"

int main() {
	struct symbol sym = {0,0,0};

	printf("SYMTYPE\tSYMATTR\tSYMVALUE\n");

	while((sym = lexhelper()).type != T_EOF) {
		if (sym.type > 0)
			symprint(&sym);
		else
			printf("line %d: %s %s\n", yywc.lc, 
				errorname[sym.type+ERR_OFFSET],sym.value);
	}

	printf(
"代码行数 : %d\n\
总字节数 : %d\n\
注释数 : %d\n\
关键字数 : %d\n\
标点符号数 : %d\n\
数字常量数 : %d\n\
字符串常量数 : %d\n\
标识符数 : %d\n\
未识别符号数 : %d\n",
		yywc.lc,
		yywc.bc,
		yywc.comment,
		yywc.keyword,
		yywc.punc,
		yywc.num,
		yywc.literal,
		yywc.id,
		yywc.null
	);

	return 0;
}
