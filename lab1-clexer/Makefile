CC=clang
CFLAGS=-g
all: lex_main mylexer_main

mylexer_main: symbol.o lexer.o lexprint.o main.o lexhelper.o
	${CC} $^ -o $@
lex_main: symbol.o lex.yy.o lexprint.o main.o lexhelper.o
	${CC} $^ -o $@
main.o: main.c symbol.h
symbol.o: symbol.c symbol.h
lexprint.o: lexprint.c symbol.h
lex.yy.o: lex.yy.c symbol.h
lexer.o: lexer.c symbol.h
lexhelper.o: symbol.h lexhelper.c
lex.yy.c: lex.l
	lex < lex.l

clean:
	rm -f *.o *_main lex.yy.c
