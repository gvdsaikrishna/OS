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

void search(struct student_records *records)
{
	int i=0,j;
	scanf("%d",&j);

	while(records[i].count)
	{
		if(records[i].roll_no==j)
			printf("%s %s %d %f\n",records[i].first_name,records[i].last_name,records[i].roll_no,records[i].cgpa);
		i++;
	}
	
}



int main()
{

	char *first,*last;
	first = (char*)malloc(sizeof(char)*20);
	last = (char*)malloc(sizeof(char)*20);
	int roll,size;
	float cg;
	int i,k=0;
	float j;

	struct student_records *records;
	int shmid,shmid2,shmid3;
	key_t key1;
	char *temp1,*temp2;

	key1 = 5678;
	

	shmid = shmget(key1,102*sizeof(struct student_records), 0666|IPC_CREAT);           //shared memory for student records


	if(shmid==-1)
	{
		perror("shmget");
        exit(1);
	}



	records = (struct student_records *)shmat(shmid,0,0);




	i=0;    
	while(records[i].count)
	{

 		i++;
	}

	size =i;

	while(1)
	{

		scanf("%d",&i);
		switch(i)
		{
			case 1:

			{
				search(records);
				break;
			}

			case 2:
			{
				
				k=0;
				scanf("%d %f",&i,&j);
				while(records[k].count)
				{
					if(records[k].roll_no==i)
					{
						
						records[k].cgpa = j;

						
						records[size+1].count=1;
						break;
					}
					k++;
				}
				break;
			}

			default:{  shmdt(records);  exit(1); }

		}

	}
	




	
	return 0;
}