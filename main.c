
//  This is the first task for programming in my univesity.
//  I wonder if my teacher is going to get me overwhelmed
//  with difficult tasks from now on or just mentally hit
//  me for writing something so frightening.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

//  Defines are still better than magic.
#define README_FILENAME "readme.md"

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif





//  Function to use in listing the directory contents with scandir.

int filter (const struct dirent* name){
  if(name->d_name[strlen(name->d_name)-1]==kPathSeparator){
    //  Checking if it is a directory in an officially approved way.
    return 0; //  ...and not listing it.
  }
  //  Otherwise, listing the file.
  return 1;
}


//  Function to change
//  0+1+...+n
//  into
//  ./ + 0/ + 1/ + ... + n/
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
  return pathptr;     //  Is it safe? Aye.
}


//  Prints files with time delays between characters.
int printFileSlowly(const char* path){
  
  FILE* fileptr = fopen(path, "r");
  
  if(fileptr == NULL){  //  It may not be openable or existent.
    fprintf(stderr, "File on local path \"%s\" could not be opened.", path);
    return EXIT_FAILURE;
  }

  //  Do print the file whole.
  char ch;
  while((ch = fgetc(fileptr)) != EOF){
    //  Placeholder for a delay function.
    printf("%c", ch);
  }


  return EXIT_SUCCESS;
}






int main(){
  
  //  Starting from a folder that has to exist. The name can be changed, 
  //  but it is designed so that it consists of a single symbol. A 
  //  path-building algorithm is relying on it, so more symbols in a 
  //  directory name shall lead to unwanted behaviour.
  char* filecode = "0";
  char* path = getPathFromCode(filecode);
  //  "My path is set." - Tassadar.

  struct dirent** namelist; //  This shall hold a list of the files in the directory and their data.

  for(;;){  //  Main loop.

    int amount = scandir(path, &namelist, filter, alphasort);
    
    if(amount>0){ //  Read contents of the current directory and work with them if they exist.

      
      free(namelist); //  Kill the pointer only if it was defined.
    }


    else{ //  If the files can not be read, quit.
      if(strlen(filecode) == 1){  //  If it is the fisrt directory, print readme file.
        printFileSlowly(README_FILENAME); //  If it does not exist, the error shall be shown. 
      }
      //  Do quit.
      break;
    }
  
  }
  
  //  Ending it all.
  free(path);
  exit(EXIT_SUCCESS);
}
