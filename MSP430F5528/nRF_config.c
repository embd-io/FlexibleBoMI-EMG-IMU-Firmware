
#include "nRF.h"

char rx_channel[8] = {0x50, 0x55, 0x60, 0x65, 0x70, 0x75, 0x80, 0x85};

//------------------------------------
// nRF24L01+ Enter TX mode
void nRF_enter_tx (void)
{
    char data[1] = {EN_IRQ_RX | EN_IRQ_TX | DIS_IRQ_RT | EN_CRC | CRCO_1BIT | PWR_UP | SET_IN_TX};
    nRF_reg_write(CONFIG_REG,data,1);
}
//------------------------------------
// nRF24L01+ Enter RX mode
void nRF_enter_rx (void)
{
    char data[1] = {EN_IRQ_RX | EN_IRQ_TX | DIS_IRQ_RT | EN_CRC | CRCO_1BIT | PWR_UP | SET_IN_RX};
    nRF_reg_write(CONFIG_REG,data,1);
}
//------------------------------------
void nRF_set_tx_pipe (char ch_id)
{
    char data[1];
    // RF_CH : RF Channel
    data[0] = rx_channel[ch_id];
    nRF_reg_write(RF_CH_REG,data,1);
}
//------------------------------------
// nRF24L01+ Node0 setting (TX mode by default)
void nRF_node0_set(void)
{
    char data[5];
    unsigned int i;
    //++++++++++++++++++++++++++
    // CONFIG : Configuration Register
    data[0] = EN_IRQ_RX | EN_IRQ_TX | DIS_IRQ_RT | EN_CRC | CRCO_1BIT | PWR_UP | SET_IN_TX;
    nRF_reg_write(CONFIG_REG,data,1);
    //++++++++++++++++++++++++++
    for(i = 65000 ; i > 0; i--);
    //++++++++++++++++++++++++++
    // EN_AA : Enhanced ShockBurst
    data[0] = ENAA_P0;
    nRF_reg_write(EN_AA_REG,data,1);
    //++++++++++++++++++++++++++
    // EN_RXADDR : Enabled RX Addresses
    data[0] = EN_RX_P0;
    nRF_reg_write(EN_RXADDR_REG,data,1);
    //++++++++++++++++++++++++++
    // SETUP_AW : Setup of Address Widths
    data[0] = SET_3BYTES;
    nRF_reg_write(SET_AW_REG,data,1);
    //++++++++++++++++++++++++++
    // SETUP_RETR : Setup of Automatic Retransmission
    data[0] = WAIT_2000us | RETR_X3;
    //data[0] = WAIT_1000us | RETR_X8;
    nRF_reg_write(SET_RETR_REG,data,1);
    //++++++++++++++++++++++++++
    // RF_CH : RF Channel
    data[0] = rx_channel[4];
    nRF_reg_write(RF_CH_REG,data,1);
    //++++++++++++++++++++++++++
    // RF_SETUP : RF Setup Register
    //data[0] = DR_2Mbps | PWR_0dBm;
    data[0] = DR_250kbps | PWR_0dBm;
    nRF_reg_write(RF_DR_REG,data,1);
    //++++++++++++++++++++++++++
    // STATUS : Status Register
    //++++++++++++++++++++++++++
    // OBSERVE_TX : Transmit observe register
    //++++++++++++++++++++++++++
    // RPD : Received Power Detector
    //++++++++++++++++++++++++++
    // RX_ADDR_P0 : Receive address data pipe 0
    data[0] = 0xB3;//0xB0;//
    data[1] = 0xAA;
    data[2] = 0xBB;
    //data[3] = 0x0E;
    //data[4] = 0xB3;
    nRF_reg_write(0x0A,data,3);
    //++++++++++++++++++++++++++
    // RX_ADDR_P1 : Receive address data pipe 1 
    //++++++++++++++++++++++++++
    // RX_ADDR_P2 : Receive address data pipe 2
    //++++++++++++++++++++++++++
    // RX_ADDR_P3 : Receive address data pipe 3
    //++++++++++++++++++++++++++
    // RX_ADDR_P4 : Receive address data pipe 4
    //++++++++++++++++++++++++++
    // RX_ADDR_P5 : Receive address data pipe 5
    //++++++++++++++++++++++++++
    // TX_ADDR : Transmit address
    data[0] = 0xB3;
    data[1] = 0xAA;
    data[2] = 0xBB;
    //data[3] = 0x0E;
    //data[4] = 0xB3;
    nRF_reg_write(0x10,data,3);
    //++++++++++++++++++++++++++
    // RX_PW_P0 : Number of bytes in RX payload in data pipe 0 ????????????
    data[0] = controle_packet_len;
    nRF_reg_write(0x11,data,1);
    //++++++++++++++++++++++++++
    // RX_PW_P1 : Number of bytes in RX payload in data pipe 1
    //++++++++++++++++++++++++++
    // RX_PW_P2 : Number of bytes in RX payload in data pipe 2
    //++++++++++++++++++++++++++
    // RX_PW_P3 : Number of bytes in RX payload in data pipe 3
    //++++++++++++++++++++++++++
    // RX_PW_P4 : Number of bytes in RX payload in data pipe 4
    //++++++++++++++++++++++++++
    // RX_PW_P5 : Number of bytes in RX payload in data pipe 5
    //++++++++++++++++++++++++++
    // FIFO_STATUS : FIFO Status Register
    //++++++++++++++++++++++++++
    // DYNPD : Enable dynamic payload length
    data[0] = EN_DPL_ALL;
    nRF_reg_write(0x1C,data,1);
    //++++++++++++++++++++++++++
    // FEATURE : Feature Register
    data[0] = EN_DPL;
    nRF_reg_write(0x1D,data,1);
    //++++++++++++++++++++++++++
}
//------------------------------------













