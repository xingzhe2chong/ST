#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"

//ALIENTEK 探索者STM32F407开发板 实验5
//外部中断实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 
/*
时间：180505
功能：串口不断打印"OK",直到按下"WK_UP"键，LED0即D1翻转，按下KEY0,LED0和LED1都翻转，按下LED1,LED1翻转
尝试使用“void EXTI9_5_IRQHandler(void)”的PB9成功了，很好
时间：180601
功能：轮询和中断实现PB8和PB9的中断服务函数
理论上if嵌套是无穷多的，但是三层嵌套之后按键不准，是程序的问题
所谓的轮询好像还是一个WHILE一直问按键的那几个GPIO有没有按下，就是正常办法
待解：轮询的按键消抖
*/

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200); 	//串口初始化 
	LED_Init();				  //初始化LED端口  
	//BEEP_Init();        //初始化蜂鸣器端口
	EXTIX_Init();       //初始化外部中断输入 
	LED0=0;					    //先点亮红灯
	
	while(1)
	{
		delay_ms(10);
  	if ( WK_UP == 1)
		{
			LED0=!LED0;
		}
		else if( KEY0==0 )
			{
				LED0 =! LED0;
				LED1 =! LED1;
			}
			else if( KEY1 == 0 )
			{
				LED1 =! LED1;
			}
	}
}


/*中断实现按键翻转源码
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200); 	//串口初始化 
	LED_Init();				  //初始化LED端口  
	//BEEP_Init();        //初始化蜂鸣器端口
	EXTIX_Init();       //初始化外部中断输入 
	LED0=0;					    //先点亮红灯
	while(1)
	{
  	printf("OK\r\n");	//打印OK提示程序运行
		delay_ms(1000);	  //每隔1s打印一次
	}
}
*/
