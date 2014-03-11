#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <plat/regs-adc.h>
#include <mach/regs-gpio.h>

/* For ts.dev.id.version */
#define S3C2410TSVERSION	0x0101

#define WAIT4INT(x)  (((x)<<8) | \
		     S3C2410_ADCTSC_YM_SEN | S3C2410_ADCTSC_YP_SEN | S3C2410_ADCTSC_XP_SEN | \
		     S3C2410_ADCTSC_XY_PST(3))

#define AUTOPST	     (S3C2410_ADCTSC_YM_SEN | S3C2410_ADCTSC_YP_SEN | S3C2410_ADCTSC_XP_SEN | \
		     S3C2410_ADCTSC_AUTO_PST | S3C2410_ADCTSC_XY_PST(0))

static char *s3c2410ts_name = "s3c2410 TouchScreen";

static	struct input_dev *dev;
static	long xp;
static	long yp;
static	int count;

extern struct semaphore ADC_LOCK;
static int OwnADC = 0;

static void __iomem *base_addr;

static inline void s3c2410_ts_connect(void)
{
	s3c2410_gpio_cfgpin(S3C2410_GPG12, S3C2410_GPG12_XMON);
	s3c2410_gpio_cfgpin(S3C2410_GPG13, S3C2410_GPG13_nXPON);
	s3c2410_gpio_cfgpin(S3C2410_GPG14, S3C2410_GPG14_YMON);
	s3c2410_gpio_cfgpin(S3C2410_GPG15, S3C2410_GPG15_nYPON);
}

static void touch_timer_fire(unsigned long data)
{
  	unsigned long data0;
  	unsigned long data1;
	int updown;

  	data0 = ioread32(base_addr+S3C2410_ADCDAT0);
  	data1 = ioread32(base_addr+S3C2410_ADCDAT1);

 	updown = (!(data0 & S3C2410_ADCDAT0_UPDOWN)) && (!(data1 & S3C2410_ADCDAT0_UPDOWN));

 	if (updown) {
 		
 		if (count != 0) {/*转换四次后进行事件汇报*/
			long tmp;
                                                                                                 
			tmp = xp;
			xp = yp;
			yp = tmp;
                          /* 求平均值 */                                                                        
                        xp >>= 2;
                        yp >>= 2;
     /* 报告x、y的绝对坐标值 */
 			input_report_abs(dev, ABS_X, xp);
 			input_report_abs(dev, ABS_Y, yp);

 /* 报告按键事件，键值为1(代表触摸屏对应的按键被按下) */
 			input_report_key(dev, BTN_TOUCH, 1);
 /* 报告触摸屏的状态，1表明触摸屏被按下 */
 			input_report_abs(dev, ABS_PRESSURE, 1);
/* 同步 */
 			input_sync(dev);
 		}

 		xp = 0;
 		yp = 0;
 		count = 0;
 		
    /*自动X/Y轴坐标转换模式的设置,自动地进行X轴和Y轴的转换操作，随后产生相应的INT_ADC中断通知转换完毕*/
 		iowrite32(S3C2410_ADCTSC_PULL_UP_DISABLE | AUTOPST, base_addr+S3C2410_ADCTSC);
 		
 		/*如果还没有启动ADC或者ADC转换四次完毕后则启动ADC*/
 		iowrite32(ioread32(base_addr+S3C2410_ADCCON) | S3C2410_ADCCON_ENABLE_START, base_addr+S3C2410_ADCCON);
 	} else {
 		count = 0;

 /* 如果触摸笔是弹起状态，则提出报告，并让触摸屏处于等待触摸的阶段 */
  /* 报告按键事件，键值为0(代表触摸屏对应的按键被释放) */
 		input_report_key(dev, BTN_TOUCH, 0);
  /* 报告触摸屏的状态，0表明触摸屏未被按下 */
 		input_report_abs(dev, ABS_PRESSURE, 0);
 /*同步 */
 		input_sync(dev);

 /* 设置触摸屏为等待中断模式，等待触摸笔按下 */
 		iowrite32(WAIT4INT(0), base_addr+S3C2410_ADCTSC);
		if (OwnADC) {
			OwnADC = 0;
			up(&ADC_LOCK);
		}
 	}
}

