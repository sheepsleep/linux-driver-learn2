#include <stdio.h>

int main()
{
	FILE *fp = NULL;
	char Buf[128];
	
	
	/*���豸�ļ�*/
	fp = fopen("/dev/memdev0","r+");
	if (fp == NULL)
	{
		printf("Open Dev memdev0 Error!\n");
		return -1;
	}
	
	/*д���豸*/
	strcpy(Buf,"memdev is char dev!");
	printf("Write BUF: %s\n",Buf);
	fwrite(Buf, sizeof(Buf), 1, fp);
	
	sleep(5);
	fclose(fp);
	
	return 0;	

}
