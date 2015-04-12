#include "seashell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

extern int yyrestart();
extern int yyparse();
extern int yylex();
extern int yylex_destroy(); 
extern int yy_scan_string();
extern int chdir(const char *path);
extern char* getcwd();
extern char **environ;  

static void printPrompt(){
	printf("\n>> ");
}

/* at initialization, 
create a local copy of the environment variables that can be scrolled through for
expansion later */
static void initialize_envs(){
	int i = 0;
		while(environ[i]){   
		const char* eq = "="; //all env vars are separated by name=value, therefore limit by '='
		char* ending = strstr(environ[i], &eq[0]);
		int dist = ending - environ[i]; //find distance between start and =, to get name 
		char* subString = (char*)malloc(dist); //allocate enough space to store name 
		strncpy(subString, environ[i], dist); //the subString now holds the name of the env var

		TABLE_ENVAR[i].varname = subString; //set the name in the local TABLE_ENVAR to be subString
		TABLE_ENVAR[i].varvalue = getenv(subString);  //use the name to get the value
		VARCOUNT++; //increment local count of env vars
	  	
	  	i += 1; //increment i for the while loop
	}
}

/* initialize the shell */
static void shell_init(){ 
	printf("\n--------------------------\n");
	printf("|   Welcome to the shell! |\n");
	printf("---------------------------\n");

	//initialize environment variables
	initialize_envs();

	//set present working directory
	chdir(getenv("PWD"));

	parsePaths();

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
	printf("Printing all environment variables: \n\n"); 

	/* EITHER ONE BELOW WORKS */

	// for (; i<VARCOUNT; ++i){
	// 	printf("%s = %s \n", TABLE_ENVAR[i].varname, TABLE_ENVAR[i].varvalue);
	// }

	while(environ[i]){
	   	printf("%s\n", environ[i++]);
	}

}

/* Set environment variable. Override if one exists, create a new one if it does not. */
int setenviro(char* var_name, char* var_value){
	int i =0;
	if (var_name == NULL){
		printf("Error on setenv: No variable name specified.\n");
		return 0;
	}
	if (var_value == NULL){
		printf("Error on setenv: No variable value specified.\n");
		return 0;
	}
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
			if (strcmp(var_name, "PATH") == 0){
				parsePaths();
			}
			printf("Variable %s successfully updated.\n", TABLE_ENVAR[envID].varname);
			return 1;
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
				if(strcmp(var_name, "PATH") == 0){
					parsePaths();
				}
				printf("New environment variable successfully created.\n ");
				return 1;
			}
		}
		else {
			/* NO SPACE */
			printf ("Max number of environment variables reached. \n");
			return 0;
		}

	}
	return 0; 
} 

/*unset environment variable */
int unsetenviro(char* var_name)
{ int i = 0; 
	if (var_name == NULL){
		printf("Unset env error: No variable name specified.\n");
		return 0;
	}
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
					return 1; 
				} 
		}
	if (found != 1){
		printf("Error: Cannot find specified variable. \n");
		return 0; 
	}
	return 0;
}


/* Create an alias if one does not exist. If it does, overwrite. */
int setalias( char* alias_name, char* alias_value){
	if (alias_value == NULL){
		printf("Error: Alias value is not specified.\n");
		return 0; 
	}
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
		return 1; 
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
			return 1; 
		}
		else {
			/* NO SPACE */
			printf ("Max number of aliases reached. \n");
			return 0;
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
int unsetalias(char* alias_name)
{ 	
	if (alias_name == NULL){
		printf("Unalias error: No alias specified. \n");
		return 0;
	}
	int i = 0; 
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
					return 1;
					break; 
				} 
		}
	if (found != 1){
		printf("Error: Cannot find the alias to unalias. \n");
		return 0; 
	}
	return 0;

}

/*reset the value of all aliases to unused */
static void resetalias(){
	int i = 0; 
	for (; i<ALIASCOUNT; i++){
		TABLE_ALIAS[i].appeared = 0;
	}
	furthestalias = NULL;
	aliasnumber = 0;
}


