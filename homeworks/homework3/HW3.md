|                | r1   | r2   | r3   | r5   | r6   | r7   |
|----------------|------|------|------|------|------|------|
|No Line Removed |  8   | -1   |  2   |  4   |  2   |  0   | 
| Line A removed |  8   | -1   |  2   |  4   |  2   |  0   | 
| Line B removed |  8   |  8   |  2   |  4   |  3   |  0   | 
| Line C removed |  8   | -1   |  2   |  4   |  2   |  0   | 
| Line D removed |  8   | -1   |  2   |  4   |  2   |  Child PID   | 



# For No Lines Removed 
- r1 = 8: first read blocks until data written to pipe
- r3 = 2: reads 2 bytes from pipe
- r5 = 4: reads 4 bytes from pipe
- r6 = 2: reads last 2 bytes from pipe before we write more data to pipe since sleep in child process finishes before sleep in parent process
- r2 = -1: child closes pipe before write
- r7 = 0: waitpid returns 0 since state of child doesn't change

There is an orphan: parent process will finish before child process because of sleep(20).

# For line A removed
- r1 = 8: no sleep so write happens right away
- r3 = 2: reads 2 bytes from pipe
- r5 = 4: reads 4 bytes from pipe
- r6 = 2: sleep(5) ends before sleep(10), so we read last two bytes in pipe
- r2 = -1: pipe closes before write
- r7 = 0: waitpid returns 0 since state of child doesn't change

There is an orphan: parent process will finish before child process does.

# For line B removed
- r1 = 8: first read blocks until data written to pipe
- r3 = 2: read 2 bytes from pipe
- r5 = 4: read 4 bytes from pipe
- r2 = 8: write occurs before pipe closes here because of the sleep(5) and the second write not waiting on a sleep line.
- r6 = 3: reads 3 bytes because we wrote more data to pipe before reading here
- r7 = 0: waitpid returns 0 since state of child doesn't change

There is an orphan: parent process will finish before child process does.

# For line C removed
- r1 = 8: first read blocks until data written to pipe
- r3 = 2: read 2 bytes from pipe
- r5 = 4: read next 4 bytes from pipe
- r6 = 2: read last two bytes in pipe.
- r2 = -1: pipe is closed before we write here
- r7 = 0: waitpid returns 0 since state of child doesn't change.

There is an orphan: parent process will still finish before child process does.

# For line D removed
- r1 = 8: first read instruction blocks until data is written to pipe
- r3 = 2: read first two bytes from pipe
- r5 = 4: read next four bytes from pipe
- r6 = 2: read last two bytes from pipe
- r2 = -1: pipe is closed before we write here
- r7 = PID of child: the child will terminate before waitpid is called, which is a change of state.

There is a zombie. The child will exit before waitpid is called
