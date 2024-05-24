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


#include <stdio.h>

#define MAX_CALLS 4096

// Global variable to keep track of the number of function calls
static int call_count = 0;
void generate_values(int *voltage, int *current, int *temp) {
    if (call_count >= MAX_CALLS) {
        // If the function is called more than MAX_CALLS times, return the final values
        *voltage = 4500;
        *current = 0;
        *temp = 50;
        return;
    }

    // Calculate the step increments
    float voltage_step = (4500.0 - 2400.0) / (MAX_CALLS - 1);
    float current_step = 2000.0 / (MAX_CALLS - 1);
    float temp_step = (50.0 - 15.0) / (MAX_CALLS - 1);

    // Generate the values based on the current call count
    *voltage = 2400 + (int)(voltage_step * call_count);
    *current = 2000 - (int)(current_step * call_count);
    *temp = 15 + (int)(temp_step * call_count);

    // Increment the call count
    call_count++;
}



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
     memory_and_cycle_positions(&bat_param);
    
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
        
        
        /*
         * 
         * 
         *    int voltage, current, temp;
        for (int i = 0; i < MAX_CALLS; i++) {
        generate_values(&voltage, &current, &temp);
        printf("Call %d: Voltage = %d, Current = %d, Temp = %d\n", i, voltage, current, temp);
         }
         */
    

    }    
}