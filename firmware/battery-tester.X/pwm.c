#include <xc.h>
#include <stdio.h>
#include "peripherials/UART.h"
#include "main.h"


void setPWMDutyCycle(unsigned int pwm_dutycycle) 
{
    uint8_t presc=1;
    
    float pwm_period = (PR2 + 1) * 4 * presc * (1 / _XTAL_FREQ);

    float pwm_dutycycle_period = (uint32_t)pwm_dutycycle * (1 / _XTAL_FREQ) * presc;

    uint16_t CCPR3L_DC3B_val = (uint16_t)((pwm_dutycycle_period * 100) / pwm_period);

    
    CCPR3L = (unsigned char)(CCPR3L_DC3B_val >> 2);
    CCP3CONbits.DC3B = (CCPR3L_DC3B_val & 0x03);   
}
