# filecopy
A program, developed in C, that uses pipes to copy a file.

The inputs are accepted at the command line in this format: ./filecopy file.source file.destination

The program first creates a shared pipe, then forks.
The parent process opens the specified file by the user then writes the contents to the pipe.
The child process then opens the read end of the pipe and creates the new file specified the user and writes the contents to the file.
