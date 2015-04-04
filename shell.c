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

/* Print all environment variables */
static void printenv(){
	int i = 0;
	printf("Printing all environment variables: \n\n"); 
	for (; i<VARCOUNT; ++i){
		printf("%s: %s \n", TABLE_ENVAR[i].varname, TABLE_ENVAR[i].varvalue);
	}
}

/* Set environment variable. Override if one exists, create a new one if it does not. */
static void setenv(var_name, var_value){
	int i =0;
	int envID = -1; //default "does not exist"  
	for ( ; i < VARCOUNT; ++i){
		if (strcmp(var_name, TABLE_ENVAR[i].varname) == 0){
			envID = i; 
			break; 
		}
	}

	/* if the environment variable is to be overwritten */
	if (envID != -1){
		TABLE_ENVAR[i].varvalue = var_value;
		printf("Variable %s successfully updated.\n", TABLE_ENVAR.varname[i]);
	}
	/* if the environment variable is new */
	else {
		/* make sure there is space in the varcount */
		if (VARCOUNT < MAXENVS){
			/* IF THERE IS SPACE */
			TABLE_ENVAR[VARCOUNT].varname = var_name; 
			TABLE_ENVAR[VARCOUNT].varvalue = var_value;

			VARCOUNT++; 
			printf("New environment variable successfully created.\n ");
		}
		else {
			/* NO SPACE */
			printf ("Max number of environment variables reached. \n");
		}

	}
} 

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

