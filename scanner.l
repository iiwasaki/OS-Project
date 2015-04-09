%{
#include <stdio.h> 
#include <string.h>
#include "y.tab.h"
#include "seashell.h"
%}

%% 

[^<>|"\& \n]*[$][{][^"\n{}]+[}][^<>|"\& \n]* 	{
													const char st[3] = "${";
													const char en[1] = "}";
													char* start = strstr(yytext, &st[1]); 
													char* end = strstr(yytext, en);
													int distance = end-start; 
													printf ("%d", distance); 
													printf("%s\n", start);
													char* subString = (char*)malloc(yyleng);
													strncpy(subString, start+1, distance-1);
													printf("%s", subString);
													return WORD;

												}

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
