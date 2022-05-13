/*
 * capteur_intertiel.c
 *
 *  Created on: 2014-10-05
 *      Author: Etudiant
 */

#include "capteur_inertiel.h"

void IMU_NORMAL_MODE ()
{
    GYRO_NORMAL_MODE;
    ACC_NORMAL_MODE;
    MAG_NORMAL_MODE;
}

void IMU_POWER_DOWN ()
{
    GYRO_POWER_DOWN;
    ACC_POWER_DOWN;
    MAG_POWER_DOWN;
}

void set_IMU ()
{
	startG();
	startXM();
}

void startG()
{
	SET_CSG_IMU1;

	//************************************ CONFIG3.1
	write_IMU(0x20, 0x7F, G_IMU1); //GODR[1:0] is used for ODR selection = 01b --> 190 Hz. BW [1:0] is used for Bandwidth selection = 11b --> 70 Hz.
	//write_IMU(0x20, 0xFF, G_IMU1); //GODR[1:0] is used for ODR selection = 11b --> 760 Hz. BW [1:0] is used for Bandwidth selection = 11b --> 100 Hz.
	write_IMU(0x21, 0x09, G_IMU1); //normal mode, set hp filter's frequency to 0.09 Hz (depends on ODR)
	write_IMU(0x23, 0x30, G_IMU1); //continuous mode, 2000 dps full scale (GAIN = 0.07)
	write_IMU(0x24, 0x13, G_IMU1); //enable high pass filter, select HPFout on CTRL_REG5_G (24h)
	write_IMU(0x2E, 0x00, G_IMU1); //bypass mode

	//INTERRUPT TEST
	/*P5SEL &= ~(BIT2+BIT3);// selection
	P5DIR &= ~BIT3;       // Set INT1_G pin to input
	P5IN  &= ~BIT3;
	//P5IE |= BIT3;         // Enable INT1_G interrupt
	//P5IES &= ~BIT3;       // low-to-hi interrupt
	//P5IFG &= ~BIT3;       // clear INT1_G interrupt
	write_IMU(0x22, 0x80, G_IMU1); //Interrupt enable on INT_G, Interrupt active High
	write_IMU(0x30, 0x7F, G_IMU1); //Interrupt Latched, XYZ enabled
	write_IMU(0x38, 0x00, G_IMU1); //WAIT = 0
	//TH X
	write_IMU(0x32, 0x7F, G_IMU1); //HIGH
	write_IMU(0x33, 0xFF, G_IMU1); //LOW
	//TH Y
	write_IMU(0x34, 0x7F, G_IMU1); //HIGH
	write_IMU(0x35, 0xFF, G_IMU1); //LOW
	//TH Z
	write_IMU(0x36, 0x7F, G_IMU1); //HIGH
	write_IMU(0x37, 0xFF, G_IMU1); //LOW*/
	//**********************************************
}

void startXM()
{
	SET_CSXM_IMU1;

	//********************************************
	write_IMU(0x20, 0x67, XM_IMU1); //AODR[3:0] = 0b0110 (6) --> 100 Hz, BDU = 0 (Continuous mode), AXYZ = 0b111 (7)
	write_IMU(0x21, 0xC0, XM_IMU1); //ABW [1:0] = 0b11 --> 50  Hz, AFS[3:0] = 0b000 --> +-2g, AST[1:0] = 0b00 (self test disabled), SIM = 0b0 --> 4 wire SPI
	write_IMU(0x22, 0x00, XM_IMU1); //INT1, all disabled
	write_IMU(0x23, 0x00, XM_IMU1); //INT2, all disabled
	write_IMU(0x24, 0x14, XM_IMU1); //TEMP_EN = 0, M_RES = 0b00 --> (low resolution?), MODR[2:0] = 0b101 --> (100 Hz)
	write_IMU(0x25, 0x00, XM_IMU1); //0x00 = +- 2 gauss //0x20 = +- 4 gauss
	write_IMU(0x26, 0x80, XM_IMU1); //Magneto normal without reset, high-pass filter active
	//write_IMU(0x26, 0x03, XM_IMU1); //Magneto normal with reset, high-pass filter not active
	//********************************************
}

int getWHO_I_AM(int8_t sensor_id)
{
	int a = 0;

	RESET_CSXM_IMU1;
	a = (int)(read_IMU(0x0F, sensor_id));
	SET_CSXM_IMU1;
	return a;

}
/********************************** Magnetometre *******************************************/
int getMagnetoX (int8_t sensor_id)
{
	return (int)((read_IMU(0x09, sensor_id) & 0xFF) << 8 ) + read_IMU(0x08, sensor_id);//lecture de OUT_X_H et OUT_X_L
}

int getMagnetoY(int8_t sensor_id)
{
	return (int)((read_IMU(0x0B, sensor_id) & 0xFF) << 8 ) + read_IMU(0x0A, sensor_id);//lecture de OUT_Y_H et OUT_Y_L et affichage
}

int getMagnetoZ(int8_t sensor_id)
{
	return (int)((read_IMU(0x0D, sensor_id) & 0xFF) << 8 ) + read_IMU(0x0C, sensor_id);//lecture de OUT_Z_H et OUT_Z_L et affichage
}
/************************************ Gyroscope ********************************************/
int getGyroX(int8_t sensor_id)
{
	return (int)((read_IMU(0x29, sensor_id) & 0xFF) << 8 ) + read_IMU(0x28, sensor_id);//lecture de OUT_X_H et OUT_X_L et affichage
}

