%{
#include <stdio.h> 
#include <string.h>
#include "y.tab.h"
%}

%% 

["][^"\n]*["] 				{
								int counter = 0;
								char* subString = (char*)malloc(yyleng);
								for (counter = 0; counter < yyleng-2; ++counter) {
		
									subString[counter] = yytext[counter+1];
		
								} 
								yytext = subString;
								yylval.string=strdup(yytext);
								return LONGWORD;
							}		

[<>|"\&]					return METACHARACTER;
setenv						return SETENV; 
printenv 					return PRINTENV;
unsetenv 					return UNSETENV; 
cd 							return CD; 
alias 						return ALIAS; 
bye 						return BYE; 
unalias 					return UNALIAS;
[^<>|"\& \n]+ 				yylval.string=strdup(yytext); return WORD;  
[ \t]+						/*ignore the whitespace for now*/; 

\n 							return NEWLINE;


%%
