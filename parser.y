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

%union 
{
	int variable; 
	char *builtincommand;
	char *word;
	char *whitespace;
	char *metacharacters; 
}

%token <word> WORD
%token <whitespace> WHITESPACE
%token <variable> VARIABLE

%token <metacharacters> LESSTHAN
%token <metacharacters> GREATERTHAN
%token <metacharacters> OR
%token <metacharacters> QUOTE
%token <metacharacters> BACKSLASH
%token <metacharacters> AND

%token <builtincommand> SETENV 
%token <builtincommand> PRINTENV 
%token <builtincommand> UNSETENV 
%token <builtincommand> CD
%token <builtincommand> ALIAS 
%token <builtincommand> UNALIAS 
%token <builtincommand> BYE 


%% 

commands: /*empty */
		|commands command
		;

command: 
		plainword | quotedword | setenv | bye | printenv | unsetenv | cd | alias | unalias
		;

plainword:
		WORD
		{
			printf("\t Input word is '%s'", $1);
		}
		;

quotedword:
		QUOTE WORD QUOTE
		{
			printf("\t Quoted word is '%s'", $2);
		}
		|
		QUOTE WORD WHITESPACE QUOTE
		{
			printf("\t Quoted word is '%s%d'", $2, $3);
		}
		|
		QUOTE WHITESPACE WORD QUOTE
		{
			printf("\t Quoted word is '%s%d'", $2, $3);
		}
		;

setenv: 
 		SETENV VARIABLE WORD {printf("\t variable setenv and word\n");}
 		;

bye: 
	BYE {printf("\t Bye back!! \n");}
	;
	
printenv: 
	PRINTENV {printf("\t Print Env selected \n");}
	;
	
unsetenv: 	
	UNSETENV {printf("\t Unset Env selected \n");}
	;
cd:
	CD {printf("\t CD selected \n");}
	;


alias:
	ALIAS {printf("\t Alias selected \n");}
	;

unalias: 
	UNALIAS {printf("\t Unalias selected \n");}
	;

%%