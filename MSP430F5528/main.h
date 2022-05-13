/*
 * main.h
 *
 *  Created on: 2 août 2017
 *      Author: Etudiant
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <math.h>                                   //pour les fonctions des calculs d'angles
#include <float.h>
#include <msp430.h>
#include <driverlib.h>
#include "nRF.h"
#include "Config.h"
#include "capteur_inertiel.h"
#include "process_IMU.h"
#include "CommUART.h"
#include "AFE_ADS1291.h"

#define HEADSET_ID      10
#define REFERENCE_ID    20
#define EMG_ID          30
#define SAFETY_KEY_ID   40
#define BASE_STATION_ID 50
#define EMG1            31
#define EMG2            32
#define EMG3            33
#define EMG4            34
#define EMG5            35

#define NB_SAMPLES      13
#define TX              1
#define RX              2
#define NO_BASE         3

#define IMU_OFF         4
#define AFE_OFF         0
#define AFE_ON          1
#define AFE_DISABLED    10
#define AFE_ENABLED     11

#define READ_IMU        71
#define SEND_DATA       72
#define WAKEUP_IMU      73
#define TURNON_EMG      74
#define TURNOFF_EMG     75
#define UPDATE_MODE     76
#define COMPRESS        77
#define NO_TASK         0

#define NO_EVENT          0
#define ENTER_CALIBRATION 1
#define ENTER_STANDBY     2
#define ENTER_CONTROL     3

#define UNKNOWN_MODE    0
#define STANDBY_MODE    80
#define CONTROL_MODE    81
#define CALIB_MODE      82

#define NB_PACKET_TO_MODE_CHECK 1000

#define DEFAULT_MODE        91
#define SAFETY_NETWORK_MODE 92

#define EMG_24BIT_PRECISION 24
#define EMG_16BIT_PRECISION 16

void send_available_RF ();
void send_available_UART ();
void sensor_mode_manager ();
void compressEMG_24to16 ();

#endif /* MAIN_H_ */
