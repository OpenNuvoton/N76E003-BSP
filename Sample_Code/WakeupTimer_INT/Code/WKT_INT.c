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
//  File Function: N76E003 wake up timer interrupt demo code
//***********************************************************************************************************
#include "N76E003.h"





void WakeUp_Timer_ISR (void)   interrupt 17     //ISR for self wake-up timer
{
		clr_GPIO1;
		Timer1_Delay10ms(5);
		set_GPIO1;
		Timer1_Delay10ms(5);
		clr_GPIO1;
		Timer1_Delay10ms(5);
		set_GPIO1;
	  clr_WKTF;                    		           //clear interrupt flag   
}


/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void main (void)
{
   
    P12_PushPull_Mode;

		clr_GPIO1;
		Timer0_Delay1ms(400);
		set_GPIO1;
		Timer0_Delay1ms(400);
		clr_GPIO1;
		Timer0_Delay1ms(400);
		set_GPIO1;
		Timer0_Delay1ms(400);
		clr_GPIO1;
		Timer0_Delay1ms(400);
		set_GPIO1;

//-----------------------------------------------------
//	WKT initial 	
//-----------------------------------------------------	
		WKCON = 0x02; 										//timer base 10k, Pre-scale = 1/16
//		RWK = 0XFF;											//	if prescale is 0x00, never set RWK = 0xff
		RWK = 0X00;
	  set_EWKT;													// enable WKT interrupt
		set_WKTR; 												// Wake-up timer run 
		EA = 1;
	
		while(1);
}

