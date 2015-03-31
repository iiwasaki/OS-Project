test: y.tab.o lex.yy.o 
	gcc -o test y.tab.o lex.yy.o

lex.yy.o: lex.yy.c y.tab.h

y.tab.c y.tab.h: parser.y 
	yacc -dy parser.y 

lex.yy.c: scanner.l 
	lex scanner.l

clean: 
	-rm -f *.o lex.yy.c *.tab.* test *.output