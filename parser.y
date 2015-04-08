%{
#include <stdio.h> 
#include <string.h> 
#include "seashell.h"

void yyerror(const char *str) 
{
	fprintf(stderr, "Error: %s\n", str); 
}

int yywrap() 
{
	return 1;
}

%}

%token METACHARACTER SETENV PRINTENV UNSETENV CD ALIAS UNALIAS BYE NEWLINE ALIASSHOW

%union 
{
	char *string;
}

%token <string> WORD
%token <string> LONGWORD


%% 

commands: /*empty */
		|commands command
		|error NEWLINE {return 1;}
		;

command: 
		newline
		|
		setenv 
		| 
		bye 
		| 
		printenv 
		| 
		unsetenv 
		| 
		cd 
		| 
		alias 
		| 
		unalias
		|
		word
		;

newline:
		NEWLINE
		{
			printf("\n");
		}

setenv: 
 		SETENV WORD WORD NEWLINE
 		{
 			BUILT_IN = SETENV;
 			ENV_ARGS.args[0] = $2;
 			ENV_ARGS.args[1] = $3;  
			YYACCEPT;
 		}
 		|
 		SETENV WORD LONGWORD NEWLINE
 		{
 			BUILT_IN = SETENV; 
 			ENV_ARGS.args[0] = $2;
 			ENV_ARGS.args[1] = $3;  
			YYACCEPT;
 		}
 		|
 		SETENV NEWLINE
 		{
 			yyerror("No arguments for setenv operation.");
 			return 1; 
 		}
 		;


bye: 
	BYE NEWLINE
	{
		BUILT_IN = BYE; 
		YYACCEPT;
	}
	;
	
printenv: 
	PRINTENV NEWLINE
	{
		BUILT_IN = PRINTENV;
		YYACCEPT;
	}
	;
	
unsetenv: 	
	UNSETENV WORD NEWLINE 
	{
		BUILT_IN = UNSETENV; 
		ENV_ARGS.args[0] = $2;
		YYACCEPT;
	}
	;
cd:
	CD NEWLINE
	{
		BUILT_IN = CD; 
		YYACCEPT;
	}
	;


alias:
	ALIAS WORD LONGWORD NEWLINE 
	{
		BUILT_IN = ALIAS; 
		ALIAS_ARGS.args[0] = $2;
		ALIAS_ARGS.args[1] = $3;
		YYACCEPT; 
	}
	|
	ALIAS WORD WORD NEWLINE
	{
		BUILT_IN = ALIAS; 
		ALIAS_ARGS.args[0] = $2;
		ALIAS_ARGS.args[1] = $3;
		YYACCEPT; 
	}
	|
	ALIAS WORD NEWLINE 
	{
		yyerror("Missing a name or value for Alias.\n");
		return 1; 
	}
	|
	ALIAS NEWLINE
	{
		BUILT_IN = ALIASSHOW; 
		YYACCEPT;
	}
	;

unalias: 
	UNALIAS WORD NEWLINE
	{
		BUILT_IN = UNALIAS; 
		ALIAS_ARGS.args[0] = $2;
		YYACCEPT; 
	}
	|
	UNALIAS NEWLINE
	{
		yyerror("Missing the alias name to unalias.\n"); 
		return 1;
	}
	;

word: 
	WORD
	{
		int i=0;
		int match = -1;  
		for (; i < ALIASCOUNT; ++i){
			if (strcmp($1, TABLE_ALIAS[i].aliasname) == 0 && TABLE_ALIAS[i].appeared > 0){ 
				yyerror ("Circular aliasing, infinite loop. \n");
				return 1; 
			}
			else if (strcmp($1, TABLE_ALIAS[i].aliasname) == 0 && TABLE_ALIAS[i].appeared == 0){
				match = 1; 
				ISALIAS = 1;
				alias_to_be_run = strcat(TABLE_ALIAS[i].aliasvalue, "\n"); 
				YYACCEPT; 
			}
		}
		if (match == -1){
		yyerror("Unrecognized command. Please check your input. \n");
		return 1;
		}
	}
