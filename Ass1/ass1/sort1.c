#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{ 
    int j,temp;
    FILE* f = fopen(argv[1], "r");
    int n = 0, i = 0,size=0;
    int arr[100],stridx=0; 

    while( fscanf(f, "%d,", &n) > 0 )
    {
        arr[i++] = n;
        size++;
    }

    fclose(f);

   
    	
    	for(i=0;i<size-1;i++)
    	   for(j=0;j<size-i-1;j++)
    	       if(arr[j]>arr[j+1])
    		{
         		temp=arr[j];
    			arr[j]=arr[j+1];
  		        arr[j+1]=temp;
    		}

        for(i=0;i<size;i++)
            printf("%d ",arr[i]);
    
  

}
