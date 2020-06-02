/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jan/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003  ADC mutli channel demo code
//***********************************************************************************************************
#include "N76E003.h"





//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
		unsigned char ADCdataH[4], ADCdataL[4];

				
/******************************************************************************
The main C function.  Program execution starts
here after stack initialization.
******************************************************************************/
void main (void) 
{
		unsigned char i;	

	
	while(1)
	{
		CKDIV = 0x02;															// IMPORTANT!! Modify system clock to 4MHz ,then add the ADC sampling clock base to add the sampling timing.
		i = 0;
			Enable_ADC_AIN0;
			clr_ADCF;
			set_ADCS;																
      while(ADCF == 0);
			ADCdataH[i] = ADCRH;
			ADCdataL[i] = ADCRL;
			Disable_ADC;
		i++;
		
			Enable_ADC_BandGap;
			clr_ADCF;
			set_ADCS;																
      while(ADCF == 0);
			ADCdataH[i] = ADCRH;
			ADCdataL[i] = ADCRL;
			Disable_ADC;
		i++;
		
			Enable_ADC_AIN3;
			clr_ADCF;
			set_ADCS;																
      while(ADCF == 0);
			ADCdataH[i] = ADCRH;
			ADCdataL[i] = ADCRL;
			Disable_ADC;
		CKDIV = 0x00;	
	}
		
	
}