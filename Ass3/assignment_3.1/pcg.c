#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)





int main()
{

	int i,j,m,n;
	int shmid,shmid1,shmid2,shmid3,shmid4,shmid5;
	int semid1, semid2;
	
	
	 int sum;
	
	struct sembuf pop, vop;

	scanf("%d %d",&m,&n);

	sum=1275*m;

	shmid = shmget(IPC_PRIVATE, 20*sizeof(int), 0777|IPC_CREAT);
	shmid1 = shmget(IPC_PRIVATE, sizeof(int), 0777|IPC_CREAT);
	shmid2 = shmget(IPC_PRIVATE, sizeof(int), 0777|IPC_CREAT); 
	shmid3 = shmget(IPC_PRIVATE, sizeof(int), 0777|IPC_CREAT); 
	shmid4 = shmget(IPC_PRIVATE, sizeof(int), 0777|IPC_CREAT);  
	shmid5 = shmget(IPC_PRIVATE, 2*sizeof(int), 0777|IPC_CREAT);

	 int *buffer=shmat(shmid,0,0);
	 int *count=shmat(shmid1,0,0);
	 int *temp_sum = shmat(shmid2,0,0);
	 int *head=shmat(shmid3,0,0);
	 int *tail = shmat(shmid4,0,0);
	 int *d=shmat(shmid5,0,0);

	 *count =0;
	 *temp_sum = 0;
	 *head=0;
	 *tail =0;
	 d[0]=d[1]=0;	


	semid1 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	semid2 = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);
	


	semctl(semid1, 0, SETVAL, 1);
	semctl(semid2, 0, SETVAL, 0);
	

	
	pop.sem_num = vop.sem_num = 0;
	pop.sem_flg = vop.sem_flg = 0;
	pop.sem_op = -1 ; vop.sem_op = 1 ;



	for(i=0;i<m+n;i++)
	{
		int id =fork();
		
			if(id==0)
			{
				if(i==m+n-1) d[1]=1;

				while(d[1]==0);


				if(i < m)
				{
					for(j=1;j<=50;j++)
					{

						P(semid1);
			 			
			 			while(*count==20);
			 			buffer[head[0]]=j;
			 			printf("producer %d = %d\n",getpid(), buffer[head[0]]);	
			 			head[0]=(head[0]+1)%20;
			 			(*count)++;
			 			
			 			V(semid2);

			 		}	

			 		d[0]++;
			 		exit(0);


				}
				else
				{

					while(1)
					{
						
						P(semid2);
	    				
	    				while((*count)==0 && (*temp_sum)<sum);

	    				if((*temp_sum)>=sum)
	    				{

	    					 V(semid2);
	    					 exit(0);
	    				}

	    				(*temp_sum)+=buffer[tail[0]];
	    				printf("Consumer %d wrote = %d\n",getpid(),buffer[tail[0]]);
	    				tail[0]=(tail[0]+1)%20;
	    				(*count)--;

	    				if(d[0]==m) V(semid2);
	    				else V(semid1);


	    			}


				}


			}

	}


	while(wait(NULL)>0) {}

		printf("SUM is %d\n",*temp_sum);

	if((*temp_sum)==sum) { printf("yes\n");}
    else printf("No\n");




}
