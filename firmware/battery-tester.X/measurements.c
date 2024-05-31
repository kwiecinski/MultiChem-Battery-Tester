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
void generate_values(int *voltage, int *current, int *temp) 
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
    *voltage = 2400 + (int)(voltage_step * call_count);
    *current = 2000 - (int)(current_step * call_count);
    *temp = 15 + (int)(temp_step * call_count);

    // Increment the call count
    call_count++;
}


void measurements(void)
{
    int voltage, current, temp;
    for (int i = 0; i < MAX_CALLS; i++) 
    {
        generate_values(&voltage, &current, &temp);
        //printf("Call %d: Voltage = %d, Current = %d, Temp = %d\n", i, voltage, current, temp);
    } 
}