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
//  File Function: N76E003 APROM program DATAFLASH as EEPROM way 
//***********************************************************************************************************
#include "N76E003.h"





#define ADDR_BASE 0x4700

#define FLASH_L_LOW			ADDR_BASE+1
#define FLASH_L_HIGH		ADDR_BASE+2
#define FLASH_H_LOW			ADDR_BASE+3
#define FLASH_H_HIGH		ADDR_BASE+4

volatile unsigned char xdata page_buffer[128];


UINT8 Read_APROM_BYTE(UINT16 code *u16_addr)
{
	UINT8 rdata;
	rdata = *u16_addr>>8;
	return rdata;
}
/*****************************************************************************************************************
write_DATAFLASH_BYTE :
user can copy all this subroutine into project, then call this function in main.
******************************************************************************************************************/		
void Write_DATAFLASH_BYTE(unsigned int u16EPAddr,unsigned char u8EPData)
{
	unsigned char looptmp=0;
	unsigned int u16_addrl_r;
	unsigned int RAMtmp;
	
//Check page start address
	u16_addrl_r=(u16EPAddr/128)*128;
//Save APROM data to XRAM0
	for(looptmp=0;looptmp<0x80;looptmp++)
	{
		RAMtmp = Read_APROM_BYTE((unsigned int code *)(u16_addrl_r+looptmp));
		page_buffer[looptmp]=RAMtmp;
	}
// Modify customer data in XRAM
		page_buffer[u16EPAddr&0x7f] = u8EPData;
	
//Erase APROM DATAFLASH page
		IAPAL = u16_addrl_r&0xff;
		IAPAH = (u16_addrl_r>>8)&0xff;
		IAPFD = 0xFF;
	  set_IAPEN; 
		set_APUEN;
    IAPCN = 0x22; 		
 		set_IAPGO; 
		
//Save changed RAM data to APROM DATAFLASH
		set_IAPEN; 
		set_APUEN;
	  IAPCN = 0x21;
		for(looptmp=0;looptmp<0x80;looptmp++)
		{
			IAPAL = (u16_addrl_r&0xff)+looptmp;
      IAPAH = (u16_addrl_r>>8)&0xff;
			IAPFD = page_buffer[looptmp];
			set_IAPGO;			
		}
		clr_APUEN;
		clr_IAPEN;
}	
	
/******************************************************************************************************************/	


void main (void) 
{
		UINT8 datatemp;
		UINT16 system16highsite;
/* -------------------------------------------------------------------------*/
/*  Dataflash use APROM area, please ALWAYS care the address of you code    */
/*	APROM 0x3800~0x38FF demo as dataflash 				      				      			*/
/* 	Please use Memory window key in C:0x3800 to check earse result					*/	      
/* -------------------------------------------------------------------------*/
		InitialUART0_Timer1(115200);
//call write byte 
		Write_DATAFLASH_BYTE (0x3802,0x34);
		Write_DATAFLASH_BYTE (FLASH_L_LOW,0x55);
		Write_DATAFLASH_BYTE (FLASH_L_HIGH,0x56);
		Write_DATAFLASH_BYTE (FLASH_H_LOW,0xaa);
		Write_DATAFLASH_BYTE (FLASH_H_HIGH,0x66);
//call read byte
		datatemp = Read_APROM_BYTE(0x3802);
		system16highsite = ((Read_APROM_BYTE(FLASH_H_HIGH)<<8)+Read_APROM_BYTE(FLASH_H_LOW));

    while(1)
		{
//				printf ("\n data temp = 0x%bx", datatemp);
		}
}
//-----------------------------------------------------------------------------------------------------------
