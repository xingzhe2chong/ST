#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"

//ALIENTEK ̽����STM32F407������ ʵ��5
//�ⲿ�ж�ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 
/*
ʱ�䣺180505
���ܣ����ڲ��ϴ�ӡ"OK",ֱ������"WK_UP"����LED0��D1��ת������KEY0,LED0��LED1����ת������LED1,LED1��ת
����ʹ�á�void EXTI9_5_IRQHandler(void)����PB9�ɹ��ˣ��ܺ�
ʱ�䣺180601
���ܣ���ѯ���ж�ʵ��PB8��PB9���жϷ�����
������ifǶ���������ģ���������Ƕ��֮�󰴼���׼���ǳ��������
��ν����ѯ������һ��WHILEһֱ�ʰ������Ǽ���GPIO��û�а��£����������취
���⣺��ѯ�İ�������
*/

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿�  
	//BEEP_Init();        //��ʼ���������˿�
	EXTIX_Init();       //��ʼ���ⲿ�ж����� 
	LED0=0;					    //�ȵ������
	
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


/*�ж�ʵ�ְ�����תԴ��
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	LED_Init();				  //��ʼ��LED�˿�  
	//BEEP_Init();        //��ʼ���������˿�
	EXTIX_Init();       //��ʼ���ⲿ�ж����� 
	LED0=0;					    //�ȵ������
	while(1)
	{
  	printf("OK\r\n");	//��ӡOK��ʾ��������
		delay_ms(1000);	  //ÿ��1s��ӡһ��
	}
}
*/
