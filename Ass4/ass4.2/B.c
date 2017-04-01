#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/signal.h>

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)



#define SEM_NAME "/semaphore"                           // semaphores for empty,full,interprocess
#define SEM_NAME1 "/empty"
#define SEM_NAME2 "/full"


pthread_t thread_id;
pthread_mutex_t mutex_count;                       //mutex
pthread_cond_t thr_con,pro_con;                    //conditional variable
int thread_count =0;
int ticket =100;


sem_t *semaphore,*empty,*full;


void hand(int sig)                                 //ctrl-c handler
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



void* book_ticket(void *p)
{
	int *x = (int *)p;
	pthread_mutex_lock(&mutex_count);                  //mutex_lock

	thread_count ++;
	sleep(2);
	 
	if(thread_count > 10)                              //count>10 wait
	 pthread_cond_wait(&thr_con,&mutex_count);
	printf("request received :%d",*x);
	if((ticket -*x)>=0)
	{
		
		if(100<=(ticket-*x))
			ticket =100;
		else
			ticket = ticket-*x;
		
		
	}
	else
	{
		printf("request cannot be fulfilled\n");
	}
	sleep(rand()%3);
   printf("tic: %d\n",ticket);
	

	if(thread_count<=5)
		pthread_cond_signal(&pro_con);                  //count<6 signal 

	pthread_cond_signal(&thr_con);
	pthread_mutex_unlock(&mutex_count);
	thread_count--;
	pthread_exit(NULL);



}


int main(int argc, char const *argv[])
{
	signal(SIGINT,hand);
	int shmid,shmid1,shmid2;
	key_t key1,key2,key3;
	key1 = 5678;
	key2 = 3567;
	key3 = 3891;
	pthread_t tid;
	
	

	shmid = shmget(key1,11*sizeof(int),IPC_CREAT | 0666);

	shmid1 = shmget(key2,sizeof(int),IPC_CREAT | 0666); 

	shmid2 = shmget(key3,sizeof(int),IPC_CREAT | 0666); 


	if(shmid==-1)
	{
		printf("shmget\n");
        exit(1);
	}

	if(shmid1==-1)
	{
		printf("shmget\n");
        exit(1);
	}

	if(shmid2==-1)
	{
		printf("shmget\n");
        exit(1);
	}


	semaphore = sem_open(SEM_NAME, O_RDWR);
	empty = sem_open(SEM_NAME1, O_RDWR);
	full = sem_open(SEM_NAME2, O_RDWR);

	int *que = (int *)shmat(shmid,0,0);

	int *head = (int *)shmat(shmid1,0,0);
	int *tail = (int *)shmat(shmid2,0,0);
	pthread_mutex_init(&mutex_count, NULL);
	pthread_cond_init(&pro_con, NULL);
	pthread_cond_init(&thr_con, NULL);

	while(1)
	{
		sleep(1);
		sem_wait(full);
		sem_wait(semaphore);
	
		if(*head == -1)
 			printf("\n\nQueue is empty.\n");
 		else
 		{

 			int item = que[*head];
 			if(thread_count>10)
 				pthread_cond_wait(&pro_con,&mutex_count);              //count>10 wait

 			

 			if(*head == *tail)
 			{
 				*head = -1;
 				*tail = -1;
 			}
 			else if(*head == 9)
 				*head = 0;
 			else
 				(*head)++;

 			//printf("\nITEM deleted: %d\n", item);
 			int p = pthread_create(&tid, NULL, book_ticket,(void *) &item);
 			
 		}
 		sem_post(semaphore);
 		sem_post(empty);
	}


	return 0;
}