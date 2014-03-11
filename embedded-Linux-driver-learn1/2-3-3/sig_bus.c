#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*自定义信号处理函数*/
void my_func(int sign_no)
{
	if(sign_no==SIGBUS)
		printf("I have get SIGBUS\n");
}
int main()
{
	printf("Waiting for signal SIGBUS \n ");
	
	/*注册信号处理函数*/
	signal(SIGBUS,my_func);
	
	pause();//将进程挂起直到捕捉到信号为止
	exit(0);
}

