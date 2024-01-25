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
#include "menu_definitions.h"
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
    OneBatMenu();
    uint8_t menu_change=0;
    while (1) 
    {
        Button_Update();
        if (Button_EventGet(OK_SW)) 
        {
            Button_Event_Reset();


            if (menu_change) 
            {
                GLCD_FillRectangle(0, 51, 32, 63, GLCD_Black);
                GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
                GLCD_GotoXY(2, 54);
                GLCD_PrintString("START");

                GLCD_FillRectangle(35, 51, 61, 63, GLCD_White);
                GLCD_DrawRectangle(35, 51, 61, 63, GLCD_Black);
                GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
                GLCD_GotoXY(37, 54);
                GLCD_PrintString("MENU");

                GLCD_Render();
                menu_change = 0;
            } else 
            {

                menu_change = 1;

                GLCD_FillRectangle(0, 51, 32, 63, GLCD_White);
                GLCD_DrawRectangle(0, 51, 32, 63, GLCD_Black);
                GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
                GLCD_GotoXY(2, 54);
                GLCD_PrintString("START");


                GLCD_FillRectangle(35, 51, 61, 63, GLCD_Black);
                GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
                GLCD_GotoXY(37, 54);
                GLCD_PrintString("MENU");
                GLCD_Render();
            }  
        }     
    }
}