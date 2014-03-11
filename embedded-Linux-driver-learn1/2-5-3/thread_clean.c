#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*�߳�������*/
void *clean(void *arg)
{
    printf("cleanup :%s\n",(char *)arg);
    return (void *)0;
}

/*�߳�1��ִ�к���*/
void *thr_fn1(void *arg)
{
    printf("thread 1 start  \n");

	/*���߳�������ѹ�����ջ����*/
    pthread_cleanup_push( (void*)clean,"thread 1 first handler");
    pthread_cleanup_push( (void*)clean,"thread 1 second hadler");
    printf("thread 1 push complete  \n");

    if(arg)
    {
        return((void *)1); //�߳����е���������������Ĵ��벻�ᱻ���С���������return�˳������Բ���ִ���߳���������
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return (void *)1;
}

/*�߳�2��ִ�к���*/
void *thr_fn2(void *arg)
{
    printf("thread 2 start  \n");

	/*���߳�������ѹ�����ջ����*/
    pthread_cleanup_push( (void*)clean,"thread 2 first handler");
    pthread_cleanup_push( (void*)clean,"thread 2 second handler");
    printf("thread 2 push complete  \n");
    
	if(arg)
    {
        pthread_exit((void *)2);//�߳����е���������������Ĵ��벻�ᱻ���С���������pthread_exit�˳������Ի�ִ���߳���������ִ�е�˳������ѹ��ջ�ĺ�ִ�У�������ȳ���
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);
}

int main(void)
{
    int err;
    pthread_t tid1,tid2;
    void *tret;

	/*�����߳�1��ִ���߳�ִ�к���*/
    err=pthread_create(&tid1,NULL,thr_fn1,(void *)1);
    if(err!=0)
    {
        printf("error .... \n");
        return -1;
    }

	/*�����߳�2��ִ���߳�ִ�к���*/
    err=pthread_create(&tid2,NULL,thr_fn2,(void *)1);
	if(err!=0)
    {
        printf("error .... \n");
        return -1;
    }

	/*�����ȴ��߳�1�˳�������ȡ�߳�1�ķ���ֵ*/
    err=pthread_join(tid1,&tret);
    if(err!=0)
    {
        printf("error .... \n");
        return -1;
    }
    printf("thread 1 exit code %d  \n",(int)tret);

	/*�����ȴ��߳�2�˳�������ȡ�߳�2�ķ���ֵ*/
    err=pthread_join(tid2,&tret);
    if(err!=0)
    {
        printf("error .... ");
        return -1;
    }
	printf("thread 2 exit code %d  \n",(int)tret);
    
    return 1;
}
