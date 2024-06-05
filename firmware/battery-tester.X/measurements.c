#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "peripherials/UART.h"
#include "main.h"
#include "settings.h"
#include "interrupts.h"
#include "SST25VF.h"
#include "MMSP.h"
#include "menu_definitions.h"
#include "memory.h"

#define MAX_CALLS 4096

// Global variable to keep track of the number of function calls
static int call_count = 0;
void generate_values(uint16_t *voltage, uint16_t *current, uint8_t *temp) 
{
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
    *voltage = 2400 + (uint16_t)(voltage_step * call_count);
    *current = 2000 - (uint16_t)(current_step * call_count);
    *temp = 15 + (uint8_t)(temp_step * call_count);

    // Increment the call count
    call_count++;
}


void measurements(BattParameters *bat_param)
{
    memory_data memory;
    
    printf("Start address before measurement: %x/%u  \n\r",check_measurment_next_addr(),check_measurment_next_addr());
    save_measurment_start_header_to_flash(bat_param, charging);
    save_measurment_data_to_flash(bat_param, &memory, INIT_LOG_MEASURMENT);
    for (int i = 0; i < 100; i++) 
    {
        generate_values(&(bat_param->batt_actual_voltage), &(bat_param->batt_actual_current), &bat_param->bat_actual_temp);
        save_measurment_data_to_flash(bat_param, &memory, LOG_MEASURMENT);
    }
    
    save_measurment_end_header_to_flash(bat_param);
    update_wear_leveling_static_buffer(UPDATE_MEASURMENT);
    
    printf("Start address after measurement: %x/%u  \n\r", check_measurment_next_addr(),check_measurment_next_addr());
    printf("Read data to serial \r\n");
    read_measurment_data_from_flash(bat_param);
}