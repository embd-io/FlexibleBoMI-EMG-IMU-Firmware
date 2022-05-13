#include "driverlib.h"
#include "nRF.h"
//---------------------------------------------------------------------
char tmp;
//---------------------------------------------------------------------
void set_RF(void)
{
	nRF_CE_low;
	nRF_DESELECT;
	__delay_cycles(60000); 			//--> 5ms at power up

	nRF_init();
	nRF_node0_set();       			//in tx mode by default
	//nRF_ENTER_RECEIVE_MODE;
	nRF_ENTER_TRANSMIT_MODE;
}
//---------------------------------------------------------------------
void nRF_init(void)
{
    unsigned int i;
	nRF_CE_low;
	for(i = 65000 ; i > 0; i--);
	nRF_SELECT;
	for(i = 65000 ; i > 0; i--);
	nRF_DESELECT;
	/*nRF_SELECT;
	__delay_cycles(60000);
	nRF_DESELECT;*/
}
//---------------------------------------------------------------------
void nRF_reg_write(char addr, char *data, unsigned data_length)
{
    char i;
    //char test = 0;
    addr = addr & 0x1F;
    addr = addr | 0x20;
    
    nRF_SELECT;
    
    UCA0TXBUF = addr;
    //while (!(UCTXIFG));
    
    for(i = data_length; i > 0; i--)
    {
        while(UCA0STAT & UCBUSY);
        UCA0TXBUF = data[data_length - i];
        // wait for TX
        //while (!(UCTXIFG));
    }
    
    while(UCA0STAT & UCBUSY);
    nRF_DESELECT;
}
//---------------------------------------------------------------------
void nRF_reg_read(char addr, char *data, unsigned data_length)
{
    char i;
    
    addr = addr & 0x1F;
    
    //SPI_buffer_flush();
    
    nRF_SELECT;
    
    UCA0TXBUF = addr;
    //while (!(UCTXIFG));
    while(UCA0STAT & UCBUSY);
    tmp = UCA0RXBUF;
    
    for(i = data_length; i > 0 ; i--)
    {
        UCA0TXBUF = 0xFF;
        while(UCA0STAT & UCBUSY);
        //while (!(UCTXIFG));
        *(data+(data_length - i)) = UCA0RXBUF;
        //while (!(UCRXIFG));
    }
    
    while(UCA0STAT & UCBUSY);
    nRF_DESELECT;    
}
//---------------------------------------------------------------------
void nRF_upload_TX_payload(char *data)
{
    unsigned int i;

    //__bic_SR_register(GIE);  // stop all interrupts

    nRF_SELECT;

    UCA0TXBUF =  0xA0;
    //while (!(UCTXIFG));

    for(i = controle_packet_len; i > 0; i--)
    {
        while(UCA0STAT & UCBUSY);
        UCA0TXBUF = data[controle_packet_len - i];
        //while (!(UCTXIFG));
        /*
        *(data+i) = UCA0TXBUF;
        send_data_UART((data+i), 1);
        */
    }

    while(UCA0STAT & UCBUSY);
    nRF_DESELECT;

    //__bis_SR_register(GIE); // reactivate all interrupts

    //SPI_buffer_flush();
}
//---------------------------------------------------------------------
void nRF_download_RX_payload(char *data)
{
    char i;

    //nRF_FLUSH_RX();

    nRF_SELECT;

    UCA0TXBUF =  0x61;
    //while (!(UCTXIFG));
    while(UCA0STAT & UCBUSY);
    tmp = UCA0RXBUF;

    for(i = controle_packet_len; i > 0; i--)
    {
        UCA0TXBUF = 0xFF;
        //while (!(UCTXIFG));
        while(UCA0STAT & UCBUSY);
        *(data+(controle_packet_len - i)) = UCA0RXBUF;
    }

    while(UCA0STAT & UCBUSY);

    nRF_DESELECT;

    nRF_FLUSH_RX();
}
//---------------------------------------------------------------------
void nRF_FLUSH_TX(void)
{
    nRF_SELECT;

    UCA0TXBUF =  0xE1;
    while(UCA0STAT & UCBUSY);
    //while (!(UCTXIFG));
    tmp = UCA0RXBUF;

    nRF_DESELECT;
}
//---------------------------------------------------------------------
void nRF_FLUSH_RX(void)
{
    nRF_SELECT;
    
    UCA0TXBUF =  0xE2;
    while(UCA0STAT & UCBUSY);
    //while (!(UCTXIFG));
    tmp = UCA0RXBUF;
    
    nRF_DESELECT;
}
//---------------------------------------------------------------------
char nRF_FIFO_STATUS(void)
{
    char status;

    nRF_SELECT;

    UCA0TXBUF =  0x17;
	while(UCA0STAT & UCBUSY);
    //while (!(UCTXIFG));
    tmp = UCA0RXBUF;

	UCA0TXBUF = 0xFF;
	while(UCA0STAT & UCBUSY);
	//while (!(UCTXIFG));
	status = UCA0RXBUF;

    nRF_DESELECT;

    return status;
}
//---------------------------------------------------------------------
char nRF_read_RX_payload_len(void)
{
    char length;

    nRF_SELECT;

    UCA0TXBUF =  0x60;
    while(UCA0STAT & UCBUSY);
    //while (!(UCTXIFG));
    tmp = UCA0RXBUF;

    UCA0TXBUF = 0xFF;
    while(UCA0STAT & UCBUSY);
    //while (!(UCTXIFG));
    length = UCA0RXBUF;

    nRF_DESELECT;

    return length;
}
//---------------------------------------------------------------------
void nRF_clear_IRQ(void)
{
    nRF_SELECT;

    UCA0TXBUF =  0x27;
    //while (!(UCTXIFG));
    while(UCA0STAT & UCBUSY);
    tmp = UCA0RXBUF;

    UCA0TXBUF =  0x70;
    //while (!(UCTXIFG));
    while(UCA0STAT & UCBUSY);
    tmp = UCA0RXBUF;

    nRF_DESELECT;
}
//---------------------------------------------------------------------
void wait_for_place_in_tx_fifo(void)
{
	volatile char fifo_status;
	fifo_status = nRF_FIFO_STATUS();
	while(fifo_status & 0x20)
	{
		__delay_cycles(100);
		fifo_status = nRF_FIFO_STATUS();
	}
}
//---------------------------------------------------------------------
void wait_for_empty_tx_fifo(void)
{
	volatile char fifo_status;
	fifo_status = nRF_FIFO_STATUS(); // Check the fifo status of the transceiver
	while(!(fifo_status & 0x10))
	{
		__delay_cycles(100);
		fifo_status = nRF_FIFO_STATUS();
	}
}
