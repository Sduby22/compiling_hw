#include "symbol.h"
#include "stdio.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


#define FILEBUF 20480
#define SYMBUF 2048

static char filebuf[2][FILEBUF];
static int filebufend[2];
static char symbuf[SYMBUF];
static int symbufpos;

static int currbuf=0, readbuf=0;
static int currpos=0;

char* yytext = symbuf;

static char peekch(int pos);
static char getch(int num);
static char eatch(int num);

static int getnum();
static int getkeyorid();
static int getstr();
static int getch_literal();
static int yyread();
static void syminit();

static void jumplinecom();
static void jumpmacro();

// -1 if unterminated.
static int jumpblockcom();

void die(const char* str) {
	printf("fatal: %s", str);
	exit(-1);
}

/**
 * @brief 识别下一个符号
 *
 * @return 返回识别到的符号类型
 */
int yylex() {
	char ch = peekch(0);
	int type;

	while(isspace(ch)) {
		if (ch == '\n')
			yywc.lc++;
		yywc.bc++;
		eatch(1);
		ch = peekch(0);
	}

	syminit();

	if (isalpha(ch) || ch == '_') {
		if ((type = getkeyorid()) > 0) {
			return type;
		} else {
			die("Failed to getid");
		}
	} else if (isdigit(ch)) {
		return getnum();
	} else switch (ch) {
		case '/':
			switch (peekch(1)) {
				case '/':
					jumplinecom();
					return yylex();
				case '*':
					if (jumpblockcom() == -1)
					// unterminated block comment
						return ERR_UNTERMINATED_BLOCK_COMMENT;
					return yylex();
				case '=':
					getch(2);
					return T_DIVEQUAL;
				default:
					return getch(1);
			}
		case '>':
			switch (peekch(1)) {
				case '>':
					if (peekch(2) == '='){
						getch(3);
						return T_RSEQUAL;
					} else {
						getch(2);
						return T_CALOP_RSHIFT;
					}
				case '=':
					getch(2);
					return T_RELOP_GE;
				default:
					return getch(1);
			}
		case '<':
			switch (peekch(1)) {
				case '<':
					if (peekch(2) == '='){
						getch(3);
						return T_LSEQUAL;
					} else {
						getch(2);
						return T_CALOP_LSHIFT;
					}
				case '=':
					getch(2);
					return T_RELOP_LE;
				default:
					return getch(1);
			}
		case '&':
			switch (peekch(1)) {
			case '&':
				getch(2);
				return T_BOOLOP_AND;
			case '=':
				getch(2);
				return T_ANDEQUAL;
			default:
				return getch(1);
			}
		case '|':
			switch (peekch(1)) {
			case '|':
				getch(2);
				return T_BOOLOP_OR;
			case '=':
				getch(2);
				return T_OREQUAL;
			default:
				return getch(1);
			}
		case '+':
			switch(peekch(1)) {
				case '+':
					getch(2);
					return T_CALOP_INC;
				case '=':
					getch(2);
					return T_PLUSEQUAL;
				default:
					return getch(1);
			}
		case '-':
			switch(peekch(1)) {
				case '>':
					getch(2);
					return T_ARROW;
				case '-':
					getch(2);
					return T_CALOP_DEC;
				case '=':
					getch(2);
					return T_MINUSEQUAL;
				default:
					return getch(1);
			}
		case '*':
			switch(peekch(1)) {
				case '=':
					getch(2);
					return T_MULEQUAL;
				default:
					return getch(1);
			}
		case '^':
			switch(peekch(1)) {
				case '=':
					getch(2);
					return T_XOREQUAL;
				default:
					return getch(1);
			}
		case '%':
			switch(peekch(1)) {
				case '=':
					getch(2);
					return T_MODEQUAL;
				default:
					return getch(1);
			}
		case '!':
			switch(peekch(1)) {
				case '=':
					getch(2);
					return T_RELOP_NEQ;
				default:
					return getch(1);
			} 
		case '=':
			if (peekch(1) == '=') {
				getch(2);
				return T_RELOP_EQ;
			}
			else 
				return getch(1);
		case '#':
			jumpmacro();
			return yylex();
		case '\"':
			return getstr();
		case '\'':
			return getch_literal();
		case '?': case '~': case ':': case ',': case ';': case '[': case ']': 
		case '{': case '}': case '(': case ')': case '.':
			return getch(1);
		case '0':
			return T_EOF;
		default:
			getch(1);
			return T_NULL;
	}
	return 0;
}

