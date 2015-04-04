#include "seashell.h"
#include <stdio.h>
#include <string.h>


int main(void){
	shell_init();

	// While(1){
	// 	printPrompt(); 

	// }
	printf("Env path is %s, value is %s", table[0].varname, table[0].varvalue);
	printf("Max variable count is %d", VARCOUNT);
	return 0; 
}

static void printPrompt(){
	printf("\n>>");
}

static void shell_init(){
	printf("Welcome to the shell! \n");

	table[0].varname = "PATH";
	table[0].varvalue = getenv("PATH");
	VARCOUNT = 1; 
}