#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	execlp("xterm","xterm","-hold","-e","./sort1",argv[1],NULL);
	return 0;
}