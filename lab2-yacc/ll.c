#include "symbols.h"
#include "stack.h"
#include <stdlib.h>

const int t2entry[128] = {
['+'] = 0,
['-'] = 1,
['*'] = 2,
['/'] = 3,
[NUM] = 4,
['('] = 5,
[')'] = 6,
[0]   = 7,
};

const int n2entry[128] = {
[E]  = 0,
[E_] = 1,
[T]  = 2,
[T_] = 3,
[F]  = 4
};

struct generator gen[11] = {
    {},
    {E, {T, E_}, 2},
    {E_, {'+', T, E_}, 3},
    {E_, {'-', T, E_}, 3},
    {E_, {}, 0},
    {T, {F, T_}, 2},
    {T_, {'*', F, T_}, 3},
    {T_, {'/', F, T_}, 3},
    {T_, {}, 0},
    {F, {'(', E, ')'}, 3},
    {F, {NUM}, 1},
};

int LL[5][8] = {
    {0,0,0,0,1,1,0,0},
    {2,3,0,0,0,0,4,4},
    {0,0,0,0,5,5,0,0},
    {8,8,6,7,0,0,8,8},
    {0,0,0,0,10,9,0,0},
};

void printgen(struct generator gen) {
    printf("%s -> ", getsymname(gen.from));
    if (gen.len == 0)
	printf("空");
    for (int i = 0; i != gen.len; i++)
	printf("%s ", getsymname(gen.to[i]));
}

int ll(int sym) {
    while(1) {
	printstack(1);
	printf("\t 输入字符:%s ", getsymname(sym));
	int n = get();
	// 符号栈为空，输入栈也为空，接受
	if (n == -1 && sym == SYM_EOF) 
	    return STATUS_ACC;
	// 只有符号栈为空，出错
	else if (n == -1)
	    return STATUS_ERR;
	if (n == sym) {
	    printf("终结符出栈\n");
	    pop(1);
	    break;
	} else {
	    int nentry = n2entry[n];
	    int tentry = t2entry[sym];
	    // 使用第ngen个生成式进行规约
	    int ngen = LL[nentry][tentry];
	    // 分析表中，没有对应项，即为错误
	    if (ngen == 0) {
		return STATUS_ERR;
	    }
	    pop(1);
	    printf("生成式: ");
	    printgen(gen[ngen]);
	    printf("\n");
	    // 把生成式的符号反向入栈
	    pusharr(gen[ngen].to, gen[ngen].len);
	}
    }
    return GOING;
}

int main() {
    push(E);
    int lex;
    int res = GOING;
    while(res == GOING) {
	lex = getsym();
	res = ll(lex);
	if (res == STATUS_ERR)
	    die("分析失败，LL(1)分析无法继续进行");
    }
    printf("LL(1)分析成功！分析过程如上\n");
}
