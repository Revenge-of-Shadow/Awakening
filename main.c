
//  This is the first task for programming in my univesity.
//  I wonder if my teacher is going to get me overwhelmed
//  with difficult tasks from now on or just mentally hit
//  me for writing something so frightening.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

//  Defines are still better than magic.
#define START_DIRNAME     "0"
#define FILENAME_README   "readme.md"
#define FILENAME_TITLE    "title.txt"
#define FILENAME_TEXT     "text.txt"
#define FILENAME_OPTIONS  "options.txt"

const char kPathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif





//  Function to concatenate strings.
//  Warning: returns pointer. Caution is advised.
char* concat(const char* first, const char* second){
  
  char* result = malloc(strlen(first)+strlen(second)+1);  //  Lengths of both and a terminating null.
  
  strcpy(result, first);
  strcat(result, second);
  result[strlen(first)+strlen(second)] = '\0';

  return result;
}


//  Function to use in listing the directory contents with scandir.

int filter (const struct dirent* name){
  if(name->d_name[strlen(name->d_name)-1]==kPathSeparator){
    //  Checking if it is a directory in an officially approved way.
    return 0; //  ...and not listing it.
  }
  //  Otherwise, listing the file.
  return 1;
}


//  Function that runs through the file and counts the symbols.
int getFileSize(const char* path){
  FILE* fileptr = fopen(path, "r");
  fseek(fileptr, 0L, SEEK_END);
  int size = ftell(fileptr);
  fclose(fileptr);
  return size;
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

  fclose(fileptr);

  return EXIT_SUCCESS;
}






int main(){
  
  //  Starting from a folder that has to exist. The name can be changed, 
  //  but it is designed so that it consists of a single symbol. A 
  //  path-building algorithm is relying on it, so more symbols in a 
  //  directory name shall lead to unwanted behaviour.
  char* filecode = START_DIRNAME;
  char* path = getPathFromCode(filecode);
  //  "My path is set." - Tassadar.

  struct dirent** namelist; //  This shall hold a list of the files in the directory and their data.

  for(;;){  //  Main loop.

    int amount = scandir(path, &namelist, filter, alphasort);
    
    if(amount>0){ //  Read contents of the current directory and work with them if they exist.

      for(int i = 0; i < amount; ++i){
        //  Iterating through the files.

        char* filepath = concat(path, namelist[i]->d_name); //  Create file path.
        
        printf("File: %s\n", filepath);

        // Regardless of the actaully existing files,
        // each iteration has to check for a set of 
        // files that can create a story node. As
        // such, title is implicitely obligatory, but
        // options are only needed for nodes that
        // redirect anywhere and not for endings.
        //
        // More explained in readme.
        if((strcmp(namelist[i]->d_name, FILENAME_TITLE) == 0) ||  //  Start with "title" file.
           (strcmp(namelist[i]->d_name, FILENAME_TEXT) == 0))     //  Text is treated the same way.
        {
          printFileSlowly(filepath);
          printf("\n\n");
        }

        else if(strcmp(namelist[i]->d_name, FILENAME_OPTIONS)){  //  This is where the fun begins.
          //  Print it properly and then let the user choose.
        }

        //  None other files than these three are of interest.


        free(filepath);     //  Each iteration it is allocated and destroyed.
        free(namelist[i]);  //  Destroy each separate pointer.
      }

      free(namelist); //  Kill the pointer only if it was defined.
    }


    else{ //  If the files can not be read, quit.
      if(strlen(filecode) == 1){  //  If it is the first directory, print readme file.
        printFileSlowly(FILENAME_README); //  If it does not exist, the error shall be shown. 
      }
      //  Do quit.
      break;
    }
  
  }
  
  //  Ending it all.
  free(path);
  exit(EXIT_SUCCESS);
}
