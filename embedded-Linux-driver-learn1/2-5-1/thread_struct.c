#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

struct menber
{
    int a;
    char *s;
};

/*�߳�ִ�к���*/
void *create(void *arg)
{
    struct menber *temp;
    temp=(struct menber *)arg;
    printf("menber->a = %d  \n",temp->a);
    printf("menber->s = %s  \n",temp->s);
    return (void *)0;
}

int main(int argc,char *argv[])
{
    pthread_t tidp;
    int error;
    struct menber *b;

	/*Ϊ�ṹ��ָ��b�����ڴ沢��ֵ*/
    b=(struct menber *)malloc( sizeof(struct menber) );
    b->a = 4;
    b->s = "zieckey";
    
	/*�����̲߳������߳�ִ�к���*/
    error = pthread_create(&tidp, NULL, create, (void *)b);
    if( error )
    {
        printf("phread is not created...\n");
        return -1;
    }

    sleep(1); //����˯��һ��ʹ�߳�ִ�������̲Ż����

    printf("pthread is created...\n");
    return 0;
}