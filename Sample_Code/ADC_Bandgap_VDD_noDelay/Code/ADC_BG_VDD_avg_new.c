/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2016
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E885 ADC demo code
//***********************************************************************************************************

#include "N76E003.h"





//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
#if 0
//#define Enable_ADC_BandGap  ADCCON0|=SET_BIT3;ADCCON0&=0xF8;                                                              //Band-gap 1.22V
#endif

double  Bandgap_Voltage,VDD_Voltage;      //please always use "double" mode for this
unsigned  char xdata ADCdataH[5], ADCdataL[5];
int ADCsumH=0, ADCsumL=0;
unsigned char ADCavgH,ADCavgL;

void READ_BANDGAP()
{
    UINT8 BandgapHigh,BandgapLow,BandgapMark;
    double Bandgap_Value,Bandgap_Voltage_Temp;
  
    set_IAPEN;
    IAPCN = READ_UID;
    IAPAL = 0x0d;
    IAPAH = 0x00;
    set_IAPGO;
    BandgapLow = IAPFD;
    BandgapMark = BandgapLow&0xF0;

    if (BandgapMark==0x80)
    {
        BandgapLow = BandgapLow&0x0F;
        IAPAL = 0x0C;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapHigh = IAPFD;
        Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
        Bandgap_Voltage_Temp = Bandgap_Value*3/4;
        Bandgap_Voltage = Bandgap_Voltage_Temp - 33;      //the actually banggap voltage value is similar this value.
    }
    if (BandgapMark==0x00)
    {
        BandgapLow = BandgapLow&0x0F;
        IAPAL = 0x0C;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapHigh = IAPFD;
        Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
        Bandgap_Voltage= Bandgap_Value*3/4;
    }
    if (BandgapMark==0x90)
    {
        IAPAL = 0x0E;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapHigh = IAPFD;
        IAPAL = 0x0F;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapLow = IAPFD;
        BandgapLow = BandgapLow&0x0F;
        Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
        Bandgap_Voltage= Bandgap_Value*3/4;
    }
    clr_IAPEN;
//      printf ("\n BG High = %bX",BandgapHigh);
//      printf ("\n BG Low = %bX",BandgapLow);
//      printf ("\n BG ROMMAP = %e",Bandgap_Voltage);
}


/******************************************************************************
The main C function.  Program execution starts
here after stack initialization.
******************************************************************************/
void main (void) 
{
    double bgvalue;
    unsigned int i;
  
    InitialUART0_Timer1(115200);
    READ_BANDGAP();
    printf ("\n BG ROMMAP = %e",Bandgap_Voltage); 
  
    while (1)
    {
        Enable_ADC_BandGap;
        CKDIV = 0x02;                              // IMPORTANT!! Modify system clock to 4MHz ,then add the ADC sampling clock base to add the sampling timing.
        for(i=0;i<5;i++)                           // All following ADC detect timing is 200uS run under 4MHz.
        {
            clr_ADCF;
            set_ADCS;
            while(ADCF == 0);
            ADCdataH[i] = ADCRH;
            ADCdataL[i] = ADCRL;
        }    
        CKDIV = 0x00;                              // After ADC sampling, modify system clock back to 16MHz to run next code.
        Disable_ADC;
        for(i=2;i<5;i++)                           // use the last 3 times data to make average 
        {
          ADCsumH = ADCsumH + ADCdataH[i];
          ADCsumL = ADCsumL + ADCdataL[i];
        }        
        ADCavgH = ADCsumH/3;
        ADCavgL = ADCsumL/3;
        bgvalue = (ADCavgH<<4) + ADCavgL;
        VDD_Voltage = (0x1000/bgvalue)*Bandgap_Voltage;
        printf ("\n VDD voltage = %e", VDD_Voltage); 
        Timer0_Delay1ms(500);
        ADCsumH = 0;
        ADCsumL = 0;
    }
}