static int yyread() {
	int rd = 0;
	if (filebufend[currbuf] < FILEBUF) {
		rd = read(0, &filebuf[currbuf][filebufend[currbuf]], FILEBUF-filebufend[currbuf]);
		if (rd < 0)
			die("read failed.");
		filebufend[currbuf] += rd;
	} else {
		if (readbuf == currbuf)
			readbuf = !currbuf;
		if (FILEBUF-filebufend[readbuf] == 0)
			die("readbuf full.");
		rd = read(0, &filebuf[readbuf][filebufend[readbuf]], FILEBUF-filebufend[readbuf]);
		if (rd < 0)
			die("read failed.");
		filebufend[readbuf] += rd;
	}
	return rd;
}

/**
 * @brief 读取当前光标前第n个字符
 *
 * @param pos 正整数，小于FILEBUF
 * @return 返回当前光标前第n个字符
 */
static char peekch(int pos) {
	if (pos > FILEBUF)
		die("peek too much.");

	int npos = pos + currpos;
	int nbuf = currbuf;
	if (npos >= FILEBUF) {
		npos %= FILEBUF;
		nbuf = !nbuf;
	}

	while (npos >= filebufend[nbuf]) {
		if (yyread() == 0)
			return 0;
	}

	return filebuf[nbuf][npos];
}

/**
 * @brief 从当前光标读取n个字符放入symbuf中
 *
 * @param num 小于SYMBUF的正整数
 * @return 返回读取到的第一个字符
 */
static char getch(int num) {
	if (num > SYMBUF)
		die("getch too much.");

	int firsthalf, secondhalf;

	peekch(num-1);

	if (num + currpos >= FILEBUF) {
		firsthalf = FILEBUF-currpos;
		secondhalf = num-firsthalf;
	} else {
		firsthalf = num;
		secondhalf = 0;
	}

	char ret = filebuf[currbuf][currpos];

	memcpy(symbuf+symbufpos, &filebuf[currbuf][currpos], firsthalf);
	memcpy(symbuf+symbufpos+firsthalf, &filebuf[readbuf][0], secondhalf);
	symbufpos += num;

	eatch(num);
	return ret;
}

/**
 * @brief 从当前光标开始，跳过n个字符
 *
 * @param num 小于FILEBUF的正整数
 * @return 返回跳过的第一个字符
 */
static char eatch(int num) {
	if (num > FILEBUF)
		die("eat too much.");

	peekch(num-1);

	char ret = filebuf[currbuf][currpos];
	if (num + currpos >= FILEBUF) {
		filebufend[currbuf] = 0;
		memset(filebuf[currbuf], 0, FILEBUF);
		currbuf = readbuf;
		currpos = (num + currpos) % FILEBUF;
	} else {
		currpos += num;
	}

	return ret;
}

/**
 * @brief 设置symbuf，识别数字常量，包括浮点数，整数n进制。
 *
 * @return 如果识别成功，返回识别到的类型
 *	   否则返回T_NULL.
 */
static int getnum() {
	char ch;
	int i;
	int state = 1;
	int type;
	for (i=0;;) {
		ch = peekch(i++);
		int ret = 0;

		if (!isdigit(ch) && !isalpha(ch) && ch != '.') {
			getch(i-1);
			if (state == 6)
				return ERR_UNEXPECTED_IDENTIFIER;
			else 
				return type;
		}

		int err = 0;
		switch (state) {
		case 1: // init
			type = T_NUM;
			if (ch == '0')
				state = 2;	// hex oct 
			else if (isdigit(ch))
				state = 5;
			else err = 1;
			break;
		case 2: // number start with 0
			if (ch == 'x') {
				type = T_NUM_HEX;
				state = 6;
			} else if (ch == 'b') {
				type = T_NUM_BIN;
				state = 6;
			} else if (ch == '.') { 
				type = T_NUM_FLOAT;
				state = 4;
			} else if (isdigit(ch)) {
				type = T_NUM_OCT;
				state = 7;
			} 
			else err = 1;
			break;
		case 3: // hexinteger complete
			if (isdigit(ch) || (ch >= 'a' && ch <= 'f')) state = 3;
			else err = 1;
			break;
		case 4: // float
			type = T_NUM_FLOAT;
			if (isdigit(ch)) state = 4;
			else if (ch == 'f' || ch == 'l') state = 6;
			else err = 1;
			break;
		case 5: // integer
			if (ch == '.') {
				state = 4;
				type = T_NUM_FLOAT;
			}
			else if (isdigit(ch)) state = 5;
			else err = 1;
			break;

		case 6: //hex oct bin not complete
			if (isdigit(ch) || isalpha(ch)) {
				switch (type) {
				case T_NUM_HEX:
					state = 3;
					break;
				case T_NUM_BIN:
					state = 8;
					break;
				}
				i--;
			} 
			else err = 1;
			break;
		case 7: // oct integer complete
			if (isdigit(ch) && ch < '8') state = 7;
			else err = 1;
			break;
		case 8: // bin integer complete
			if (isdigit(ch) && ch < '2') state = 8;
			else err = 1;
			break;
			
		}
		if (err)
			break;
	}

	// unexpected identifier
	while (isalpha(ch) || ch == '_' || isdigit(ch) || ch == '.') {
		ch = peekch(i++);
	}
	getch(i-1);
	return ERR_UNEXPECTED_IDENTIFIER;

}

