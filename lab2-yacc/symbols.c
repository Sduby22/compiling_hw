#include "symbols.h"
#include <ctype.h>
#include <string.h>

static char buf[100];

const char* getsymname(int sym) {
    if(isprint(sym)) {
	buf[0] = sym;
	buf[1] = 0;
    } else {
	strcpy(buf, symname[sym]);
    }
    return buf;
}

const char* symname[] = {
    [SYM_EOF]="$",
    [NUM]="NUM",
    [E_]="E'",
    [E]="E",
    [T]="T",
    [T_]="T'",
    [F]="F",
};
