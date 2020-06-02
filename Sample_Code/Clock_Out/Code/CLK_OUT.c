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
//  File Function: N76E003 Clock Output demo code
//***********************************************************************************************************

#include "N76E003.h"





//======================================================================================================

void main(void)
{
	
    /* Note
       MCU power on system clock is HIRC (16.0000MHz), if need change Fsys source pleaes copy hange system closk source demo
    */
    
		Set_All_GPIO_Quasi_Mode;                    //in Function_define.h
    set_CLOEN;                                  //System Clock Output Enable
//     Please measure P1.1 (CLO) pin for system clock output, it should as system clock;

    while(1)
    {
        clr_GPIO1;
        Timer0_Delay1ms(500);
        set_GPIO1;
        Timer0_Delay1ms(500);
    }
/* =================== */
}