int getGyroY(int8_t sensor_id)
{
	return (int)((read_IMU(0x2B, sensor_id) & 0xFF) << 8 ) + read_IMU(0x2A, sensor_id);//lecture de OUT_Y_H et OUT_Y_L et affichage
}

int getGyroZ(int8_t sensor_id)
{
	return (int)((read_IMU(0x2D, sensor_id) & 0xFF) << 8 ) + read_IMU(0x2C, sensor_id);//lecture de OUT_Z_H et OUT_Z_L et affichage
}
/********************************** Accelerometre ******************************************/
int getAcceleroX(int8_t sensor_id)
{
	return (int)((read_IMU(0x29, sensor_id) & 0xFF) << 8 ) + read_IMU(0x28, sensor_id);//lecture de OUT_X_H et OUT_X_L
}

int getAcceleroY(int8_t sensor_id)
{
	return (int)((read_IMU(0x2B, sensor_id) & 0xFF) << 8 ) + read_IMU(0x2A, sensor_id);//lecture de OUT_Y_H et OUT_Y_L
}

int getAcceleroZ(int8_t sensor_id)
{
	return (int)((read_IMU(0x2D, sensor_id) & 0xFF) << 8 ) + read_IMU(0x2C, sensor_id);//lecture de OUT_Z_H et OUT_Z_L
}
/******************************** Communication SPI ****************************************/
void init_SPI_IMU ()
{
	//Set slave select pins
	//IMU1
	P1DIR |= BIT3 + BIT6;

	/*********************************************************************/
	// UCB0 SPI settings for IMUs
	// Clock freq = SMCLK
	// Idle clock polarity = low
	// Data is captured on the rising (first) clock edge
	// CLK = P3.2, MISO = P3.1, MOSI = P3.0, IMU1 SS = P1.6-P1.3 //F5528

	//MISO, MOSI, CLK output
	P3DIR |= BIT0 + BIT2;
	P3DIR &= ~BIT1;
	P3SEL |= BIT0 + BIT1 + BIT2;

	UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;
	UCB0CTL1 = UCSSEL1 + UCSSEL0 + UCSWRST;
	UCB0BR0 = 1; // clock prescaler = 1 --> CLK = SMCLK
	UCB0BR1 = 0; //

	UCB0CTL1 &= ~UCSWRST; // bring the state machine output of reset
}

void Send_by_SPIB0 (uint8_t regAdress, uint8_t data)
{
	//Wait for slave to initialize
	__delay_cycles(100);
	WAIT_TIL_SPI_B0_GETS_READY;
	//Transmit Data to slave
	USCI_B_SPI_transmitData(USCI_B0_BASE, regAdress);
	USCI_B_SPI_transmitData(USCI_B0_BASE, data);
	WAIT_TIL_SPI_B0_GETS_READY;
	__delay_cycles(40);
}

void write_IMU(uint8_t regAdress, uint8_t data, int8_t sensor_id)
{
	switch (sensor_id)
	{
	case XM_IMU1 :
		RESET_CSXM_IMU1;
		Send_by_SPIB0 (regAdress, data);
		SET_CSXM_IMU1;
		break;
	case G_IMU1 :
		RESET_CSG_IMU1;
		Send_by_SPIB0 (regAdress, data);
		SET_CSG_IMU1;
		break;
	}
}

void Read_from_SPIB0 (uint8_t regAdress, uint8_t * preceive)
{
	//Wait for slave to initialize
	//__delay_cycles(100);
	WAIT_TIL_SPI_B0_GETS_READY;
	//Transmit Data to slave
	USCI_B_SPI_transmitData(USCI_B0_BASE, regAdress | 0x80);
	USCI_B_SPI_transmitData(USCI_B0_BASE, 0xFF);
	WAIT_TIL_SPI_B0_GETS_READY;
	*preceive = USCI_B_SPI_receiveData(USCI_B0_BASE);
	//__delay_cycles(40);
}

uint8_t read_IMU(uint8_t regAdress, int8_t sensor_id)
{
	uint8_t received_data;
	switch (sensor_id)
	{
	case XM_IMU1 :
		RESET_CSXM_IMU1;
		Read_from_SPIB0 (regAdress, &received_data);
		SET_CSXM_IMU1;
		break;
	case G_IMU1 :
		RESET_CSG_IMU1;
		Read_from_SPIB0 (regAdress, &received_data);
		SET_CSG_IMU1;
		break;
	}
	return received_data;
}

void read_incremental_IMU (uint8_t regAdress, uint8_t* data_out, int8_t nb_bytes, int8_t sensor_id)
{
	volatile int j;

	switch (sensor_id)
	{
	case XM_IMU1 :
		RESET_CSXM_IMU1;
		break;
	case G_IMU1 :
		RESET_CSG_IMU1;
		break;
	}

	WAIT_TIL_SPI_B0_GETS_READY;
	USCI_B_SPI_transmitData(USCI_B0_BASE, regAdress | 0xC0);
	for (j = nb_bytes; j>0; j--)
	{
		USCI_B_SPI_transmitData(USCI_B0_BASE, 0xFF);
		WAIT_TIL_SPI_B0_GETS_READY;
		*(data_out+(nb_bytes-j)) = USCI_B_SPI_receiveData(USCI_B0_BASE);
	}

	switch (sensor_id)
	{
	case XM_IMU1 :
		SET_CSXM_IMU1;
		break;
	case G_IMU1 :
		SET_CSG_IMU1;
		break;
	}
}
