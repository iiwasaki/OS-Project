%{
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include "seashell.h"

void yyerror(const char *str) 
{
	fprintf(stderr, "Error: %s\n", str); 
}

int yywrap() 
{
	return 1;
}

extern int yy_scan_string(char* string);

%}

%token SETENV PRINTENV UNSETENV CD ALIAS UNALIAS BYE NEWLINE LESSTHAN GREATERTHAN AMP PIPE ALERROR ERDIRECT A1

%union 
{
	char *string;
}

%token <string> ALWORD
%token <string> WORD
%token <string> LONGWORD


%% 
commands: /*empty */
		|commands command
		;

command:
		pipes input_redir output_redir error_redir background NEWLINE
		{
			printf("Successfully piped. \n"); 
			YYACCEPT;
		}
		|
		alis NEWLINE
		{
			printf("This is alias yes\n");
			printf("%d", ISALIAS);
			YYACCEPT;
		}
		NEWLINE
		{
			printf("Newl");
			YYACCEPT;
		}
		;

alis: 
	alis ali
	|
	;

ali: 
	ALWORD {
		ISALIAS = 1; 
		COMCOUNT++;
		printf("Recognized alias");
	}
	|
	WORD {
		printf("Adding \n");
		strcat(str, $1);
		strcat(str, " ");
		printf("\n%s", str);
	}
	|
	PIPE {
		strcat(str, "| ");
		printf("\n%s", str);
	}
	|
	SETENV {
		strcat(str, "setenv ");
		printf("\n%s", str);
	}
	|
	PRINTENV {
		strcat(str, "printenv ");
		printf("\n%s", str);
	}
	|
	UNSETENV {
		strcat(str, "unsetenv ");
		printf("\n%s", str);
	}
	|
	ALIAS {
		strcat(str, "alias ");
		printf("\n%s", str);
	}
	|
	UNALIAS { 
		strcat(str, "unalias ");
		printf("\n%s", str);
	}
	|
	BYE {
		strcat(str, "bye ");
		printf("\n%s", str);
	}
	|
	CD {
		strcat(str, "cd ");
		printf("\n%s", str);
	}
	|
	AMP {
		strcat(str, "& ");
		printf("\n%s", str);
	}
	|
	LESSTHAN {
		strcat(str, "< ");
		printf("\n%s", str);
	}
	|
	GREATERTHAN {
		strcat(str, "> ");
		printf("\n%s", str);
	}
	|
	ALERROR NEWLINE { yyerror("Bad alias"); YYABORT; }
	;

background: 
		AMP 
		{
			printf("This will run in the background. \n");
			doInBackground = 1; 
		}
		|
		/*empty*/
		;

input_redir: 
		LESSTHAN WORD 
		{
			printf("Input will be redirected to %s\n", $2);
			ifile = $2;
			inredir = 1;
		}
		| 
		LESSTHAN LONGWORD
		{
			printf("Input will be redirected to %s\n", $2);
			ifile = $2;
			inredir = 1;
		}
		LESSTHAN 
		{
			printf("Error, no word supplied!\n");
		}
		|
		/*could be nothing*/
		;

output_redir: 
		GREATERTHAN GREATERTHAN WORD 
		{
			printf("Output will be appended to %s\n", $3);
			isappend = 1; 
			ofile = $3;
			outredir = 1;
		}
		|
		GREATERTHAN GREATERTHAN LONGWORD
		{
			printf("Output will be appended to %s\n", $3);
			isappend = 1; 
			ofile = $3;
			outredir = 1;
		}
		|
		GREATERTHAN WORD 
		{
			printf("Output will be redirected to %s\n", $2);
			ofile = $2;
			outredir = 1;
		}
		|
		GREATERTHAN LONGWORD 
		{
			printf("Output will be redirected to %s\n",$2);
			ofile = $2;
			outredir = 1;
		}
		|
		GREATERTHAN GREATERTHAN 
		{
			printf("Output error no file set\n");
			YYABORT; 
		}
		|
		GREATERTHAN 
		{
			printf("Output error no file set\n");
			YYABORT; 
		}
		|
		/*could be nothing*/
		;
