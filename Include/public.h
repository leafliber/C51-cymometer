#ifndef _public_H
#define _public_H

#include "reg52.h"

typedef unsigned int u16;	//对系统默认数据类型进行重定义
typedef unsigned char u8;
typedef unsigned long u32;

//函数声明
void delay_10us(u16 ten_us);
void delay_ms(u16 ms);

/*******************************************************************************
* 函 数 名       : delay_10us
* 函数功能		 : 延时函数，ten_us=1时，大约延时10us
* 输    入       : ten_us
* 输    出    	 : 无
*******************************************************************************/
void delay_10us(u16 ten_us)
{
	while(ten_us--);	
}

/*******************************************************************************
* 函 数 名       : delay_ms
* 函数功能		 : ms延时函数，ms=1时，大约延时1ms
* 输    入       : ms：ms延时时间
* 输    出    	 : 无
*******************************************************************************/
void delay_ms(u16 ms)
{
	u16 i,j;
	for(i=ms;i>0;i--)
		for(j=110;j>0;j--);
}


#endif
