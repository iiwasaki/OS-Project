#include "seashell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void printPrompt(){
	printf("\n>>");
}

static void shell_init(){
	printf("Welcome to the shell! \n");

	table[0].varname = "PATH";
	table[0].varvalue = getenv("PATH");
	
	table[1].varname = "HOME";
	table[1].varvalue = getenv("HOME");
}

int main(void){
	shell_init();

	// While(1){
	// 	printPrompt(); 

	// }
	printf("Env path is %s, value is %s", table[0].varname, table[0].varvalue);
	printf("Max variable count is %d", VARCOUNT);
	return 0; 
}

