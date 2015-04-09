/* 
seashell.h - Global header for shell 
Yufan Lin and Ishin Iwasaki COP4600 
*/

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





ENVVAR TABLE_ENVAR[MAXENVS]; //table of environment variables
ALIASES TABLE_ALIAS[MAXALIAS]; //table of aliases

ARGTAB ENV_ARGS;
ARGTAB ALIAS_ARGS; 
ARGTAB CD_ARGS;

char* alias_to_be_run; 

int VARCOUNT; 	//number of environment variables 
int ALIASCOUNT; //number of aliases
int BUILT_IN; //is this a built in command?  
int RUNNING;  //is our shell running? 
int ISALIAS;    //is this an alias? 