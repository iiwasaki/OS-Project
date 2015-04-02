%{#include <stdio.h> 
#include "y.tab.h"
%}

%% 

[A-Za-z_][A-Za-z0-9_]* 	yylval.word=strdup(yytext); return WORD;
[0-9]+					yylval.variable=atoi(yytext); return VARIABLE;
[ \t]+					yylval.whitespace=strdup(yytext); return WHITESPACE;
\<						yylval.metacharacters=strdup(yytext); return LESSTHAN;
\>						yylval.metacharacters=strdup(yytext); return GREATERTHAN;
\|						yylval.metacharacters=strdup(yytext); return OR; 
\"						yylval.metacharacters=strdup(yytext); return QUOTE;
\\						yylval.metacharacters=strdup(yytext); return BACKSLASH;
\&						yylval.metacharacters=strdup(yytext); return AND;

setenv		yylval.builtincommand=strdup(yytext); return SETENV; 
printenv 	yylval.builtincommand=strdup(yytext); return PRINTENV;
unsetenv 	yylval.builtincommand=strdup(yytext); return UNSETENV; 
cd 			yylval.builtincommand=strdup(yytext); return CD; 
alias 		yylval.builtincommand=strdup(yytext); return ALIAS; 
bye 		yylval.builtincommand=strdup(yytext); return BYE; 
unalias 	yylval.builtincommand=strdup(yytext); return UNALIAS; 

\n; 		/*ignore end of line*/

%%

