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
//  File Function: N76E003 Watch Dog reste functiondemo code
//***********************************************************************************************************

#include "N76E003.h"





#define     CFG_READ            0xC0
#define     CFG_ERASE           0xE2
#define     CFG_BYTE_PROGRAM    0xE1

/***********************************************************************
	WDT CONFIG enable 
	warning : this macro is only when ICP not enable CONFIG WDT function
	copy this marco code to you code to enable WDT reset.
************************************************************************/
void Enable_WDT_Reset_Config(void)
{
	  set_IAPEN;
    IAPAL = 0x04;
    IAPAH = 0x00;
    IAPFD = 0x0F;
    IAPCN = CFG_BYTE_PROGRAM;
    set_CFUEN;
    set_IAPGO;                                  //trigger IAP
		while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
    clr_CFUEN;
    clr_IAPEN;
}
/***********************************************************************
	WDT CONFIG disable 
	warning : this macro is for CONFIG already enable WDT Reset to disable.
	Since erase CONFIG is full page 
	step 1. storage CONFIG value in RAM
	step 2. modify RAM CONFIG4 WDT value to disable 
	step 3. erase CONFIG page
	step 4. re writer CONFIG.
************************************************************************/
void Disable_WDT_Reset_Config(void)
{
	UINT8 cf0,cf1,cf2,cf3,cf4;
	
	  set_IAPEN;
    IAPAL = 0x00;
    IAPAH = 0x00;
    IAPCN = CFG_READ;
    set_IAPGO;                                  //Storage CONFIG0 data
		cf0 = IAPFD;
		IAPAL = 0x01;
		set_IAPGO;                                  //Storage CONFIG1 data
		cf1 = IAPFD;
		IAPAL = 0x02;
	  set_IAPGO;                                  //Storage CONFIG2 data
		cf2 = IAPFD;
		IAPAL = 0x03;
	  set_IAPGO;                                  //Storage CONFIG3 data
		cf3 = IAPFD;
		IAPAL = 0x04;
	  set_IAPGO;                                  //Storage CONFIG4 data
		cf4 = IAPFD;
		cf4 |= 0xF0;																//Moidfy Storage CONFIG4 data disable WDT reset
		
		set_CFUEN;	
		IAPCN = CFG_ERASE;													//Erase CONFIG all
		IAPAH = 0x00;
		IAPAL = 0x00;
		IAPFD = 0xFF;
		set_IAPGO;
		
		IAPCN = CFG_BYTE_PROGRAM;										//Write CONFIG
		IAPFD = cf0;
		set_IAPGO;
		IAPAL = 0x01;
		IAPFD = cf1;
		set_IAPGO;
		IAPAL = 0x02;
		IAPFD = cf2;
		set_IAPGO;
		IAPAL = 0x03;
		IAPFD = cf3;
		set_IAPGO;
		IAPAL = 0x04;
		IAPFD = cf4;
		set_IAPGO;

    clr_CFUEN;
    clr_IAPEN;
}

/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void main (void)
{

  Set_All_GPIO_Quasi_Mode;

	clr_GPIO1;
	Timer0_Delay1ms(50);					//toggle I/O to show MCU Reset
	set_GPIO1;
	Timer0_Delay1ms(50);
	clr_GPIO1;
	Timer0_Delay1ms(50);
	set_GPIO1;
	Timer0_Delay1ms(50);

	
//----------------------------------------------------------------------------------------------
// WDT Init !!! ENABLE CONFIG WDT FIRST !!!
// Warning:
// Always check CONFIG WDT enable first, CONFIG not enable, SFR can't enable WDT reset
// Please call Enable_WDT_Reset_Config() function to enable CONFIG WDT reset
//----------------------------------------------------------------------------------------------

//	Enable_WDT_Reset_Config();							//Software enable WDT reset CONFIG setting
	  TA=0xAA;TA=0x55;WDCON|=0x07;						//Setting WDT prescale 
		set_WDCLR;															//Clear WDT timer
		while((WDCON|~SET_BIT6)==0xFF);					//confirm WDT clear is ok before into power down mode
//		EA = 1;
		set_WDTR;																//WDT run
		Disable_WDT_Reset_Config();						//Software disable WDT reset CONFIG setting

    while (1)
		{
			clr_GPIO1;
			Timer0_Delay1ms(100);
			set_GPIO1;
			Timer0_Delay1ms(100);
			clr_GPIO1;
			Timer0_Delay1ms(100);
			set_GPIO1;
			Timer0_Delay1ms(100);
			clr_GPIO1;
			set_PD;
		}
}

