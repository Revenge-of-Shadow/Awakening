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
  
  // Read files from a specified folder.
  struct dirent** namelist;
  int amount = scandir(path, &namelist, filter, alphasort); 
  // Function saved dirent** to the variable. Now I process it.
  if(amount != -1){
    for(int i = 0; i < amount; ++i){
      // Print the current entry.
      printf("Entry %d: %s\n", i, namelist[i]->d_name);

      if(namelist[i]->d_type == DT_REG){ // If it is a file, try to read it.
        // Path to the file must be assembled.
        char* filepath = malloc(strlen(path)+strlen(namelist[i]->d_name)+1);
        strcpy(filepath, path); //  Write the directory path to the file path.
        strcat(filepath, namelist[i]->d_name); // Append the filename to the filepath.
        filepath[strlen(path)+strlen(namelist[i]->d_name)] = '\0';
        // Now the operations can begin.
        
       // Preparing the file. 
        FILE *fptr;
  
        fptr = fopen(filepath, "r");
        fseek(fptr, 0L, SEEK_END);
        const int sz = ftell(fptr);
        fseek(fptr, 0L, SEEK_SET);
        //  File prepared; size calculated.

        printf("File: %s; size: %d.\n", filepath, sz);

        char buffer[sz];
        fgets(buffer, sz, fptr);

        printf("\n\"%s\" contents:\n%s\n\n", filepath, buffer);

        // Cleaning up after using the file.
        fclose(fptr);
        free(filepath);
      } //  "if the entry is a file" end.

      free(namelist[i]);
    } // end of "for through the dirents".

    free(namelist);
  }// end of "if amount of entries is not -1".

  free(path);

  return 0;
}
