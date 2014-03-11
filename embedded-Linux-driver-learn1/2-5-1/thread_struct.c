#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

struct menber
{
    int a;
    char *s;
};

/*线程执行函数*/
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

	/*为结构体指针b分配内存并赋值*/
    b=(struct menber *)malloc( sizeof(struct menber) );
    b->a = 4;
    b->s = "zieckey";
    
	/*创建线程并运行线程执行函数*/
    error = pthread_create(&tidp, NULL, create, (void *)b);
    if( error )
    {
        printf("phread is not created...\n");
        return -1;
    }

    sleep(1); //进程睡眠一秒使线程执行完后进程才会结束

    printf("pthread is created...\n");
    return 0;
}
