/*
 * capteur_inertiel.h
 *
 *  Created on: 2014-10-05
 *      Author: Etudiant
 */

#ifndef CAPTEUR_INERTIEL_H_
#define CAPTEUR_INERTIEL_H_

#include "msp430.h"
#include "driverlib.h"

//#include "msp430f5528.h"

#define 	XM_IMU1				11
#define 	G_IMU1				12

#define     GYRO_POWER_DOWN     write_IMU(0x20, 0xF0, G_IMU1)
#define     GYRO_SLEEP_MODE     write_IMU(0x20, 0xF7, G_IMU1)
#define     GYRO_NORMAL_MODE    write_IMU(0x20, 0xFF, G_IMU1)

#define     ACC_POWER_DOWN      write_IMU(0x20, 0x00, XM_IMU1)
#define     ACC_NORMAL_MODE     write_IMU(0x20, 0x67, XM_IMU1)

#define     MAG_POWER_DOWN      write_IMU(0x26, 0x83, XM_IMU1)
#define     MAG_NORMAL_MODE     write_IMU(0x26, 0x80, XM_IMU1)


#define 	SET_CSXM_IMU1		GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6)
#define 	RESET_CSXM_IMU1		GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6)
#define 	SET_CSG_IMU1		GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3)
#define 	RESET_CSG_IMU1		GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN3)

#define		READ_INT1_SRC		read_IMU(0x31, G_IMU1)

#define		WAIT_TIL_SPI_B0_GETS_READY while(!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT))

#define		pi 			3.141592653589793

//Comm SPI
void read_incremental_IMU (uint8_t regAdress, uint8_t* data_out, int8_t nb_bytes, int8_t sensor_id);

void init_SPI_IMU ();
void write_IMU(uint8_t regAdress, uint8_t data, int8_t sensor_id);
void Send_by_SPIB0 (uint8_t regAdress, uint8_t data);
uint8_t read_IMU(uint8_t regAdress, int8_t sensor_id);
void Read_from_SPIB0 (uint8_t regAdress, uint8_t * preceive);

//IMU
void IMU_NORMAL_MODE ();
void IMU_POWER_DOWN ();
void set_IMU ();
void startXM();
void startG();

//Accelero
int getAcceleroX(int8_t sensor_id);
int getAcceleroY(int8_t sensor_id);
int getAcceleroZ(int8_t sensor_id);

//Gyro
int getGyroX(int8_t sensor_id);
int getGyroY(int8_t sensor_id);
int getGyroZ(int8_t sensor_id);
int getWHO_I_AM(int8_t sensor_id);

//Magneto
int getMagnetoX(int8_t sensor_id);
int getMagnetoY(int8_t sensor_id);
int getMagnetoZ(int8_t sensor_id);

#endif /* CAPTEUR_INERTIEL_H_ */
