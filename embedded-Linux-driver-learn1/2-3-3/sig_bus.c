#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*�Զ����źŴ�����*/
void my_func(int sign_no)
{
	if(sign_no==SIGBUS)
		printf("I have get SIGBUS\n");
}
int main()
{
	printf("Waiting for signal SIGBUS \n ");
	
	/*ע���źŴ�����*/
	signal(SIGBUS,my_func);
	
	pause();//�����̹���ֱ����׽���ź�Ϊֹ
	exit(0);
}

