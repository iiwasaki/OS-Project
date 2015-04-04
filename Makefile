shell.c: seashell.h 
	gcc -o shell shell.c 

lex.yy.o: lex.yy.c y.tab.h

y.tab.c y.tab.h: parser.y seashell.h
	yacc -dy parser.y 

lex.yy.c: scanner.l 
	lex scanner.l

clean: 
	-rm -f *.o lex.yy.c *.tab.* shell *.output