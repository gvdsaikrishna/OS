#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

int main()
{
	int size=100;
	char *fir_str,*sec_str,*thi_str;
	fir_str = (char*)malloc(100*sizeof(char));
	sec_str = (char*)malloc(100*sizeof(char));
	thi_str = (char*)malloc(100*sizeof(char));
	char* buffer = (char*)malloc(100*sizeof(char));
	char* input = (char*)malloc(100*sizeof(char));
	char* result = (char*)malloc(100*sizeof(char));
	char* result1 = (char*)malloc(100*sizeof(char));
	char* result2 = (char*)malloc(100*sizeof(char));
	char* temp = (char*)malloc(100*sizeof(char));
	DIR *cur_dir;
	struct dirent *cur_file;
	int argv_count;
	struct stat status;
    int fstatus,i,pipe_status = 0,status3,pid;
    int fd[4][2];
    int fd1,fd2;
        while(1)
        {

			printf("%s>",getcwd(buffer,size));
			

			
			fgets(input,100,stdin);

			if(input[0]=='\n') continue;
			if(input[strlen(input)-1]=='\n') input[strlen(input)-1]='\0';

			fir_str = strtok_r(input," ",&result);
			
            sec_str = strtok_r(NULL," ",&result);
            //printf("%s\n",sec_str);
            result1 = result;
            if(strcmp(fir_str,"exit")==0)
            {
            	exit(0);
            }

            else if(strcmp(fir_str,"cd")==0)
            {
            	if(sec_str==NULL)
            	{
            		printf("shdj");

            		chdir("/home");
            		continue;
            	}
            	else
            	{
            		if(chdir(sec_str)==-1)
            			perror("");
            		else
            			chdir(sec_str);
            	}
            }

            else if(strcmp(fir_str,"pwd")==0)
            {
            	printf("%s\n",buffer);
            	continue;
            }

            else if(strcmp(fir_str,"mkdir")==0)
            {
            	if(mkdir(sec_str,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1)
            	{
            		perror("");
            	}
            	continue;
            }

            else if(strcmp(fir_str,"rmdir")==0)
            {
            	if(rmdir(sec_str)==-1)
            		perror("");
            	continue;
            }

            else if(strcmp(fir_str,"ls")==0)
            {
            	
            	if(sec_str==NULL)
            	{

            		cur_dir = opendir(buffer);
            		if(cur_dir == NULL)
            			perror("");
                    else
                    {

            			while((cur_file = readdir(cur_dir))!=NULL) 
            		    {
            				printf("%s\t",cur_file->d_name);
            			}
            			closedir(cur_dir);
            			printf("\n");
            		}
            	}
            	else if(strcmp(sec_str,"-l")==0)
            	{
            		cur_dir = opendir(buffer);
            		if(cur_dir == NULL)
            			perror("");
            		else
            		{
            			while((cur_file=readdir(cur_dir))!=NULL)
						{
							fstatus=stat(cur_file->d_name, &status);
							if(fstatus>=0)
							{
								printf( (S_ISDIR(status.st_mode)) ? "d" : "-");
								printf( (status.st_mode & S_IRUSR) ? "r" : "-");
								printf( (status.st_mode & S_IWUSR) ? "w" : "-");
								printf( (status.st_mode & S_IXUSR) ? "x" : "-");
								printf( (status.st_mode & S_IRGRP) ? "r" : "-");
								printf( (status.st_mode & S_IWGRP) ? "w" : "-");
								printf( (status.st_mode & S_IXGRP) ? "x" : "-");
								printf( (status.st_mode & S_IROTH) ? "r" : "-");
								printf( (status.st_mode & S_IWOTH) ? "w" : "-");
								printf( (status.st_mode & S_IXOTH) ? "x" : "-");
								char* timestr;
								timestr=ctime(&status.st_atime);
								printf(" %d\t%d\t%s\t%s\n",(int)status.st_nlink,(int)status.st_size,timestr,cur_file->d_name);
							}
						}
            		}


            	}
            }


            else if(strcmp(fir_str,"cp")==0)
            {
            	
            	thi_str = strtok(result,"");
            	
            	FILE *fp1=fopen(sec_str,"r");
            	FILE *fp2=fopen(thi_str,"w");
 
 				struct stat status1,status2;

 				int fstatus1=stat(sec_str,&status1);
 				int fstatus2=stat(thi_str,&status2);


 				if(fstatus2 <0 || fstatus1<0 )
 				{

 					continue;
 				}

 				double d=difftime(status1.st_atime,status2.st_atime);

 				if(d < 0.0)
 				{
 					printf("first file is accessed late\n" );
 					continue;
 				}


 				if(fp1==NULL || fp2==NULL) {
 					printf("files cannot be copied\n");
 					continue;
 				}  

   				char ch;
   				/* Read from stdin and write to stdout, as if nothing has happened */
   				while (fscanf(fp1,"%c", &ch)==1) {
    			      /* Reading is done from the input file */
      				
      				fprintf(fp2,"%c", ch);  /* Writing is done to the output file */
   				}

               fclose(fp1);
               fclose(fp2);

            }
            else 
            {
            	char *argv[100];
            	int str_len = strlen(fir_str);
            	bool background;
            	if(fir_str[strlen(fir_str)-1]=='&')
            	{
            		background = true;
            		fir_str[str_len-1]='\0';
            		argv[0] = fir_str;
            		argv[1] = NULL;

            	}

            	else
            	{

            		
            		argv[0] = fir_str;
            		argv[1] = sec_str;


                    if(argv[1]!=NULL)
                    {
                    	i = 2;

                    	temp = strtok_r(result1," ",&result2);
                    	while(temp!=NULL)
                    	{
                    		if(temp[strlen(temp)-1]=='&')
                    		{
                    			temp[strlen(temp)-1]='\0';
                    			background = true;
                    			break;
                    		}

                    		result1 = result2;
                    		argv[i] = temp;
                    		printf("%s\n",argv[i]);
                    		i++;
                    		temp = strtok_r(result1," ",&result2);
                    	}
                    	argv_count =i;
                    	argv[i]=NULL;

            		

                    }


            	}

                int pipe_count = 0,inp_out_status =0;
                int j;
            	for(j=0;j<argv_count-1;j++)
            	{
            		if(strcmp(argv[j],"|")==0)
            		{
            			pipe_status = 1;
            			pipe(fd[pipe_count]);
            			pipe_count++;
            			
            		}
            		if(strcmp(argv[j],"<")==0||strcmp(argv[j],">")==0)
            			inp_out_status = 1;

            	}
            
            	
            	if(pipe_status)
            	{
            		char ***argv1;
            		argv1 = (char***)malloc(sizeof(char**)*5);
            		for(i =0 ;i<5;i++)
            		{
            			argv1[i]=(char**)malloc(sizeof(char*)*20);
            			for(j=0;j<20;j++)
            			{
            				argv1[i][j] = (char*)malloc(sizeof(char)*20);
            			}
            		}

            		int k=0,l=0;
            		for(j=0;j<i;j++)
            		{
            			if(strcmp(argv[j],"|")==0)
            			{
            				argv1[k][l]=NULL;
            				l=0;
            				k++;

            			}
            			else
            			{
            				strcpy(argv1[k][l],argv[j]);
            				l++;
            			}
            		}
            		for(i=0; i<=pipe_count; i++)
					{
						pipe(fd[i]);
					}
            		for(i=0;i<=pipe_count;i++)
            		{
            			int pid = fork();
            			if(pid==0)
            			{
            				if(i==0)
            				{
            					close(1);
            					dup(fd[0][1]);
            					close(fd[0][1]);
            					if(execvp(argv1[i][0],argv1[i]))
            					{
            						puts(strerror(errno));
                					exit(127);
            					}
            				}
            				else if(i==pipe_count)
            				{
            					close(0);
            					dup(fd[i-1][0]);
            					close(fd[i-1][0]);
            					if(execvp(argv1[i][0],argv1[i]))
            					{
            						puts(strerror(errno));
                					exit(127);
            					}
            				}
            				else
            				{
            					close(0);
            					close(1);
            					dup(fd[i-1][0]);
            					dup(fd[i][1]);
            					close(fd[i-1][0]);
            					close(fd[i][1]);
            					if(execvp(argv1[i][0],argv1[i]))
            					{
            						puts(strerror(errno));
                					exit(127);
            					}
            				}

            			}
            			else
            				waitpid(pid,&status3,0);
            		}





            	}
            	else
            	{
            		if(inp_out_status)
            		{
            			if(strcmp(argv[argv_count-2],">")==0)
            			{
            				if(argv_count < 4 || strcmp(argv[argv_count-4],"<")!=0)
            				{
            					pid = fork();

            					if(pid == 0)
            					{
            						fd1 = open(argv[argv_count-1],O_CREAT | O_WRONLY |O_TRUNC,S_IRUSR | S_IWUSR| S_IRGRP|S_IROTH);
            						close(1);
            						dup(fd1);
            						close(fd1);
            						argv[argv_count-1]= NULL;
            						argv[argv_count-2] = NULL;
            						if( execvp(argv[0], argv))
	              					{
	                					puts(strerror(errno));
	                					exit(127);
	                				}
            					}
            					else
            					{

            						waitpid(pid,&status3,0);
            					}
            				}
            				else if(strcmp(argv[argv_count-4],"<")==0)
            				{
            					pid = fork();
            					if(pid == 0)
            					{
            						close(0);
            						close(1);
            						fd1 = open(argv[argv_count-3],O_RDONLY);
            						fd2 = open(argv[argv_count-1],O_CREAT | O_WRONLY |O_TRUNC,S_IRUSR | S_IWUSR| S_IRGRP|S_IROTH);
            						dup(fd1);
            						dup(fd2);
            						argv[argv_count-1] = NULL;
            						argv[argv_count-2] = NULL;
            						argv[argv_count-3] = NULL;
            						argv[argv_count-4] = NULL;
            						if(execvp(argv[0],argv))
            						{
            							puts(strerror(errno));
            							exit(127);
            						}
            					}
            					else
            					{
            						waitpid(pid,&status3,0);
            					}


            				}


            			}
            			else if(strcmp(argv[argv_count-2],"<")==0)
            			{
            				if(argv_count<4 || strcmp(argv[argv_count-4],">")!=0)
            				{
            					pid = fork();
            					if(pid==0)
            					{
            						close(0);
            						fd1 = open(argv[argv_count-1],O_RDONLY);
            						dup(fd1);
            						close(fd1);
            						argv[argv_count-1] = NULL;
            						argv[argv_count-2] = NULL;
            						if(execvp(argv[0],argv))
            						{
            							puts(strerror(errno));
            							exit(127);
            						}

            				    }
            				    else
            				    {
            				    	waitpid(pid,&status3,0);
            				    }
            				}
            				else if(strcmp(argv[argv_count-4],">")==0)
            				{
            					pid = fork();
            					if(pid==0)
            					{
            						close(0);
            						close(1);
            						fd1 = open(argv[argv_count-1],O_RDONLY);
            						fd2 = open(argv[argv_count-3],O_CREAT|O_WRONLY,O_TRUNC,S_IRUSR | S_IWUSR| S_IRGRP|S_IROTH);
            						dup(fd1);
            						dup(fd2);
            						close(fd1);
            						close(fd2);
            						if(execvp(argv[0],argv))
            						{
            							puts(strerror(errno));
            							exit(127);
            						}
            					}
            					else
            					{
            						waitpid(pid,&status3,0);
            					}
            				}

            			}
            		}
            		else
            		{
            			
            			pid = fork();
            			if(pid ==0)
            			{
            				
            				if(execvp(argv[0],argv))
            				{
            					puts(strerror(errno));
       							exit(127);
            				}

            			}
            			else
            			{
            				if(background==false)
            					waitpid(pid,&status3,0);

            			}
            			background = false;
            		}

            	}

            }


        }

            
return 0;

    }	
