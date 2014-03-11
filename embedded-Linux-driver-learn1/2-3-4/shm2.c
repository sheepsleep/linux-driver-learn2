#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_com.h"

int main(void)
{
    int running=1;
	void *shared_memory=(void *)0;
	struct shared_use_st *shared_stuff;
	char buffer[BUFSIZ];
	int shmid;

	/*���������ڴ�*/
	shmid=shmget((key_t)1234,sizeof(struct shared_use_st),0666|IPC_CREAT);
	if(shmid==-1)
	{
	    fprintf(stderr,"shmget failed\n");
		exit(EXIT_FAILURE);
	}

	/*ӳ�乲���ڴ�*/
	shared_memory=shmat(shmid,(void *)0,0);
	if(shared_memory==(void *)-1)
	{
	    fprintf(stderr,"shmat failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Memory attached at %X\n",(int)shared_memory);

	/*�ýṹ��ָ��ָ����鹲���ڴ�*/
	shared_stuff=(struct shared_use_st *)shared_memory;

	/*ѭ���������ڴ���д���ݣ�ֱ��д���Ϊ��end��Ϊֹ*/
	while(running)
	{
	    while(shared_stuff->written_by_you==1)
		{
		    sleep(1);//�ȵ������̶���֮����д
			printf("waiting for client...\n");
		}
		printf("Ener some text:");
		fgets(buffer,BUFSIZ,stdin);
		strncpy(shared_stuff->some_text,buffer,TEXT_SZ);
		shared_stuff->written_by_you=1;
		if(strncmp(buffer,"end",3)==0)
		{
		    running=0;  //����ѭ��
		}
	}

	/*ɾ�������ڴ�*/
	if(shmdt(shared_memory)==-1)
	{
	    fprintf(stderr,"shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
