
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

struct thread_data
{
	int t_index;
	
};

int k,n,x,thread_count=0,col_count=0,status1=0,status2=0;
int **arr;
pthread_t thread_id[5];
pthread_mutex_t cnt_mutex;
pthread_cond_t *cnt_cond;


void row_swap(int tid)
{
	int i,j;
	
for(i = tid * (n/x) ;i<(tid+1)*(n/x);i++)
{	int tmp = arr[i][0];
					for(j=0;j<n-1;j++)
					{
						arr[i][j] = arr[i][j+1];
					}
					arr[i][n-1] = tmp;
				}



} 

void col_swap(int tid)
{
	int i,j;
	for(i = tid * (n/x);i<(tid+1)*(n/x) ;i++)
	{
		int tmp = arr[n-1][i];
		for(j=n-2;j>=0;j--)
		{
			arr[j+1][i] = arr[j][i];
		}
		arr[0][i] = tmp;
	}

}


void *start_routine(void *param)
{

	struct thread_data *t_param = (struct thread_data *) param;
	int tid = (*t_param).t_index;
	int i,j,c;

	pthread_mutex_lock(&cnt_mutex);
    sleep(1);
	for(c=0;c<k;c++)
	{
		pthread_cond_wait(cnt_cond+tid, &cnt_mutex);
	

    if(tid!=x-1)
	 row_swap(tid);
else
{
	for(i = tid * (n/x) ;i<n;i++)

{	int tmp = arr[i][0];
					for(j=0;j<n-1;j++)
					{
						arr[i][j] = arr[i][j+1];
					}
					arr[i][n-1] = tmp;
				}

}

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	

	if(tid != x-1)
	pthread_cond_signal(cnt_cond+tid+1);
else
	pthread_cond_signal(cnt_cond);

	pthread_cond_wait(cnt_cond+tid, &cnt_mutex);

	if(tid!=x-1)
	col_swap(tid);
else
{
	for(i = tid * (n/x);i<n ;i++)
	{
		int tmp = arr[n-1][i];
		for(j=n-2;j>=0;j--)
		{
			arr[j+1][i] = arr[j][i];
		}
		arr[0][i] = tmp;
	}
}
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	if(tid!=x-1)
	pthread_cond_signal(cnt_cond+tid+1);
else
	pthread_cond_signal(cnt_cond);


	pthread_mutex_unlock(&cnt_mutex);

    }


	pthread_exit(NULL);



}



int main()
{
	int i,j;
	

	scanf("%d %d %d",&n,&k,&x);
	arr = (int **)malloc(sizeof(int *)*n);
	pthread_mutex_init(&cnt_mutex, NULL);
	cnt_cond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t )*x);
	for(i = 0;i < x;i++)
	{

	    pthread_cond_init(cnt_cond+i, NULL);
	}
	for(i = 0;i < n;i++)
		arr[i]=(int *)malloc(sizeof(int)*n);

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			scanf("%d",&arr[i][j]);
		}
	}

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	

	struct thread_data param[x];
	
		for(i=0; i<x; i++)
		{
			 param[i].t_index = i;
		 	pthread_create(&thread_id[i], NULL, start_routine, (void *) &param[i]);
		}
		sleep(3);
		pthread_cond_signal(cnt_cond);
		for(i=0; i<x; i++)
		{
			pthread_join(thread_id[i], NULL);
		}


	pthread_mutex_destroy(&cnt_mutex);
	for(i = 0;i < x;i++)
	{
	pthread_cond_destroy(cnt_cond+i);
}

}
