#include <stdio.h>

int main()
{
	FILE *fp = NULL;
	char Buf[128];
	
	/*初始化Buf*/
	strcpy(Buf,"memdev is char dev!");
	printf("BUF: %s\n",Buf);
	
	/*打开设备文件*/
	fp = fopen("/dev/memdev0","r+");
	if (fp == NULL)
	{
		printf("Open memdev0 Error!\n");
		return -1;
	}
	
	/*清除Buf*/
	strcpy(Buf,"Buf is NULL!");
	printf("Read BUF1: %s\n",Buf);
	
	/*读出数据*/
	fread(Buf, sizeof(Buf), 1, fp);
	
	/*检测结果*/
	printf("Read BUF2: %s\n",Buf);
	
	fclose(fp);
	
	return 0;	

}
