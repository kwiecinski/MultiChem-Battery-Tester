#include <xc.h>
#include <stdio.h>
#include "peripherials/UART.h"
#include "main.h"
#include "settings.h"
#include "interrupts.h"
#include "SST25VF.h"
#include "MMSP.h"
#include "memory.h"
#include "menu_definitions.h"

#define ADDRESS     0
#define LENGTH      1

#define PARAMETER_SECTOR_ADDR                       0x0
#define MEASUREMENT_SECTOR_ADDR                     0x1000

#define WEAR_LEVELING_PARAM_START_ADDR              0x0     
#define WEAR_LEVELING_PARAM_CYCLES                  27      // 1 Sector size (4096B) / Size of parameters table (144B)
#define WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH     4       // value is up rounded from WEAR_LEVELING_PARAM_CYCLES/8 
#define DATA_COPY_LENGTH                            64

#define PARAMETER_START_ADDR                        0x40    //64
#define PARAMETERS_DATA_SIZE                        144

#define WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR              0X10     // 0
#define WEAR_LEVELING_MEASUREMENT_DATA_CYCLES                  32      // 1 Sector size (4096B) / Size of parameters table (144B)
#define WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH     4       // value is up rounded from WEAR_LEVELING_PARAM_CYCLES/8 

#define MEASUREMENT_START_ADDR                       0x1000
#define MEASUREMENT_DATA_SIZE                                  16384

/********************************************************************************/

/*
 * @brief Copies, erases, and restores parameter sector.
 */
void parameter_sector_copy_erase_restore(void)
{
    uint8_t tab[DATA_COPY_LENGTH];
    read_bytes(PARAMETER_START_ADDR, &tab[0], DATA_COPY_LENGTH);
    for (uint8_t i = 0; i < WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH; ++i)
    {
        tab[WEAR_LEVELING_PARAM_START_ADDR + i] = 0xFF;
    }
    sector_erase(PARAMETER_SECTOR_ADDR);
    write_byte_table_auto_address_increment(PARAMETER_START_ADDR, &tab[0], DATA_COPY_LENGTH);
}

/*
 * @brief Checks the current offset position.
 * @return The calculated current offset position.
 */
uint16_t check_offset_position(void)
{
    uint8_t current_param_address[4], zero_bit_count;
    int8_t i, j;

    read_bytes(WEAR_LEVELING_PARAM_START_ADDR, &current_param_address[0], WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH);

    // Check how many zeros are in bytes are in Wear Leveling Status Buffer
    zero_bit_count = 0;
    for (i = 0; i < WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH; ++i) 
    {
        for (j = 7; j >= 0; --j) 
        {
            if (((current_param_address[i] >> j) & 0x01) == 0) 
            {
                zero_bit_count++;
            }
        }
    }
    return zero_bit_count; 
}

/*
 * @brief Checks the current parameter offset.
 * @return The calculated current parameter offset.
 */
uint16_t check_current_param_offset(void)
{ 
    return (check_offset_position() * PARAMETERS_DATA_SIZE + PARAMETERS_DATA_SIZE);
}

/*
 * @brief Saves parameter to flash memory.
 *
 * This function is responsible for saving parameters to flash memory. If the wear leveling
 * parameter cycles threshold is reached, it clears the memory and restores it. It then
 * searches for the first bit that is set to 1 (MSB) in the current parameter address array.
 * Once found, it changes that bit to 0 and writes the updated parameter to the flash memory.
 */
void update_wear_leveling_static_buffer(void)
{
    uint8_t current_param_address[4];
    int8_t i, j;

    if (check_offset_position() >= WEAR_LEVELING_PARAM_CYCLES)
    {
        parameter_sector_copy_erase_restore();
        printf("CLEAR MEMORY! \n\r");
        return;
    }
    read_bytes(WEAR_LEVELING_PARAM_START_ADDR, &current_param_address[0], WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH);

    for (i = 0; i < WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH; ++i)
    {
        // Checking for first bit that is 1 (MSB)
        for (j = 7; j >= 0; --j)
        {
            if (((current_param_address[i] >> j) & 0x01) == 1)
            {
                // Found bit 1, change it to 0
                current_param_address[i] &= ~(1 << j);
                write_byte(WEAR_LEVELING_PARAM_START_ADDR + i, current_param_address[i]);    // save new offset position to flash
                return; // End for after finding first 1 bit
            }
        }
    }
}

