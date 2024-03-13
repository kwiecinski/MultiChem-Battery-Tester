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
    
    uint8_t taba[20],tab[20]={5,8,5,96,74,236,144,178,196,32,1,58,66,23,55,44,88,99,66,33};
    WriteByteTable_AutoAddressIncrement(32,&tab[0],20);
    ReadBytes(32,&taba[0],20);
            
    while (1) 
    {
        Menu(&bat_param);
        
        if(counter_test>=10000)
        {
            
        for (int i = 0; i < 20; ++i) 
        {
            printf("%d ", taba[i]);
        }
            counter_test=0;
        }
    }    
}