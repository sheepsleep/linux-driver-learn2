#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#include "memdev.h"

#define DEVICE_NAME "leds"

static unsigned long led_table [] = {
	S3C2410_GPB5,
	S3C2410_GPB6,
	S3C2410_GPB7,
	S3C2410_GPB8,
};

static unsigned int led_cfg_table [] = {
	S3C2410_GPB5_OUTP,
	S3C2410_GPB6_OUTP,
	S3C2410_GPB7_OUTP,
	S3C2410_GPB8_OUTP,
};

static int sbc2440_leds_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	int i = 0;
	    /* ����������Ч�� */
  if (_IOC_TYPE(cmd) != MEMDEV_IOC_MAGIC) 
      return -EINVAL;
  if (_IOC_NR(cmd) > MEMDEV_IOC_MAXNR) 
      return -EINVAL;
      
  /* �������ִ����Ӧ�Ĳ��� */
	switch(cmd) {
	case MEMDEV_IOCOFF:
		/*��ȫ��*/
		cmd = 1;
		for(i=0; i<4; i++)
			s3c2410_gpio_setpin(led_table[i], cmd);
		return 0;
		
	case MEMDEV_IOCON:
		/*��ȫ��*/
		for(i=0; i<4; i++)
			s3c2410_gpio_setpin(led_table[i], !cmd);
		return 0;
	default:
		return -EINVAL;
	}
}
/*�ļ������ṹ��*/
static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	sbc2440_leds_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	int i;
	/*����GPIO���ƼĴ�����GPIO����Ϊ���ģʽ,Ĭ���µ�ȫ��*/
	for (i = 0; i < 4; i++) {
		s3c2410_gpio_cfgpin(led_table[i], led_cfg_table[i]);
		s3c2410_gpio_setpin(led_table[i], 1);
	}
	/*ע��������ַ��豸����*/
	ret = misc_register(&misc);

	printk (DEVICE_NAME"\tinitialized\n");

	return ret;
}

static void __exit dev_exit(void)
{
	/*ע���������ַ��豸����*/
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_AUTHOR("David Xie");
MODULE_LICENSE("GPL");

