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
 			BUILT_IN = SETENV; 
			YYACCEPT;
 		}
 		|
 		SETENV WORD LONGWORD
 		{
 			BUILT_IN = SETENV; 
			YYACCEPT;
 		}
 		|
 		SETENV NEWLINE
 		{
 			BUILT_IN = SETENV; 
			YYACCEPT;
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
	UNSETENV NEWLINE 
	{
		BUILT_IN = UNSETENV; 
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