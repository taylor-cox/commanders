Installation - 
-There are three files - multi.c, multi.h, and multi.txt
-Command to run the file - gcc multi.c -o multi
-Run - ./multi 1 3

Description - 
-First using the file pointer, the commands are fetched inside the program.
-Secondly, two character pointer arrays are used to store the commands and manipulate them.

Data Structures being used for background tasks-
-Character pointer arrays are being used for storing commands.
-Fork is being used to maintain the child process.
-Wait/Waitpid is temporarily suspending the execution of a parent process while the child process is running.  

In linux we have used the signal library which contains the function kill(), which is used to kill unwanted processes. 
To keep a time record of the processes from beggining to end, a time library has been used.
These ensure that the program is executed in the background and is killed when work is done.
Furthermore, it is important to note that the argument maximum length can be 128 having 32 arguments, as the status is assigned to each task, 
which will confirm if process is running or not. After proper execution of the task, the kill command will kill the process.

