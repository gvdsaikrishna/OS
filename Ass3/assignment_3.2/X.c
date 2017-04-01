#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

struct student_records
{
	char first_name[20];
	char last_name[20];
	int roll_no;
	float cgpa;
	int count;

};


int main()
{

	
	char *first,*last;
	first = (char*)malloc(sizeof(char)*20);
	last = (char*)malloc(sizeof(char)*20);
	int roll;
	float cg;
	char *c;

	FILE* file = fopen("test.txt","r");
	int i=0,size;
	int shmid;
	key_t key1;
	
	
	key1 = 5678;
	
	shmid = shmget(key1,102*sizeof(struct student_records),IPC_CREAT | 0666);    // shared memory for student records
	

	

	if(shmid==-1)
	{
		printf("shmget\n");
        exit(1);
	}


	struct student_records *records = (struct student_records *)shmat(shmid,0,0);
	


	while(fscanf(file,"%s %s %d %f\n",first, last, &roll, &cg)!=EOF)
	{
		
		strcpy(records[i].first_name,first);
		strcpy(records[i].last_name,last);
		records[i].roll_no = roll;
		records[i].cgpa =cg;

		i++;

		records[i-1].count = i;
	
	}
	records[i].count = 0;

	

	size = i;

	fclose(file);
	
	for(i=0;i<size;i++)
	{
		printf("%s %s %d %f\n",records[i].first_name,records[i].last_name,records[i].roll_no,records[i].cgpa);
	}

	while(1)
	{

		sleep(5);
		
		
		i=0;
		
		
			file = fopen("test.txt","w");
			while(records[i].count)
			{
				fprintf(file, "%s %s %d %f\n",records[i].first_name,records[i].last_name,records[i].roll_no,records[i].cgpa);
				i++;

			}
		
			fclose(file);
		
	}

    
}
