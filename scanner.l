%{
#include <stdio.h> 
#include <string.h>
#include "y.tab.h"
#include "seashell.h"
%}

%% 

[^<>|"\& \n]*[$][{][^"\n{}]+[}][^<>|"\& \n]* 	{
													const char st[3] = "${";
													const char en[2] = "}";
													char* start = strstr(yytext, &st[1]); 
													char* end = strstr(yytext, &en[0]);
													int distance = end-start; 
													printf ("%d", distance); 
													printf("start: %s\n", start);
													printf("end: %s\n", end);
													char* subString = (char*)malloc(yyleng);
													strncpy(subString, start+1, distance-1);
													printf("%s", subString);
													
													int distToStart = start - yytext; 
													char* temp = (char*)malloc(distToStart);
													strncpy(temp, yytext, distToStart-1);
													printf("\n%s",temp);
													int i = 0;
													for(; i<VARCOUNT; ++i)
													{
														if (strcmp(subString, TABLE_ENVAR[i].varname) == 0)
														{	
															
															return WORD; 
														}
													}


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
