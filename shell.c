#include "seashell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

extern int yyparse();
extern int yylex();
extern int chdir(const char *path);

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

	TABLE_ENVAR[2].varname = "CURRENT WORKING DIRECTORY"; //set current working directory
	TABLE_ENVAR[2].varvalue = getenv("HOME"); 

	VARCOUNT = 3; 

	RUNNING = 1; 
}

/* Use the parser to receive commands */
static int getCommand(){ 
	if(yyparse())
	{
		return SYSERR;
	}
	else 
		return OK; 
}

/* Print all environment variables */
static void printenv(){
	int i = 0;
	printf("Printing all environment variables: \n"); 
	for (; i<VARCOUNT; ++i){
		printf("\n%s: %s \n", TABLE_ENVAR[i].varname, TABLE_ENVAR[i].varvalue);
	}
}

/* Set environment variable. Override if one exists, create a new one if it does not. */
void setenviro(char* var_name, char* var_value){
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
		TABLE_ENVAR[envID].varvalue = var_value;
		printf("Variable %s successfully updated.\n", TABLE_ENVAR[envID].varname);
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

static void unsetenviro(char* var_name)
{ int i = 0; 
	int found = -1; //not found
	for(; i < VARCOUNT; i++) 
		{ 
			if(strcmp(var_name, TABLE_ENVAR[i].varname) == 0)
				{ 
					int j = i;
					for (; j<VARCOUNT;j++){
						TABLE_ENVAR[j].varname = TABLE_ENVAR[j+1].varname; 
						TABLE_ENVAR[j].varvalue = TABLE_ENVAR[j+1].varname;
					}
					TABLE_ENVAR[VARCOUNT-1].varname = NULL; 
					TABLE_ENVAR[VARCOUNT-1].varvalue = NULL;
					VARCOUNT--;
					found = 1; 
					printf("Unset variable successfully\n");
					break; 
				} 
		}
	if (found != 1){
		printf("\t ERRORRR! CAN'T FIND!!! \n"); 
	}

}

static void change_directory(char *directory_path)
{
	if(!chdir(directory_path))
	{
		if(strcmp(directory_path, getenv("HOME")) == 0)
		{
			printf("You are already at home\n");
		}
		else
		{setenviro("CURRENT WORKING DIRECTORY", directory_path);}
	}
	else
	{
		printf("Directory Not Found\n");
	}
}

static void do_it(){
	switch(BUILT_IN){
		case BYE: 
			RUNNING = 0;
			break; 

		case SETENV: 
			setenviro(ENV_ARGS.args[0], ENV_ARGS.args[1]);
			break;

		case UNSETENV:
			unsetenviro(ENV_ARGS.args[0]);
			break;

		case PRINTENV: 
			printenv();
			break; 

		case ALIAS: 
			printf("\t Alias selected \n");
			break;

		case UNALIAS:
			printf("\t Unalias selected \n"); 
			break; 

		case CD:
			change_directory(CD_ARGS.args[0]);
			break;
	}
}

static void processCommand(){
	if (BUILT_IN){
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
				break;
		}
	}
	printf("\n\nGood bye!! \n\n");
	return 0; 
}

