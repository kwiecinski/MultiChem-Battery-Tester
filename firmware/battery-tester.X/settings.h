#ifndef SETTINGS_H
#define	SETTINGS_H

#ifdef	__cplusplus
extern "C" {
#endif

void setupUART(void);
void setupClock(void);
void setupPorts(void);
void setupInterrupts(void);
void setupPWM(void);

void delay_ms(unsigned int milliseconds);

#define LED_BACKLIGHT_ON    LATAbits.LA6 = 1
#define DISPLAY_RESET_OFF   LATDbits.LD2 = 1;
 


#ifdef	__cplusplus
}
#endif

#endif	


