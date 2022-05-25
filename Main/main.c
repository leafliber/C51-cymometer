#include<reg52.h>
#include<stdio.h>

#include"public.h"
#include"lcd1602.h"
#include"key.h"
#include"timer.h"
#include"com_send.h"

u16 time_sig;
u16 time_50ms;
u16 time_gate_ms;
u8 send;
u8 pause;
u8 range_mode;
u8 gate_mode;
double result;

u8* range_mode_name[3] = {"L","M","H"};
u8* gate_mode_name[4] = {" 1"," 2"," 5","10"};
u8 gate_mode_number[4] = {1,2,5,10};

u8 timer0_th = 0x4c;
u8 timer0_tl[3] = {0xa0,0x16,0x14};


u8 range_auto;
u8 auto_count;
u16 range_h[3] = {100,50000,460000};

void show_string_mode()
{
	
	lcd1602_show_string(5,1,range_mode_name[range_mode]);
	lcd1602_show_string(13,1,gate_mode_name[gate_mode]);
	if(range_auto){
		lcd1602_show_string(6,1,"a");
	}
	else{
		lcd1602_show_string(6,1," ");
	}
}

void show_string_init()
{
	lcd1602_show_string(0,0,"Frq:");
	lcd1602_show_string(14,0,"Hz");
	lcd1602_show_string(0,1,"Mode:   Time:");
	if(range_mode){
		lcd1602_show_string(15,1,"s");
	}
	else{
		lcd1602_show_string(15,1,"T");
	}
}

void reset(){
	timer0_reset(timer0_th,timer0_tl[range_mode]);
	time_50ms=0;
	if(range_mode)
	{
		timer1_reset(0x00,0x00);
	}else
	{
		timer1_reset(0xFF,0xFF);
	}
	time_sig=0;
	result = 0;
}

void change_range()
{
	range_mode %= 3;
	show_string_init();
	show_string_mode();
}

int main()
{
	u8 key=0;
	range_mode = 1;
	gate_mode = 0;
	time_gate_ms = 20*gate_mode_number[gate_mode];
	
	lcd1602_init();
	send_init();

	show_string_init();
	show_string_mode();
	
	
	timer0_init(0x01,timer0_th,timer0_tl[range_mode]);
	timer1_init(0x50,0x00,0x00);
	timer0_open();
	timer1_open();
	
	while(1)
	{
		key=key_scan(0);
		if(key==KEY1_PRESS)
		{
			range_mode++;
			change_range();
			reset();
		}
		else if(key==KEY2_PRESS)
		{
			gate_mode++;
			gate_mode %= 4;
			time_gate_ms = 20*gate_mode_number[gate_mode];
			show_string_mode();
			reset();
		}
		else if(key==KEY3_PRESS)
		{
			range_auto = ~range_auto;
			show_string_mode();
		}
		else if(key==KEY4_PRESS)
		{
			lcd1602_clear();
			show_string_init();
			show_string_mode();
		}
		if(send==1)
		{
			char disp[10];
			if(range_mode)
			{
				result = (double)(time_sig*65536+TH1*256+TL1)/(double)(time_50ms*50+((TH0-timer0_th)*256+TL0-timer0_tl[range_mode])/1000)*1000;
			}
			else
			{
				result = (double)1000000*gate_mode_number[gate_mode]/(double)(time_50ms*50006+((TH0-timer0_th)*256)+TL0-timer0_tl[range_mode]);
			}
			if(range_auto && auto_count>0)
			{
				auto_count = 0;
				if(range_mode < 2 && result>range_h[range_mode])
				{
					range_mode += 1;
					change_range();
				}
				if(range_mode > 0 && result<range_h[range_mode-1])
				{
					range_mode += 2;
					change_range();
				}
			}
			else
			{
				auto_count++;
			}
			if(range_mode ==2){
				sprintf(disp,"%.2f",result);
			}
			else
			{
				sprintf(disp,"%.3f",result);
			}
			lcd1602_show_string(4,0,disp);
			send_string(disp);
			send_string("\r\n");
			reset();
			timer0_open();
			timer1_open();
			send=0;
		}
	}
}

void timer0() interrupt 1 //定时器0中断函数
{
	if(range_mode){
		timer0_close();
		timer1_close();
		timer0_reset(timer0_th,timer0_tl[range_mode]);
		time_50ms++;
		if(time_50ms==time_gate_ms)
		{
			send=1;
		}
		else
		{
			timer0_open();
			timer1_open();
		}
	}
	else
	{
		timer0_reset(timer0_th,timer0_tl[range_mode]);
		time_50ms++;
	}
}

void timer1() interrupt 3
{
	if(range_mode)
	{
		timer1_reset(0x00,0x00);
		time_sig++;
	}
	else
	{
		static u8 start = 0;
		timer1_reset(0xFF,0xFF);
		if(!start)
		{
			timer0_reset(timer0_th,timer0_tl[range_mode]);
			time_50ms = 0;
			start = 1;
		}
		else
		{
			time_sig++;
		}

		if(time_sig==gate_mode_number[gate_mode])
		{
			timer0_close();
			timer1_close();
			send=1;
			start = 0;
		}
	}
}