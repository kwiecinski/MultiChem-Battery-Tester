#include <xc.h>
#include <stdio.h>

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

void print_tab(uint8_t *tab, uint8_t lenght)
{
    for(uint8_t i=0; i<lenght; i++)
    {
        printf("%x | ",*(tab+i));
    }
}

void print_data_tab(uint8_t *tab, uint8_t lenght)
{
    for(uint8_t i=0; i<lenght; i++)
    {
        printf("%d: %d \r\n",i,*(tab+i));
    }
}