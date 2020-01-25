/*
 * Taylor Cox and Shika Pandey
 */

#ifndef BORING_H
#define BORING_H

#include <sys/time.h>
#include <sys/resource.h> 

int runCommand(char* cmd);
int printStatiscs(struct timeval tb, struct timeval ta,
			struct rusage rb, struct rusage ra);

#endif