/* Change directory */
int change_directory(char *directory_path)
{
	if (directory_path == NULL){
		chdir(getenv("HOME"));
		char* buff = malloc(400);
		setenviro("PWD", getcwd(buff, 400));
		printf("Changed to HOME directory. \n");
		return 1; 
	}
	if(!chdir(directory_path))
	{
	if(strcmp(directory_path, getenv("PWD")) == 0)
		{
			printf("Current working directory is unchanged.\n");
			return 0;
		}
	else
		{
			char* buff = malloc(400);
			setenviro("PWD", getcwd(buff, 400));
			return 1; 
		}
		return 0;
}
else
	{
	printf("Directory Not Found\n");
	return 0;
	}
}



static void do_it(){
	switch(TABLE_COMMAND[currentCommand].builtcmd){
		case BYE: 
			RUNNING = 0;
			printf("Bye!!! \n");
			break; 

		case SETENV:  
			succ = setenviro(TABLE_COMMAND[currentCommand].aptr.args[0], TABLE_COMMAND[currentCommand].aptr.args[1]);
			break;

		case UNSETENV:
			succ = unsetenviro(TABLE_COMMAND[currentCommand].aptr.args[0]);
			break;

		case PRINTENV: 
			printenv();
			break; 

		case ALIAS: 
			if(TABLE_COMMAND[currentCommand].aptr.args[0] == NULL){
				listalias();
			}
			else {
				succ = setalias(TABLE_COMMAND[currentCommand].aptr.args[0], TABLE_COMMAND[currentCommand].aptr.args[1]);
				}
			break;

		case UNALIAS:
			succ = unsetalias(TABLE_COMMAND[currentCommand].aptr.args[0]);
			break; 

		case CD:
			succ = change_directory(TABLE_COMMAND[currentCommand].aptr.args[0]);
			break;
	}
}

static void execute(){
	if (isExecutable() == -1){
		printf("Not executable");
	}
	else{
		if (inredir == 1){
			int sxs;
			sxs = access(ifile, R_OK);
			if (sxs == 0){
				printf("Input is successful\n");
			}
			else {
				printf("IO Not findable");
				return;
			}
		}
		if (outredir == 1){
			int sxs;
			sxs = access(ofile, W_OK);
			if (sxs == 0){
				printf("Output is successful\n");
			}
			else {
				printf("Output IO Not findable");
				return;
			}
		}
		printf("Executable!");
	}
}

static void processCommand(){
			if (BUILT_IN && COMCOUNT == 1 && inredir == 0 && outredir == 0){
			do_it();
			}
			else if (ISALIAS){
			printf("Got here");
			strcat(str, "\n");
			yy_scan_string(str);
			resetINTS();//reset aliasing and others. 
			switch(getCommand()){
			case OK:
				processCommand();
				yylex_destroy();
				break;
			case SYSERR:
				printf("Syserr\n");
				break;
				}
			} //end elseif 

		else 
		{
			printf("Comm count is %d", COMCOUNT);
			execute();
		}
}

/* Clears the command table */
static void cleartable(){
	int i = 0;
	for (; i < COMCOUNT; i++){
		TABLE_COMMAND[i].name = NULL; 
		TABLE_COMMAND[i].inputfile = 0;
		TABLE_COMMAND[i].outputfile = 0; 
		TABLE_COMMAND[i].builtcmd = 0; 
		int j = 0; 
		for (; j<TABLE_COMMAND[i].argcount; j++){
			TABLE_COMMAND[i].aptr.args[j] = NULL;
		}
		TABLE_COMMAND[i].argcount = 0;
	}
}

int main(void){
	shell_init();

	while(RUNNING){
		resetINTS();
		printPrompt(); 
		switch ( getCommand()){
			case OK:
				printf("CASE OK");
				processCommand(); 
				break;
			case SYSERR:
				printf("PSyserr\n");
				break;
		}
		if (ISALIAS == 1){
			resetalias();
		}
		cleartable();
	}
	printf("\n\nGood bye!! \n\n");
	return 0; 
}

