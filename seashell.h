/* 
seashell.h - Global header for shell 
Yufan Lin and Ishin Iwasaki COP4600 
*/

#define MAXCMDS 35
#define MAXARGS 100 
#define MAXALIAS 100 
#define MAXENVS 3

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




ENVVAR TABLE_ENVAR[MAXENVS];
int VARCOUNT;
int BUILT_IN; 
int RUNNING; 