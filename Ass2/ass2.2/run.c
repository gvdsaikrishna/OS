#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	execlp("xterm","xterm","-hold","-e","./shell",NULL);
	return 0;
}