static struct timer_list touch_timer =
		TIMER_INITIALIZER(touch_timer_fire, 0, 0);


 /*当触摸按下时，产生INT_TC中断，会进入到stylus_updown中断处理函数*/
static irqreturn_t stylus_updown(int irq, void *dev_id)
{
	unsigned long data0;
	unsigned long data1;
	int updown;

/*注意在触摸屏驱动模块中，这个ADC_LOCK的作用是保证任何时候都只有一个驱动程序使用ADC的中断线，
因为在mini2440的adc模块中也会使用到ADC,这样只有拥有了这个锁，才能进入到启动ADC代码部分,
注意尽管LDD3中说过信号量因为休眠不适合使用在ISR中，但down_trylock是一个例外，它不会休眠*/

	if (down_trylock(&ADC_LOCK) == 0) {
		OwnADC = 1;
		data0 = ioread32(base_addr+S3C2410_ADCDAT0);
		data1 = ioread32(base_addr+S3C2410_ADCDAT1);

		updown = (!(data0 & S3C2410_ADCDAT0_UPDOWN)) && (!(data1 & S3C2410_ADCDAT0_UPDOWN));
 /*判断目前触摸笔的状态
 		updown = 1  触摸笔按下;
 		updown = 0  触摸笔弹起； 
  */
		if (updown) { 
			touch_timer_fire(0);  /*这是一个定时器函数，当然在这里只是作为普通函数调用，用来启动ADC*/
		} else {  
			OwnADC = 0;
			up(&ADC_LOCK);
		}
	}

	return IRQ_HANDLED;
}

/*INT_ADC 中断处理函数*/
static irqreturn_t stylus_action(int irq, void *dev_id)
{
	unsigned long data0;
	unsigned long data1;

	if (OwnADC) {
		data0 = ioread32(base_addr+S3C2410_ADCDAT0);
		data1 = ioread32(base_addr+S3C2410_ADCDAT1);

		xp += data0 & S3C2410_ADCDAT0_XPDATA_MASK;
		yp += data1 & S3C2410_ADCDAT1_YPDATA_MASK;
		count++;

	    if (count < (1<<2)) { /*如果小于四次重新启动ADC转换*/

			iowrite32(S3C2410_ADCTSC_PULL_UP_DISABLE | AUTOPST, base_addr+S3C2410_ADCTSC);
			iowrite32(ioread32(base_addr+S3C2410_ADCCON) | S3C2410_ADCCON_ENABLE_START, base_addr+S3C2410_ADCCON);
		} else {
			/*如果超过四次，启动1个时间滴答的定时器，停止ADC,然后调用定时器处理函数touch_timer_fire进行数据上报*/
			mod_timer(&touch_timer, jiffies+1);
			iowrite32(WAIT4INT(1), base_addr+S3C2410_ADCTSC);
		}
	}

	return IRQ_HANDLED;
}

static struct clk	*adc_clock;

