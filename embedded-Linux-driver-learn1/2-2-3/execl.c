#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
    /*�ж������û�д����ļ���*/
	if(argc<2)
	{
	    perror("you haven,t input the filename,please try again!\n");
		exit(EXIT_FAILURE);
	
	}
	/*����execl�������ÿ�ִ�г���file_creat�滻������*/
	if(execl("./file_creat","file_creat",argv[1],NULL)<0)
		perror("execl error!");

}