/**
 * @brief 调用yyappend() yystrinit()识别字符串
 *
 * @return 识别到返回长度，否则返回0
 */
static int getstr() {
	enum {
		IN_STR,
		EXCEED_STR
	};
	int i = 1;
	int state = IN_STR;
	yystrinit();
	char ch = peekch(i++);
	while (ch != '"') {
		int err = 0;
		switch (ch) {
		case '\\':
			if (peekch(i) == '\n')
				yywc.lc++;
			i++;
			break;
		case '\n':
			yywc.lc++;
			err = 1;
			break;
		}
		if (err) {
			getch(i);
			return ERR_UNTERMINATED_STRING_LITERAL;
		}
		ch = peekch(i++);
	}
	if (i>MAXSTRING)
		return ERR_MAX_STRING_LENGTH_EXCEEDED;
	getch(i);
	yyappend(symbuf);
	return T_LITERAL;
}

/**
 * @brief 设置symbuf，识别标识符
 *
 * @return 如果识别到则返回长度，否则返回0
 */
static int getkeyorid() {
	int i = 1;
	char ch = peekch(i);
	while (isdigit(ch) || isalpha(ch) || ch == '_') {
		ch = peekch(++i);
	}
	getch(i);
	
	for (int i = 0; i != 31; i++) {
		if (strcmp(symbuf, keywords[i]) == 0)
			return keyword_type[i];
	}

	return T_ID;
}

/**
 * @brief 设置symbuf，识别字符常量
 *
 * @return 识别到则返回类型，否则0
 */
static int getch_literal() {
	switch (peekch(1)) {
	case '\\':
		if (peekch(3) == '\'') {
			getch(4);	
			return T_NUM;
		} else {
			// Unterminated
			getch(3);
			return ERR_UNTERMINATED_CHAR_LITERAL;
		}
		break;
	case '\'':
			// Null char literal
		getch(2);
		return ERR_NULL_CHARACTER_LITERAL;
	case '\n':
			// unexpected new line
		yywc.lc++;
		getch(1);
		return ERR_UNTERMINATED_CHAR_LITERAL;
	default:
		if (peekch(2) == '\'') {
			getch(3);
			return T_NUM;
		} else {
			// Unterminated
			getch(2);
			return ERR_UNTERMINATED_CHAR_LITERAL;
		}
	}
}

/**
 * @brief 调用eatch跳过行注释
 *
 */
static void jumplinecom() {
	int i;
	for(i = 0; peekch(i) != '\n'; i++);
	eatch(i);
	yywc.bc+=i;
	yywc.comment++;
}

/**
 * @brief 调用eatch跳过宏
 *
 */
static void jumpmacro() {
	printf("macro\n");
	int i;
	for(i = 0; peekch(i) != '\n'; i++);
	eatch(i);
	yywc.bc+=i;
}

// -1 if unterminated.
static int jumpblockcom() {
	int i;
	for(i = 2; !(peekch(i) == '*' && peekch(i+1) == '/'); i++) {
		char ch = peekch(i);
		switch(ch) {
			case 0:	
				yywc.bc+=i;
				eatch(i);
				return -1;
				break;
			case '\n': yywc.lc++;
				break;
		}
	}
	eatch(i+2);
	yywc.bc+=i+2;
	yywc.comment++;
	return i+2;
}

/**
 * @brief 初始化符号缓冲数组
 *
 */
static void syminit() {
	symbufpos = 0;
	memset(symbuf, 0, sizeof(symbuf));
}
