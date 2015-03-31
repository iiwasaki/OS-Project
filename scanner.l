%{include <stdio.h> 
#include "y.tab.h"
%}

%% 

setenv		return SETENV; 
printenv 	return PRINTENV;
unsetenv 	return UNSETENV; 
cd 			return CD; 
alias 		return ALIAS; 
bye 		return BYE; 
unalias 	return UNALIAS; 

\"[^"\n]* {
	int counter = 0; 
	for (counter = 0; counter < yyleng; ++counter){
		yytext[counter] = yytext[counter+1];
	}
	printf("NEw text is %s\n", yytext );
	return WORD; 
} 

[A-Za-z_][A-Za-z0-9_]* 	return VARIABLE;

\n; 		/*ignore end of line*/

%%

