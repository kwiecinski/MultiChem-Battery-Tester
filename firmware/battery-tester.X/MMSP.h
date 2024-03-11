/* 
 * File:   pwm.h
 * Author: RaD
 *
 * Created on 30 listopada 2023, 09:16
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

void setSPI_Interface(void);
uint8_t SPI_Exchange(uint8_t data);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

