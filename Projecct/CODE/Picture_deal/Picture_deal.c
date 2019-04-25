#include "Picture_deal.h"
#define ROW              60 //188            
#define COL              128//120  


extern volatile int threshold;
extern int16 x_x;
int16 beacon_x=0;
int16 beacon_y=0;
int16 twinkle_delay=0;


int16 Coordinate_Filter(int16 Channal,int16 Data_In)
{
	static int16	Data[3][3]={0};
	int16 Temp=0;
	Data[Channal][0]=Data[Channal][1];
	Data[Channal][1]=Data[Channal][2];
	Data[Channal][2]=Data_In;
	Temp=PaiXu(Data[Channal][0],Data[Channal][1],Data[Channal][2]);
	
	return Temp;
}

int16 X1[COL+20]= {0};
int16 Y1[COL+20]= {0};
u16 total_count = 0;

void Scan_Point(void)
{	   
	int16 i = 0, j = 0;
	int16 X=0,Y=0;
	uint8 pot_1=0;
	uint8 pot_2=0;
	int16 count_x = 0;
	int16 first = 1;
	u8 flag = 0;
	u8 total_flag = 0;
	u16 total = 0;
	static char delay_flag=0;
	for(u8 z =0;z<COL+20;z++)
	{
		X1[z]=0;
		Y1[z]=0;
	}
	
	for( i =0; i<ROW;i++)
	{				
		for(j =0; j < COL;j++)
		{
			if(image[i][j] > threshold)                //�����ֵ�����õ��׵�
			{  
				count_x++;                          //�׵���ͳ��
			}
			else 
			{
				if(count_x>=2)
				{
				    X = j-count_x/2;   //һ���ɫ������е�λ��
				    Y = i;
				    if(first==1)
				    {
							X1[pot_1++] =X;
							Y1[pot_2++] =Y;
							first = 2;
				    }

				    else if (Error(X,X1[pot_1-1])<4&&Error(Y,Y1[pot_1-1])<=2)
				    {
							X1[pot_1++] =X;
							Y1[pot_2++] =Y;
							flag =1;
				    } 
				}
					count_x = 0;
			}
		}
	}

	if(flag ==1)  //�ҵ��ű��
	{
		delay_flag=1;
		beacon_x=X1[pot_1>>1];
		beacon_y=Y1[pot_2>>1];
	}
	else if(delay_flag == 1 && flag ==0)
	{
		beacon_x+=COL/2;
		twinkle_delay=10;//���Time.c�����ע�Ϳ���仰�Ͷ���
		delay_flag=0;
		total_flag =1;
	}
	else
	{
		if(twinkle_delay == 0)
		{
			beacon_x = 0;
			beacon_y = 0;
			total_flag =0;
		}
		else
		{
			beacon_x+=COL/2;
			total_flag =1;
		}
	}
	
	beacon_x -= COL/2;
	beacon_x=limit_s16(beacon_x,-COL/2,COL/2);
	beacon_y=limit_s16(beacon_y,0,60);
	beacon_x =Coordinate_Filter(0,beacon_x);
	beacon_y =Coordinate_Filter(1,beacon_y);
	if(beacon_x == -COL/2 && beacon_y == 0)
		beacon_x = 0;

	if(beacon_x ==0 && beacon_y==0)
	{
		total_count = 0;
	}
	else                                     //�����ԣ�������ͷ�����İ׵���ʱ������Զ���������İ׵��ʱ�����ڽ�����������仰�жϰ׵������Դ����жϵƵ�Զ��
	{
		for(int m =0; m<ROW;m++)	
			for(int n =0; n < COL;n++)
			{
				if(image[m][n] > threshold)                //�����ֵ�����õ��׵�
				{  
					total++;                          //�׵���ͳ��
				}
			}
		if(total_flag == 0)
			total_count = total;
	}
	total_count =Coordinate_Filter(2,total_count);
x_x=beacon_x;
}
//����ű�Ƶ�Զ��������
unsigned char  Judge_distance(int16 x,int16 y)   //���ű�Ƶ������ȥ
{
	static u16 beacon_maybe_blocked =0;  //�ƿ��ܱ���ס  ��Ϊ���ĸ߶Ⱦ�20cm���Ƶĸ߶���12cm�����������һ��ֱ�����ж���ƣ�������˸���ű�ƻᱻ�����Ƶ�ס����������ͷ����������ʱ������ԭ��תȦ���Ҳ����Ļ�����ʹ������
	if((x ==0 && y==0 )|| (x>=58 && x<=-58))   //���û���ҵ��ƣ������ҵ��ĵ�̫ƫ������
	{
		beacon_maybe_blocked ++;
		if(beacon_maybe_blocked >=280 && beacon_maybe_blocked <= 320) //תȦ2���ƽ��0.3��
		{
			return FOREC_RAMPAGE;  //ǿ�ƺ��� FOREC_RAMPAGE
		}
		else
		{
			if(beacon_maybe_blocked > 320)
				beacon_maybe_blocked =0;
			return roll;   //û��ʱԭ�ش�ת
		}
	}
	else
	{
		beacon_maybe_blocked=0;
		if(total_count<16)                          //������Ҫ����
			return far;
		if(total_count<65 && total_count>=16)
		{
			return near;
		}
		if(total_count>=65 && total_count<=95)	
		{
			return very_near;
		}
		if(total_count>95)	
		{
			return very_very_near;
		}
	}
	return 0;   //������仰�о���
}

