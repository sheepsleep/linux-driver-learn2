#include <stdio.h>
#include <linux/unistd.h>

main()
{
	int result;
	result = syscall(361, 1 ,2);
	printf("resulst= %d\n",result);
}
