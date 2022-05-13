/*
 * process_IMU.h
 *
 *  Created on: 2014-07-21
 *      Author: Philippe
 */

#ifndef PROCESS_IMU_H_
#define PROCESS_IMU_H_

#include "driverlib.h"

# define 	IMU1		0

# define ALPHA_mag  	0.00//0.5//1.00//
# define ALPHA_acc 		0.20
# define ALPHA_gyr 		0.80
# define GAIN_2000dps 	2000.0/32768.0
# define FREQ_IMU 		62.5//35//
# define Ts 			1/FREQ_IMU//0.01613
# define filty_10Hz 	(FREQ_IMU/10)*Ts
# define filty_25Hz 	(FREQ_IMU/25)*Ts

int8_t sign (int var);
void Read_IMU_data (int **out);
//void Read_IMU_data (uint8_t *out);
int Calcul_Attitute (int **raw_data, int *pitch, int *roll, int *yaw, float alpha_acc, float alpha_gyro, float alpha_mag, char filt_en_pr, char filt_en_y);
//void Calcul_Attitute (uint8_t *raw_data, int *pitch, int *roll, int *yaw, float alpha_acc, float alpha_gyro, float alpha_mag, char filt_en_pr, char filt_en_y);
void Filter_Attitude (int *p, int *r, int *y, char filt_pr, char filt_y);

#endif /* PROCESS_IMU_H_ */
