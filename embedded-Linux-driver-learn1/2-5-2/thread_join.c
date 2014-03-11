#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
void *thread(void *str)
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        sleep(2);
        printf( "This in the thread : %d\n" , i );
    }
    return NULL;
}

int main()
{
    pthread_t pth;
    int i;

	/*创建线程并执行线程执行函数*/
    int ret = pthread_create(&pth, NULL, thread, NULL);
    
	printf("The main process will be to run,but will be blocked soon\n");
	
	/*阻塞等待线程退出*/
    pthread_join(pth, NULL);

    printf("thread was exit\n");
    for (i = 0; i < 3; ++i)
    {
        sleep(1);
        printf( "This in the main : %d\n" , i );
    }
    
    return 0;
}
