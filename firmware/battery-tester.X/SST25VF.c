#include <xc.h>
#include <stdio.h>
#include "main.h"
#include "MMSP.h"
#include "settings.h"



#define		READ_ID					0X90
#define		READ_ID_ADD				0X00
#define		JEDEC_READ_ID			0X9F
#define		READ					0x03
#define		BYTE_PROGRAM			0X02
#define 	WRITE_EN				0x06
#define		WRITE_DB				0X04
#define		EBSY					0x70
#define		READ_REGISTER_STATUS	0x05
#define		CHIP_ERASE				0X60
#define		SRB_BUSY				0x01	//status register bit: busy		
#define		SRB_WRITEEN				0x02
#define		WRITE_STAT_REG_EN		0x50
#define     SECTOR_ERASE            0x20
#define     BLOCK_ERASE_32KB        0x52
#define     BLOCK_ERASE_64KB        0xD8

#define ENABLE_SST25        LATFbits.LF7=0
#define DISABLE_SST25       LATFbits.LF7=1


/*
*	Function checks BUSY bit 
*	
*	RETURN 1 if device BUSY
*	RETURN 0 if device FREE
*
*/

uint8_t CheckBusy(void)
{
	uint8_t StatusRegisterByte;

	ENABLE_SST25;

	SPI_Exchange(READ_REGISTER_STATUS);
	StatusRegisterByte=SPI_Exchange(0);

	if((StatusRegisterByte & SRB_BUSY)==1)
	{
		DISABLE_SST25;
		return 1;
	}

	DISABLE_SST25;
	return 0;
}

uint8_t CheckWriteEN (void)
{
	ENABLE_SST25;

	uint8_t StatusRegisterByte;

	SPI_Exchange(READ_REGISTER_STATUS);
	StatusRegisterByte=SPI_Exchange(0xFF);
	if((StatusRegisterByte & SRB_WRITEEN)>>1==1)
	{
		DISABLE_SST25;
		return 1;
	}

	DISABLE_SST25;
	return 0;
}

void ReadID (void)
{
	ENABLE_SST25;

    uint8_t tab[2];
    
	SPI_Exchange(READ_ID);
	SPI_Exchange(0x00);
	SPI_Exchange(0x00);
	SPI_Exchange(READ_ID_ADD);
    tab[0]=SPI_Exchange(0xFF);
    tab[1]=SPI_Exchange(0xFF);

	printf("Manufacurer ID: %x \r\nDevice ID: %x \r\n", tab[0], tab[1]);
    
	DISABLE_SST25;
}

void ReadID_JEDEC(void)
{
	ENABLE_SST25;

    uint8_t tab[3];
    
	SPI_Exchange(JEDEC_READ_ID);
    tab[0]=SPI_Exchange(0xFF);
    tab[1]=SPI_Exchange(0xFF);
    tab[2]=SPI_Exchange(0xFF);
    
	printf("Manufacurer ID: %x \r\nDevice ID: %x \r\nMemory Capacity: %x \r\n", tab[0], tab[1], tab[2]);
    
	DISABLE_SST25;
}

///////////////////////////////////////////////////////////
void ReadBytes (uint32_t Add, uint8_t *data ,uint8_t BytesCount)
{
	uint8_t i;
	ENABLE_SST25;
	//fprintf(KMP,"\n\r Add: %Lu\n\r",Add);
	SPI_Exchange(READ);
	SPI_Exchange(((Add & 0xFFFFFF) >> 16));
	SPI_Exchange(((Add & 0x00FFFF) >> 8));
	SPI_Exchange(Add & 0x0000FF);
	for(i=0;i<BytesCount;i++)
	{
		*(data+i)=SPI_Exchange(0);
	}

	DISABLE_SST25;
}

void SST25VF_init_Enable_Write(void)
{

	ENABLE_SST25;
	SPI_Exchange(WRITE_STAT_REG_EN);
	DISABLE_SST25;

	SPI_Exchange(0x00);

	ENABLE_SST25;
	SPI_Exchange(0x01);
	SPI_Exchange(0x00);
	DISABLE_SST25;

}


void WriteByte(uint32_t Add,uint8_t data)
{
	//enable end checking WRITE bit------------------------

	ENABLE_SST25;
	SPI_Exchange(WRITE_EN);
	DISABLE_SST25;

	while(1)
	{
		if(CheckWriteEN()==1)		
		{
			break;
		}
	}

	//-----------------------------------------------------
	//writing data to FLASH--------------------------------
	ENABLE_SST25;
	SPI_Exchange(BYTE_PROGRAM);
	SPI_Exchange(((Add & 0xFFFFFF) >> 16));
	SPI_Exchange(((Add & 0x00FFFF) >> 8));
	SPI_Exchange(Add & 0x0000FF);
	SPI_Exchange(data);

	DISABLE_SST25;
	//-----------------------------------------------------
	//Waiting to finish writing - Checking Busy flag

	while(1)
	{
		if(CheckBusy()==0)
		{
			break;
		}

	}
}

/////////////////////////////////////////////////////////////////////////

void ChipErase(void)
{
	ENABLE_SST25;
	SPI_Exchange(WRITE_EN);
	DISABLE_SST25;

	while(1)
	{
		if(CheckWriteEN()==1)		
		{
			break;
		}
	}

	ENABLE_SST25;
	SPI_Exchange(CHIP_ERASE);
	DISABLE_SST25;


	while(1)
	{
		if(CheckBusy()==0)
		{
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void SectorErase(uint8_t Add)
{

    ENABLE_SST25;
    SPI_Exchange(WRITE_EN);
    DISABLE_SST25;
    
    while(1)
	{
		if(CheckWriteEN()==1)		
		{
			break;
		}
	}

	ENABLE_SST25;
	SPI_Exchange(SECTOR_ERASE);
  	SPI_Exchange(((Add & 0xFFFFFF) >> 16));
	SPI_Exchange(((Add & 0x00FFFF) >> 8));
	SPI_Exchange(Add & 0x0000FF);
    DISABLE_SST25;

	while(1)
	{
		if(CheckBusy()==0)
		{
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void BlockErase(uint8_t Add, uint8_t block_type)
{

    ENABLE_SST25;
    SPI_Exchange(WRITE_EN);
    DISABLE_SST25;
    
    while(1)
	{
		if(CheckWriteEN()==1)		
		{
			break;
		}
	}

	ENABLE_SST25;
	SPI_Exchange(block_type);
  	SPI_Exchange(((Add & 0xFFFFFF) >> 16));
	SPI_Exchange(((Add & 0x00FFFF) >> 8));
	SPI_Exchange(Add & 0x0000FF);
    DISABLE_SST25;

	while(1)
	{
		if(CheckBusy()==0)
		{
			break;
		}
	}
}