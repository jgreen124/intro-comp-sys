#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
/*
The variables are named argc (argument count) and argv (argument vector) by
convention, but they can be given any valid identifier.
argc will be the number of strings pointed to by argv.
This will (in practice) be 1 plus the number of arguments, as the name of the program
is prepended to the array.
So argc >= 1
*/
int main(int argc, char *argv[]) {
int i, status;
pid_t pid;
pid = fork();
    if (pid<0) { // error occured
    fprintf(stderr,"Fork Failed");
    return 1;
    }
    else if (pid ==0) { //child process
    
    // some experiment: use home dir or current dir as 3d arg
    //execlp("ls","ls-1","-laux","../../",NULL);
    //execl("pstree", "hello", "-np -C age");
    execl("pstree", "hello", "-np -C age", NULL);
    //execl("/usr/bin/ls", "ls", "luaqweswwqewql!?", "../../", NULL);")
    fprintf(stderr, "System Call execlp was unsuccessful - did not complete\n");
    }
else { //parent process
wait(NULL);
fprintf(stderr,"Child Complete\n");
}
return 0;
}
