#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define BUFSIZE 100

int main(int argc, char *argv[])
{


    int fd_pipe1[2],fd_pipe2[2];
    int ifd,ofd;
    int n=0,i,ret;
    int w_ret,read1,read2;
    char buf1[BUFSIZE+1],buf2[2]="0";
   
    
    char c;
    if(argc != 3)
    {
        printf("give only two arguments for file names");
        exit(-1);
    }
    ifd = open(argv[1], O_CREAT | O_RDONLY);
    if (ifd < 0) 
    {
      fprintf(stderr, "Unable to open input file in read mode...\n");
      exit(1);
    } 
    else 
    {
      fprintf(stderr, "New file descriptor obtained = %d\n", ifd);
    }

    ofd = open(argv[2], O_CREAT | O_WRONLY |O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
   if (ofd < 0) {
      fprintf(stderr, "Unable to open output file in write mode...\n");
      exit(2);
   } else {
      fprintf(stderr, "New file descriptor obtained = %d\n", ofd);
   }

   pipe(fd_pipe1);
   pipe(fd_pipe2);

 


if (fork() == 0) 
{

        close(fd_pipe1[1]);                                        // close write end of pipe1 
        close(fd_pipe2[0]);                                        //close read end of pipe2
        while((read2=read(fd_pipe1[0],&buf1,BUFSIZE)) >0)
        {

            strcpy(buf2,"0");
          
            write(ofd,&buf1,read2);
            if(read2==BUFSIZE)
            {
                buf2[1]=0;
                write(fd_pipe2[1],&buf2,1);

            }
            else 
            {
                strcpy(buf2,"0");
               
                write(fd_pipe2[1],&buf2,1);
                printf("filed copied successfully\n");
                exit(0);
            }
            

        }
        if(read1<0)
        {
            strcpy(buf2,"-1");
            write(fd_pipe2[1],&buf2,2);
            exit(1);
        }
           
        
            
        
    }
    else {

        close(fd_pipe1[0]);                                                          //close read end of pipe1
        close(fd_pipe2[1]);                                                           //close write end of pipe2

        
            while((read1=read (ifd, &buf1,BUFSIZE)) >0)
            {
                
                buf1[read1]=0;                                                   //end of string is assigned 0 
                write(fd_pipe1[1],&buf1,read1);
                
                
                read(fd_pipe2[0],&buf2,2);
                if(read1<BUFSIZE&&(!strcmp(buf2,"0")))
                    break;
                else if(!strcmp(buf2,"-1"))
                   break;
            }
           
            }
   close(ifd);
   close(ofd);
}