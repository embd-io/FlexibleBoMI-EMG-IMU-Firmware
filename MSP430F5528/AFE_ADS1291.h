/*
 * AFE_ADS1291.h
 *
 *  Created on: 2016-06-14
 *      Author: Etudiant
 */

#ifndef AFE_ADS1291_H_
#define AFE_ADS1291_H_

#include "driverlib.h"
#include "msp430.h"

//OPCODES
#define SDATAC 		0x11
#define RDATAC 		0x10
#define RDATA 		0x12
#define START 		0x08
#define STOP 		0x0A
#define WAKEUP 		0x02
#define STANDBY 		0x04
//REGISTERS
#define CONFIG1		0x01
#define CONFIG2		0x02
#define LOFF		0x03
#define CH1SET		0x04
#define CH2SET		0x05
#define RLDSENS		0x06
#define LOFFSENS	0x07
#define LOFFSTAT	0x08
#define RESP1		0x09
#define RESP2		0x0A
#define GPIO		0x0B

#define SET_CS_AFE 		P1OUT &= ~BIT2
#define CLR_CS_AFE		P1OUT |= BIT2
#define CLR_DRDY_FLAG	P1IFG &= ~BIT3

#define Fs_125_Hz 		0x00
#define Fs_500_Hz 		0x02
#define Fs_1000_Hz 		0x03
#define Fs_2000_Hz 		0x04
#define Fs_4000_Hz 		0x05
#define Fs_8000_Hz 		0x06


#define NORM_GAIN_1 	0x10
#define NORM_GAIN_2 	0x20
#define NORM_GAIN_3 	0x30
#define NORM_GAIN_4 	0x40
#define NORM_GAIN_6 	0x00
#define NORM_GAIN_8 	0x50
#define NORM_GAIN_12 	0x60
#define TEST_1Hz 		0x15//0x05

#define	WAIT_TIL_SPI_B0_GETS_READY while(!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT))

#define START_AFE       P1OUT |= BIT4
#define STOP_AFE        P1OUT &= ~BIT4

#define DISABLE_AFE     P1IE &= ~BIT1//; STOP_AFE
#define ENABLE_AFE      P1IE |= BIT1//; START_AFE

#define CONFIG_AFE      TA0CCTL0 |= 0x10//; //WAIT TO START AFE        // TimerA0 enabled

void init_AFE (void);
void set_AFE (void);
void config_AFE (char TEST_STATUS);
void AFE_reg_write (char ADDR, char VALUE);
void write_OPCODE (char OPCODE);
void set_test_signal (void);

#endif /* AFE_ADS1291_H_ */
