all: lr ll
lr: lr.o stack.o symbols.o lex.yy.o lexer.o
ll: ll.o stack.o symbols.o lex.yy.o lexer.o
stack.o: stack.h
symbols.o: symbols.h
lex.yy.c: symbols.h lex.l
	lex < lex.l
