#include <xc.h>


void UART_SendChar(char data) 
{
    while (!TXSTA2bits.TRMT);
    TXREG2 = data;
}

void putch(char txData)
{    
    UART_SendChar(txData);
}

void UART_SendString(const char *string) 
{
    while (*string) 
    {
        UART_SendChar(*string++);
    }
}
