#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define READ_END	0
#define WRITE_END	1

int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "An insufficent amount of arguments were provided.\n");
		return 1;
	}
	else
	{
		//Initialize variables
		pid_t pid;
		int fd[2];
		char * src = argv[1];
		char * dest = argv[2];
		
		//Get the size of the file being copied
		struct stat st;
		stat(src, &st);
		size_t SIZE = st.st_size;
		
		//Initialize arrays to the size of the file
		char write_msg[SIZE];
		char read_msg[SIZE];
		
		//Create pipe
		if(pipe(fd) == -1)
		{
			fprintf(stderr,"Pipe failed");
			return 1;
		}
		
		//Fork
		pid = fork();
		
		//Error has occurred while forking
		if (pid < 0)
		{
			fprintf(stderr, "Fork has failed.\n");
			return 1;
		}
		//Parent Process
		else if (pid > 0)
		{
			//Close the read end of the pipe
			close(fd[READ_END]);
			
			//Open a file that can only be read from
			int file = open(src, O_RDONLY);
			
			//Read from the file into write_msg
			read(file, write_msg, SIZE);
			
			//Write into the pipe
			write(fd[WRITE_END], write_msg, strlen(write_msg));
			
			//Close the write end of the pipe
			close(fd[WRITE_END]);
		}
		//Child Process
		else
		{
			//Close the write end of the pipe
			close(fd[WRITE_END]);
			
			//Read file contents from the pipe
			read(fd[READ_END], read_msg, SIZE);
			
			//Close the read end of the pipe
			close(fd[READ_END]);
			
			//Open a file, or create one if one doesn't exist
			int file = open(dest, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
			
			//Write the contents of read_msg to the file
			write(file, read_msg, strlen(read_msg));
		}
	}
	return 0;
}