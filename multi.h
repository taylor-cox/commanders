//
// Taylor Cox and Shikha Pandey
//
#ifndef SHELLPROGRAMMING_MULTI_H
#define SHELLPROGRAMMING_MULTI_H
#include <sys/time.h>
#include <sys/resource.h>

int LINE_MAX = 128;

int main(int argc, char *argv[]);
void printStatistics(struct timeval processBegin, struct timeval ProcessEnds, struct rusage statsToBePrinted); 
void taskExecute(char* commandLineArgs, int shouldWait, int localProc);

#endif //SHELLPROGRAMMING_MULTI_H
