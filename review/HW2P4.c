#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>


int main(int argc, char *argv[]) {
    int i, status;
    pid_t pid;
    int res, resWait;
    const char *cmd;
    pid = fork();
    
    if (pid<0) { // error occured
        fprintf(stderr,"Fork Failed");
        return 1;
    } else if (pid >0) { //parent process
        //I use as example my own directory path, you must provide YOUR OWN to run this program
        cmd="/home/joshg/Documents/comp-sys/review/HW2P4";
        char *arguments[] = {NULL};
        res = execvp(cmd,arguments);

        if (res==-1)
            fprintf(stderr,"Parent did not execute execl\n");
        } else { //child process
        fprintf(stderr,"Child says Hello\n");
        }
    return 0;
}