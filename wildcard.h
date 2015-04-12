#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
 
int wild_card_matching(const char *wild_card, const char *file_name) {

  const char *compare_pattern = NULL, *match_pattern = NULL;

  while ((*file_name) && (*wild_card != '*')) 
  {
    if ((*wild_card != *file_name) && (*wild_card != '?')) 
    {
      return 0;
    }
    wild_card++;
    file_name++;
  }

  while (*file_name) 
  {
    if (*wild_card == '*') 
    {
      if (!*++wild_card) 
      {
        return 1;
      }
      
      match_pattern = wild_card;
      compare_pattern = file_name+1;
    
    } 
    else if ((*wild_card == *file_name) || (*wild_card == '?')) 
    {
      wild_card++;
      file_name++;
    
    } 
    else 
    {
      wild_card = match_pattern;
      file_name = compare_pattern++;
    }
  }

  while (*wild_card == '*') 
  {
    wild_card++;
  }

  return !*wild_card;
}

static void list_file_names(char * string){

  char* buffer = malloc(200);
  char* CWD = getcwd(buffer, 200);

  DIR *directory;
  struct dirent *ent;
  directory = opendir(CWD);
  while((ent = readdir(directory)) != NULL)
  {
      if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
      {
        if(wild_card_matching(string, ent->d_name))
        {
          printf("%s\n", ent->d_name); 
        }
      }
  }
  printf("\n");
  closedir(directory);

}
