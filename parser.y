%{
#include <stdio.h> 
#include <string.h> 

void yyerror(const char *str) 
{
	fprintf(stderr, "Error: %s\n", str);
}

int yywrap() 
{
	return 1;
}

main()
{	
	yyparse();
}

%}

%token METACHARACTER SETENV PRINTENV UNSETENV CD ALIAS UNALIAS BYE

%union 
{
	char *string;
}

%token <string> WORD
%token <string> WHITESPACE
%token <string> QUOTE

%type <string> quotedword

%% 

commands: /*empty */
		|commands command
		;

command: 
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

setenv: 
 		SETENV WORD WORD
 		{
 			printf("\t variable is '%s' and word is '%s'  \n", $2, $3);
 		}
 		|
 		SETENV WORD quotedword
 		{
 			printf("\t variable is '%s' and word is '%s'  \n", $2, $3);
 		}
 		;

quotedword:
			QUOTE WORD QUOTE
			{
				$$ = $2;
			}
			|
			QUOTE WHITESPACE QUOTE
			{
				$$ = $2;
			}
			;

bye: 
	BYE 
	{
		printf("\t Bye back!! \n");
	}
	;
	
printenv: 
	PRINTENV 
	{
		printf("\t Print Env selected \n");
	}
	;
	
unsetenv: 	
	UNSETENV 
	{
		printf("\t Unset Env selected \n");
	}
	;
cd:
	CD 
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