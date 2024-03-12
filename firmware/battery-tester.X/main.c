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
#include "menu_navigation.h"
#include "button.h"
#include "LCD-KS0108/font5x8.h"
#include "interrupts.h"
#include "SST25VF.h"
#include "MMSP.h"
#include "memory.h"


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
    setSPI_Interface();
    SST25VF_init_Enable_Write();
            
    BattParameters bat_param;
    InitBattParameters(&bat_param);
    SingleBat_Menu(&bat_param);
    
    uint8_t tab[4];
   
    
    while (1) 
    {
        Menu(&bat_param);
        
        if(counter_test>=2000)
        {
            printf("address %u\n\r", CheckCurrentParamOffset());
            SaveParamToFlash();
            counter_test=0;

        }
    }    
}