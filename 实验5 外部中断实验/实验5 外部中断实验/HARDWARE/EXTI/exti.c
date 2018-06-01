#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

/*
时间：180506
功能：串口打印"OK"，按下"WK_UP"键，LED0即D1翻转
后面的中断怎么写实在是不知道，只改了一下EXT0的中断服务程序
目的：尝试使用“void EXTI9_5_IRQHandler(void)”的PB9成功了，很好
时间：180601

*/

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(WK_UP==1)	 
	{
		LED0=!LED0;
//BEEP=!BEEP; //蜂鸣器翻转 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
}	
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(KEY2==0)	  
	{				 
   LED0=!LED0; 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE2上的中断标志位 
}
//外部中断3服务程序
void EXTI8_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(KEY1==0)	 
	{
		LED1=!LED1;
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line8);  //清除LINE3上的中断标志位  
}
//外部中断4服务程序
void EXTI9_5_IRQHandler(void)
{
	 //ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);

	delay_ms(10);	//消抖，消抖函数为什么在最上面啊
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)//好用的
	{				 
		LED0=!LED0;	
		LED1=!LED1;	
	}	
		else
		{
			if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
			{
				LED1=!LED1;
			}
		}
	 EXTI_ClearITPendingBit(EXTI_Line8);  
	 EXTI_ClearITPendingBit(EXTI_Line9);//清除LINE4上的中断标志位
}
/*原来KEY0运行成功的语句
		delay_ms(10);	//消抖，消抖函数为什么在最上面啊
	if(KEY0==0)	 
	{				 
		LED0=!LED0;	
		LED1=!LED1;	
	}	
	*/
	
/*增加的运行KEY1的语句，可以运行
   else
   {
			if(KEY1==0)//竟然不用判断哪个线，用KEY来判断就可以了
			{
				LED1=!LED1;
			}
	 }	
	 EXTI_ClearITPendingBit(EXTI_Line8);  
	 EXTI_ClearITPendingBit(EXTI_Line9);//清除LINE4上的中断标志位  
}
*/
	
/*
EXTI9_5的服务程序实例，中间应该会有怎么判断通道8和通道9的， 

void EXTI3_IRQHandler(void) 
{ 
if(EXTI_GetITStatus(EXTI_Line3)!=RESET)//判断某个线上的中断是否发生     
  { 
    EXTI_ClearITPendingBit(EXTI_Line3);    //清除LINE上的中断标志位   
}       

} 
这个我不准备写了，不知道寄存器应该是什么数字
void EXTI9_5_IRQHandler(void)
{			
	delay_ms(5);    			 
	if(X05==1)    
	{
	 TIM3->CR1&=0XFE;
	} 
 else if(X06==1)
 { 
}    
	EXTI->PR=1<<5;     //??LINE5???????  
	EXTI->PR=1<<6;     //??LINE6??????? 
}

*/  
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //按键对应的IO口初始化
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
/*
原程序中断线3改为8，中断线4改为9
	*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);//PB8 连接到中断线8
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);//PB9 连接到中断线9
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0
	
  /* 配置EXTI_Line0 */ 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	/* 配置EXTI_Line9,8 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9 | EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断8
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断9
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置   
}












