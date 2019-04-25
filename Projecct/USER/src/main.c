/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴LPC546XX_config.h�ļ��ڰ汾�궨��
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/

#include "headfile.h"




//----------------------------------------------------
//----------------------------------------------------
volatile int threshold=0;
uint8 min_gray=75;	
int flag2=0;
int flag1=0;
int num;
extern int16 beacon_x;
extern int16 beacon_y;
int16 x_x;


void pwm_init();

 //----------------------------------------------------
int main(void)
{   
    get_clk();
    gpio_init(B2,GPO,1,PULLUP);
		//uart_init(USART_0,115200,UART0_TX_A25,UART0_RX_A24); 
	  pwm_init();
	  lcd_init();  
		gpio_set(B2,0);
    //mrt_pit_init_ms(MRT_CH0,5);
		pit_init_ms(5); 
	
	  Encoder_Init();
	  camera_init(); 		
	  EnableInterrupts;
	  //uart_putchar(USART_0,0xA5);  
	  //McNamm_wheel_control(0,20,0);
    while(1)
    {

			
		if(mt9v032_finish_flag)
		{
			//pit_start();
			//mrt_start(MRT_CH0);
			threshold=MyOSTU(128,60,*image);
				
				if(threshold<min_gray)threshold=min_gray;
			
			//lcd_displayimageOUST(image[0],MT9V032_W,MT9V032_H,threshold);
				//lcd_displayimage032(image[0],MT9V032_W,MT9V032_H);
			
			/*if(flag1==0)lcd_displayimage032(image[0],MT9V032_W,MT9V032_H);
			else {
				lcd_displayimageOUST(image[0],MT9V032_W,MT9V032_H,threshold);
			}//*/
			Scan_Point();
			Car_Control();
			//Judge_distance(beacon_x,beacon_y);
			mt9v032_finish_flag = 0;		
			gpio_toggle(B2);
		}//


    }
}

//----------------------------------------------------------------
//----------------------------------------------------------------
void pwm_init()
{
    sct_pwm_init(SCT0_OUT2_A19,50,0);
		sct_pwm_init(SCT0_OUT3_A31,50,0);
	
		sct_pwm_init(SCT0_OUT4_A23,50,0);
	  sct_pwm_init(SCT0_OUT5_A26,50,0);
	
		sct_pwm_init(SCT0_OUT6_A27,50,0);
	  sct_pwm_init(SCT0_OUT7_A28,50,0);
	
		sct_pwm_init(SCT0_OUT8_A29,50,0);
	  sct_pwm_init(SCT0_OUT9_A30,50,0);
}

/*
����:PWMռ�ձȸ�ֵ
��ڲ���:��
����ֵ:��
����:  �˴���BUG   ��ȷ˳����  ��ǰ--��ǰ-���--�Һ�
*/
void control(int Right_rear_goalspeed,int Left_rear_goalspeed,int Right_front_goalspeed,int Left_front_goalspeed)//Left_rear_goalspeed
{
	if (Left_front_goalspeed>=0){//��ǰ
		sct_pwm_duty(SCT0_OUT2_A19,Left_front_goalspeed); 
		sct_pwm_duty(SCT0_OUT3_A31,0); 
	}else{
		Left_front_goalspeed=~Left_front_goalspeed+1;
		sct_pwm_duty(SCT0_OUT2_A19,0); 
		sct_pwm_duty(SCT0_OUT3_A31,Left_front_goalspeed); 	
	}
	if (Right_front_goalspeed>=0){//��ǰ
	  sct_pwm_duty(SCT0_OUT4_A23,Right_front_goalspeed); 
		sct_pwm_duty(SCT0_OUT5_A26,0); 	
	}else{
		Right_front_goalspeed=~Right_front_goalspeed+1;
	  sct_pwm_duty(SCT0_OUT4_A23,0); 
		sct_pwm_duty(SCT0_OUT5_A26,Right_front_goalspeed); 		
	}
	if (Left_rear_goalspeed>=0){//���
	  sct_pwm_duty(SCT0_OUT6_A27,Left_rear_goalspeed); 
		sct_pwm_duty(SCT0_OUT7_A28,0);
	}else{
		Left_rear_goalspeed=~Left_rear_goalspeed+1;
	  sct_pwm_duty(SCT0_OUT6_A27,0); 
		sct_pwm_duty(SCT0_OUT7_A28,Left_rear_goalspeed);
	}
		if (Right_rear_goalspeed>=0){//�Һ�
		sct_pwm_duty(SCT0_OUT8_A29,0); 
		sct_pwm_duty(SCT0_OUT9_A30,Right_rear_goalspeed);
		}else{
		Right_rear_goalspeed=~Right_rear_goalspeed+1;
		sct_pwm_duty(SCT0_OUT8_A29,Right_rear_goalspeed); 
		sct_pwm_duty(SCT0_OUT9_A30,0);
		}
}
//----------------------------------------------------------------
/*������
volatile int num=0;

ctimer_count_init(TIMER0_COUNT1_A2);

num = ctimer_count_read(TIMER0_COUNT1_A2);
*/


/*led*
 gpio_init(B2,GPO,1,PULLUP);

gpio_set(B2,0);
gpio_toggle(B2); //��ת
*/
/* PWM
		int pwm=0,flag=0;

		sct_pwm_init(SCT0_OUT2_A19,50,0);
		sct_pwm_init(SCT0_OUT3_A31,50,0);
		//sct_pwm_duty(SCT0_OUT2_A19,5000); 




			if(flag==0)pwm++;
			if(flag==1)pwm--;
			if(pwm>=10000) flag=1;
			if(pwm==0) flag=0;
			sct_pwm_duty(SCT0_OUT3_A31,pwm); 
			systick_delay_ms(10);
*/


