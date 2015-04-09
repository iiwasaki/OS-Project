#include "seashell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

extern int yyparse();
extern int yylex();
extern int yylex_destroy(); 
extern int yy_scan_string();
extern int chdir(const char *path);
extern char **environ;  

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
	TABLE_ENVAR[2].varvalue = getenv("PWD");

	chdir(getenv("PWD"));

	CURRENT_WORKING_DIRECTORY = TABLE_ENVAR[2].varvalue; 

	VARCOUNT = 3; 

	ALIASCOUNT = 0; 
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
	// printf("Printing all environment variables: \n\n"); 
	// for (; i<VARCOUNT; ++i){
	// 	printf("%s = %s \n", TABLE_ENVAR[i].varname, TABLE_ENVAR[i].varvalue);
	// }
	while(environ[i]){
	  printf("%s\n", environ[i++]);
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
		int success = setenv(var_name, var_value, 1);
		if (!success){
			printf("Variable %s successfully updated.\n", TABLE_ENVAR[envID].varname);
			}
	}
	/* if the environment variable is new */
	else {
		/* make sure there is space in the varcount */
		if (VARCOUNT < MAXENVS){
			/* IF THERE IS SPACE */
			TABLE_ENVAR[VARCOUNT].varname = var_name; 
			TABLE_ENVAR[VARCOUNT].varvalue = var_value;

			int success = setenv(var_name, var_value, 1);

			VARCOUNT++; 
			if (!success){
				printf("New environment variable successfully created.\n ");

			}
		}
		else {
			/* NO SPACE */
			printf ("Max number of environment variables reached. \n");
		}

	}
} 

/*unset environment variable */
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
						TABLE_ENVAR[j].varvalue = TABLE_ENVAR[j+1].varvalue;
					}
					TABLE_ENVAR[VARCOUNT-1].varname = NULL; 
					TABLE_ENVAR[VARCOUNT-1].varvalue = NULL;
					VARCOUNT--;
					found = 1; 
					unsetenv(var_name);
					printf("Unset variable successfully\n");
					break; 
				} 
		}
	if (found != 1){
		printf("\t ERRORRR! CAN'T FIND!!! \n"); 
	}

}

/* Create an alias if one does not exist. If it does, overwrite. */
static void setalias( char* alias_name, char* alias_value){
	int i =0;
	int aliasID = -1; //default "does not exist"  
	for ( ; i < ALIASCOUNT; ++i){
		if (strcmp(alias_name, TABLE_ALIAS[i].aliasname) == 0){
			aliasID = i; 
			break; 
		}
	}

	/* if the alias is to be overwritten */
	if (aliasID != -1){
		TABLE_ALIAS[aliasID].aliasvalue = alias_value;
		printf("Alias %s successfully updated.\n", TABLE_ALIAS[aliasID].aliasname);
	}
	/* if the alias is new */
	else {
		/* make sure there is space in the alias count */
		if (ALIASCOUNT < MAXALIAS){
			/* IF THERE IS SPACE */
			TABLE_ALIAS[ALIASCOUNT].aliasname = alias_name; 
			TABLE_ALIAS[ALIASCOUNT].aliasvalue = alias_value;
			TABLE_ALIAS[ALIASCOUNT].appeared = 0; 

			ALIASCOUNT++; 
			printf("New alias successfully created.\n ");
		}
		else {
			/* NO SPACE */
			printf ("Max number of aliases reached. \n");
		}

	}
}
/* list all aliases */
static void listalias(){
	int i = 0;
	/* make sure there are aliases to show */
	if (ALIASCOUNT > 0 ){
			printf("Printing all aliases: \n\n"); 
	for (; i<ALIASCOUNT; ++i){
		printf("%s = %s \n", TABLE_ALIAS[i].aliasname, TABLE_ALIAS[i].aliasvalue);
		}
	}
	else 
	{
		printf("No aliases made. \n");
	}

}

/*unset the alias set */
static void unsetalias(char* alias_name)
{ 	int i = 0; 
	int found = -1; //default alias not found 
	for(; i < ALIASCOUNT; i++) 
		{ 
			if(strcmp(alias_name, TABLE_ALIAS[i].aliasname) == 0)
				{ 
					int j = i;
					for (; j<ALIASCOUNT;j++){
						TABLE_ALIAS[j].aliasname = TABLE_ALIAS[j+1].aliasname; 
						TABLE_ALIAS[j].aliasvalue = TABLE_ALIAS[j+1].aliasvalue;
					}
					TABLE_ALIAS[ALIASCOUNT-1].aliasname = NULL; 
					TABLE_ALIAS[ALIASCOUNT-1].aliasvalue = NULL;
					ALIASCOUNT--;
					found = 1; 
					printf("Unalias is done successfully\n");
					break; 
				} 
		}
	if (found != 1){
		printf("\t ERRORRR! CAN'T FIND!!! \n"); 
	}

}

/*reset the value of all aliases to unused */
static void resetalias(){
	int i = 0; 
	for (; i<ALIASCOUNT; i++){
		TABLE_ALIAS[i].appeared = 0;
	}
}


/* Change directory */
static void change_directory(char *directory_path)
{
if(!chdir(directory_path))
{
	if(strcmp(directory_path, CURRENT_WORKING_DIRECTORY) == 0)
		{
			printf("Current working directory is unchanged.\n");
		}
	else
		{
			setenviro("CURRENT WORKING DIRECTORY", directory_path);
			CURRENT_WORKING_DIRECTORY = directory_path;
		}
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
			setalias(ALIAS_ARGS.args[0], ALIAS_ARGS.args[1]);
			break;

		case ALIASSHOW: 
			listalias();
			break;

		case UNALIAS:
			unsetalias(ALIAS_ARGS.args[0]);
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
	else if (ISALIAS){
		yy_scan_string(alias_to_be_run);
		switch(getCommand()){
			case OK:
				processCommand();
				break;
			case SYSERR:
				printf("Syserr\n");
				break;
		}
		yylex_destroy();
	}
	else 
	{
		printf("Do nothing for now ");
	}
}

int main(void){
	shell_init();

	while(RUNNING){
		BUILT_IN = ISALIAS = 0;
		printPrompt(); 
		switch ( getCommand()){
			case OK:
				processCommand(); 
				break;
			case SYSERR:
				break;
		}
		if (ISALIAS == 1){
			resetalias();
		}
	}
	printf("\n\nGood bye!! \n\n");
	return 0; 
}

