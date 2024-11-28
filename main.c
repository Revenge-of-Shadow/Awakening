
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
#define OPTIONS_SEPARATOR ':'
#define TYPING_PAUSE      50            //  Milliseconds to wait between characters typed.

#ifdef _WIN32

const char kPathSeparator = '\\';

#include <conio.h>

#include <windows.h>
void sleepfor(int milliseconds){
  if(milliseconds < 0) return;
  Sleep(milliseconds);
}

#else

const char kPathSeparator = '/';

#define clrscr() system("clear");

#include <unistd.h>
#include <time.h>
void sleepfor(int milliseconds){
  if(milliseconds < 0) return;
  usleep(milliseconds * 1000);
}

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


//  Pseudobool function to check for a file.
int fileExistsAndCanBeOpened(const char* path){
  FILE* fptr = fopen(path, "r");
  int result = !(fptr == NULL);
  
  if(result)  fclose(fptr);

  return result;
}


//  Function that runs through the file and counts the symbols.
int getFileSize(const char* path){
  FILE* fileptr = fopen(path, "r");
  if(fileptr == NULL){
    return 0; //  Technically, it contains zero bytes if it does not exist.
  }

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
  const short pathlength = 3+strlen(filecode)*2; 
  
  char path [pathlength];   //  "./", then "n/", then terminator.
  path[0] = '.';
  path[1] = '/';
  for(int i = 0; i < strlen(filecode); ++i){
    path[2+i*2] = filecode[i];
    path[2+i*2+1] = '/';
  }
  path[pathlength-1] = '\0';
  
  char* pathptr = malloc(pathlength);
  strcpy(pathptr, path);
  return pathptr;     //  Is it safe? Aye.
}


//  Prints files with time delays between characters.
int printFileSlowly(const char* path){
  
  FILE* fileptr = fopen(path, "r");
  
  if(fileptr == NULL){  //  It may not be openable or existent.
    return EXIT_FAILURE;
  }

  printf("\n");   //  Cosmetic paragraph.
  //  Do print the file whole.
  char ch;

  while((ch = fgetc(fileptr)) != EOF){
    sleepfor(TYPING_PAUSE);
    putchar(ch);
    fflush(stdout);
  }

  fclose(fileptr);


  printf("\n");   //  Cosmetic paragraph.

  return EXIT_SUCCESS;
}




int main(){
  
  //  Starting from a folder that has to exist. The name can be changed, 
  //  but it is designed so that it consists of a single symbol. A 
  //  path-building algorithm is relying on it, so more symbols in a 
  //  directory name shall lead to unwanted behaviour.
  char* filecode = malloc(strlen(START_DIRNAME)); //  I do know it is 1.
  memcpy(filecode, START_DIRNAME, strlen(START_DIRNAME));
  //  "My path is set." - Tassadar.

  for(;;){  //  Main loop.

    clrscr();

    char* dirpath = getPathFromCode(filecode);
    char* filepath;

        //  Iterating through the files.

        // Regardless of the actaully existing files,
        // each iteration has to check for a set of 
        // files that can create a story node. As
        // such, title is implicitly obligatory, but
        // options are only needed for nodes that
        // redirect anywhere and not for endings.
        //
        // More explained in readme.
     
    //  First, title.
    filepath = concat(dirpath, FILENAME_TITLE); //  Create file path.
    printFileSlowly(filepath);
    free(filepath);

    //  Then, text.
    filepath = concat(dirpath, FILENAME_TEXT); //  Create file path.
    printFileSlowly(filepath);
    free(filepath);


    //  This is where the fun begins.
    filepath = concat(dirpath, FILENAME_OPTIONS); //  Create file path.
    if(fileExistsAndCanBeOpened(filepath))  {
              
          //  Print it properly and then let the user choose.
          
      const int filesize = getFileSize(filepath); //  Allows to store a lot of things in a char[] by getting the file size whole.
      char line [filesize];   //  Multifunctional variable to read lines.

      FILE* optionsFilePtr = fopen(filepath, "r");

      if(optionsFilePtr != NULL){

        int iteration = 0;  //  To mark the options.

        while(fgets(line, filesize, optionsFilePtr) != NULL){
          //  Got the options line; print the readable part if it exists.
          ++iteration;  //  Safety measures, really.

          for(int j = 0; j < strlen(line); ++j){
                
            if(line[j] == OPTIONS_SEPARATOR){ //  First occurence of : is located.
              printf("%d. %s\n", iteration, line + j + 1);  //  Print the option number and then the option text.
              break;  //  Line is divided and printed; go to the next one.
            }

          }

        } //  Options read end.

        rewind(optionsFilePtr);  //  Unread it instead of closing because it shall be used again.


        printf("\n\nType the option nubmer from the shown ones \nor type -1 to return to the previous choice\nor type 0 to quit\n then  press <Enter>.\nInput:\n");
        int option;

        do{
          fgets(line, filesize, stdin);
        }
        while(!sscanf(line, "%d", &option));
            

        //  If got 0, quit.
        if(option == 0){
          fclose(optionsFilePtr);
          free(filepath);   //  Free the filepath created for options file.
          free(dirpath);    //  Free the dirpath created at the beginning of the main loop.
          break;  //  Get out of the main loop.
        }
        //  If read is -1, go back. If possible.
        else if(option == -1 && strlen(filecode) > 1){
            filecode[strlen(filecode)-1] = '\0'; //  Safe.
        }
        //  Option is positive. Look for the fitting option.
        else if(option > 0){  

          //  Choice in algorigthm is based on the line order. There is no way anyone is going to write INT_MAX options.
          iteration = 0;

          while(fgets(line, filesize, optionsFilePtr) != NULL){

            if(++iteration == option){  //  The line corresponds the one that contains the needed option.
              for(int j = 0; j < strlen(line); ++j){
                if(line[j] == OPTIONS_SEPARATOR){
                  free(filecode);
                  filecode = malloc(j+1);
                  memcpy(filecode, line, j);  //  What is before : is saved as the filecode for redirection.
                  filecode[j] = '\0';

                  break;  //  Needed symbol is found; exit.
                }
              }
              break;  //  Needed line is found; exit.
            } 
          } //  Option seeking is over.
          
        }   //  "Option input" if-else chain end.
        
        fclose(optionsFilePtr);  // File is not NULL, close it.

      } //  "Options opened" 'if' end.
    } //  "Options file exists and can be opened; and so we open it" end.
    
    else{     //  Options were not opened. Maybe something else was. Maybe not.
      printf("\n\nType -1 to return to the previous choice\nor type 0 to quit\n then  press <Enter>.\nInput:");
      int option;
      const int linelength = 3; //  2 for -1 or 1 for 0 and... just in case there shall be a problem with \0, another 1.
      char line[linelength];

      do{
        fgets(line, linelength, stdin);
      }
      while(!sscanf(line, "%d", &option));
            
      //  If got 0, quit.
      if(option == 0){
        free(filepath);   //  Free the filepath created for options file.
        free(dirpath);    //  Free the dirpath created at the beginning of the main loop.
        break;  //  Get out of the main loop.
      }
      //  If read is -1, go back. If possible.
      if(option == -1 && strlen(filecode) > 1){
        filecode[strlen(filecode)-1] = '\0'; //  Safe.
      }
    }   //  Files if-else chain end.
    
    free(filepath);   //  Free the filepath created for options file.     
    

    //  None other files than these three are of interest.
   
    free(dirpath);
  
      
    }   //  Main loop end.
  
  //  Ending it all.
  free(filecode);
  exit(EXIT_SUCCESS);
}
