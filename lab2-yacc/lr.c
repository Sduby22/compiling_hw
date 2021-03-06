#include "symbols.h"
#include "stack.h"

struct generator gen[9] = {
{E_, {E           }, 1 },
{E,  {E, '+', T   }, 3 },
{E,  {E, '-', T   }, 3 },
{E,  {T           }, 1 },
{T,  {T, '*', F   }, 3 },
{T,  {T, '/', F   }, 3 },
{T,  {F           }, 1 },
{F,  {'(', E, ')' }, 3 },
{F,  {NUM         }, 1 },
};

const int symtoentry[128] = {
    ['+'] = 0,
    ['-'] = 1,
    ['*'] = 2,
    ['/'] = 3,
    ['('] = 4,
    [')'] = 5,
    [NUM] = 6,
    [0]   = 7,
    [E_]  = 8,
    [E]   = 9,
    [T]   = 10,
    [F]   = 11,
};

struct action LR[16][12] = {
      //+     -      *      /      (      )       NUM    $      E'  E       T       F
/*0 */{{},    {},    {},    {},    {S,4}, {},     {S,5}, {},    {}, {G,1},  {G,2},  {G,3}},
/*1 */{{S,6}, {S,7}, {},    {},    {},    {},     {},    {ACC}, {}, {},     {},     {}},
/*2 */{{R,3}, {R,3}, {S,8}, {S,9}, {},    {R,3},  {},    {R,3}, {}, {},     {},     {}},
/*3 */{{R,6}, {R,6}, {R,6}, {R,6}, {},    {R,6},  {},    {R,6}, {}, {},     {},     {}},
/*4 */{{},    {},    {},    {},    {S,4}, {},     {S,5}, {},    {}, {G,10}, {G,2},  {G,3}},
/*5 */{{R,8}, {R,8}, {R,8}, {R,8}, {},    {R,8},  {},    {R,8}, {}, {},     {},     {}},
/*6 */{{},    {},    {},    {},    {S,4}, {},     {S,5}, {},    {}, {},     {G,11}, {G,3}},
/*7 */{{},    {},    {},    {},    {S,4}, {},     {S,5}, {},    {}, {},     {G,12}, {G,3}},
/*8 */{{},    {},    {},    {},    {S,4}, {},     {S,5}, {},    {}, {},     {},     {G,13}},
/*9 */{{},    {},    {},    {},    {S,4}, {},     {S,5}, {},    {}, {},     {},     {G,14}},
/*10*/{{S,6}, {S,7}, {},    {},    {},    {S,15}, {},    {},    {}, {},     {},     {}},
/*11*/{{R,1}, {R,1}, {S,8}, {S,9}, {},    {R,1},  {},    {R,1}, {}, {},     {},     {}},
/*12*/{{R,2}, {R,2}, {S,8}, {S,9}, {},    {R,2},  {},    {R,2}, {}, {},     {},     {}},
/*13*/{{R,4}, {R,4}, {R,4}, {R,4}, {},    {R,4},  {},    {R,4}, {}, {},     {},     {}},
/*14*/{{R,5}, {R,5}, {R,5}, {R,5}, {},    {R,5},  {},    {R,5}, {}, {},     {},     {}},
/*15*/{{R,5}, {R,5}, {R,7}, {R,7}, {},    {R,7},  {},    {R,7}, {}, {},     {},     {}},
};

void printgen(struct generator gen) {
    printf("%s -> ", getsymname(gen.from));
    if (gen.len == 0)
	printf("???");
    for (int i = 0; i != gen.len; i++)
	printf("%s ", getsymname(gen.to[i]));
}

int lr(int sym) {
    int goto_target;
    while(1) {
	int status = get();
	if (status == -1) {
	    die("status stack empty");
	    return STATUS_ERR;
	}
	printstack(0);
	printf("????????????: %s ", getsymname(sym));
	int symentry = symtoentry[sym];
	struct action act = LR[status][symentry];
	switch(act.type) {
	    case S:
		printf("??????????????????%d\n", act.go);
		push(act.go);
		// ??????????????????????????????????????????????????????????????????
		goto next_chr;
		break;
	    case R:
		printf("???????????????");
		printgen(gen[act.go]);
		/* printf("\n"); */
		if(pop(gen[act.go].len) == -1) {
		    die("pop too much.");
		    return STATUS_ERR;
		}
		// ?????????????????????????????????????????????????????????????????????????????????????????????
		// GOTO???
		goto_target = gen[act.go].from;
		if((status = get()) == -1) {
		    die("stack empty");
		    return STATUS_ERR;
		}
		symentry = symtoentry[goto_target];
		struct action act = LR[status][symentry];
		printf("GOTO: ???????????????%d\n", act.go);
		push(act.go);
		break;
	    case ACC:
		return STATUS_ACC;
		break;
	    case ACTION_ERR:
		return STATUS_ERR;
		break;
	    default:
		exit(1);
	}
    }
next_chr:
    return GOING;
}

int main() {
    push(0);
    int lex;
    int res = GOING;
    while((lex = yylex()) != ERR && res == GOING) {
	res = lr(lex);
	if (res == STATUS_ERR) {
	    die("???????????????SLR(1)????????????????????????");
	    exit(1);
	}
    }
    if (res == STATUS_ACC)
	printf("SLR(1)?????????????????????????????????\n");
};
