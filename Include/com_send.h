#ifndef _com_send_H
#define _com_send_H

#include "reg52.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L      //System frequency
#define BAUD 9600       //UART baudrate

bit busy;

void send_init();
void send_data(BYTE dat);
void send_string(char *s);

void send_init()
{
    SCON = 0x50;
		TL2 = RCAP2L = (65536-(FOSC/32/BAUD)); //Set auto-reload vaule
    TH2 = RCAP2H = (65536-(FOSC/32/BAUD)) >> 8;
    T2CON = 0x34;           //Timer2 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}

/*----------------------------
UART interrupt service routine
----------------------------*/
void uart_isr() interrupt 4
{
    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag
    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void send_data(BYTE dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)
    busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void send_string(char *s)
{
    while (*s)              //Check the end of the string
    {
        send_data(*s++);     //Send current char and increment string ptr
    }
}

#endif