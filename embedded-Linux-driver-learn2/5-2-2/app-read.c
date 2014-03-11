#include <stdio.h>

int main()
{
	FILE *fp = NULL;
	char Buf[128];
	
	/*��ʼ��Buf*/
	strcpy(Buf,"memdev is char dev!");
	printf("BUF: %s\n",Buf);
	
	/*���豸�ļ�*/
	fp = fopen("/dev/memdev0","r+");
	if (fp == NULL)
	{
		printf("Open memdev0 Error!\n");
		return -1;
	}
	
	/*���Buf*/
	strcpy(Buf,"Buf is NULL!");
	printf("Read BUF1: %s\n",Buf);
	
	/*��������*/
	fread(Buf, sizeof(Buf), 1, fp);
	
	/*�����*/
	printf("Read BUF2: %s\n",Buf);
	
	fclose(fp);
	
	return 0;	

}
