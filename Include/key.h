#ifndef _key_H
#define _key_H

#include "public.h"

//��������������ƽ�
sbit KEY1=P3^1;
sbit KEY2=P3^0;
sbit KEY3=P3^2;
sbit KEY4=P3^3;


//ʹ�ú궨������������µļ�ֵ
#define KEY1_PRESS	1
#define KEY2_PRESS	2
#define KEY3_PRESS	3
#define KEY4_PRESS	4
#define KEY_UNPRESS	0


u8 key_scan(u8 mode);

/*******************************************************************************
* �� �� ��       : key_scan
* ��������		 : �����������Ƿ��£������򷵻ض�Ӧ��ֵ
* ��    ��       : mode=0������ɨ�谴��
				   mode=1������ɨ�谴��
* ��    ��    	 : KEY1_PRESS��K1����
				   KEY2_PRESS��K2����
				   KEY3_PRESS��K3����
				   KEY4_PRESS��K4����
				   KEY_UNPRESS��δ�а�������
*******************************************************************************/
u8 key_scan(u8 mode)
{
	static u8 key=1;

	if(mode)key=1;//����ɨ�谴��
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))//���ⰴ������
	{
		delay_10us(1000);//����
		key=0;
		if(KEY1==0)
			return KEY1_PRESS;
		else if(KEY2==0)
			return KEY2_PRESS;
		else if(KEY3==0)
			return KEY3_PRESS;
		else if(KEY4==0)
			return KEY4_PRESS;	
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)	//�ް�������
	{
		key=1;			
	}
	return KEY_UNPRESS;		
}

#endif