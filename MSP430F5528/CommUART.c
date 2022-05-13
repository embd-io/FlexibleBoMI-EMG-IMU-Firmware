/*
 * CommUART.c
 */

#include "CommUART.h"
#include "msp430.h"
#include "driverlib.h"

uint16_t i = 0;

//Latyr********************************************************
void send_data_UART (char data[], int size)
{
	for(i=size; i>0; i--)
	{
		while (!(UCA1IFG&UCTXIFG));
		UCA1TXBUF = data[size-i]; 	// Send byte i
	}
}
//*************************************************************