error_redir: 
		ERDIRECT WORD 
		{
			printf("Error redirection to %s", $2);
			errorfile = $2;
			errorredir = 1; 
		}
		|
		ERDIRECT A1
		{
			printf("Default error redirection.\n");
			errorredir = 0;
		}
		|
		ERDIRECT
		{
			printf("Error: No error redirection file specified.\n");
			YYABORT; 
		}
		|
		{
			printf("Def error\n");
		}
		/*could be nothing*/
		;
pipes: 
		pipes PIPE cmdargs {COMCOUNT++;}
		|
		cmdargs {COMCOUNT++;}
		;
cmdargs:
		cmdword arguments
		;

arguments: 
		arguments argument
		|
		;
		
argument:
		WORD {
				printf("Argument %d is %s\n", TABLE_COMMAND[COMCOUNT].argcount +1, $1);
				TABLE_COMMAND[COMCOUNT].aptr.args[TABLE_COMMAND[COMCOUNT].argcount +1] = $1;
				TABLE_COMMAND[COMCOUNT].argcount++; 
			
		}

		|
		LONGWORD
		{
				TABLE_COMMAND[COMCOUNT].aptr.args[TABLE_COMMAND[COMCOUNT].argcount +1] = $1;
				TABLE_COMMAND[COMCOUNT].argcount++; 			
		}
		;

cmdword:
		WORD {
			TABLE_COMMAND[COMCOUNT].name=$1;
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = $1;
			printf("Cmd is %s\n", $1);

		}
		|
		LONGWORD{
			TABLE_COMMAND[COMCOUNT].name=$1;
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = $1;
			printf("Cmd longword is %s\n", $1);
		}
		|
		SETENV {
			TABLE_COMMAND[COMCOUNT].name = "SETENV";
			TABLE_COMMAND[COMCOUNT].builtcmd = SETENV; 
			TABLE_COMMAND[COMCOUNT].isbuilt = 1; 
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = "setenv";

			BUILT_IN = 1; 
		}
		|
		PRINTENV {
			TABLE_COMMAND[COMCOUNT].name = "PRINTENV";
			TABLE_COMMAND[COMCOUNT].builtcmd = PRINTENV;
			TABLE_COMMAND[COMCOUNT].isbuilt = 1;  
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = "PRINTENV";
			BUILT_IN = 1; 
		}
		|
		UNSETENV{
			TABLE_COMMAND[COMCOUNT].name = "UNSETENV";
			TABLE_COMMAND[COMCOUNT].builtcmd = UNSETENV; 
			TABLE_COMMAND[COMCOUNT].isbuilt = 1; 
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = "UNSETENV";
			BUILT_IN = 1; 
		}
		|
		CD {
			TABLE_COMMAND[COMCOUNT].name = "CD";
			TABLE_COMMAND[COMCOUNT].builtcmd = CD;
			TABLE_COMMAND[COMCOUNT].isbuilt = 1; 
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = "CD";
			BUILT_IN = 1; 
		}	
		|
		ALIAS {
			TABLE_COMMAND[COMCOUNT].name = "ALIAS";
			TABLE_COMMAND[COMCOUNT].builtcmd = ALIAS;
			TABLE_COMMAND[COMCOUNT].isbuilt = 1; 
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = "ALIAS";
			BUILT_IN = 1; 
		}
		|
		UNALIAS {
			TABLE_COMMAND[COMCOUNT].name = "UNALIAS";
			TABLE_COMMAND[COMCOUNT].builtcmd = UNALIAS;
			TABLE_COMMAND[COMCOUNT].isbuilt = 1; 
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = "UNALIAS";
			BUILT_IN = 1; 
		}
		|
		BYE {
			TABLE_COMMAND[COMCOUNT].name = "BYE";
			TABLE_COMMAND[COMCOUNT].builtcmd = BYE;
			TABLE_COMMAND[COMCOUNT].isbuilt = 1;  
			TABLE_COMMAND[COMCOUNT].aptr.args[0] = "BYE";
			BUILT_IN = 1; 
		}
		;

%%
