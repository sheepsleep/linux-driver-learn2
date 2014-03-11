#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static char *name = "DAVID";
static int age=30;
module_param(age, int ,S_IRUGO);
module_param(name, charp ,S_IRUGO);
static int __init hello_init(void)
{
	printk(KERN_EMERG "Name:%s\n",name);
	printk(KERN_EMERG "Age:%d\n",age);
	return 0;
}
static void __exit hello_exit(void)
{
	printk(KERN_EMERG"Module exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);
