
#include <msp430.h>
#include <driverlib.h>
//#include "utils.h"

extern void set_RF(void);

extern void nRF_init(void);

extern void nRF_reg_write(char addr, char *data, unsigned data_length);
extern void nRF_reg_read(char addr, char *data, unsigned data_length);

extern void nRF_upload_TX_payload(char *data);
extern void nRF_download_RX_payload(char *data);

extern void nRF_FLUSH_TX(void);
extern void nRF_FLUSH_RX(void);

char nRF_FIFO_STATUS(void);
extern void nRF_clear_IRQ(void);

extern void nRF_enter_tx (void);
extern void nRF_enter_rx (void);
extern void nRF_node0_set(void);
//extern void nRF_base_set (void);
extern void nRF_set_tx_pipe (char ch_id);

extern void wait_for_place_in_tx_fifo(void);
extern void wait_for_empty_tx_fifo(void);

//---------------------------------------------------------------------
#define LOW  0
#define HIGH 1

#define nRF_DESELECT  P2OUT |= BIT5//while(UCA0STAT & UCBUSY);
#define nRF_SELECT    P2OUT &= ~BIT5

#define nRF_CE_high P2OUT |= BIT4
#define nRF_CE_low  P2OUT &= ~BIT4

#define PULSE_CE nRF_CE_high; __delay_cycles(150); nRF_CE_low // The delay value must be set according to the CPU clock frequency and must be at least 10 microseconds

#define nRF_ENTER_RECEIVE_MODE  nRF_init(); \
							    nRF_enter_rx (); \
							    nRF_clear_IRQ()//; \
							    nRF_CE_high;\
							    __delay_cycles(1560)//-->130 us

#define nRF_ENTER_TRANSMIT_MODE nRF_init(); \
								nRF_clear_IRQ(); \
								nRF_enter_tx ();\
								__delay_cycles(1800)//-->150 us

#define START_PERIODICAL_LISTENING  TA1CCTL0 |= 0x10
#define STOP_PERIODICAL_LISTENING   TA1CCTL0 &= ~0x10

#define HEADSET_SENSOR 		10
#define REFERENCE_SENSOR 	20
#define EMG_SENSOR 			30
#define EMG1_SENSOR 		31
#define EMG2_SENSOR 		32

#define nRF_packet_len 		32
#define controle_packet_len 32
//---------------------------------------------------------------------

#define CONFIG_REG    0x00
#define EN_AA_REG     0x01
#define EN_RXADDR_REG 0x02
#define SET_AW_REG    0x03
#define SET_RETR_REG  0x04
#define RF_CH_REG     0x05
#define RF_DR_REG     0x06
#define RX_ADDR_P0    0x0A
#define RX_ADDR_P1    0x0B
#define RX_ADDR_P2    0x0C
#define RX_ADDR_P3    0x0D
#define RX_ADDR_P4    0x0E
#define RX_ADDR_P5    0x0F
#define TX_ADDR       0x10
#define RX_PW_P0      0x11
#define RX_PW_P1      0x12
#define RX_PW_P2      0x13
#define RX_PW_P3      0x14
#define RX_PW_P4      0x15
#define RX_PW_P5      0x16
#define DYNPD_REG     0x1C
#define FEATURE_REG   0x1D
////CONFIG REG////////////////
#define EN_IRQ_RX     0x00
#define EN_IRQ_TX     0x00
#define DIS_IRQ_RT    0x10

#define EN_CRC        0x08

#define CRCO_1BIT     0x00
#define CRCO_2BIT     0x04

#define PWR_UP        0x02

#define SET_IN_TX     0x00
#define SET_IN_RX     0x01
////EN_AA/////////////////////
#define ENAA_P0       0x01
#define ENAA_P1       0x02
#define ENAA_P2       0x04
#define ENAA_P3       0x08
#define ENAA_P4       0x10
#define ENAA_P5       0x20
////EN_RXADDR/////////////////
#define EN_RX_P0       0x01
#define EN_RX_P1       0x02
#define EN_RX_P2       0x04
#define EN_RX_P3       0x08
#define EN_RX_P4       0x10
#define EN_RX_P5       0x20
////SETUP_AW//////////////////
#define SET_3BYTES     0x01
#define SET_4BYTES     0x10
#define SET_5BYTES     0x11
////SETUP_RTR/////////////////
#define WAIT_250us     0x00
#define WAIT_500us     0x10
#define WAIT_750us     0x20
#define WAIT_1000us    0x30
#define WAIT_1250us    0x40
#define WAIT_1500us    0x50
#define WAIT_1750us    0x60
#define WAIT_2000us    0x70
#define WAIT_2250us    0x80
#define WAIT_2500us    0x90
#define WAIT_2750us    0xA0
#define WAIT_3000us    0xB0
#define WAIT_3250us    0xC0
#define WAIT_3500us    0xD0
#define WAIT_3750us    0xE0
#define WAIT_4000us    0xF0

#define RETR_DIS       0x00
#define RETR_X1        0x01
#define RETR_X2        0x02
#define RETR_X3        0x03
#define RETR_X4        0x04
#define RETR_X5        0x05
#define RETR_X6        0x06
#define RETR_X7        0x07
#define RETR_X8        0x08
#define RETR_X9        0x09
#define RETR_X10       0x0A
#define RETR_X11       0x0B
#define RETR_X12       0x0C
#define RETR_X13       0x0D
#define RETR_X14       0x0E
#define RETR_X15       0x0F
////RF_SETUP//////////////////
#define PWR_0dBm       0x06
#define DR_2Mbps       0x08
#define DR_1Mbps       0x00
#define DR_250kbps     0x20
////DYNPD/////////////////////
#define EN_DPL_ALL     0x3F
////FEATURE///////////////////
#define EN_DPL         0x04

#define IMU1_ch 0
#define IMU2_ch 1
#define IMU3_ch 2
#define IMU4_ch 3
#define EMG1_ch 4
#define EMG2_ch 5
#define EMG3_ch 6
#define EMG4_ch 7




