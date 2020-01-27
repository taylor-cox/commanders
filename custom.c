/*
 * Taylor Cox and Shika Pandey
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include "custom.h"
#include <string.h>

#define MAX_ARGS 100

int runCommand(char* cmd) {
	char* argv[MAX_ARGS];
	printf("Running command: %s\n", cmd);
	char new_cmd[MAX_ARGS];
	strcpy(new_cmd, cmd);
	char* token = strtok(new_cmd, " ");
	// Splits the string for the execvp command
	int counter = 0;
	while(token != NULL) {
		argv[counter] = token;
		token = strtok(NULL, " ");
		counter++;
	}

	argv[counter] = NULL;
	execvp(argv[0], argv);
	// Only returns if execvp fails
	return 1;
}

int printStatistics(struct timeval tb, struct timeval ta,
				struct rusage ub, struct rusage ua) {
	// Prints the statistics given rusage before and after and timevals before and after
	printf("\n-- Statistics ---\n");
	printf("Elapsed time: %ld milliseconds\n", 
					(ta.tv_usec - tb.tv_usec) / 1000);
	printf("Page Faults: %ld\n", ua.ru_majflt - ub.ru_majflt);
	printf("Page Faults (reclaimed): %ld\n", ua.ru_minflt - ub.ru_minflt);
	printf("-- End of Statistics --\n\n");
}

int main(int argc, char* argv[]) {
	// Commands to be run
	struct timeval before, after;
	struct rusage rbefore, rafter;
	
	FILE *fp;
	fp = fopen("custom.txt", "r");
	if(fp == NULL) {
		printf("ERROR! Could not open file.");
		return 1;
	}
	
	char command[MAX_ARGS];	

	// Main for loop to run all the commands in commands[]
	while(fgets(command, MAX_ARGS, fp) != NULL) {
		command[strcspn(command, "\n")] = 0;
		gettimeofday(&before, NULL);
		getrusage(RUSAGE_SELF, &rbefore);
		char* originalCommand = malloc(MAX_ARGS * sizeof(char));
		strcpy(originalCommand, command);
		if(strcmp(strtok(command, " "), "ccd") == 0) {
			char* dir = strtok(NULL, " "); 
			chdir(dir);
			printf("Running command: %s %s\n", command, dir);
			printf("Changed to directory: %s\n\n", dir);
			continue;
		} else if(strcmp(strtok(command, " "), "cpwd") == 0) {	
			char current[MAX_ARGS];
			printf("Running command: %s\n", command);
			printf("%s\n\n", getcwd(current, MAX_ARGS));
			continue;
		} else {
			pid_t id = fork();
			if(id == 0) {
				int returnCode = runCommand(originalCommand);
				if(returnCode != 0) printf("Error: Command did not run.\n");
				return returnCode;
			}
			wait(NULL);
		}
		gettimeofday(&after, NULL);
		getrusage(RUSAGE_SELF, &rafter);
		printStatistics(before, after, rbefore, rafter);
	}
	fclose(fp);
}
