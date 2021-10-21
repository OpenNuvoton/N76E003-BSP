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
//  File Function: N76E003 read HIRC and modify to 16.6MHz demo code
//***********************************************************************************************************
#include "N76E003.h"





void MODIFY_HIRC_166(void)				// Modify HIRC to 16.6MHz, more detail please see datasheet V1.02
{
		UINT8 hircmap0,hircmap1;
		UINT16 trimvalue16bit;
/* Check if power on reset, modify HIRC */
		if ((PCON&SET_BIT4)==SET_BIT4)				
		{
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
		}
}

void main(void)
{
	CKDIV = 0x00;
	P11_PushPull_Mode;
	MODIFY_HIRC_166();
	CKDIV = 50;					//HIRC devider 160
	set_CLOEN;						//Check HIRC out wavefrom to confirm the HIRC modified
	while(1);
}