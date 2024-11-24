#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ./ + 0/ + 1/ + ... + n/

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
  char* filecode = "01";
  printf("Filecode: %s\n", filecode);
  char* path = getPathFromCode(filecode);
  printf("Path: %s\n",path);
  free(path);
  return 0;
}
