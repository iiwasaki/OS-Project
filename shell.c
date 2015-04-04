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

static void shell_init(){
	printf("Welcome to the shell! \n");

	table[0].varname = "PATH"; //set PATH
	table[0].varvalue = getenv("PATH");
	
	table[1].varname = "HOME"; //set HOME
	table[1].varvalue = getenv("HOME");

	VARCOUNT = 2; 


}

static int getCommand(){
	if(yyparse())
	{
		printf("Error in yyparse");
		return SYSERR; 
	}
	else 
		return OK; 
}

int main(void){
	shell_init();

	while(1){
		printPrompt(); 
		switch ( getCommand()){
			case OK: 
				printf("No error\n" );
				break;
			case SYSERR:
				printf("Syserr\n");
				break;
		}
	}
	// printf("Env path is %s, value is %s", table[0].varname, table[0].varvalue);
	// printf("Max variable count is %d", VARCOUNT);
	return 0; 
}

