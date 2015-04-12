%{
#include <stdio.h> 
#include <string.h>
#include "y.tab.h"
#include "seashell.h"
%}


char* astring;

%%  
setenv						{wordcount++; return SETENV; }
printenv 					{wordcount++; return PRINTENV; }
unsetenv 					{wordcount++; return UNSETENV; }
cd 							{wordcount++; return CD; }
alias 						{wordcount++; return ALIAS; }
bye 						{wordcount++; return BYE; }
unalias 					{wordcount++; return UNALIAS; }
"2>"						{wordcount++; return ERDIRECT;}
"&1"						{wordcount++; return A1;}
"<"							{wordcount++; return LESSTHAN; }
">"							{wordcount++; return GREATERTHAN; }
"|"							{wordcount++; return PIPE; }
"&"							{wordcount++; return AMP;}

[ \t]+						/*ignore the whitespace for now*/; 

\n 							return NEWLINE;


([a-z]*(\\[\&<>|"\\])+[a-z]*)+		{ /* NEED TO TAKE CARE OF THIS */ yylval.string=strdup(yytext); 									
									return WORD; }

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
													
													int i = 0;
													for(; i<VARCOUNT; ++i)
													{
														if (strcmp(subString, TABLE_ENVAR[i].varname) == 0)
														{	
															yytext = (char*) malloc(strlen(TABLE_ENVAR[i].varvalue));
															strcpy(yytext, TABLE_ENVAR[i].varvalue);
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
								wordcount++;
								return LONGWORD;
							}		


[^<>|"\& \n]+ 				{yylval.string=strdup(yytext);
								if(wordcount == 0){
									int isAnAlias= aliasmatch(yytext);
									if (isAnAlias == -2){
										printf("Error: Infinite Alias loop for alias %s. Please reset your aliases. \n", yytext);
										return ALERROR;
									} 
										else if (isAnAlias == 1)
										{
										ISALIAS = 1;
										printf("Ova here!\n");
								        strcpy(str, furthestalias);
								        strcat(str, " ");
								        printf("%s", str);
								        wordcount++;
								    	return ALWORD;
										}
										else {
											wordcount++;
											return WORD;
										}
									}
							    else return WORD; 
							}

%%


int aliasmatch(char* name){
	int i=0;
		int match = -1;  
		for (; i < ALIASCOUNT; i++){
			if (strcmp(name, TABLE_ALIAS[i].aliasname) == 0 && TABLE_ALIAS[i].appeared > 0){ 
				return -2; 
			}
			else if (strcmp(name, TABLE_ALIAS[i].aliasname) == 0 && TABLE_ALIAS[i].appeared == 0){
				match = 1; 
				ISALIAS = 1;
				TABLE_ALIAS[i].appeared = 1; 
				int check = aliasmatch(TABLE_ALIAS[i].aliasvalue);
				if (check == -1 || check == 1){
					aliasnumber = i;
					return 1;
				}
				else if (check == -2){
					return -2;
				}
			}
		}
		if (match == -1){
		furthestalias = name;
		return -1; 
		}
	return -1;
}