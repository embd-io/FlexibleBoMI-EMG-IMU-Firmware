/*
 * Config.h
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "msp430.h"
#include "driverlib.h"

#define		NB_IMU_SENSORS			3
#define		SOF_SIZE				1
#define		FREQ_SIZE				1
#define		MODE_SIZE				1
#define		EOF_SIZE				1
#define 	NB_IMU_ANGLES			4

#define     DELAY_LISTENING         1024   //125ms //250ms
#define     DELAY_NOT_LISTENING     2*2048 //500ms

#define		SET_IMU_SENSOR			SELECT_Gyroscope; 		\
									startGyro(); 	  		\
									SELECT_Accelerometre;	\
									startAccelero(); 		\
									SELECT_Magnetometre;	\
									startMagneto();

#define     SET_LED     P4OUT |= BIT6
#define     RESET_LED   P4OUT &= ~BIT6
#define     TOGGLE_LED  P4OUT ^= BIT6

void set_MCU (char sensor_mode);
void init_Clock ();
void init_SPI_nRF ();
void init_ADC ();
void init_DMA_ADC(int16_t * mes_get);
void init_TimerA0 ();
void init_TimerA1 ();
void init_TimerB0 ();
void init_UCA1_UART ();
void init_Config_IO ();

#endif /* CONFIG_H_ */
