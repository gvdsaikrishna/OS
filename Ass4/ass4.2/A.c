#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>


#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)

#define SEM_NAME "/semaphore"                                             // semaphore for sync b/w a and b
#define SEM_NAME1 "/empty"                                                //semaphore for empty queue
#define SEM_NAME2 "/full"                                                 //semaphore for full queue
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 1


sem_t *semaphore,*empty,*full;


void hand(int sig)                                                 // clear semaphores when ctrl-c pressed
{
	printf("clearing\n");
	sem_close(semaphore);
	sem_close(empty);
	sem_close(full);
	sem_unlink(SEM_NAME1);
	sem_unlink(SEM_NAME2);
	sem_unlink(SEM_NAME);
	exit(0);

}


int main()
{
	signal(SIGINT,hand);
	srand(time(NULL));
	
	
	int shmid,shmid1,shmid2;
	key_t key1,key2,key3;
	key1 = 5678;
	key2 = 3567;
	key3 = 3891;
	
	
	shmid = shmget(key1,11*sizeof(int),IPC_CREAT | 0666);                      // memory for queue

	shmid1 = shmget(key2,sizeof(int),IPC_CREAT | 0666);                        //memory for head

	shmid2 = shmget(key3,sizeof(int),IPC_CREAT | 0666);                          //memory for tail

	if(shmid==-1)
	{
		printf("shmget\n");
        exit(1);
	}

	if(shmid1==-1)
	{
		printf("shmget1\n");
        exit(1);
	}

	if(shmid2==-1)
	{
		printf("shmget2\n");
        exit(1);
	}

//initialize semaphores
	semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE); 

	empty = sem_open(SEM_NAME1, O_CREAT | O_EXCL, SEM_PERMS, 10);

	full = sem_open(SEM_NAME2, O_CREAT | O_EXCL, SEM_PERMS, 0);

 	if (semaphore == SEM_FAILED) {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
     if (empty == SEM_FAILED) {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
     if (full == SEM_FAILED) {
        perror("sem_open(3) error");
        exit(EXIT_FAILURE);
    }
	 
//attach memory 

	int *que = (int *)shmat(shmid,0,0);

	int *head = (int *)shmat(shmid1,0,0);
	*head = -1;
	int *tail = (int *)shmat(shmid2,0,0);
	*tail = -1;
	 
		
	while(1)
	{
		int rd1 = rand() % 3;
		
		if(rd1 != 0)
			sleep(rd1);

		int rd2 = rand() % 11;
		rd2 = rd2-5;
		
		
		
		sem_wait(empty);                      
		sem_wait(semaphore);
		
 		//enqueue
 		if((*head==0 && *tail== 9) || (*head==(*tail)+1))
 			printf("\n\nQueue is full.");
 		else
 		{
 			
 			if(*tail == -1)
 			{
 				*tail = 0;
 				*head = 0;
 			}
 			else if(*tail == 9)
 				*tail = 0;
 			else
				 (*tail)++;

 			que[*tail] = rd2;
 			printf("\nItem inserted: %dhead : %d tail : %d\n", rd2,*head,*tail);
 		} 
 		sem_post(semaphore);
 		sem_post(full);
 		
	}
// clear semaphores

sem_close(semaphore);
sem_close(empty);
sem_close(full);
sem_unlink(SEM_NAME1);
sem_unlink(SEM_NAME2);
sem_unlink(SEM_NAME);


	
	return 0;
}