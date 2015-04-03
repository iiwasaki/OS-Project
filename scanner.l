%{
#include <stdio.h> 
#include <string.h>
#include "y.tab.h"
%}

%% 

\"						yylval=strdup(yytext); return QUOTE;
[<>|"\&]				return METACHARACTER;
setenv					return SETENV; 
printenv 				return PRINTENV;
unsetenv 				return UNSETENV; 
cd 						return CD; 
alias 					return ALIAS; 
bye 					return BYE; 
unalias 				return UNALIAS;
[A-Za-z_][A-Za-z0-9_]* 	yylval=strdup(yytext); return VARIABLE;
[^<>|"\&\n \t]+			yylval=strdup(yytext); return WORD;  
[ \t]+					/*ignore the whitespace*/;
\n 						/*ignore end of line*/;

%%

