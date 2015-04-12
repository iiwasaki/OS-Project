shell: y.tab.o lex.yy.o shell.o redir.h
	gcc -o shell *.o

shell.o: shell.c seashell.h 
	gcc -c shell.c 

lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c 

y.tab.o: y.tab.c y.tab.h
	gcc -c y.tab.c 

y.tab.c y.tab.h: parser.y seashell.h
	yacc -dy parser.y 

lex.yy.c: scanner.l 
	flex scanner.l

clean: 
	-rm -f *.o lex.yy.c *.tab.* shell *.output