/*
 * Taylor Cox and Shika Pandey
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include "boring.h"
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
	char* commands[MAX_ARGS] = {"whoami", "last", "ls -la /home", NULL};
	struct timeval before, after;
	struct rusage rbefore, rafter;
	
	// Main for loop to run all the commands in commands[]
	for(int i = 0; commands[i] != NULL; i++) {
		pid_t id = fork();
		gettimeofday(&before, NULL);
		getrusage(RUSAGE_SELF, &rbefore);
		if(id == 0) {
			runCommand(commands[i]);
			printf("Error: Command did not run.\n");
		}
		wait(NULL);
		gettimeofday(&after, NULL);
		getrusage(RUSAGE_SELF, &rafter);
		printStatistics(before, after, rbefore, rafter);
	}
}
