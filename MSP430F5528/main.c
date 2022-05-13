//***************************************************************************************
// MSP430F5528 _ Board_EMG_IMU_v011016
// Auteur : Cheikh Latyr Fall
// latyr.fall@gmail.com
// Features:
// - MSP430F5528 controller
// - nRF24L01 : UCA0
// - LSM9DS0  : UCB0
// - ADS1292  : UCB1
//***************************************************************************************

#include "main.h"

// RF Packet data
int FREQ        = 0;
char NODE_ID    = EMG1;
int  PACKET_NUMBER_RX = 0;
int  PACKET_NUMBER_TX = 0;
char data_OUT   [32];
char data_IN    [32];
uint8_t index_OUT = 0;

// sensors variables
char wakeup     = 0;
char AFE_STATE  = AFE_OFF;
char AFE_DETECT = AFE_DISABLED;

// EMG sensor data
char data_IN_EMG [8];
char tmp_EMG;
char SOF_EMG = 0xFA;
char EOF_EMG = 0xFF;
int  cpt_LED = 0;
char nb_samp = 0;
uint8_t  index_EMG = 0;
uint32_t var_EMG;
char EMG_PRECISION = 0;

// power mode - safety key network
char EVENT      = 0;
char calibration_done = 0;
char SENSOR_MODE = UNKNOWN_MODE;
char CURRENT_SENSOR_MODE = UNKNOWN_MODE;

// sensor communication
char nRF_IRQ_flag;
const char SOF_PACKET = 0x33;
const char EOF_PACKET = 0x34;
char TASK_ID    = 0;
char uart_SEND  = 0;
char nRF_SEND   = 0;
char nRF_STATE  = 0;
char LISTENING  = 0;
char RF_IRQ_IS_ENABLED;

bool READ_EMG_1000Hz = 0;

// sensor network mode (default_mode / safety_network_mode)
char SENSOR_NETWORK_MODE = 0;

void main(void)
 {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	//default_mode = broadcast, safety_network_mode = safety_network
	SENSOR_NETWORK_MODE = DEFAULT_MODE;//SAFETY_NETWORK_MODE;//
	RF_IRQ_IS_ENABLED = 1;//0;//

	set_MCU (SENSOR_NETWORK_MODE);
	//TA1CCTL0 = 0x10;                 // Enable Timer A0 interrupts (rx mode)


	init_SPI_nRF ();
	set_RF();
	nRF_STATE = TX;//RX;//
	nRF_SEND  = 0;//1;//

	init_SPI_IMU ();
	set_IMU ();
	IMU_POWER_DOWN();

	init_AFE ();
	set_AFE ();
	CONFIG_AFE;       //waits to call config_AFE (0) using TimerA0 interrupt
	DISABLE_AFE;      //STOPS AFE by tying down P1.1 pin (ADS1292 START pin)
	AFE_DETECT = AFE_DISABLED;
	EMG_PRECISION = EMG_16BIT_PRECISION;//EMG_24BIT_PRECISION;//

	unsigned int i = 0;
	for (i=32; i>0; i--)
	{
		data_OUT[32-i] = 0;
		data_IN [32-i] = 0;
	}
	data_OUT[0]  = SOF_PACKET;	//SOF
	data_OUT[1]  = NODE_ID;  //Sensor node ID
	data_OUT[31] = EOF_PACKET;	//EOF
	if (EMG_PRECISION == EMG_24BIT_PRECISION)
	    index_OUT    = 4;
	else if (EMG_PRECISION == EMG_16BIT_PRECISION)
	    index_OUT    = 5;

	// data_IN_EMG init
	for (i = 8; i > 0; i--)
		data_IN_EMG[8-i] = 0;
	data_IN_EMG[0] = SOF_EMG;
	data_IN_EMG[7] = EOF_EMG;

	if (SENSOR_NETWORK_MODE == SAFETY_NETWORK_MODE)
	    START_PERIODICAL_LISTENING; //sets periodical RX mode

	if (SENSOR_NETWORK_MODE == DEFAULT_MODE && !RF_IRQ_IS_ENABLED)
	    P2IE &= ~BIT3;   // IRQ interrupt disabled

	__bis_SR_register(LPM4_bits + GIE);     // LPM0 with interrupts enabled

	while(1)
	{
		switch (TASK_ID)
		{

		case UPDATE_MODE:
            TASK_ID = NO_TASK;
            sensor_mode_manager ();
            if (TASK_ID == NO_TASK)
                LPM4;
            break;

		case COMPRESS:
		    compressEMG_24to16 ();
		    if (nRF_SEND && index_OUT >= 30)
		        TASK_ID = SEND_DATA;
		    if (TASK_ID == NO_TASK)
                LPM4;
            break;

		case SEND_DATA:
		    TASK_ID = NO_TASK;
			send_available_RF();
			if (TASK_ID == NO_TASK)
			    LPM4;
			break;

		case NO_TASK:
			LPM4;
			break;
		}

	}
}

