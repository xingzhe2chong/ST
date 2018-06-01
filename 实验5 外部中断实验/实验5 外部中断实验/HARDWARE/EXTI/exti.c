#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

/*
ʱ�䣺180506
���ܣ����ڴ�ӡ"OK"������"WK_UP"����LED0��D1��ת
������ж���ôдʵ���ǲ�֪����ֻ����һ��EXT0���жϷ������
Ŀ�ģ�����ʹ�á�void EXTI9_5_IRQHandler(void)����PB9�ɹ��ˣ��ܺ�
ʱ�䣺180601

*/

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//����
	if(WK_UP==1)	 
	{
		LED0=!LED0;
//BEEP=!BEEP; //��������ת 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
}	
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY2==0)	  
	{				 
   LED0=!LED0; 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line2);//���LINE2�ϵ��жϱ�־λ 
}
//�ⲿ�ж�3�������
void EXTI8_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY1==0)	 
	{
		LED1=!LED1;
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line8);  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�4�������
void EXTI9_5_IRQHandler(void)
{
	 //ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);

	delay_ms(10);	//��������������Ϊʲô�������氡
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)//���õ�
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
	 EXTI_ClearITPendingBit(EXTI_Line9);//���LINE4�ϵ��жϱ�־λ
}
/*ԭ��KEY0���гɹ������
		delay_ms(10);	//��������������Ϊʲô�������氡
	if(KEY0==0)	 
	{				 
		LED0=!LED0;	
		LED1=!LED1;	
	}	
	*/
	
/*���ӵ�����KEY1����䣬��������
   else
   {
			if(KEY1==0)//��Ȼ�����ж��ĸ��ߣ���KEY���жϾͿ�����
			{
				LED1=!LED1;
			}
	 }	
	 EXTI_ClearITPendingBit(EXTI_Line8);  
	 EXTI_ClearITPendingBit(EXTI_Line9);//���LINE4�ϵ��жϱ�־λ  
}
*/
	
/*
EXTI9_5�ķ������ʵ�����м�Ӧ�û�����ô�ж�ͨ��8��ͨ��9�ģ� 

void EXTI3_IRQHandler(void) 
{ 
if(EXTI_GetITStatus(EXTI_Line3)!=RESET)//�ж�ĳ�����ϵ��ж��Ƿ���     
  { 
    EXTI_ClearITPendingBit(EXTI_Line3);    //���LINE�ϵ��жϱ�־λ   
}       

} 
����Ҳ�׼��д�ˣ���֪���Ĵ���Ӧ����ʲô����
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
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //������Ӧ��IO�ڳ�ʼ��
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
/*
ԭ�����ж���3��Ϊ8���ж���4��Ϊ9
	*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);//PB8 ���ӵ��ж���8
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);//PB9 ���ӵ��ж���9
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��ж���0
	
  /* ����EXTI_Line0 */ 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
	/* ����EXTI_Line9,8 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9 | EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�8
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�9
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����   
}












