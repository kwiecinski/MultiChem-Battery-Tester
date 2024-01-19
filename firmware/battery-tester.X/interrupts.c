
#include "interrupts.h"


volatile uint16_t button_counter;

// Function to initialize Timer0
void Init_Timer0() 
{
   
    T0CONbits.TMR0ON = 1;   //Enables Timer0 
    T0CONbits.T08BIT = 0;   //Timer0 is configured as an 16-bit timer/counter
    T0CONbits.T0CS = 0;     //Internal instruction cycle clock (CLKO)
    T0CONbits.PSA = 0;     // Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
    
    T0CONbits.T0PS = 0b010;  
    
    /* If internal clock is selected as Timer0 source
     * Timer0_clock = Fosc/4
     * 
     * T0CONbits.T0PS:
        111 = 1:256 Prescale value
        110 = 1:128 Prescale value
        101 = 1:64 Prescale value
        100 = 1:32 Prescale value
        011 = 1:16 Prescale value
        010 = 1:8 Prescale value
        001 = 1:4 Prescale value
        000 = 1:2 Prescale value
     * 
    */
    INTCONbits.TMR0IE = 1;  // Enables the TMR0 overflow interrupt     

    // Set the initial value for Timer0 (you may need to adjust this based on your requirements)
 }

// Interrupt Service Routine for Timer0 overflow
void __interrupt() ISR() 
{
    // Check if Timer0 overflowed
    if (INTCONbits.TMR0IF) 
    {
       //TMR0 overflow = Fosc/(4*prescaler*desired frequency)
        TMR0 = 0xFFFF-750;

        //Code to be executed evry desired time
        
        button_counter++;

        // Clear Timer0 overflow flag
        INTCONbits.TMR0IF = 0;
    }
}