// Timer used to enter RX mode periodically
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
    if (nRF_STATE == RX)
    {
        switch (LISTENING)
        {
        case 1:
            nRF_CE_high;
            LISTENING = 0;
            TA1CCR0 = DELAY_LISTENING;
            break;

        case 0:
            nRF_CE_low;
            LISTENING = 1;
            TA1CCR0 = DELAY_NOT_LISTENING;
            break;
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IFG &= ~BIT1;
	AFE_STATE = AFE_ON;
	READ_EMG_1000Hz ^= 1;

	/*if (nRF_SEND && nb_samp == 0)
	{
	    TASK_ID = SEND_DATA;
        LPM4_EXIT;
	}*/

	if (READ_EMG_1000Hz)
	{
        // IF ACK ACTIVATED, HELP KIPPING THE SENSOR SENDING DATA WHEN NO ACK RECEIVED
        if (nRF_SEND == 1 && nb_samp < NB_SAMPLES && nb_samp > 9 && (SENSOR_NETWORK_MODE == SAFETY_NETWORK_MODE || RF_IRQ_IS_ENABLED))
            nRF_IRQ_flag = 1;

        // READ DATA FROM AFE
        SET_CS_AFE;
        for(index_EMG = 0; index_EMG < 7; index_EMG ++)
        {
            UCB1TXBUF = 0xFF;
            data_IN_EMG[index_EMG] = UCB1RXBUF;
            if (EMG_PRECISION == EMG_24BIT_PRECISION)
            {
                if (index_EMG > 3)
                {
                    data_OUT [index_OUT] = UCB1RXBUF; //EMG from data_OUT[4] to data_OUT[30]
                    index_OUT ++;
                }
            }
            while(UCB1STAT & UCBUSY);
        }
        CLR_CS_AFE;
        // COMPRESS SAMPLE IF LOWER THAN 24BIT CHOOSEN
        if (EMG_PRECISION == EMG_16BIT_PRECISION)
        {
            compressEMG_24to16 ();
        }
        nb_samp++;

        if (nb_samp == NB_SAMPLES)
        {
            nRF_SEND = 1;
            nb_samp = 0;
            TASK_ID = SEND_DATA;
            LPM4_EXIT;
        }
	}
}

// nRF24L01+ IRQ
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	P2IFG &= ~BIT3; 							// IFG cleared
	if (nRF_SEND == 1)
	{
		nRF_IRQ_flag = 1;
	}
	else // means that SENSOR_NETWORK_MODE == SAFETY_KEY_MODE
	{
		if (AFE_STATE == AFE_OFF)
		{
			config_AFE (0);
			__delay_cycles(150);
		}

		nRF_IRQ_flag = 0;
		nRF_clear_IRQ();
		nRF_download_RX_payload(data_IN);
		PACKET_NUMBER_RX = 0;
		PACKET_NUMBER_TX = 0;

		//PROCESS RECEIVED PACKET
		if (data_IN[0] == 0xFA && data_IN[controle_packet_len - 1] == 0xFF) //data_IN[1] indique si nouvelle config il y a
		{
		    if (data_IN [1] == SAFETY_KEY_ID)
            {
                STOP_PERIODICAL_LISTENING;
                //TOGGLE_LED;

                EVENT = data_IN [14];
                switch (EVENT)
                {
                case ENTER_CALIBRATION:
                    SENSOR_MODE = CALIB_MODE;
                    break;
                case ENTER_CONTROL:
                    SENSOR_MODE = CONTROL_MODE;
                    break;
                case ENTER_STANDBY:
                    SENSOR_MODE = STANDBY_MODE;
                    break;
                case NO_EVENT:
                    SENSOR_MODE = UNKNOWN_MODE;
                    break;
                }

                if (SENSOR_MODE != CURRENT_SENSOR_MODE && SENSOR_MODE != UNKNOWN_MODE)
                {
                    TASK_ID = UPDATE_MODE;
                    LPM4_EXIT;
                    ///////////
                    DISABLE_AFE;
                    AFE_DETECT = AFE_DISABLED;
                }
            }
            else if ((data_IN [1] == BASE_STATION_ID) || (data_IN [1] == 0))
            {
                SENSOR_MODE = CALIB_MODE; //by default
                CURRENT_SENSOR_MODE = SENSOR_MODE;
                STOP_PERIODICAL_LISTENING;

                //ENTER Transmit mode
                if (nRF_STATE == RX)
                    nRF_ENTER_TRANSMIT_MODE;
                nRF_STATE = TX;
                ///////////
                ENABLE_AFE;
                AFE_DETECT = AFE_ENABLED;
            }
		}
	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    if (AFE_STATE == AFE_OFF)
    {
        config_AFE (0);
        __delay_cycles(150);
        ENABLE_AFE;
        AFE_DETECT = AFE_ENABLED;
        TA0CCTL0 &= 0xEF;  // TimerA0 interrupt disabled
    }
}

void sensor_mode_manager ()
{
    switch (SENSOR_MODE)
    {
    case STANDBY_MODE:
        //LISTENS PERIODICALLY FOR INCOMING PACKET + IMU in POWER DOWN MODE
        //SET_LED;
        if (nRF_STATE == TX)
            nRF_ENTER_RECEIVE_MODE;
        nRF_STATE = RX;

        START_PERIODICAL_LISTENING;

        //DISABLE_AFE
        DISABLE_AFE;
        AFE_DETECT = AFE_DISABLED;

        break;

    case CALIB_MODE:
        //SENDS DATA CONTINUOUSLY TO BASE STATION
        //SET_LED;
        STOP_PERIODICAL_LISTENING;

        //ENABLE AFE
        ENABLE_AFE;
        AFE_DETECT = AFE_ENABLED;

        //ENTER Transmit mode
        if (nRF_STATE == RX)
            nRF_ENTER_TRANSMIT_MODE;
        nRF_STATE = TX;

        break;

    case CONTROL_MODE:
        //SENDS A DEFINED NUMBER OF DATA (NB_TO_SEND) AND ENTER RX MODE PERIODICALLY TO CHECK SAFETY KEY
        //SET_LED;
        STOP_PERIODICAL_LISTENING;

        //ENABLE AFE
        ENABLE_AFE;
        AFE_DETECT = AFE_ENABLED;

        if (nRF_STATE == RX)
            nRF_ENTER_TRANSMIT_MODE;
        nRF_STATE = TX;

        break;
    }
    CURRENT_SENSOR_MODE = SENSOR_MODE;
}

void compressEMG_24to16 ()
{
    if (index_OUT < 30)
    {
        var_EMG = 0;
        volatile uint32_t var1, var2, var3;
        var1 = data_IN_EMG[4];
        var2 = data_IN_EMG[5];
        var3 = data_IN_EMG[6];
        var_EMG = ( var1 << 16) + (var2 << 8) + (var3);
        var_EMG = (var_EMG >> 8);// --> /256
        data_OUT [index_OUT ]     = (var_EMG & 0xFF00) >> 8;
        data_OUT [index_OUT + 1]  = (var_EMG & 0x00FF);
        index_OUT = index_OUT + 2;
    }
}

void send_available_RF ()
{
	if (nRF_SEND)
	{
		if (nRF_STATE == RX)
		{
			nRF_ENTER_TRANSMIT_MODE;
			nRF_STATE = TX;
		}
		//mesure frequence d'acquisition
		FREQ = (int) TB0R*2/32.768;
		TB0R = 0x0000;

		data_OUT[0]  = SOF_PACKET;  //SOF
        data_OUT[1]  = NODE_ID;  //Sensor node ID
        data_OUT[2]  = (char)FREQ;
        data_OUT[3]  = PACKET_NUMBER_TX;
        if (EMG_PRECISION == EMG_16BIT_PRECISION)
            data_OUT[4]  = 0; //NOTHING
        data_OUT[31] = EOF_PACKET;  //EOF

        //wait_for_place_in_tx_fifo();
        PACKET_NUMBER_TX ++; // used in default_mode to send packet number between 0 and 255
        PACKET_NUMBER_RX ++; // used in safety_key_mode to put sensor into rx mode to check for safety_key
        if (EMG_PRECISION == EMG_24BIT_PRECISION)
            index_OUT = 4;
        else if (EMG_PRECISION == EMG_16BIT_PRECISION)
            index_OUT = 5;


        nRF_upload_TX_payload(data_OUT);
		nRF_CE_high;
		// The delay value must be set according to the CPU clock frequency and must be at least 10 microseconds
		__delay_cycles(150);
		nRF_CE_low;
		if (SENSOR_NETWORK_MODE == SAFETY_NETWORK_MODE || RF_IRQ_IS_ENABLED)
            while(!nRF_IRQ_flag);
		nRF_IRQ_flag = 0;
		nRF_clear_IRQ();
		nRF_SEND = 0;


		switch (SENSOR_NETWORK_MODE)
		{
		case DEFAULT_MODE:
		    if (PACKET_NUMBER_TX == 256)
                PACKET_NUMBER_TX = 0;
		    //nb_samp = 0;
		    break;

		case SAFETY_NETWORK_MODE:
		    if (PACKET_NUMBER_RX >= NB_PACKET_TO_MODE_CHECK)
            {
                if (nRF_STATE == TX)
                    nRF_ENTER_RECEIVE_MODE;
                nRF_STATE = RX;
                SENSOR_MODE = STANDBY_MODE;
                TASK_ID = UPDATE_MODE;
                PACKET_NUMBER_RX = 0;
            }
		    else
            {
                //nb_samp = 0;
                ENABLE_AFE;
                AFE_DETECT = AFE_ENABLED;
            }
		    break;
		}
	}
}

void send_available_UART ()
{
	if (uart_SEND == 1)
	{
		send_data_UART (data_OUT, controle_packet_len);
		uart_SEND = 0;
	}
}
