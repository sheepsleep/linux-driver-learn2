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

	/*�����̲߳�ִ���߳�ִ�к���*/
    int ret = pthread_create(&pth, NULL, thread, NULL);
    
	printf("The main process will be to run,but will be blocked soon\n");
	
	/*�����ȴ��߳��˳�*/
    pthread_join(pth, NULL);

    printf("thread was exit\n");
    for (i = 0; i < 3; ++i)
    {
        sleep(1);
        printf( "This in the main : %d\n" , i );
    }
    
    return 0;
}
