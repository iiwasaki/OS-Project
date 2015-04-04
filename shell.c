#include "seashell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

extern int yyparse();
extern int yylex();

static void printPrompt(){
	printf("\n>> ");
}

/* initialize the shell */
static void shell_init(){ 
	printf("\n----------------------------\n");
	printf("|   Welcome to the shell!  |\n");
	printf("----------------------------\n");

	TABLE_ENVAR[0].varname = "PATH"; //set PATH
	TABLE_ENVAR[0].varvalue = getenv("PATH");
	
	TABLE_ENVAR[1].varname = "HOME"; //set HOME
	TABLE_ENVAR[1].varvalue = getenv("HOME");

	VARCOUNT = 2; 

	RUNNING = 1; 
}

/* Use the parser to receive commands */
static int getCommand(){ 
	if(yyparse())
	{
		printf("Error in yyparse\n");
		return SYSERR; 
	}
	else 
		return OK; 
}

static void printenv(){
	int i = 0;
	printf("Printing all environment variables: \n\n"); 
	for (; i<VARCOUNT; ++i){
		printf("%s: %s \n", TABLE_ENVAR[i].varname, TABLE_ENVAR[i].varvalue);
	}
}

static void 

static void do_it(){
	switch(builtin){
		case BYE: 
			RUNNING = 0;
			printf("Bye!!! \n");
			break; 
		case SETENV: 
			break; 
		case UNSETENV: 
			break;
		case PRINTENV: 
			printenv();
			break; 
		case ALIAS: 
			break;
		case UNALIAS: 
			break; 
		case CD:
			break;
	}
}

static void processCommand(){
	if (builtin){
		do_it();
	}
	else 
	{
		printf("Do nothing for now ");
	}
}

int main(void){
	shell_init();

	while(RUNNING){
		printPrompt(); 
		switch ( getCommand()){
			case OK:
				processCommand(); 
				break;
			case SYSERR:
				printf("Syserr\n");
				break;
		}
	}
	printf("\n\nGood bye!! \n\n");
	// printf("Env path is %s, value is %s", TABLE_ENVAR[0].varname, TABLE_ENVAR[0].varvalue);
	// printf("Max variable count is %d", VARCOUNT);
	return 0; 
}

