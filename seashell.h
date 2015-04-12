/* 
seashell.h - Global header for shell 
Yufan Lin and Ishin Iwasaki COP4600 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXCMDS 35
#define MAXARGS 100 
#define MAXALIAS 100 
#define MAXENVS 100
#define MAXALIASLENGTH 100
#define MAXPATHS 100 

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
	int isbuilt; //is it built flag
	int pathtouse;
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

char* patharray[MAXPATHS]; //path variable array
int pathCount; 

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
int inredir; //is there in/out redirection? 1= yes, 0 = no. 
int outredir; 

int VARCOUNT; 	//number of environment variables 
int ALIASCOUNT; //number of aliases

int RUNNING;  //is our shell running? 
int ISALIAS;    //is this an alias? 

int succ; //success flag

int wordcount;

int erroneousCMD; 

int aliasmatch(char* name);
int aliasnumber; //what number is the current alias? 
char* furthestalias; //the deepest in the trail of aliases to be used
char* copiedalias; 
char str[100];
int aliastracker; //

void addToParser(char* string);

static void resetINTS(){
	BUILT_IN = wordcount = ISALIAS = COMCOUNT = currentCommand = doInBackground = 
	inredir = outredir = erroneousCMD = aliasnumber = 0;

}

static void parsePaths(){
	char copy[500];
	strcpy(copy, getenv("PATH"));
	char *token = strtok(copy, ":");
	int i = 0;
	pathCount = 0;
	while(token){
		patharray[i] = (char*)malloc(strlen(token));
		strcpy(patharray[i], token);
		pathCount++;
		token = strtok(NULL, ":");
		i++;
	}
	for (i=0; i<pathCount; i++){
		printf("Path is %s\n\n", patharray[i]);
	}
}

static int isExecutable(){
	int i = 0; 
	for (; i<COMCOUNT; i++){
		if(TABLE_COMMAND[i].isbuilt == 1 ){
			continue;
		}
		else {
		int error = -1;
		char * slash; 
		slash = strchr(TABLE_COMMAND[i].name, '/');
		if(!slash){
			int j = 0;				
			for (; j<pathCount;j++){
				char pathnm[200]; 
				strcpy(pathnm, patharray[j]);
				strcat(pathnm, "/");
				strcat(pathnm, TABLE_COMMAND[i].name);
				int exists = access(pathnm, X_OK);
				if (exists == 0){
					printf("This is the exec path %s", patharray[j]);
					TABLE_COMMAND[i].pathtouse = j;
					error = 0;
				} 
			}
		}
		else{
			int exists = access(TABLE_COMMAND[i].name, X_OK);
			if(exists == 0){
				error = 0; //opened!
			}
		}
		if(error != 0){
			erroneousCMD = i+1;
			printf("Command # %d is erroneous", erroneousCMD);
			return -1; //error can't open

		}
		}

	}
	return 1; //everything is executable
}
