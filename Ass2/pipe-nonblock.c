/****
Program to illustrate nonblocking read;
Illustration uses pipe, method similar for 
any blocking file read
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>

int main()
{
	int id, fd[2], n, err, errno;
	char buffer[25];

	err = pipe(fd);
	if (err == -1)
	{
		perror("pipe error: ");
		exit(-1);
	}
	id = fork();
	if (id == -1)
	{
		perror("fork failed: ");
		exit(-1);
	}
	if (id != 0)
	{	
		close(fd[0]);
		printf("PARENT ID = %d, CHILD ID = %d\n", getpid(), id);
		strcpy(buffer, "TEST_STRING");

		// parent sleeps to give child chance to read to
		// check that it does not block
		sleep(5);

		// parent writes into pipe once only
		printf("PARENT: writing into pipe\n");
		n = write(fd[1], buffer, 100);
		if (n == -1)
		{
			perror("write failed:");
			exit(-1);
		}
	}
	else {
		close(fd[1]);

		// set the non-blocking flag for the fd, 
                // in this case the read end of the pipe
		fcntl(fd[0], F_SETFL, O_NONBLOCK);

		// Now try to read ad long as data is not found
		n = -1;	
		while (n == -1)
		{
			n = read(fd[0], buffer, 100);
			if (n == -1)
			{
				// check if error is due to no data
				// or some other error
				if (errno == EAGAIN) 
				{
					// no data yet. Wait for 1 second
					// and try again	
					printf("CHILD: Waiting for data\n");
					sleep(1);
					continue;
				}
				else {
					perror("write failed:");
					exit(-1);
				}
			}
			else {
				printf("CHILD: Data read: %s\n", buffer);	
			}
		}
	     }	
}
