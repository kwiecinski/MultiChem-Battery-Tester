#include <xc.h>
#include <stdio.h>
#include "peripherials/UART.h"
#include "main.h"


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
uint8_t SPI_Exchange(uint8_t data) 
{
    SSP1BUF = data;              // Load data into the buffer
    while (!SSP1STATbits.BF);    // Wait for the transfer to complete
    return SSP1BUF;              // Return received data
}

