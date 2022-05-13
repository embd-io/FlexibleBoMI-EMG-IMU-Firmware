/*
 * AFE_ADS1291.c
 *
 *  Created on: 2016-06-14
 *      Author: Etudiant
 */

#include "AFE_ADS1291.h"

/******************
 * void init_AFE (void)
 * initializes MSP430-ADS1291 interface
 */
void init_AFE (void)
{
	// CLKSEL (internal)
	P1DIR |= BIT7;
	P1OUT |= BIT7;

	// SET I/O
	// MISO, MOSI, CLK output
	P4DIR |= BIT1 + BIT3;
	P4DIR &= ~BIT2;
	P4SEL |= BIT1 + BIT2 + BIT3;

	// CS = 1.2, START = 1.4, /PWDN = 1.5, (CLKSEL = 1.7)
	P1DIR |= BIT2 + BIT4 + BIT5;
	//P1OUT &= ~BIT4;

	// /DRDY = 1.1 (interrupt high-to-low)
	GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN1);
	GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
	GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);

	//SET UCB1 SPI
	UCB1CTL0 |= /*UCCKPH +*/ UCMSB + UCMST + UCSYNC;
	UCB1CTL1 |= UCSSEL1 + UCSSEL0 + UCSWRST;
	UCB1BR0 = 0x1A; // clock prescaler = 1 --> CLK = SMCLK
	UCB1BR1 = 0x00; //
	UCB1CTL1 &= ~UCSWRST; // bring the state machine output of reset

	//TIE START AND /PWDN HIGH
	P1OUT &= ~BIT5; //PWDN LOW
	__delay_cycles(100);
	P1OUT &= ~BIT4; //START LOW
	P1OUT |= BIT5;  //PWDN HIGH
	__delay_cycles(30);

	//P1OUT |= BIT4;//START HIGH //TEST
}


/*******************
 * void set_AFE (void)
 * sets ADS1291 pins (page 63 datasheet)
 */
void set_AFE (void)
{

	//TIE START AND /PWDN HIGH
	P1OUT &= ~BIT5; //PWDN LOW
	__delay_cycles(100);
	P1OUT &= ~BIT4; //START LOW
	P1OUT |= BIT5;  //PWDN HIGH
	__delay_cycles(30);

	//P1OUT |= BIT4;//START HIGH //TEST
}


/*******************
 * void config_AFE (void)
 * sets ADS1291's register values
 */
void config_AFE (char TEST_STATUS)
{
	char VALUE;

	// SDATAC command
	write_OPCODE (SDATAC);
	__delay_cycles(10);

	SET_CS_AFE;

	// CONFIG1 register
	VALUE = Fs_2000_Hz;
	AFE_reg_write (CONFIG1, VALUE);
	__delay_cycles(50);
	// CONFIG2 register
	VALUE = 0xA0;//0x80;//
	AFE_reg_write (CONFIG2, VALUE);
	__delay_cycles(50);
	// CH1SET register
	VALUE = NORM_GAIN_12;
	AFE_reg_write (CH1SET,  VALUE);
	__delay_cycles(50);
	// CH2SET  register
	VALUE = 0x91;
	AFE_reg_write (CH2SET,  VALUE);
	__delay_cycles(50);
	// RLDSENS  register
	VALUE = 0xA0;
	AFE_reg_write (RLDSENS, VALUE);
	__delay_cycles(50);
	// LOFFSENS  register
	VALUE = 0x00;
	AFE_reg_write (LOFFSENS,VALUE);
	__delay_cycles(50);
	// RESP2     register
	//VALUE = 0x01;
	//AFE_reg_write (RESP2,   VALUE);
	//__delay_cycles(50);
	// LOFFSTAT  register
	//VALUE = 0x00;
	//AFE_reg_write (LOFFSTAT,VALUE);
	//__delay_cycles(50);

	if (TEST_STATUS == 1) //Test signal, Amp = VREFP/2400
	{
	/* TEST****************************/
		VALUE = Fs_125_Hz;
		AFE_reg_write (CONFIG1, VALUE);
		__delay_cycles(50);
		// WREG CONFIG2 A3h
		VALUE = 0xA3;
		AFE_reg_write (CONFIG2, VALUE);
		__delay_cycles(50);
		// WREG CHnSET 05h
		VALUE = TEST_1Hz;
		AFE_reg_write (CH1SET,  VALUE);
		__delay_cycles(50);
		// WREG CHnSET 05h
		VALUE = 0x81;
		AFE_reg_write (CH2SET,  VALUE);
		__delay_cycles(50);
	/***********************************/
	}

	CLR_CS_AFE;

	// Set START = 1
	//P1OUT |= BIT4;//START HIGH
	write_OPCODE (START);
	__delay_cycles(10);

	// RDATAC command
	write_OPCODE (RDATAC);
	__delay_cycles(10);

	// command test
	//write_OPCODE (STANDBY);
	//write_OPCODE (WAKEUP);
}


/*******************
 * void AFE_reg_write (char ADDR, char VALUE)
 * Param
 * 	- ADDR  : address of register or opcode
 * 	- VALUE : register value to be set
 */
void AFE_reg_write (char ADDR, char VALUE)
{
    ADDR = ADDR & 0x1F;
    ADDR = ADDR | 0x40;
    //VALUE = VALUE & 0x1F;

    //SET_CS_AFE;

    UCB1TXBUF = ADDR;
    while(UCB1STAT & UCBUSY);
    UCB1TXBUF = 0x00;
    while(UCB1STAT & UCBUSY);
    UCB1TXBUF = VALUE;
    while(UCB1STAT & UCBUSY);

    //CLR_CS_AFE;
}

/*******************
 * void write_OPCODE (char OPCODE)
 * sends opcodes
 * Param : target opcode
 */
void write_OPCODE (char OPCODE)
{
	SET_CS_AFE;

	UCB1TXBUF = OPCODE;

	while(UCB1STAT & UCBUSY);

	CLR_CS_AFE;
}


void set_test_signal (void)
{
	SET_CS_AFE;

	char VALUE;
	// Activate a (1 mV VREF/2.4) Square-Wave Test Signal
	// On All Channels
	write_OPCODE (SDATAC);
	// WREG CONFIG2 A3h
	VALUE = 0xB3;
	AFE_reg_write (CONFIG2, VALUE);
	__delay_cycles(50);
	// WREG CHnSET 05h
	VALUE = 0x05;
	AFE_reg_write (CH1SET,  VALUE);
	__delay_cycles(50);
	// WREG CHnSET 05h
	VALUE = 0x05;
	AFE_reg_write (CH2SET,  VALUE);
	__delay_cycles(50);
	// CONFIG2 register
	/*VALUE = 0xB3;
	AFE_reg_write (CONFIG2, VALUE);
	__delay_cycles(50);*/
	/*****************/
	VALUE = 0x00;
	AFE_reg_write (CONFIG1, VALUE);
	__delay_cycles(50);
	/*****************/
	write_OPCODE (RDATAC);

	CLR_CS_AFE;
}
