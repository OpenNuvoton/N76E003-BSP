/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 CONFIG program demo code
//***********************************************************************************************************

#include "N76E003.h"





/*
	Since the DATAFLASH is in the APROM. Program command is same as program APROM
*/

#define     PAGE_ERASE_CF       0xE2
#define     BYTE_READ_CF        0xC0
#define     BYTE_PROGRAM_CF     0xE1

#define     ERASE_FAIL          0x70
#define     PROGRAM_FAIL        0x71
#define     IAPFF_FAIL          0x72
#define     IAP_PASS            0x00

		

/********************************************************************************************
 Following IAP command register is also define in SFR_Macro.h
 
	#define set_IAPEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON |= SET_BIT0 ;EA=BIT_TMP
	#define clr_IAPEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON &= ~SET_BIT0;EA=BIT_TMP
	#define set_CFUEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPUEN|=SET_BIT2;EA=BIT_TMP
	#define clr_CFUEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPUEN&=~SET_BIT2;EA=BIT_TMP
**********************************************************************************************/
void IAP_ERROR_LED(void)
{
	while (1)
	{
		clr_P03;
		Timer0_Delay1ms(100);
		set_P03;
		Timer0_Delay1ms(100);
	}

}

//-----------------------------------------------------------------------------------------------------------/
void Trigger_IAP(void)
{   
    set_IAPGO;																	//trigger IAP
    if((CHPCON&SET_BIT6)==SET_BIT6)             // if fail flag is set, toggle error LED and IAP stop
		{
			clr_IAPFF;
			IAP_ERROR_LED();
		}
}
/*
		WARNING:
	No matter read or writer, when IAPFF is set 1, 
	this step process is fail. DATA should be ignore.
*/
//-----------------------------------------------------------------------------------------------------------/

/*****************************************************************************************************************
Write CONFIG subroutine:
******************************************************************************************************************/		

void Enable_WDT_Reset_Config(void)
{
	  set_IAPEN;																	// Enable IAP function
    IAPAL = 0x04;
    IAPAH = 0x00;
    IAPFD = 0x0F;
    IAPCN = BYTE_PROGRAM_CF;
    set_CFUEN;																	// Enable CONFIG writer bit
    set_IAPGO;                                  //trigger IAP
		while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
    clr_CFUEN;
    clr_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
void main (void) 
{

  Set_All_GPIO_Quasi_Mode;
//---------toggle GPIO1---------	
	clr_GPIO1;
	Timer0_Delay1ms(100);
	set_GPIO1;
	Timer0_Delay1ms(100);
	clr_GPIO1;
	Timer0_Delay1ms(100);
	set_GPIO1;
	Timer0_Delay1ms(100);
//---------end toggle GPIO1---------
	
    Enable_WDT_Reset_Config();
    while(1);
}
//-----------------------------------------------------------------------------------------------------------

