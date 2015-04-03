%{
#include <stdio.h> 
#include <string.h>
#include "y.tab.h"
%}

%% 

\"							yylval.string=strdup(yytext); return QUOTE;
[<>|"\&]					return METACHARACTER;
setenv						return SETENV; 
printenv 					return PRINTENV;
unsetenv 					return UNSETENV; 
cd 							return CD; 
alias 						return ALIAS; 
bye 						return BYE; 
unalias 					return UNALIAS;
[^<>|"\& \n]+ 				yylval.string=strdup(yytext); return WORD;  
[ \t]+						yylval.string=strdup(yytext); return WHITESPACE;
\n 							/*ignore end of line*/;

%%

