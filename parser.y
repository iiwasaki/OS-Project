%{#include <stdio.h> 
#include <string.h> 

void yyerror(const char *str) {fprintf(stderr, "Error: %s\n", str);}
int yywrap() {return 1;}
main(){yyparse();}
%}

%union {
	int i; 
	char *s; 
	char *w;
}

%token <i> SETENV PRINTENV UNSETENV CD ALIAS UNALIAS BYE 
%token <w> WORD
%token <s> VARIABLE 

%% 

commands: /*empty */
	|commands command;
command: 
	setenv|bye|printenv|unsetenv|cd|alias|unalias;

setenv: SETENV {printf("\t setenv selected\n");}
 	|SETENV WORD {printf("\t setenv with a word\n");}
 	|SETENV VARIABLE WORD {printf("\t variable setenv and word\n");};

bye: 
	BYE {printf("\t Bye back!! \n");};
	
printenv: 
	PRINTENV {printf("\t Print Env selected \n");};
	
unsetenv: 	
	UNSETENV {printf("\t Unset Env selected \n");};
cd:
	CD {printf("\t CD selected \n");};


alias:
	ALIAS {printf("\t Alias selected \n");};

unalias: 
	UNALIAS {printf("\t Unalias selected \n");};

%%