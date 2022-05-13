/*
 * Config.c
 */

#include "Config.h"

#define 	UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ   12000  			   //Target frequency for MCLK in kHz
#define 	UCS_MCLK_FLLREF_RATIO   			366				   //MCLK/FLLRef Ratio
#define 	NB_CHANNELS 						3

#define     DEFAULT_MODE        91
#define     SAFETY_NETWORK_MODE 92

void set_MCU (char sensor_mode)
{
	init_Config_IO ();
	init_Clock ();
	init_UCA1_UART();
	init_TimerB0 ();  // measures sensor frequency
	init_TimerA1 ();  // used to set rx mode periodically
	init_TimerA0 ();  // wait to start AFE
	TA0CCTL0 &= 0xEF; // AFE start TimerA0 interrupt disabled
	if (sensor_mode == DEFAULT_MODE)
	    TA1CCTL0 &= 0xEF; // Disable Timer A0 interrupts (rx mode)
	P4OUT &= ~BIT6;
}


void init_Clock ()
{
	//Set VCore = 1 for 12MHz clock
	PMM_setVCore(PMM_CORE_LEVEL_1);  // tension de la carte principale permettant un max de 12MHz selon datasheet "Processor frequency (maximum MCLK frequency)"

	//Set DCO FLL reference = REFO
	UCS_clockSignalInit(
			UCS_FLLREF,
			UCS_REFOCLK_SELECT,
			UCS_CLOCK_DIVIDER_1
			);
	//Set ACLK = REFO
	UCS_clockSignalInit(
			UCS_ACLK,
			UCS_REFOCLK_SELECT,
			UCS_CLOCK_DIVIDER_1
			);
	//Set SMCLK = REFO
	UCS_clockSignalInit(
			UCS_SMCLK,
			UCS_REFOCLK_SELECT,
			UCS_CLOCK_DIVIDER_1
			);

	//Set Ratio and Desired MCLK Frequency  and initialize DCO
	UCS_initFLLSettle(
			UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ,
			UCS_MCLK_FLLREF_RATIO
			);

	P2DIR |= BIT2;                            // MCLK set out to pins
	P2SEL |= BIT2;
}


void init_SPI_nRF ()
{
	// CE, CSN and IRQ pins
	P2DIR |= BIT4;        // Set nRF_CE (Chip Enable) pin to output
	P2DIR |= BIT5;        // Set nRF_CSN (Chip Select) pin to output

	P2DIR &= ~BIT3;       // Set nRF_IRQ pin to input
	P2IE |= BIT3;          // Enable nRF_IRQ interrupt
	P2IES |= BIT3;         // hi-to-low interrupt
	P2IFG &= ~BIT3;       // clear nRF_IRQ interrupt

	// UCA0 SPI settings for nRF24L01+
	// Clock freq = SMCLK
	// Idle clock polarity = low
	// Data is captured on the rising (first) clock edge
	// CLK = P2.7, MISO = P3.4, MOSI = P3.3, Nordic Slave-Select = P2.5

	P2DIR |= BIT7;        // Set CLK pin to output
	P3DIR |= BIT3;        // Set MOSI pin to output
	P3DIR &= ~BIT4;       // Set MISO pin to input

	P3SEL |= BIT3 + BIT4; // MISO and MOSI pin functionality select
	P2SEL |= BIT7; // CLK pin functionality select

	UCA0CTL0 = UCCKPH + UCMSB + UCMST + /*UCMODE0 +*/ UCSYNC;
	UCA0CTL1 = UCSSEL1 + UCSSEL0 + UCSWRST;
	UCA0BR0 = 1; // clock prescaler = 1 --> CLK = SMCLK
	UCA0BR1 = 0; //

	UCA0CTL1 &= ~UCSWRST; // bring the state machine output of reset

}

