#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

extern int add_integar(int a,int b); 
extern int sub_integar(int a,int b);

static int __init hello_init(void)
{
	int res=add_integar(1,2);
	printk(KERN_EMERG"hello init , res=%d\n",res);
	return 0;
}
static void __exit hello_exit()
{
	int res=sub_integar(2,1);
	printk(KERN_EMERG"hello exit,res=%d\n",res);
}

module_init(hello_init);
module_exit(hello_exit);
