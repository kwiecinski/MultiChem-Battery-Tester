/*
 * File:   main.c
 * Author: RaD
 *
 * Created on August 16, 2023, 11:01 AM
 */

#include <xc.h>
#include <stdio.h>
#include "LCD-KS0108/KS0108.h"
#include "peripherials/UART.h"
#include "main.h"
#include "settings.h"
#include "menu.h"
#include "button.h"
#include "LCD-KS0108/font5x8.h"
#include "interrupts.h"


void main(void) 
{   
    setupPorts();
    setupInterrupts();
    GLCD_Setup();
    setupUART();
    setupClock();
    setupPWM();
    Button_Init();
    Init_Timer0();
    LATAbits.LA6=1;
    mainMenu();
    uint8_t inc=0,i=0, inca=0;
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Overwrite, GLCD_Non_Inverted);
    GLCD_GotoXY(100, 0);
    GLCD_PrintInteger(inc);
    GLCD_Render();
    
    while (1) 
    { 
        
        if(button_counter>250)
        {
            button_counter=0;
                    
            i++;
            if(i>3)
            {
                i=0;
                GLCD_GotoXY(100, 0);
                GLCD_PrintInteger(inc++);
                GLCD_Render();
            }
        }
        
        
        
         
        Button_Update();
        /*
        if(Button_EventGet(OK_SW))
        {
             Button_Event_Reset();
            
            GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
            GLCD_GotoXY(110, 0);
            GLCD_PrintInteger(inca++);
            GLCD_Render();
             
        }
         * */
         
    }
}