void init_ADC()
{
	//Initialisation pour l'ADC
		//P6.0 ADC option select
		GPIO_setAsPeripheralModuleFunctionOutputPin(
				GPIO_PORT_P6,
				GPIO_PIN1
				);
		//Initialize the ADC12_A_A Module
		ADC12_A_init(ADC12_A_BASE,// Base address of ADC12_A_A Module
					 ADC12_A_SAMPLEHOLDSOURCE_SC,// Use internal ADC12_A_A bit as sample/hold signal to start conversion
					 ADC12_A_CLOCKSOURCE_SMCLK,//USE SMCLK = 12MHz as clock source
					 ADC12_A_CLOCKDIVIDER_1);//Use default clock divider of 1

		ADC12_A_enable(ADC12_A_BASE);

		//Configure Timer and enables Multiple Channel Sampling
		ADC12_A_setupSamplingTimer(ADC12_A_BASE,// Base address of ADC12_A_A Module
								   ADC12_A_CYCLEHOLD_32_CYCLES,// For memory buffers 0-7  sample/hold, 2.5us
								   ADC12_A_CYCLEHOLD_32_CYCLES,// For memory buffers 8-15 sample/hold, 2.5us
								   ADC12_A_MULTIPLESAMPLESENABLE);// Enable Multiple Sampling

		//Configure Memory Buffer for Sequential Sampling
		ADC12_A_memoryConfigure(ADC12_A_BASE,				//Base address of the ADC12_A_A Module
								ADC12_A_MEMORY_3,			//Configure memory buffer 3
								ADC12_A_INPUT_A4,			//Map input A4 to memory buffer 3
								ADC12_A_VREFPOS_AVCC,		//Vref+ = AVcc
								ADC12_A_VREFNEG_AVSS,		//Vr- = AVss
								ADC12_A_ENDOFSEQUENCE);	//Memory buffer 3 is the end of a sequence

		//ADC Memory mapping*****
		ADC12MCTL0 = 0x01;	//CHANNEL1 P6.1
		ADC12MCTL1 = 0x02;	//CHANNEL2 P6.2
		ADC12MCTL2 = 0x83;	//CHANNEL3 P6.3 (END OF CHANNEL)

}

void init_TimerA1()//RX mode
{
    //Initialisation d'un timer pour l'activation de l'interrupt
        //TA1CCR0 = 2048;//70;//           // 32768/(4*2048)=4 Hz --> 250 ms // Count limit (16 bit)
    TA1CCR0 = DELAY_LISTENING;
    TA1CTL = TASSEL_1 + MC_1 + ID_2; // Timer A0 with ACLK, count UP @ 32.768KHz.
}

void init_TimerA0()//Acquisition
{
	//Initialisation d'un timer pour l'activation de l'interrupt
		TA0CCR0 = 3000;                 	 // Count limit (16 bit) ----> 32768/(4*160) = 50 Hz
		TA0CCTL0 = 0x10;                 // Enable Timer A0 interrupts, bit 4 = 1
		TA0CTL = TASSEL_1 + MC_1 + ID_2; // Timer A0 with ACLK, count UP @ 32.768KHz.
}

void init_TimerB0()//Measurement
{
	//Initialisation d'un timer pour mesurer la frequence d'acquisition
		TB0CTL = TBSSEL_1 + MC_2 + ID_1; // Timer B0 with ACLK, count UP @ 32.768KHz.
}

void init_UCA1_UART()
{
	//initialisation de la communication UART pour le transfert des données vers l'ordinateur
		P4SEL = BIT5+BIT4;                        // P4.4 et P4.5 = USCI_A1 TXD/RXD
		UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
		UCA1CTL1 |= UCSSEL_2;                     // CLK = SMCLK
		UCA1BR0 = 0x68;                           // 12MHz/115200=0x68 defines the Baudrate (see User's Guide)
		UCA1BR1 = 0x00;                           //
		UCA1MCTL = 0x02;		                  // Modulation UCBRSx=1, UCBRFx=0
		UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

		UCA1IE = UCRXIE;						  // Enable RX Interrupt
}

void init_Config_IO()
{
	//Set P1.0 to output direction pour led rouge
	//GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

	//Set P4.6 to output direction pour led verte
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);
}

void init_DMA_ADC(int16_t * mes_get)
{
	DMA_init(DMA_CHANNEL_0,
			 DMA_TRANSFER_REPEATED_BLOCK,
	         NB_CHANNELS,
	         DMA_TRIGGERSOURCE_24,
	         DMA_SIZE_SRCWORD_DSTWORD,
	         DMA_TRIGGER_RISINGEDGE);

  	DMA_setTransferSize(DMA_CHANNEL_0,
  						NB_CHANNELS);

    DMA_setSrcAddress(DMA_CHANNEL_0,
    				  (uint32_t)&ADC12MEM0,
    				  DMA_DIRECTION_INCREMENT);

    DMA_setDstAddress(DMA_CHANNEL_0,
    				  (uint32_t)&mes_get[0],
                      DMA_DIRECTION_INCREMENT);

    DMA_enableTransfers(DMA_CHANNEL_0);

    DMA_enableInterrupt(DMA_CHANNEL_0);
}
