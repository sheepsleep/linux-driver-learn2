#include <time.h>
#include <stdio.h>

int main(void)
{
    struct tm *ptr;
    time_t lt;
    
    /*获取日历时间*/
    lt=time(NULL);
    
    /*转化为本地时间*/
    ptr=localtime(&lt);
    
    /*以本地时间的字符串方式打印*/
    printf("%s\n",asctime(ptr));
    
    /*以本地时间的字符串方式打印*/
    printf("%s\n",ctime(&lt));
    return 0;
}
