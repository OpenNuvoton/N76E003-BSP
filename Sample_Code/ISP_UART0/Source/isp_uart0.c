/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  File Function: ML51 UART0 ISP subroutine
//***********************************************************************************************************
#include "N76E003.h"
#include "isp_uart0.h"

bit BIT_TMP;
  xdata volatile uint8_t uart_rcvbuf[64]; 
  xdata volatile uint8_t uart_txbuf[64];
  data volatile uint8_t bufhead;
  data volatile uint16_t flash_address; 
  data volatile uint16_t AP_size;
  data volatile uint8_t g_timer1Counter;
  data volatile uint8_t count; 
  data volatile uint16_t g_timer0Counter;
  data volatile uint32_t g_checksum;
  data volatile uint32_t g_totalchecksum;
  bit volatile bUartDataReady;
  bit volatile g_timer0Over;
  bit volatile g_timer1Over;
  bit volatile g_programflag;

unsigned char PID_highB,PID_lowB,DID_highB,DID_lowB,CONF0,CONF1,CONF2,CONF4;
unsigned char recv_CONF0,recv_CONF1,recv_CONF2,recv_CONF4;

void UART0_ini_115200(void)
{
    P06_Quasi_Mode;    
    P07_Quasi_Mode;
  
    SCON = 0x52;     //UART0 Mode1,REN=1,TI=1
    TMOD |= 0x20;    //Timer1 Mode1
    
    set_SMOD;        //UART0 Double Rate Enable
    set_T1M;
    clr_BRCK;        //Serial port 0 baud rate clock source = Timer1

    TH1 = 256 - (1037500/115200);               /*16 MHz */
    set_TR1;
    ES=1;
    EA=1;
}

void MODIFY_HIRC_16588(void)
{
    UINT8 hircmap0,hircmap1;
    UINT16 trimvalue16bit;
/* Check if power on reset, modify HIRC */
//    if ((PCON&SET_BIT4)==SET_BIT4)
//    {
        hircmap0 = RCTRIM0;
        hircmap1 = RCTRIM1;
        trimvalue16bit = ((hircmap0<<1)+(hircmap1&0x01));
        trimvalue16bit = trimvalue16bit - 14;
        hircmap1 = trimvalue16bit&0x01;
        hircmap0 = trimvalue16bit>>1;
        TA=0XAA;
        TA=0X55;
        RCTRIM0 = hircmap0;
        TA=0XAA;
        TA=0X55;
        RCTRIM1 = hircmap1;
/* Clear power on flag */
        PCON &= CLR_BIT4;
//    }
}

void MODIFY_HIRC_16(void)
{
    unsigned char data hircmap0,hircmap1;
    IAPAH = 0x00;
    IAPAL = 0x30;
    IAPCN = READ_UID;
    set_IAPGO;
    hircmap0 = IAPFD;
    IAPAL = 0x31;
    set_IAPGO;
    hircmap1 = IAPFD;

    TA=0XAA;
    TA=0X55;
    RCTRIM0 = hircmap0;
    TA=0XAA;
    TA=0X55;
    RCTRIM1 = hircmap1;
}

void READ_ID(void)
{
//    set_CHPCON_IAPEN;
    IAPCN = BYTE_READ_ID;
    IAPAH = 0x00;
    IAPAL = 0x00;
    set_IAPGO;
    DID_lowB = IAPFD;
    IAPAL = 0x01;
    set_IAPGO;
    DID_highB = IAPFD;
    IAPAL = 0x02;
    set_IAPGO;
    PID_lowB = IAPFD;
    IAPAL = 0x03;
    set_IAPGO;
    PID_highB = IAPFD;
}
void READ_CONFIG(void)
{
    IAPCN = BYTE_READ_CONFIG;
    IAPAH = 0x00;
    IAPAL = 0x00;
    set_IAPGO;
    CONF0 = IAPFD;
    IAPAL = 0x01;
    set_IAPGO;
    CONF1 = IAPFD;
    IAPAL = 0x02;
    set_IAPGO;
    CONF2 = IAPFD;
    IAPAL = 0x04;
    set_IAPGO;
    CONF4 = IAPFD;
}

void TM0_ini(void)
{    
  TH0=TL0=0;    //interrupt timer 140us
  set_TR0;      //Start timer0
  set_PSH;       // Serial port 0 interrupt level2
  set_ET0;
}

void Package_checksum(void)
{
  g_checksum=0;
   for(count=0;count<64;count++)
  {
    g_checksum =g_checksum+ uart_rcvbuf[count];    
  }
  uart_txbuf[0]=g_checksum&0xff;
  uart_txbuf[1]=(g_checksum>>8)&0xff;
  uart_txbuf[4]=uart_rcvbuf[4]+1;
  uart_txbuf[5]=uart_rcvbuf[5];
  if(uart_txbuf[4]==0x00)
  uart_txbuf[5]++;

}


void Send_64byte_To_UART0(void)
{
   for(count=0;count<64;count++)
  {
     TI = 0;  
     SBUF = uart_txbuf[count];
     while(TI==0);
     set_WDCLR;
  }
}

void Serial_ISR (void) interrupt 4 
{
    if (RI == 1)
    {   
      uart_rcvbuf[bufhead++]=  SBUF;    
      clr_RI;                                           // Clear RI (Receive Interrupt).
    }
    if (TI == 1)
    {       
        clr_TI;                                         // Clear TI (Transmit Interrupt).
    }
    if(bufhead ==1)
    {
      g_timer1Over=0;
      g_timer1Counter=90; //for check uart timeout using
    }
  if(bufhead == 64)
    {
      
      bUartDataReady = TRUE;
      g_timer1Counter=0;
      g_timer1Over=0;
      bufhead = 0;
    }    
}

void Timer0_ISR (void) interrupt 1
{
if(g_timer0Counter)
  {
  g_timer0Counter--;
    if(!g_timer0Counter)
    {
    g_timer0Over=1;
    }
  }
  
  if(g_timer1Counter)
  {
  g_timer1Counter--;
    if(!g_timer1Counter)
    {
    g_timer1Over=1;
    }
  }
}  
