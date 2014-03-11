/*************************************************
Function name: ���ǻ���ʵ���һ��ģ��
Parameter    : ��
Description	 : ������ʵ�飬ֱ�ӵ��ø�ģ�弴��
Return		 : ��
Argument     : ��
Autor & date : Daniel
**************************************************/
#define	GLOBAL_CLK		1
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"
#include "memtest.h"


void Led_port_init(void);
void Disp_run(void);
void Disp_num(int data);

#define LED1_ON   ~(1<<5)
#define LED2_ON   ~(1<<6)
#define LED3_ON   ~(1<<7)
#define LED4_ON   ~(1<<8)

#define LED1_OFF   (1<<5)
#define LED2_OFF   (1<<6)
#define LED3_OFF   (1<<7)
#define LED4_OFF   (1<<8)
/*************************************************
Function name: delay
Parameter    : times
Description	 : ��ʱ����
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void delay(int times)
{
    int i;
    for(;times>0;times--)
      for(i=0;i<400;i++);
}
/*************************************************
Function name: Main
Parameter    : void
Description	 : �����ܺ���
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)
{	
    int i;
    Led_port_init();
    //Disp_run();  
	for(i=0;i<16;i++)
	{
	  Disp_num(i);
	  delay(1000);
	}
	while(1);
}	

/*************************************************
Function name: Led_port_init
Parameter    : void
Description	 : mini2440��������4������������LED����
               �����ܣ��ֱ���GPB5--LED1��GPB6��LED2
               GPB7--LED3��GPB8--LED4���ú���������
               �Ƕ�������Ž��г�ʼ����
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/   
void Led_port_init(void)
{
   rGPBCON &= ~((3<<10)|(3<<12)|(3<<14)|(3<<16));
   rGPBCON |= (1<<10)|(1<<12)|(1<<14)|(1<<16);
   
    //rGPBCON &= ~(3<<0);
    //rGPBCON |= (2<<0);
}

/*************************************************
Function name: Disp_run()
Parameter    : void
Description	 : mini2440��������4������������LED����
               �����ܣ��ú���ʵ������ˮ��
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/   
void Disp_run(void)
{
    int i;
    int ledval=0x1;
    rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
    while(1)
    {
     rGPBDAT = rGPBDAT&(LED1_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
     rGPBDAT = rGPBDAT&(LED2_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
     rGPBDAT = rGPBDAT&(LED3_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
     rGPBDAT = rGPBDAT&(LED4_ON);
     delay(1000);
     rGPBDAT = rGPBDAT|(LED1_OFF)|(LED2_OFF)|(LED3_OFF)|(LED4_OFF);
     delay(1000);
    }
}

/*************************************************
Function name: Disp_num()
Parameter    : void
Description	 : mini2440��������4������������LED����
               �����ܣ��ú���ʵ������ʾ0x0--0xF��ʮ��
               ������
Return		 : void
Argument     : void
Autor & date : Daniel
**************************************************/   
void Disp_num(int data)
{
  if(data & 0x08)   rGPBDAT = rGPBDAT&(LED4_ON);
     else           rGPBDAT = rGPBDAT|(LED4_OFF);
  if(data & 0x04)   rGPBDAT = rGPBDAT&(LED3_ON);
     else           rGPBDAT = rGPBDAT|(LED3_OFF);
  if(data & 0x02)   rGPBDAT = rGPBDAT&(LED2_ON);
     else           rGPBDAT = rGPBDAT|(LED2_OFF);
  if(data & 0x01)   rGPBDAT = rGPBDAT&(LED1_ON);
     else           rGPBDAT = rGPBDAT|(LED1_OFF);
    
}