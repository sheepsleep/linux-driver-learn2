#include <time.h>
#include <stdio.h>

int main(void)
{
    struct tm *ptr;
    time_t lt;
    
    /*��ȡ����ʱ��*/
    lt=time(NULL);
    
    /*ת��Ϊ����ʱ��*/
    ptr=localtime(&lt);
    
    /*�Ա���ʱ����ַ�����ʽ��ӡ*/
    printf("%s\n",asctime(ptr));
    
    /*�Ա���ʱ����ַ�����ʽ��ӡ*/
    printf("%s\n",ctime(&lt));
    return 0;
}
