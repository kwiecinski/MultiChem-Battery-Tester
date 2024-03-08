#include <xc.h>
#include <stdio.h>
#include "peripherials/UART.h"
#include "main.h"




#define SST25_CS_PIN	PIN_B5

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


#define ENABLE_SST25        LATFbits.LF7=0
#define DISABLE_SST25       LATFbits.LF7=1

void setSPI_Interface(void) 
{
    // SPI1 module
    TRISCbits.RC3 = 0;    //SST25_CLK
    TRISCbits.RC4 = 1;    //SST25_DI
    TRISCbits.RC5 = 0;    //SST25_DO
    TRISFbits.RF7 = 0;    //SST25_CS
    
    SSP1CON1bits.SSPEN = 1;
    SSP1CON1bits.SSPM1=1;
    //Mode 3 SPI
    SSP1CON1bits.CKP = 1; //1 = Idle state for clock is a high level
    SSP1STATbits.CKE = 0; //0 = Transmit occurs on transition from Idle to active clock state
    
}
uint8_t SPI_Transfer(uint8_t data) 
{
    SSP1BUF = data;              // Load data into the buffer
    while (!SSP1STATbits.BF);    // Wait for the transfer to complete
    return SSP1BUF;              // Return received data
}

void ReadID (void)
{
	ENABLE_SST25;

    uint8_t tab[2];
    
	SPI_Transfer(READ_ID);
	SPI_Transfer(0x00);
	SPI_Transfer(0x00);
	SPI_Transfer(READ_ID_ADD);
    tab[0]=SPI_Transfer(0xFF);
    tab[1]=SPI_Transfer(0xFF);

	printf("Manufacurer ID: %x \r\nDevice ID: %x \r\n", tab[0], tab[1]);
    
	DISABLE_SST25;
}

void ReadID_JEDEC(void)
{
	ENABLE_SST25;

    uint8_t tab[3];
    
	SPI_Transfer(JEDEC_READ_ID);
    tab[0]=SPI_Transfer(0xFF);
    tab[1]=SPI_Transfer(0xFF);
    tab[2]=SPI_Transfer(0xFF);
    
	printf("Manufacurer ID: %x \r\nDevice ID: %x \r\nMemory Capacity: %x \r\n", tab[0], tab[1], tab[2]);
    
	DISABLE_SST25;
}