#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

int main(void)
{
	pid_t child;

	/* 创建子进程 */
	if((child=fork())==-1)
	{
		printf("Fork Error : %s\n", strerror(errno));
		exit(1);
	}
	else 
		if(child==0) // 子进程
		{
			printf("I am the child: %d\n", getpid());
			exit(0);
		}
		else        //父进程
		{
			printf("I am the father:%d\n",getpid());
			return 0;
		}
}	
