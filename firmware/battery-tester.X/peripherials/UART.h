/* 
 * File:   UART.h
 * Author: RaD
 *
 * Created on 8 wrze?nia 2023, 13:33
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

void setupUART(void);
void UART_SendChar(char data);
void UART_SendString(const char *string);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

