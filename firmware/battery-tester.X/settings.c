    
#include <xc.h>
#include <stdio.h>
#include "menu_definitions.h"
#include "memory.h"
#include "main.h"
#include "settings.h"

void init_settings_ptr(BattParameters *bat_param)
{
     if(bat_param->bat_chem==liion)
    {   
        bat_param->settings_ptr = bat_param->liion_settings_ptr;   
    
    }else if(bat_param->bat_chem==pb)
    {
        bat_param->settings_ptr = bat_param->pb_settings_ptr;
         
    }else if(bat_param->bat_chem==nimh)
    {
        bat_param->settings_ptr = bat_param->nimh_settings_ptr;
    }
}

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
    TRISDbits.RD2 = 0;  //LCD RST PIN
    TRISAbits.RA6 = 0;  //LCD backlight pin output
    LATAbits.LA6 = 1;   //LCD backlight on
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


