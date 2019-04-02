#include "Encoder.h"


int16 Left_rear_speed;/*��ǰ���ٶ�*/
int16 Left_front_speed;/*��ǰ���ٶ�*/
int16 Right_front_speed;/*��ǰ���ٶ�*/
int16 Right_rear_speed;/*�Һ����ٶ�*/

void Encoder_Init()
{
	ctimer_count_init(TIMER0_COUNT1_A2);
	ctimer_count_init(TIMER3_COUNT0_A4);
	ctimer_count_init(TIMER4_COUNT0_A15);
	ctimer_count_init(TIMER1_COUNT0_A16);
	gpio_init(A3,GPI,0,PULLUP);
  gpio_init(A5,GPI,0,PULLUP);
	gpio_init(B18,GPI,0,PULLUP);
  gpio_init(B6,GPI,0,PULLUP);
	Pid_Init();
}


/*�ٶȲɼ���������ʱ��������*/
void speed_get(void)
{ 
			//ctimer_count_clean(TIMER0_COUNT0_A1);
	if(gpio_get(A3))Left_rear_speed=ctimer_count_read(TIMER0_COUNT1_A2); 
		else  Left_rear_speed=-ctimer_count_read(TIMER0_COUNT1_A2); 
	 
	if(gpio_get(A5))Right_rear_speed=-ctimer_count_read(TIMER3_COUNT0_A4); 
		else  Right_rear_speed=ctimer_count_read(TIMER3_COUNT0_A4); 
	 
	if(gpio_get(B18))Left_front_speed=ctimer_count_read(TIMER4_COUNT0_A15); 
		else  Left_front_speed=-ctimer_count_read(TIMER4_COUNT0_A15); 
	 
	if(gpio_get(B6))Right_front_speed=-ctimer_count_read(TIMER1_COUNT0_A16); 
		else  Right_front_speed=ctimer_count_read(TIMER1_COUNT0_A16); 
	
	ctimer_count_clean(TIMER0_COUNT1_A2);
	ctimer_count_clean(TIMER3_COUNT0_A4);
	ctimer_count_clean(TIMER4_COUNT0_A15);
	ctimer_count_clean(TIMER1_COUNT0_A16);

}


/*��ͨ���ٶ��˲�����*///�����˲����֣��ڳ�����ɼӿɲ��ӣ���û�ӣ�����Ҳ���ԡ����ǿ���������Ч��
int16 Speed_Filter(int16 Channal,int16 Data_In)
{
	static int16	Data[2][3]={0};
	int16 Temp=0;
	Data[Channal][0]=Data[Channal][1];
	Data[Channal][1]=Data[Channal][2];
	Data[Channal][2]=Data_In;
	Temp=PaiXu(Data[Channal][0],Data[Channal][1],Data[Channal][2]);
	
	return Temp;
}