static int __init s3c2410ts_init(void)
{
	struct input_dev *input_dev;

/*获取时钟，触摸屏是挂载在APB BUS上的外围设备，需要时钟控制，见数据手册第7章*/
	adc_clock = clk_get(NULL, "adc");
	if (!adc_clock) {
		printk(KERN_ERR "failed to get adc clock source\n");
		return -ENOENT;
	}
	clk_enable(adc_clock);

/*I/O内存是不能直接进行访问的，必须对其进行映射 */

	base_addr=ioremap(S3C2410_PA_ADC,0x20);
	if (base_addr == NULL) {
		printk(KERN_ERR "Failed to remap register block\n");
		return -ENOMEM;
	}

	/* Configure GPIOs */
	s3c2410_ts_connect(); /*GPIO 设置*/

  /*使能预分频和设置分频系数*/
	iowrite32(S3C2410_ADCCON_PRSCEN | S3C2410_ADCCON_PRSCVL(0xFF),\
		     base_addr+S3C2410_ADCCON););
  /*设置ADC延时，在等待中断模式下表示产生INT_TC的间隔时间,ADC转换周期等待定时器*/

	iowrite32(0xffff,  base_addr+S3C2410_ADCDLY);
	
	/*ADCTSC = 0xd3 ,将触摸屏置为等待中断模式，等待触摸屏被按下产生INT_TC中断*/
	iowrite32(WAIT4INT(0), base_addr+S3C2410_ADCTSC);

 /* 以下配置2.6内核划分出来的输入设备 */
	/* Initialise input stuff */
	input_dev = input_allocate_device();

	if (!input_dev) {
		printk(KERN_ERR "Unable to allocate the input device !!\n");
		return -ENOMEM;
	}

	dev = input_dev;
	
	 /* 
       evbit字段用来定义该输入设备可以支持的（产生和响应）的事件的类型，
       触摸屏设置为支持同步(EN_SYN)、按键(EN_KEY)、绝对坐标(EV_ABS)事件
    */

	dev->evbit[0] = BIT(EV_SYN) | BIT(EV_KEY) | BIT(EV_ABS);
	
	/* 设置所支持的按键(键值)，触摸屏可以看成只有一个按键的设备 */
	dev->keybit[BITS_TO_LONGS(BTN_TOUCH)] = BIT(BTN_TOUCH);
	
	/* 设置绝对坐标x的最小最大值(0-0x3FF) */
	input_set_abs_params(dev, ABS_X, 0, 0x3FF, 0, 0);
	
	/* 设置绝对坐标y的最小最大值(0-0x3FF) */
	input_set_abs_params(dev, ABS_Y, 0, 0x3FF, 0, 0);
	input_set_abs_params(dev, ABS_PRESSURE, 0, 1, 0, 0);

	dev->name = s3c2410ts_name;
	dev->id.bustype = BUS_RS232;
	dev->id.vendor = 0xDEAD;
	dev->id.product = 0xBEEF;
	dev->id.version = S3C2410TSVERSION;

  /*INT_TC, INT_ADC中断注册,问题：两种中断的产生时机？哪种中断会先产生？*/
	/* Get irqs */
	if (request_irq(IRQ_ADC, stylus_action, IRQF_SHARED|IRQF_SAMPLE_RANDOM,
		"s3c2410_action", dev)) {
		printk(KERN_ERR "s3c2410_ts.c: Could not allocate ts IRQ_ADC !\n");
		iounmap(base_addr);
		return -EIO;
	}
	if (request_irq(IRQ_TC, stylus_updown, IRQF_SAMPLE_RANDOM,
			"s3c2410_action", dev)) {
		printk(KERN_ERR "s3c2410_ts.c: Could not allocate ts IRQ_TC !\n");
		iounmap(base_addr);
		return -EIO;
	}

	printk(KERN_INFO "%s successfully loaded\n", s3c2410ts_name);

	/* All went ok, so register to the input system */
	/*注册输入设备*/
	input_register_device(dev);

	return 0;
}

static void __exit s3c2410ts_exit(void)
{
	disable_irq(IRQ_ADC);
	disable_irq(IRQ_TC);
	free_irq(IRQ_TC,dev);
	free_irq(IRQ_ADC,dev);

	if (adc_clock) {
		clk_disable(adc_clock);
		clk_put(adc_clock);
		adc_clock = NULL;
	}

	input_unregister_device(dev);
	iounmap(base_addr);
}


module_init(s3c2410ts_init);
module_exit(s3c2410ts_exit);

