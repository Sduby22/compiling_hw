#include "stdio.h"
#include "symbols.h"

int stack[10000] = {0};
int top = 0;

void push(int i) {
    stack[top++] = i;
}

void pusharr(int *arr, int len) {
    for (int i = len-1; i != -1; --i)
	push(arr[i]);
}

int pop(int num) { 
    if (top < num)
	return -1;
    top -= num;
    return 0;
}

int get() {
    if (top == 0)
	return -1;
    return stack[top-1];
}

void reset() {
    stack[0] = 0;
    top = 1;
}

void printstack(int isll) {
    printf("栈:$ ");
    for (int i = 0; i != top; i++)
	if (isll)
	    printf("%s ", getsymname(stack[i]));
	else 
	    printf("%d ", stack[i]);
}

void die(const char* str) {
    printf("%s", str);
}

