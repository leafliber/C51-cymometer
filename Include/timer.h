#ifndef _timer_H
#define _timer_H

#include "public.h"

void timer0_init(u8 tmod, u8 th0, u8 tl0);
void timer0_reset(u8 th0, u8 tl0);
void timer1_init(u8 tmod, u8 th0, u8 tl0);
void timer1_reset(u8 th0, u8 tl0);

void timer0_init(u8 tmod, u8 th0, u8 tl0)
{
	TMOD=(TMOD&0xF0)|(tmod&0x0F);//选择为定时器0模式
	TH0=th0;
	TL0=tl0;
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=0;
}

void timer0_open()
{
	TR0=1;//打开定时器
}

void timer0_close()
{
	TR0=0;
}

void timer0_reset(u8 th0, u8 tl0)
{
	TH0=th0;
	TL0=tl0;
}

void timer1_init(u8 tmod, u8 th1, u8 tl1)
{
	TMOD=(TMOD&0x0F)|(tmod&0xF0);//选择为定时器1模式
	TH1=th1;
	TL1=tl1;	
	ET1=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR1=0;//打开定时器
}

void timer1_reset(u8 th1, u8 tl1)
{
	TH1=th1;
	TL1=tl1;
}

void timer1_open()
{
	TR1=1;//打开定时器
}

void timer1_close()
{
	TR1=0;
}

#endif