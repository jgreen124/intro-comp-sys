#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    long i,j = 0;
    pid_t pid;
    
    i++; 
    printf("Process %d: i = %ld\n", getpid(), i);
    
    pid = fork();
    i+=7;
    for (j=0; j<27000; j++) {
        if(j%2==0) {
            i+=(j+1);
        }
        else {
            i+=j;
        }
    }

    printf("Process %d (Parent: %d): i = %ld\n", getpid(), getppid(), i);
    
    pid = fork();
    i+=17;
    printf("Process %d (Parent: %d): i = %ld\n", getpid(), getppid(), i);
    
    pid = fork();
    i+=23;
    printf("Process %d (Parent: %d): i = %ld\n", getpid(), getppid(), i);
    
    return 0;
}