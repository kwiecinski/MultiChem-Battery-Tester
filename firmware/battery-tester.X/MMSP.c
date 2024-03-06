#include <xc.h>
#include <stdio.h>
#include "peripherials/UART.h"
#include "main.h"


void setI2C_Interface(void) 
{
      TRISBbits.RB0 = 0;    //SDA
      TRISBbits.RB1 = 0;    //SDCL
}
