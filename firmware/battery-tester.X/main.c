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

void init_settings_ptr(BattParameters *bat_param)
{
     if(bat_param->bat_chem==liion)
    {   
        bat_param->settings_ptr = bat_param->liion_settings_ptr;   
    
    }else if(bat_param->bat_chem==pb)
    {
        bat_param->settings_ptr = bat_param->pb_settings_ptr;
         
    }else if(bat_param->bat_chem==nimh)
    {
        bat_param->settings_ptr = bat_param->nimh_settings_ptr;
    }
}

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
    sst25vf_init_enable_write();
            
    BattParameters bat_param;
    BattTypeSettings nimh_settings;
    BattTypeSettings pb_settings;
    BattTypeSettings liion_settings;
    bat_param.liion_settings_ptr=&liion_settings;
    bat_param.pb_settings_ptr=&pb_settings;
    bat_param.nimh_settings_ptr=&nimh_settings;
    
    InitBattParameters(&bat_param);
  
    //bat_param.bat_chem = liion;
    //bat_param.settings_ptr = bat_param.liion_settings_ptr;
 
    //save_parameters_to_flash(&bat_param);
    read_parameters_from_flash(&bat_param);
    
 
    init_settings_ptr(&bat_param);
    
    switch_between_battery_types(&bat_param, INITILIZE_SETTINGS);
    
    
    SingleBat_Menu(&bat_param);
    while (1) 
    {
        Menu(&bat_param);
        
        if(counter_test>=10000)
        {
            printf("alive \r\n");
            counter_test=0;
        }
        Button_Update();
        
          if (BTN_STATE_LONG == Button_EventGet(OK_SW)) 
        {
            printf("CHIP ERASE! \r\n");
            chip_erase();
            delay_ms(5000);
        } 
          
        if (BTN_STATE_LONG == Button_EventGet(UP_SW)) 
        {
            InitBattParameters(&bat_param);
            save_parameters_to_flash(&bat_param);
            read_parameters_from_flash(&bat_param);
            printf("INIT PARAM DONE! \r\n");
            delay_ms(5000);
        } 
        
        

    }    
}