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

int siblings, taskStatus;
pid_t processes[1000];
int lastBkrdProcess;

void printStatistics(struct timeval processBegin, struct timeval ProcessEnds, struct rusage statsToBePrinted) {
	printf("\n-- Statistics ---\n");
	printf("Elapsed time: %d milliseconds\n", (int)(1000*(ProcessEnds.tv_sec - processBegin.tv_sec) + (ProcessEnds.tv_usec - processBegin.tv_usec) / 1000));
	printf("Page Faults: %ld\n", statsToBePrinted.ru_majflt);
	printf("Page Faults (reclaimed): %ld\n", statsToBePrinted.ru_minflt);
	printf("-- End of Statistics --\n\n");
}

void taskExecute(char* commandLineArgs, int shouldWait, int localProc){
    printf("Running command: %s\n", commandLineArgs);
    struct rusage statsToBePrinted;
    // Making Two arrays: one command array, and another temporary array to be used in functioning
    char *arguments[1000], temporaryArray[1000];
    struct timeval processBegin, ProcessEnds;

	// Copying the array for temporary work
    strcpy(temporaryArray, commandLineArgs);
    
	// Checks arg length and command length
    if(strlen(temporaryArray) > 128) {
		printf("Argument Maximum length can be 128 having 32 arguments!\n");
    }

	// Checks waiting
    if(shouldWait){
        printf("Processs waiting\n");
    } else {
        printf("Process not waiting\n");
    }

	// Creating functionality for directory commands such as ccd, cpwd
    if(strlen(commandLineArgs) > 4 && strncmp(commandLineArgs, "ccd", 4) == 0) {
        // For loop for directory
        for(int k=0; k<4; k++){
			*commandLineArgs++;
		}
        chdir(commandLineArgs);
        printf("Directory is Changed to: %s   [Output of ccd]\n\n", commandLineArgs);
        return;
    } else if (strlen(commandLineArgs) >= 4 && strncmp(commandLineArgs, "cpwd", 4) == 0) {
        printf("Fetching Current directory: %s    [Output of cpwd]\n\n", getcwd(temporaryArray, 1000));
        return;
    } else if (strlen(commandLineArgs) >= 9 && strncmp(commandLineArgs, "cproclist", 9) == 0) {
		printf("NEED TO IMPLEMENT\n\n");
		return;
	}

    //Single for Loop being used 
    for (int i=0; commandLineArgs[i]; commandLineArgs[i] == ' ' ? i++ : *commandLineArgs++){}

    //Checking for max argument length
    for(int i=0; (arguments[i]=strtok(i == 0 ? temporaryArray : NULL, " ")) != NULL; i++) { }
    
	if((siblings=fork()) == 0){
        gettimeofday(&processBegin, NULL);
		// 
        if ((siblings=fork()) == 0){
            execvp(arguments[0], arguments);
            printf("Exec failed\n");
        } else {
            while(waitpid(siblings, &taskStatus, 0) == -1){}
            gettimeofday(&ProcessEnds, NULL);
            getrusage(siblings, &statsToBePrinted);
			printStatistics(processBegin, ProcessEnds, statsToBePrinted);
            kill(getpid(), SIGINT);
        }
    } else {
		if(!shouldWait) {
			printf("\n%d\n\n", getpid());
			processes[lastBkrdProcess] = siblings;
			lastBkrdProcess++;
		}
        while(shouldWait && waitpid(siblings, &taskStatus, 0) == -1) { }
    }
}

int main(int argc, char **argv){
    FILE* filePointer = fopen("multi.txt","r");
    char* singleLine = NULL;
    size_t singleLineLength = 0;
    int position = 0;
    int i = 1;
	lastBkrdProcess = 0;
	for(int j = 0; j < 1000; j++) {
		processes[j] = -1;
	}

    //Getting each line and executing the command
    while (getline(&singleLine, &singleLineLength, filePointer) != -1){
        //Fetching 
        singleLine[strcspn(singleLine, "\n")] = 0;
		if(singleLine[strlen(singleLine) - 1] == '\r') {
			singleLine[strlen(singleLine) - 1] = 0;
		}
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
	printf("%d\n", processes[0]);
	for(int j = 0; j < lastBkrdProcess; j++) {
		printf("%d\n", processes[j]);
		while(waitpid(processes[j], &taskStatus, 0) == -1) { }
	}
}
