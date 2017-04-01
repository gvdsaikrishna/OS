#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int check(int numbers[],int stridx,int endidx,int k)
{
  int i;
	for(i=stridx;i<=endidx;i++) if(numbers[i]==k) return 1;

	return -1;
}


void search(int numbers[],int k,int stridx,int endidx)
{
    int size=endidx-stridx+1,id[2],mid=stridx+size/2,res[2],i,status[2];

   
            id[0]=fork();
            if(id[0]==0)
            {
	
		 int p;
		 if(size<=10) 
		 {
			p=check(numbers,stridx,mid,k);
			exit(p);
		 }
                 
                 search(numbers,k,stridx,mid);

	    }

               
        
         id[1]=fork();
            if(id[1]==0)
            {
	
		 int p;
		 if(size<=10) 
		 {
			p=check(numbers,mid+1,endidx,k);
			exit(p);
		 }
                
                 search(numbers,k,mid+1,endidx);

	    }

        waitpid(id[0],&status[0],0);
        waitpid(id[1],&status[1],0);
        
        if(status[0]!=65280||status[1]!=65280) exit(1);
        else exit(-1);


}


int main()
{
    FILE* f = fopen("output.txt", "r");
    int n = 0, i = 0,size=0,k,mid,res[2];
    int numbers[100],id[2],status[2],stridx=0; 

    while( fscanf(f, "%d,", &n) > 0 )
    {
        numbers[i++] = n;
        size++;
    }

    fclose(f);
while(1)
{
    scanf("%d",&k);
    if(k<0) exit(1);

    if(size<=10)
    {
        for(i=0;i<size;i++)
            if(numbers[i]==k)
            {
                printf("1.found\n");
                
            }
         printf("1.not found\n");
         
    }
    else
    {
        for(i=0;i<2;i++)
        {
            id[i]=fork();
            if(id[i]==0)
            {
                mid=size/2;
                search(numbers,k,stridx,mid);
                stridx=mid+1;
                mid=size-1;
            }
        }

        waitpid(id[0],&status[0],0);
        waitpid(id[1],&status[1],0);
        if(status[0]!=65280||status[1]!=65280)
        {
            printf("found\n");
           

        }
        else
        {
            printf("not found\n");
            
        }
    }
    
}

}

