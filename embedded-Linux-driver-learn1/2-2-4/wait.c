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
			printf("the child process is run\n");
			sleep(1);  //子进程睡眠一秒，但并没有去运行父进程
			printf("I am the child: %d\n", getpid());
			exit(0);
		}
		else        //父进程
		{
			wait(NULL); //等到子进程退出，父进程才会运行
			printf("the father process is run\n");
			printf("I am the father:%d\n",getpid());
			return 0;
		}
}	
