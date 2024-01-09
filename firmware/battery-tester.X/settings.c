    
#include <xc.h>
#include <stdio.h>
#include "main.h"


void setupUART(void)
{
    TRISGbits.TRISG1 = 0;   // Set RG0 as output
    SPBRG2 = 155;
    TXSTA2bits.BRGH = 1;
    TXSTA2bits.TXEN = 1;
    RCSTA2bits.SPEN = 1;
    TXSTA2bits.SYNC = 0;     
}


void setupClock(void)
{
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS0 = 0;
    OSCCONbits.SCS1 = 0;
    OSCTUNEbits.PLLEN = 1;
}

void setupPorts(void)
{
    TRISDbits.RD2 = 0;
    TRISAbits.RA6 = 0;  
}

void setupInterrupts(void)
{

    INTCONbits.GIE = 1;     // Global Interrupt Enable bit
    INTCONbits.PEIE = 1;    //Peripheral Interrupt Enable bit
}
 
void setupPWM(void) 
{     
    TRISGbits.TRISG0 = 0;   // Set RG0 as output
    T2CONbits.TMR2ON = 1;
    CCP3CONbits.CCP3M0 = 0;
    CCP3CONbits.CCP3M1 = 0;
    CCP3CONbits.CCP3M2 = 1;
    CCP3CONbits.CCP3M3 = 1; 
    PR2=200;
}

void delay_ms(unsigned int milliseconds)
{
    unsigned int i;
    for (i = 0; i < milliseconds; i++) 
    {
        __delay_ms(1); 
    }
}


