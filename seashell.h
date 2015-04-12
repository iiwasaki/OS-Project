/* 
seashell.h - Global header for shell 
Yufan Lin and Ishin Iwasaki COP4600 
*/
#include <stdio.h>

#define MAXCMDS 35
#define MAXARGS 100 
#define MAXALIAS 100 
#define MAXENVS 100
#define MAXALIASLENGTH 100

#define OK 0 
#define SYSERR 1 
#define MEMERR 2  //for lex 
#define SYSCALLERR -1 

#define BADFD -2 
#define EOFILE 2 

/*table of environment variables */
typedef struct {
	char *varname; 
	char *varvalue; 
} ENVVAR; 

/*table of command line arguments */
typedef struct {
	char *args[MAXARGS];
} ARGTAB;

/*table of aliases */
typedef struct{
	int appeared; //checks to make sure there can be no circular aliases 
	char *aliasname; 
	char *aliasvalue; 
} ALIASES; 

/*Table of commands */
typedef struct{ 
	char* name;
	int inputfile; 
	int outputfile; 
	int argcount; 
	int builtcmd; //the built in command
	ARGTAB aptr; 

} COMMANDS; 

static void printcommand( COMMANDS command){
	printf("Command name: %s", command.name);
	printf("Command input? %d", command.inputfile);
	printf("Command.out? %d", command.outputfile);
	printf("How many arguments? %d", command.argcount);
}

ENVVAR TABLE_ENVAR[MAXENVS]; //table of environment variables
ALIASES TABLE_ALIAS[MAXALIAS]; //table of aliases
COMMANDS TABLE_COMMAND[MAXCMDS]; //table of commands 



ARGTAB ENV_ARGS;
ARGTAB ALIAS_ARGS; 
ARGTAB CD_ARGS;

char* alias_to_be_run; 

/* Command-related variables */
int COMCOUNT; //number of commands. Initialized at 0. 
int currentCommand; //current command being executed. 
int doInBackground; //do this command in the background 
int BUILT_IN; //is this a built in command? 

char* ifile; //input file to redirect to 
char* ofile; //output file to redirect to 
int ioredir; //is there in/out redirection? 1= yes, 0 = no. 

int VARCOUNT; 	//number of environment variables 
int ALIASCOUNT; //number of aliases

int RUNNING;  //is our shell running? 
int ISALIAS;    //is this an alias? 

int succ; //success flag

int wordcount;

int aliasmatch(char* name);
int aliasnumber; //what number is the current alias? 
char* furthestalias; //the deepest in the trail of aliases to be used
char* copiedalias; 
char str[100];
int aliastracker; //

void addToParser(char* string);

static void resetINTS(){
	BUILT_IN = wordcount = ISALIAS = COMCOUNT = currentCommand = doInBackground = ioredir = aliasnumber = 0;
}
