/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : May/02/2018
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 Watch Dog reste functiondemo code
//***********************************************************************************************************

#include "N76E003.h"





/***********************************************************************
	WDT CONFIG enable 
	warning : this macro is only when ICP not enable CONFIG WDT function
	copy this marco code to you code to enable WDT reset.
************************************************************************/
void Check_WDT_Reset_Config(void)
{
		set_IAPEN;
		IAPAL = 0x04;
		IAPAH = 0x00;	
		IAPFD = 0xFF;
		IAPCN = 0xC0;						//Read config command
		set_IAPGO;  
		if ((IAPFD&0xF0)==0xF0)
		{
				IAPFD = 0x0F;
				IAPCN = 0xE1;
        set_IAPGO;                         						//trigger IAP
				while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
				clr_CFUEN;
				clr_IAPEN;
				EA = 0;
				TA = 0xAA;
				TA = 0x55;
				CHPCON &= 0xFD;
				TA = 0xAA;
				TA = 0x55;
				CHPCON |= 0x80;
		}
		clr_IAPEN;
}


/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void main (void)
{

  Set_All_GPIO_Quasi_Mode;
//	CKDIV = 0X01;
	clr_GPIO1;
//	Timer0_Delay1ms(1);					//toggle I/O to show MCU Reset
	set_GPIO1;

	
//----------------------------------------------------------------------------------------------
// WDT Init !!! ENABLE CONFIG WDT FIRST !!!
// Warning:
// Always check CONFIG WDT enable first, CONFIG not enable, SFR can't enable WDT reset
// Please call Enable_WDT_Reset_Config() function to enable CONFIG WDT reset
//----------------------------------------------------------------------------------------------
		Check_WDT_Reset_Config();
	
	  TA=0xAA;TA=0x55;WDCON|=0x07;						//Setting WDT prescale 
		set_WDCLR;														//Clear WDT timer
		while((WDCON|~SET_BIT6)==0xFF);				//confirm WDT clear is ok before into power down mode
//		EA = 1;
		set_WDTR;															//WDT run
//		
    while(1)
		{
			clr_P05;
			Timer0_Delay1ms(1);
			set_P05;
			Timer0_Delay1ms(1);
			clr_P05;
			Timer0_Delay1ms(1);
			set_P05;
			Timer0_Delay1ms(1);
			clr_P05;
			set_PD;
		}
}