/*
 * @brief Represents value in binary.
 * @param value: The value to be represented.
 */
void represent_value_in_binary(uint8_t value)
{
    for (int8_t j = 7; j >= 0; j--) 
    {
        printf("%d", (value & (1 << j)) ? 1 : 0);
    }
}

/**
 * @brief Saves parameter data to a table.
 *
 * @param length: The length of the data.
 * @param data: The data to be saved.
 * @param parameter_position: The position of the parameter in the table.
 * @param param_tab: The table where the parameter data will be saved.
 */
void save_param_to_table(uint8_t length, uint8_t data, uint8_t *parameter_position, uint8_t *param_tab)
{
    for (uint8_t j = 0; j <= length; j++)
    {
        *(param_tab + *parameter_position + j) = (uint8_t)data >> (8 * j);
    }
}  

/*
 * @brief Saves parameters to flash.
 * @param bat_param: The battery parameters to be saved.
 */
void save_parameters_to_flash(BattParameters *bat_param)
{
    uint8_t param_tab[144], parameter_position;

        
    parameter_position = 0;
    save_param_to_table(bat_param->batt_capacitance_cycle1, sizeof(bat_param->batt_capacitance_cycle1), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->batt_capacitance_cycle2, sizeof(bat_param->batt_capacitance_cycle2), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->batt_capacitance_cycle3, sizeof(bat_param->batt_capacitance_cycle3), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->batt_capacitance_cycle4, sizeof(bat_param->batt_capacitance_cycle4), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->bat_chem, sizeof(bat_param->bat_chem), &parameter_position, &param_tab[0]);

    
    
    for(uint8_t i=0; i<3;i++)
    {
        switch(i)
        {
            case 1:  bat_param->settings_ptr = bat_param->liion_settings_ptr; break;
            case 2:  bat_param->settings_ptr = bat_param->pb_settings_ptr; break;
            case 3:  bat_param->settings_ptr = bat_param->nimh_settings_ptr; break;
        }
        
        
        save_param_to_table(bat_param->settings_ptr->cell_count, sizeof(bat_param->settings_ptr->cell_count), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->selected_mode, sizeof(bat_param->settings_ptr->selected_mode), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->set_cycle, sizeof(bat_param->settings_ptr->set_cycle), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->batt_set_voltage, sizeof(bat_param->settings_ptr->batt_set_voltage), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->batt_set_min_discharge_voltage, sizeof(bat_param->settings_ptr->batt_set_min_discharge_voltage), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->batt_set_trickle_voltage, sizeof(bat_param->settings_ptr->batt_set_trickle_voltage), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->batt_set_trickle_current, sizeof(bat_param->settings_ptr->batt_set_trickle_current), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->set_max_time, sizeof(bat_param->settings_ptr->set_max_time), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->batt_max_temp, sizeof(bat_param->settings_ptr->batt_max_temp), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->charge_current_1, sizeof(bat_param->settings_ptr->charge_current_1), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->discharge_current_1, sizeof(bat_param->settings_ptr->discharge_current_1), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->charge_current_2, sizeof(bat_param->settings_ptr->charge_current_2), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->charge_current_2_percent, sizeof(bat_param->settings_ptr->charge_current_2_percent), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->charge_current_3, sizeof(bat_param->settings_ptr->charge_current_3), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->charge_current_3_percent, sizeof(bat_param->settings_ptr->charge_current_3_percent), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->charge_current_4, sizeof(bat_param->settings_ptr->charge_current_4), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->charge_current_4_percent, sizeof(bat_param->settings_ptr->charge_current_4_percent), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->discharge_current_2, sizeof(bat_param->settings_ptr->discharge_current_2), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->discharge_current_2_percent, sizeof(bat_param->settings_ptr->discharge_current_2_percent), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->discharge_current_3, sizeof(bat_param->settings_ptr->discharge_current_3), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->discharge_current_3_percent, sizeof(bat_param->settings_ptr->discharge_current_3_percent), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->discharge_current_4, sizeof(bat_param->settings_ptr->discharge_current_4), &parameter_position, &param_tab[0]);
        save_param_to_table(bat_param->settings_ptr->discharge_current_4_percent, sizeof(bat_param->settings_ptr->discharge_current_4_percent), &parameter_position, &param_tab[0]);
    }
  
    write_byte_table_auto_address_increment(check_current_param_offset(), &param_tab[0], parameter_position);
}


