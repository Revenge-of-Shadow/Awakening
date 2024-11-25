#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

// ./ + 0/ + 1/ + ... + n/

int filter (const struct dirent* name){
  return 1;
}

char* getPathFromCode(const char* filecode){
  // Making the actual path string out of the digit code of the story node.
  const short pathlength = 2+strlen(filecode)*2; 
  
  char path [pathlength];   //  "./", then "n/", then terminator.
  path[0] = '.';
  path[1] = '/';
  for(int i = 0; i < strlen(filecode); ++i){
    path[2+i*2] = filecode[i];
    path[2+i*2+1] = '/';
  }
  
  char* pathptr = malloc(pathlength);
  strcpy(pathptr, path);
  return pathptr;     //  Is it safe?
}


int main(){
  char* filecode = "03";
  printf("Filecode: %s\n", filecode);
  char* path = getPathFromCode(filecode);
  printf("Path: %s\n",path);
  
  struct dirent** namelist;
  int amount = scandir(path, &namelist, filter, alphasort); 
  if(amount != -1){
    for(int i = 0; i < amount; ++i){
      printf("Entry %d: %s\n", i, namelist[i]->d_name);

      if(namelist[i]->d_type == DT_REG){
         char* titlepath;
         FILE *fptr;
  
        fptr = fopen(path, "r");
        fseek(fptr, 0L, SEEK_END);
        const int sz = ftell(fptr);
  
        printf("File: %s; size: %d.\n", titlepath, sz);

        fclose(fptr);

        free(titlepath);
      }

      free(namelist[i]);
    }
    free(namelist);
  }

  free(path);

  exit(EXIT_SUCCESS);
}
