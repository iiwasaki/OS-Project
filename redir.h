#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void in_redir(){
		if (inredir == 1){
			int sxs;
			sxs = access(ifile, R_OK);
			if (sxs == 0){
				inputrefile = open(ifile, O_RDONLY);
				printf("Input is successful\n");
			}
			else {
				printf("IO Not findable");
				return;
			}
		}
		else{
			inputrefile = dup(STDIN_FILENO); //default input 
		}
}

static void out_redir(){
					if(outredir==1){
					if (isappend == 1){
						outputrefile = open(ofile, O_WRONLY|O_CREAT|O_APPEND, 00600);
						if (outputrefile == -1){
							printf("\nError: output file is not writable.\n");
							return; 
						}
					}

					else 
					{
						outputrefile = open(ofile, O_WRONLY|O_CREAT|O_TRUNC, 00600);
						if (outputrefile == -1){
							printf("\nError: Output file is not writable.\n");
							return;
						}
					}
					}
				else{ //no output redirection
					outputrefile = dup(STDOUT_FILENO);
				} 
}