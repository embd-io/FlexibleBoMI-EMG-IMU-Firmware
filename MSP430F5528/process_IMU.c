/*
 * process_IMU.c
 *
 *  Created on: 2014-07-21
 *      Author: Philippe
 *  Fonctions pour effectuer les calculs sur le MSP430
 */

#include "math.h"  // pour les fonctions des calculs d'angles
#include "process_IMU.h"
#include "capteur_inertiel.h"

#define 	FILTER_EN		0
#define 	X_axis			0
#define 	Y_axis			1
#define 	Z_axis			2
#define 	G_BIAS_X		0.6164

uint8_t  data_inc_read [18];

int ACC  	[3] = {0,0,0};
int GYRO 	[3] = {0,0,0};
int MAG  	[3] = {0,0,0};

float NORM	[3] = {0,0,0};

float Yaw_Angle;
float Pitch_Angle;
float Roll_Angle;
float XH, YH, cos_pitch, cos_roll, sin_pitch, sin_roll;
const char USE_MAGNET = 1;// 0;//
char COMPENSATE = 3;// 2;// 1;// 0;//

float Y0_pitch = 0.0;
float Y0_roll  = 0.0;
float Y0_yaw   = 0.0;
float Y1_pitch = 0.0;
float Y1_roll  = 0.0;
float Y1_yaw   = 0.0;
float Tcutoff_pr = 0.0;
float Tcutoff_y  = 0.0;


int Calcul_Attitute (int **raw_data, int *pitch, int *roll, int *yaw, float alpha_acc, float alpha_gyro, float alpha_mag, char filt_en_pr, char filt_en_y)
//void Calcul_Attitute (uint8_t *raw_data, int *pitch, int *roll, int *yaw, float alpha_acc, float alpha_gyro, float alpha_mag, char filt_en_pr, char filt_en_y)
{
	//int RES = 0;
	Read_IMU_data(raw_data);

	//PITCH
	//Pitch_Angle = 0 * atan2(-ACC[Y_axis], -ACC[X_axis]) + 1 * (Pitch_Angle + ((-GYRO[Z_axis]*(GAIN_2000dps))*pi/180.0) * 1.0/FREQ_IMU);
	Pitch_Angle = ALPHA_acc * atan2(-ACC[Y_axis], -ACC[X_axis]) + ALPHA_gyr * (Pitch_Angle + ((-GYRO[Z_axis]*(GAIN_2000dps))*pi/180.0) * 1.0/FREQ_IMU);
	if ((Pitch_Angle/pi)>-1 && (Pitch_Angle/pi)<1)
		pitch[0] = (Pitch_Angle/pi)*180*100;

	//ROLL
	//Roll_Angle = 0 * atan2(-ACC[Z_axis], -ACC[X_axis]) + 1 * (Roll_Angle + ((GYRO[Y_axis]*GAIN_2000dps)*pi/180.0) * 1.0/FREQ_IMU);
	Roll_Angle = ALPHA_acc * atan2(-ACC[Z_axis], -ACC[X_axis]) + ALPHA_gyr * (Roll_Angle + ((GYRO[Y_axis]*GAIN_2000dps)*pi/180.0) * 1.0/FREQ_IMU);
	if ((Roll_Angle/pi)>-1 && (Roll_Angle/pi)<1)
		roll[0] = (Roll_Angle/pi)*180*100;

	//YAW
	Yaw_Angle = (Yaw_Angle + ((GYRO[X_axis]*(GAIN_2000dps))*pi/180.0) * 1.0/FREQ_IMU);
	if ((Yaw_Angle/pi)>-1 && (Yaw_Angle/pi)<1)
		yaw[0] = (Yaw_Angle/pi)*180*100;

	Filter_Attitude (pitch, roll, yaw, 2, filt_en_y);
	return ACC[X_axis];
}

void Read_IMU_data (int **out)
//void Read_IMU_data (uint8_t * out)
{
	//int i;
	//*out = data_inc_read;

	read_incremental_IMU (0x28, (data_inc_read), 6, XM_IMU1);//read ACCX, ACCY, ACCZ
	read_incremental_IMU (0x28, (data_inc_read+6), 6, G_IMU1); //read GYROX, GYROY, GYROZ
	read_incremental_IMU (0x08, (data_inc_read+12), 6, XM_IMU1);//read MAGX, MAGY, MAGZ
	//for (i = 0; i < 18; i++)
		//out[i] = data_inc_read[i];

	//|  			ACC   				  |					GYRO     		  | 		 MAG		  |
	//| X_L | X_H | Y_L | Y_H | Z_L | Z_H | X_L | X_H | Y_L | Y_H | Z_L | Z_H | Y_L | Y_H | Z_L | Z_H |
	out[0] = ACC;
	ACC  [X_axis] = data_inc_read [0] + (data_inc_read [1] << 8);//getAcceleroX(XM_IMU1);//
	ACC  [Y_axis] = data_inc_read [2] + (data_inc_read [3] << 8);//getAcceleroY(XM_IMU1);//
	ACC  [Z_axis] = data_inc_read [4] + (data_inc_read [5] << 8);//getAcceleroZ(XM_IMU1);//

	out[1] = GYRO;
	GYRO [X_axis] = data_inc_read [6] + (data_inc_read [7] << 8) + G_BIAS_X;
	GYRO [Y_axis] = data_inc_read [8] + (data_inc_read [9] << 8);
	GYRO [Z_axis] = data_inc_read [10] + (data_inc_read [11] << 8);

	out[2] = MAG;
	MAG  [X_axis] = (data_inc_read [12] + (data_inc_read [13] << 8));
	MAG  [Y_axis] = (data_inc_read [14] + (data_inc_read [15] << 8));
	MAG  [Z_axis] = (data_inc_read [16] + (data_inc_read [17] << 8));
}

void Filter_Attitude (int *p, int *r, int *y, char filt_pr, char filt_y)
{
	if (filt_pr == 1)//filter at 10Hz
		Tcutoff_pr = filty_10Hz;
	else if (filt_pr == 2)//filter at 25Hz
		Tcutoff_pr = filty_25Hz;

	if (filt_y == 1)//filter at 10Hz
		Tcutoff_y = filty_10Hz;
	else if (filt_y == 2)//filter at 25Hz
		Tcutoff_y = filty_25Hz;

	if (filt_pr != 0)
	{
		Y1_pitch = Y0_pitch*(1 - Ts / Tcutoff_pr) + p[0] * (Ts / Tcutoff_pr);
		Y0_pitch = Y1_pitch;
		p[0] = Y1_pitch;

		Y1_roll = Y0_roll*(1 - Ts / Tcutoff_pr) + r[0]*Ts / Tcutoff_pr;
		Y0_roll = Y1_roll;
		r[0] = Y1_roll;
	}

	if (filt_y != 0)
	{
		Y1_yaw = Y0_yaw*(1 - Ts / Tcutoff_y) + y[0]*Ts / Tcutoff_y;
		Y0_yaw = Y1_yaw;
		y[0] = Y1_yaw;
	}
}

int8_t sign (int var)
{
	if (var >= 0)
		return 1;
	else if (var < 0)
		return -1;
	return 0;
}
