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
 		SETENV WORD WORD
 		{
 			printf("\t variable is '%s' and word is '%s'  \n", $2, $3);
 		}
 		|
 		SETENV WORD LONGWORD
 		{
 			printf("\t variable is '%s' and word is '%s'  \n", $2, $3);
 		}
 		|
 		SETENV NEWLINE
 		{
 			printf("\t ERRORRR");
 		}
 		;


bye: 
	BYE NEWLINE
	{
		builtin = BYE; 
		YYACCEPT;
	}
	;
	
printenv: 
	PRINTENV NEWLINE
	{
		builtin = PRINTENV; 
		YYACCEPT; 
	}
	;
	
unsetenv: 	
	UNSETENV NEWLINE 
	{
		printf("\t Unset Env selected \n");
	}
	;
cd:
	CD NEWLINE
	{
		printf("\t CD selected \n");
	}
	;


alias:
	ALIAS 
	{
		printf("\t Alias selected \n");
	}
	;

unalias: 
	UNALIAS 
	{
		printf("\t Unalias selected \n");
	}
	;