/*
 
   save_param_to_table(bat_param->pb_settings->cell_count, sizeof(bat_param->pb_settings->cell_count), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->selected_mode, sizeof(bat_param->pb_settings->selected_mode), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->set_cycle, sizeof(bat_param->pb_settings->set_cycle), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->batt_set_voltage, sizeof(bat_param->pb_settings->batt_set_voltage), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->batt_set_min_discharge_voltage, sizeof(bat_param->pb_settings->batt_set_min_discharge_voltage), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->batt_set_trickle_voltage, sizeof(bat_param->pb_settings->batt_set_trickle_voltage), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->batt_set_trickle_current, sizeof(bat_param->pb_settings->batt_set_trickle_current), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->set_max_time, sizeof(bat_param->pb_settings->set_max_time), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->batt_max_temp, sizeof(bat_param->pb_settings->batt_max_temp), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->charge_current_1, sizeof(bat_param->pb_settings->charge_current_1), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->discharge_current_1, sizeof(bat_param->pb_settings->discharge_current_1), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->charge_current_2, sizeof(bat_param->pb_settings->charge_current_2), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->charge_current_2_percent, sizeof(bat_param->pb_settings->charge_current_2_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->charge_current_3, sizeof(bat_param->pb_settings->charge_current_3), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->charge_current_3_percent, sizeof(bat_param->pb_settings->charge_current_3_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->charge_current_4, sizeof(bat_param->pb_settings->charge_current_4), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->charge_current_4_percent, sizeof(bat_param->pb_settings->charge_current_4_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->discharge_current_2, sizeof(bat_param->pb_settings->discharge_current_2), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->discharge_current_2_percent, sizeof(bat_param->pb_settings->discharge_current_2_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->discharge_current_3, sizeof(bat_param->pb_settings->discharge_current_3), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->discharge_current_3_percent, sizeof(bat_param->pb_settings->discharge_current_3_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->discharge_current_4, sizeof(bat_param->pb_settings->discharge_current_4), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->pb_settings->discharge_current_4_percent, sizeof(bat_param->pb_settings->discharge_current_4_percent), &parameter_position, &param_tab[0]);

    save_param_to_table(bat_param->nimh_settings->cell_count, sizeof(bat_param->nimh_settings->cell_count), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->selected_mode, sizeof(bat_param->nimh_settings->selected_mode), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->set_cycle, sizeof(bat_param->nimh_settings->set_cycle), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->batt_set_voltage, sizeof(bat_param->nimh_settings->batt_set_voltage), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->batt_set_min_discharge_voltage, sizeof(bat_param->nimh_settings->batt_set_min_discharge_voltage), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->batt_set_trickle_voltage, sizeof(bat_param->nimh_settings->batt_set_trickle_voltage), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->batt_set_trickle_current, sizeof(bat_param->nimh_settings->batt_set_trickle_current), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->set_max_time, sizeof(bat_param->nimh_settings->set_max_time), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->batt_max_temp, sizeof(bat_param->nimh_settings->batt_max_temp), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->charge_current_1, sizeof(bat_param->nimh_settings->charge_current_1), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->discharge_current_1, sizeof(bat_param->nimh_settings->discharge_current_1), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->charge_current_2, sizeof(bat_param->nimh_settings->charge_current_2), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->charge_current_2_percent, sizeof(bat_param->nimh_settings->charge_current_2_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->charge_current_3, sizeof(bat_param->nimh_settings->charge_current_3), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->charge_current_3_percent, sizeof(bat_param->nimh_settings->charge_current_3_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->charge_current_4, sizeof(bat_param->nimh_settings->charge_current_4), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->charge_current_4_percent, sizeof(bat_param->nimh_settings->charge_current_4_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->discharge_current_2, sizeof(bat_param->nimh_settings->discharge_current_2), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->discharge_current_2_percent, sizeof(bat_param->nimh_settings->discharge_current_2_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->discharge_current_3, sizeof(bat_param->nimh_settings->discharge_current_3), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->discharge_current_3_percent, sizeof(bat_param->nimh_settings->discharge_current_3_percent), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->discharge_current_4, sizeof(bat_param->nimh_settings->discharge_current_4), &parameter_position, &param_tab[0]);
    save_param_to_table(bat_param->nimh_settings->discharge_current_4_percent, sizeof(bat_param->nimh_settings->discharge_current_4_percent), &parameter_position, &param_tab[0]);

 */