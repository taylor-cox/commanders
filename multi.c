//
// Taylor Cox and Shikha Pandey
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>

int siblings, taskStatus, i;
void taskExecute(char* commandLineArgs, int shouldWait, int localProc){
    printf("Running command: %s\n", commandLineArgs);
    struct rusage statsToBePrinted;
    if (shouldWait){
        printf("Processs waiting\n");
    }else{
        printf("Process not waiting\n");
    }
    //Making Two arrays: one command array, and another temporary array to be used in functioning
    char *arguments[1000], temporaryArray[1000];
    //Copying the array for temporary work
    strcpy(temporaryArray, commandLineArgs);
    //Initiating the task while checking the time
    //For time, two variables will be used to indicate the start and end time of the process
    struct timeval processBegin, ProcessEnds;
    //Creating functionality for directory commands such as ccd, cpwd
    if (strlen(commandLineArgs) > 4 && strncmp(commandLineArgs, "ccd", 4) == 0){
        //For loop for directory
        for (int k=0; k<4; k++){*commandLineArgs++;}
        //Changing the directory
        chdir(commandLineArgs);
        //Logging the description of directory being changed
        printf("Directory is Changed to: %s   [Output of ccd]\n\n", commandLineArgs);
        return;
    }else if (strlen(commandLineArgs) >= 4 && strncmp(commandLineArgs, "cpwd", 4) == 0){
        //Showing the current directory
        printf("Fetching Current directory: %s    [Output of cpwd]\n\n", getcwd(temporaryArray, 1000));
        return;
    }else if (strlen(commandLineArgs) >= 9 && strncmp(commandLineArgs, "cproclist", 9) == 0){

    }
    //Single for Loop being used 
    for (i=0; commandLineArgs[i]; commandLineArgs[i] == ' ' ? i++ : *commandLineArgs++){}
    //Checking for max argument length
    if (strlen(temporaryArray) > 128 || i > 32) printf("Argument Maximum length can be 128 having 32 arguments!\n");
    for(i=0; (arguments[i]=strtok(i == 0 ? temporaryArray : NULL, " ")) != NULL; i++){}
    if ((siblings=fork()) == 0){
        //Seting process for beginning time
        gettimeofday(&processBegin, NULL);
        if ((siblings=fork()) == 0){
            execvp(arguments[0], arguments);
            printf("Exec failed\n");
        }else{
            while(waitpid(siblings, &taskStatus, 0) == -1){}
            //Getting the process end time
            gettimeofday(&ProcessEnds, NULL);
            getrusage(siblings, &statsToBePrinted);
            //Printf command for statistics
            printf("\n-- Statistics ---\nElapsed time: %d milliseconds\nPage Faults: %ld\nPage Faults (reclaimed): %ld\n-- End of Statistics --\n\n", (int)(1000*(ProcessEnds.tv_sec - processBegin.tv_sec) + (ProcessEnds.tv_usec - processBegin.tv_usec) / 1000), statsToBePrinted.ru_majflt, statsToBePrinted.ru_minflt);
            //Killing the process
            kill(getpid(),SIGINT);
        }
    }else{
        while(shouldWait && waitpid(siblings, &taskStatus, 0) == -1){}
    }
}
int main(int argc, char **argv){
    //Pointer for the file
    FILE * filePointer=fopen("multi.txt","r");
    //Fetching single Line
    char* singleLine=NULL;
    //Making a length variable
    size_t singleLineLength=0;
    //Making a position integer for the line
    int position=0;
    i=1;
    //Getting each line and executing the command
    while (getline(&singleLine, &singleLineLength, filePointer) != -1){
        //Fetching 
        singleLine[strlen(singleLine) - 1]='\0';
        //Checking for given arguments
        if (position + 1 < argc && i == atoi(argv[position + 1])){
            //Checking the position
            taskExecute(singleLine, 0, position);
            position++;
        }else{
            //Executing command
            taskExecute(singleLine, 1, position);
        }
        i++;
    }
}