%{
#include <stdio.h> 
#include <string.h> 
#include "seashell.h"
#include <stdlib.h>

void yyerror(const char *str) 
{
	fprintf(stderr, "Error: %s\n", str);
}

int yywrap() 
{
	return 1;
}

%}

%token METACHARACTER SETENV PRINTENV UNSETENV CD ALIAS UNALIAS BYE NEWLINE

%union 
{
	char *string;
}

%token <string> WORD
%token <string> LONGWORD


%% 

commands: /*empty */
		|commands command
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
			
			YYABORT;
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
		CD_ARGS.args[0] = getenv("HOME");
		YYACCEPT;
	}
	|
	CD WORD NEWLINE
	{
		BUILT_IN = CD;
		CD_ARGS.args[0] = $2;
		YYACCEPT;
	}
	;


alias:
	ALIAS NEWLINE
	{
		BUILT_IN = ALIAS; 
		YYACCEPT;
	}
	;

unalias: 
	UNALIAS NEWLINE
	{
		BUILT_IN = UNALIAS; 
		YYACCEPT;
	}
	;