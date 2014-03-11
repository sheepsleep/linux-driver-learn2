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

	/*���ƶ�д˳��*/
	shared_stuff->written_by_you=0;

	/*ѭ���Ĵӹ����ڴ��ж����ݣ�ֱ��������end��Ϊֹ*/
	while(running)
	{
	   if(shared_stuff->written_by_you)
	   {
	       printf("You wrote:%s",shared_stuff->some_text);
		   sleep(1);  //������˯һ�룬ͬʱ�ᵼ��д����˯һ�룬������������֮����д
		   shared_stuff->written_by_you=0;
		   if(strncmp(shared_stuff->some_text,"end",3)==0)
		   {
		       running=0; //����ѭ��
		   }
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
