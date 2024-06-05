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

#define ADDRESS     0
#define LENGTH      1



// ERASE PARAMETERS-------------------------------------------------------------
#define PARAMETER_4KB_SECTOR_ERASE_ADDRESS_1        0x0
#define PARAMETER_4KB_SECTOR_ERASE_ADDRESS_2        0x1000
#define MEASUREMENT_64KB_BLOCK_ERASE_ADDRESS        0xFFFF
#define MEASUREMENT_64KB_BLOCK_ERASE_COUNT          7
#define MEASUREMENT_32KB_BLOCK_ERASE_ADDRESS        0x8000
#define MEASUREMENT_4KB_SECTOR_ERASE_ADDRESS        0x3000
#define MEASUREMENT_4KB_BLOCK_ERASE_COUNT           6
// -----------------------------------------------------------------------------

// WEAR LEVELING FOR PARAMETERS-------------------------------------------------
#define WEAR_LEVELING_PARAM_START_ADDR              0x0     
#define WEAR_LEVELING_PARAM_CYCLES                  27      // 1 Sector size (4096B) / Size of parameters table (144B)
#define WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH     4       // value is up rounded from WEAR_LEVELING_PARAM_CYCLES/8 
#define PARAMETER_START_ADDR                        0x10    //64
#define PARAMETERS_DATA_SIZE                        144
// -----------------------------------------------------------------------------

// WEAR LEVELING FOR MEASUREMENT------------------------------------------------
#define WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR              0x4000    // 
#define WEAR_LEVELING_MEASUREMENT_DATA_CYCLES                  31        // 1 Sector size (4096B) / Size of parameters table (144B)
#define WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH     4         // value is up rounded from WEAR_LEVELING_PARAM_CYCLES/8 
#define MEASURMENT_START_ADDR                                  0x4000
#define TEMP_START_ADDR                                        0x2007
#define TEMP_DATA_SIZE                                         264
#define MEASURMENT_DATA_SIZE                                   4096
// -----------------------------------------------------------------------------

// BYTE POSITIONS IN FLASH-----------------------------------------------------
#define BAT_ID              0
#define SAMPLING_TIME       1
#define DONE_CYCLE_TYPE     2
#define CAPACITANCE_MSB_POS 3
#define CAPACITANCE_LSB_POS 4
#define SET_CURRENT_MSB_POS 5
#define SET_CURRENT_LSB_POS 6
#define SET_VOLTAGE_MSB_POS 7
#define SET_VOLTAGE_LSB_POS 8
#define MAX_TEMP            9

// BIT POSITIONS OF DONE_CYCLE BYTE---------------------------------------------
#define CYCLE_TYPE_CHARGING_BIT_POS     0
#define CYCLE_TYPE_DISCHARGING_BIT_POS  1
#define COMPLETE_CYCLE_BIT_POS          2


uint8_t check_wear_leveling_buffer_value(uint16_t addr, uint8_t lenght);
uint16_t check_measurment_next_addr(void);
uint16_t check_temperature_next_addr(void);


/*
* 0               bat id	
* 1               sampling time	
* 2             whole cycle flag, cycle type (chg, disc)	
* 3,4            measured capacitance (2bytes)	
* 5,6            set current (2bytes)	
* 7,8            set voltage(2bytes)	
* 9               max temp	
* 10 to 4095 	  measurment data	
*/

uint8_t calculate_measurment_sampling_time (BattParameters *bat_param)
{
    return (bat_param->set_max_time/100*60+bat_param->set_max_time%100)*60/(MEASURMENT_DATA_SIZE/2);  
}

uint8_t calculate_temp_sampling_time (BattParameters *bat_param)
{ 
    return (bat_param->set_max_time/100*60+bat_param->set_max_time%100)*60/TEMP_DATA_SIZE;   
}

void save_measurment_start_header_to_flash(BattParameters *bat_param, uint8_t charger_state)
{
    uint8_t param_tab[10];
    memset(&param_tab[0], 0xFF, sizeof(param_tab));
    
    param_tab[BAT_ID] = bat_param->bat_id;
    param_tab[SAMPLING_TIME] = calculate_measurment_sampling_time(bat_param);
   
    if(charger_state == state_charging)
    {
        param_tab[DONE_CYCLE_TYPE] &= ~(1 << CYCLE_TYPE_CHARGING_BIT_POS);           
    }else if(charger_state == state_discharging)
    {
        param_tab[DONE_CYCLE_TYPE] &= ~(1 << CYCLE_TYPE_DISCHARGING_BIT_POS); 
    }
    
    param_tab[SET_CURRENT_MSB_POS] = (uint8_t)(bat_param->batt_set_current >> 8) & 0xFF; 
    param_tab[SET_CURRENT_LSB_POS] = (uint8_t)(bat_param->batt_set_current & 0xFF); 
    param_tab[SET_VOLTAGE_MSB_POS] = (uint8_t)(bat_param->batt_set_voltage >> 8) & 0xFF; 
    param_tab[SET_VOLTAGE_LSB_POS] = (uint8_t)(bat_param->batt_set_voltage & 0xFF);
    write_byte_table_auto_address_increment(check_measurment_next_addr(), &param_tab[0], sizeof(param_tab));
}

void save_measurment_end_header_to_flash(BattParameters *bat_param)
{
     uint8_t param_tab[10];
     memset(&param_tab[0], 0xFF, sizeof(param_tab));  
     
     param_tab[DONE_CYCLE_TYPE] &= ~(1 << COMPLETE_CYCLE_BIT_POS); 
     if(bat_param->current_cycle == 1)
     {
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle1 >> 8) & 0xFF; 
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle1 & 0xFF); 
     }else if(bat_param->current_cycle == 2)
     {
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle2 >> 8) & 0xFF; 
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle2 & 0xFF); 
     }if(bat_param->current_cycle == 3)
     {
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle3 >> 8) & 0xFF; 
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle3 & 0xFF); 
     }if(bat_param->current_cycle == 4)
     {
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle4 >> 8) & 0xFF; 
        param_tab[CAPACITANCE_MSB_POS] = (uint8_t)(bat_param->batt_capacitance_cycle4 & 0xFF); 
     }
     
     param_tab[MAX_TEMP] = bat_param->bat_actual_max_temp;
     write_byte_table_auto_address_increment(check_measurment_next_addr(), &param_tab[0], sizeof(param_tab));
}

void save_measurment_data_to_flash(BattParameters *bat_param, memory_data *memory, uint8_t init)
{
    if(init == INIT_LOG_MEASURMENT)
    {
          memory->measurment_sampling_time = calculate_measurment_sampling_time(bat_param);
          memory->temp_sampling_time = calculate_temp_sampling_time(bat_param);
          memory->memory_offset_temp = check_temperature_next_addr();
          memory->memory_offset_data = check_measurment_next_addr();
          memset(&memory->measured_data[0], 0xFF, sizeof(memory->measured_data));  
          memory->data_pos=0;
          memory->temp_pos=0;
    }else if(init == LOG_MEASURMENT)
    {
        //if(measurement_data_sample_timer >= memory->measurment_sampling_time*1000)  
        if(1)
        {
            memory->measured_data[memory->data_pos] = (uint8_t)(bat_param->batt_actual_voltage >> 8) & 0xFF; 
            memory->measured_data[memory->data_pos+1] = (uint8_t)(bat_param->batt_actual_voltage & 0xFF); 
            memory->measured_data[memory->data_pos+2] = (uint8_t)(bat_param->batt_actual_current >> 8) & 0xFF; 
            memory->measured_data[memory->data_pos+3] = (uint8_t)(bat_param->batt_actual_current & 0xFF);
            
          
            printf("Write parameter address: %lu  \n\r", (memory->memory_offset_data + memory->data_pos));
            write_byte_table_auto_address_increment(memory->memory_offset_data+memory->data_pos, &(memory->measured_data[0]), sizeof(memory->measured_data));
            memory->data_pos = 0;
            memset(&(memory->measured_data[0]), 0xFF, sizeof(memory->measured_data));

            memory->data_pos = memory->data_pos + 4;  
            
            

        }
        if(temp_data_sample_timer >= memory->temp_sampling_time*1000)  
        {
            write_byte(memory->memory_offset_temp + memory->temp_pos,bat_param->bat_actual_temp);
            memory->temp_pos++;
        }
    } 
}


void memory_and_cycle_positions(BattParameters *bat_param)
{
    uint8_t param_tab[1];
    uint32_t offset;
    
    
    bat_param->max_memory_cycle = WEAR_LEVELING_MEASUREMENT_DATA_CYCLES;
    bat_param->current_measurment_cycle = check_wear_leveling_buffer_value(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH); 
    
    offset = check_measurment_next_addr();
    if(offset == MEASURMENT_START_ADDR)
    {
        bat_param->bat_id = 1;
    }else
    {
         //to read bat_id need to read heder of previous cycle so i need to do offset-MEASURMENT_DATA_SIZE
         read_bytes(offset-MEASURMENT_DATA_SIZE, &param_tab[0], sizeof(param_tab));        
         bat_param->bat_id = param_tab[BAT_ID]+1;     
    }
}

void represent_value_in_binary(uint8_t value);
/********************************************************************************/

/*
 * @brief Copies, erases, and restores parameter sector.
 */
void parameter_erase(void)
{
    sector_erase(PARAMETER_4KB_SECTOR_ERASE_ADDRESS_1);
    sector_erase(PARAMETER_4KB_SECTOR_ERASE_ADDRESS_2);
}

void measurment_erase(void)
{
    uint8_t i;
    
    for(i=0; i<MEASUREMENT_64KB_BLOCK_ERASE_COUNT;i++)
    {
        block_erase((MEASUREMENT_64KB_BLOCK_ERASE_ADDRESS + 0xFFFF*i),BLOCK_ERASE_64KB);
    }
    
    block_erase(MEASUREMENT_32KB_BLOCK_ERASE_ADDRESS,BLOCK_ERASE_32KB);
    
    for(i=0; i<MEASUREMENT_4KB_BLOCK_ERASE_COUNT;i++)
    {
        sector_erase(MEASUREMENT_4KB_SECTOR_ERASE_ADDRESS + 0x1000*i);
    }
}

/*
 * @brief Checks the current offset position.
 * @return The calculated current offset position.
 */
uint8_t check_wear_leveling_buffer_value(uint16_t addr, uint8_t lenght)
{
    uint8_t current_param_address[4], zero_bit_count;
    int8_t i, j;

    read_bytes(addr, &current_param_address[0], lenght);

    // Check how many zeros are in bytes are in Wear Leveling Status Buffer
    zero_bit_count = 0;
    for (i = 0; i < lenght; ++i) 
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
uint16_t check_param_next_addr(void)
{ 
    return (check_wear_leveling_buffer_value(WEAR_LEVELING_PARAM_START_ADDR, WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH) * PARAMETERS_DATA_SIZE + PARAMETER_START_ADDR);
}


uint16_t check_measurment_next_addr(void)
{ 
    return (check_wear_leveling_buffer_value(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH) * MEASURMENT_DATA_SIZE + MEASURMENT_START_ADDR);
}

uint16_t check_temperature_next_addr(void)
{ 
    return (check_wear_leveling_buffer_value(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH) * TEMP_DATA_SIZE + TEMP_START_ADDR);
}
/*
 * @brief Saves parameter to flash memory.
 *
 * This function is responsible for saving parameters to flash memory. If the wear leveling
 * parameter cycles threshold is reached, it clears the memory and restores it. It then
 * searches for the first bit that is set to 1 (MSB) in the current parameter address array.
 * Once found, it changes that bit to 0 and writes the updated parameter to the flash memory.
 */
void update_wear_leveling_static_buffer(uint8_t wear_leveling_type)
{
    uint8_t current_param_address[4],lenght;
    int8_t i, j;
    uint32_t addr;

    if(wear_leveling_type == UPDATE_PARAMETERS)
    {
        addr = WEAR_LEVELING_PARAM_START_ADDR;
        lenght = WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH;
        if (check_wear_leveling_buffer_value(WEAR_LEVELING_PARAM_START_ADDR, WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH) >= WEAR_LEVELING_PARAM_CYCLES)
        {
            parameter_erase();
            printf("CLEAR PARAMETER MEMORY! \n\r");
            return;
        }
    }else if (wear_leveling_type == UPDATE_MEASURMENT)
    {
        
        addr = WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR;
        lenght = WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH;
        
        if (check_wear_leveling_buffer_value(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH) >= WEAR_LEVELING_MEASUREMENT_DATA_CYCLES)
        {
            measurment_erase();
            printf("CLEAR MEASURMENT MEMORY! \n\r");
            return;
        }
    }
    
    
    read_bytes(addr, &current_param_address[0], lenght);
    
    printf("wear leveling buffer status before update: \r\n");
    print_tab(&current_param_address[0],sizeof(current_param_address));
    printf("\r\n");
    
    for(i=0; i<sizeof(current_param_address); i++)
    {
        represent_value_in_binary(current_param_address[i]);
    }
    
    for (i = 0; i < lenght; ++i)
    {
        // Checking for first bit that is 1 (MSB)
        for (j = 7; j >= 0; --j)
        {
            
            if (((current_param_address[i] >> j) & 0x01) == 1)
            {
                // Found bit 1, change it to 0
                current_param_address[i] &= ~(1 << j);
                
                printf("wear leveling buffer status after update: \r\n");
                print_tab(&current_param_address[0],sizeof(current_param_address));
                
                write_byte(addr + (uint32_t)i, current_param_address[i]);    // save new offset position to flash
              
                 
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
     printf(" ");
}

/**
 * @brief Saves parameter data to a table.
 *
 * @param length: The length of the data.
 * @param data: The data to be saved.
 * @param parameter_position: The position of the parameter in the table.
 * @param param_tab: The table where the parameter data will be saved.
 */
void save_parameters_to_table(uint16_t data, uint8_t length, uint8_t *parameter_position, uint8_t *param_tab)
{
    for (uint8_t j = 0; j < length; j++)
    {
        *(param_tab + *parameter_position + j) = (unsigned char)((data >> (8 * (length - 1 - j))) & 0xFF);
    }
    *parameter_position = *parameter_position + length;
}  

 uint16_t save_table_to_parameters(uint8_t length, uint8_t *parameter_position, uint8_t *param_tab)
{
    uint16_t data=0;
    
    for (uint8_t j = 0; j < length; j++)
    {
          data |= ((uint16_t)*(param_tab + *parameter_position + j)) << (8 * (length - 1 - j));
          
    }
    *parameter_position = *parameter_position + length;
      
    return data;
} 


/*
 * @brief Saves parameters to flash.
 * @param bat_param: The battery parameters to be saved.
 */
void save_parameters_to_flash(BattParameters *bat_param)
{
    uint8_t param_tab[144], parameter_position;

    printf("Saving parameters to flash. Copying data to table \n\r");
    
    switch_between_battery_types(bat_param, 0);
    
    parameter_position = 0;
    save_parameters_to_table(bat_param->batt_capacitance_cycle1, sizeof(bat_param->batt_capacitance_cycle1), &parameter_position, &param_tab[0]);
    save_parameters_to_table(bat_param->batt_capacitance_cycle2, sizeof(bat_param->batt_capacitance_cycle2), &parameter_position, &param_tab[0]);
    save_parameters_to_table(bat_param->batt_capacitance_cycle3, sizeof(bat_param->batt_capacitance_cycle3), &parameter_position, &param_tab[0]);
    save_parameters_to_table(bat_param->batt_capacitance_cycle4, sizeof(bat_param->batt_capacitance_cycle4), &parameter_position, &param_tab[0]);
    save_parameters_to_table(bat_param->bat_chem, sizeof(bat_param->bat_chem), &parameter_position, &param_tab[0]);
    
    for(uint8_t i=0; i<3;i++)
    {
        switch(i)
        {
            case 1:  bat_param->settings_ptr = bat_param->liion_settings_ptr; break;
            case 2:  bat_param->settings_ptr = bat_param->pb_settings_ptr; break;
            case 3:  bat_param->settings_ptr = bat_param->nimh_settings_ptr; break;
        }

        save_parameters_to_table(bat_param->settings_ptr->cell_count, sizeof(bat_param->settings_ptr->cell_count), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->selected_mode, sizeof(bat_param->settings_ptr->selected_mode), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->set_cycle, sizeof(bat_param->settings_ptr->set_cycle), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->batt_set_voltage, sizeof(bat_param->settings_ptr->batt_set_voltage), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->batt_set_min_discharge_voltage, sizeof(bat_param->settings_ptr->batt_set_min_discharge_voltage), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->batt_set_trickle_voltage, sizeof(bat_param->settings_ptr->batt_set_trickle_voltage), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->batt_set_trickle_current, sizeof(bat_param->settings_ptr->batt_set_trickle_current), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->set_max_time, sizeof(bat_param->settings_ptr->set_max_time), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->batt_max_temp, sizeof(bat_param->settings_ptr->batt_max_temp), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->charge_current_1, sizeof(bat_param->settings_ptr->charge_current_1), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->discharge_current_1, sizeof(bat_param->settings_ptr->discharge_current_1), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->charge_current_2, sizeof(bat_param->settings_ptr->charge_current_2), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->charge_current_2_percent, sizeof(bat_param->settings_ptr->charge_current_2_percent), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->charge_current_3, sizeof(bat_param->settings_ptr->charge_current_3), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->charge_current_3_percent, sizeof(bat_param->settings_ptr->charge_current_3_percent), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->charge_current_4, sizeof(bat_param->settings_ptr->charge_current_4), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->charge_current_4_percent, sizeof(bat_param->settings_ptr->charge_current_4_percent), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->discharge_current_2, sizeof(bat_param->settings_ptr->discharge_current_2), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->discharge_current_2_percent, sizeof(bat_param->settings_ptr->discharge_current_2_percent), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->discharge_current_3, sizeof(bat_param->settings_ptr->discharge_current_3), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->discharge_current_3_percent, sizeof(bat_param->settings_ptr->discharge_current_3_percent), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->discharge_current_4, sizeof(bat_param->settings_ptr->discharge_current_4), &parameter_position, &param_tab[0]);
        save_parameters_to_table(bat_param->settings_ptr->discharge_current_4_percent, sizeof(bat_param->settings_ptr->discharge_current_4_percent), &parameter_position, &param_tab[0]);
    }
    
    printf("Write table: \n\r");
    print_data_tab(&param_tab[0], parameter_position);
    
    update_wear_leveling_static_buffer(UPDATE_PARAMETERS);
    printf("Write to flash, parameter address: %u  \n\r", check_param_next_addr());
    write_byte_table_auto_address_increment(check_param_next_addr(), &param_tab[0], parameter_position);
  
}


void read_parameters_from_flash(BattParameters *bat_param)
{
    uint8_t param_tab[144], parameter_position;

    read_bytes(check_param_next_addr(), &param_tab[0], sizeof(param_tab));

    printf("Parameter addr read:%u \r\n", check_param_next_addr());
    
    parameter_position = 0;
    bat_param->batt_capacitance_cycle1 = save_table_to_parameters(sizeof(bat_param->batt_capacitance_cycle1), &parameter_position, &param_tab[0]);
    bat_param->batt_capacitance_cycle2 = save_table_to_parameters(sizeof(bat_param->batt_capacitance_cycle2), &parameter_position, &param_tab[0]);
    bat_param->batt_capacitance_cycle3 = save_table_to_parameters(sizeof(bat_param->batt_capacitance_cycle3), &parameter_position, &param_tab[0]);
    bat_param->batt_capacitance_cycle4 = save_table_to_parameters(sizeof(bat_param->batt_capacitance_cycle4), &parameter_position, &param_tab[0]);
    bat_param->bat_chem = (uint8_t)save_table_to_parameters(sizeof(bat_param->bat_chem), &parameter_position, &param_tab[0]);
 
    for(uint8_t i=0; i<3;i++)
    {
        switch(i)
        {
            case 1:  bat_param->settings_ptr = bat_param->liion_settings_ptr; break;
            case 2:  bat_param->settings_ptr = bat_param->pb_settings_ptr; break;
            case 3:  bat_param->settings_ptr = bat_param->nimh_settings_ptr; break;
        }
  
        bat_param->settings_ptr->cell_count = (uint8_t)save_table_to_parameters(sizeof(bat_param->settings_ptr->cell_count), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->selected_mode = (uint8_t)save_table_to_parameters(sizeof(bat_param->settings_ptr->selected_mode), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->set_cycle = (uint8_t)save_table_to_parameters(sizeof(bat_param->settings_ptr->set_cycle), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_voltage = save_table_to_parameters(sizeof(bat_param->settings_ptr->batt_set_voltage), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_min_discharge_voltage = save_table_to_parameters(sizeof(bat_param->settings_ptr->batt_set_min_discharge_voltage), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_trickle_voltage = save_table_to_parameters(sizeof(bat_param->settings_ptr->batt_set_trickle_voltage), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_trickle_current = save_table_to_parameters(sizeof(bat_param->settings_ptr->batt_set_trickle_current), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->set_max_time = save_table_to_parameters(sizeof(bat_param->settings_ptr->set_max_time), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_max_temp = (uint8_t)save_table_to_parameters(sizeof(bat_param->settings_ptr->batt_max_temp), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_1 = save_table_to_parameters(sizeof(bat_param->settings_ptr->charge_current_1), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_1 = save_table_to_parameters(sizeof(bat_param->settings_ptr->discharge_current_1), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_2 = save_table_to_parameters(sizeof(bat_param->settings_ptr->charge_current_2), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_2_percent = (uint8_t)save_table_to_parameters(sizeof(bat_param->settings_ptr->charge_current_2_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_3 = save_table_to_parameters(sizeof(bat_param->settings_ptr->charge_current_3), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_3_percent = (uint8_t)save_table_to_parameters(sizeof(bat_param->settings_ptr->charge_current_3_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_4 = save_table_to_parameters(sizeof(bat_param->settings_ptr->charge_current_4), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_4_percent = (uint8_t)save_table_to_parameters(sizeof(bat_param->settings_ptr->charge_current_4_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_2 = save_table_to_parameters(sizeof(bat_param->settings_ptr->discharge_current_2), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_2_percent = save_table_to_parameters(sizeof(bat_param->settings_ptr->discharge_current_2_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_3 = save_table_to_parameters(sizeof(bat_param->settings_ptr->discharge_current_3), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_3_percent = save_table_to_parameters(sizeof(bat_param->settings_ptr->discharge_current_3_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_4 = save_table_to_parameters(sizeof(bat_param->settings_ptr->discharge_current_4), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_4_percent = save_table_to_parameters(sizeof(bat_param->settings_ptr->discharge_current_4_percent), &parameter_position, &param_tab[0]);
    }

    printf("Read table: \n\r");
    print_data_tab(&param_tab[0], parameter_position);
    bat_param->settings_ptr = bat_param->pb_settings_ptr; 
    printf("batt_pb_cycle: %u \r\n",  bat_param->settings_ptr->set_cycle);
    //printf("batt_capacitance_cycle2: %u \r\n", bat_param->batt_capacitance_cycle2);
    //printf("batt_capacitance_cycle3: %u \r\n", bat_param->batt_capacitance_cycle3);
     //printf("batt_capacitance_cycle4: %u \r\n", bat_param->batt_capacitance_cycle4);
   // printf("bat_chem: %u \r\n", bat_param->bat_chem);
    
    
   // parameter_position=0;
   // param_tab[0]=0x45;
   // param_tab[1]=0xa3;
   //  printf("test: %x \r\n",  save_table_to_param(2, &parameter_position, &param_tab[0])); 
}

void check_if_any_changes_in_parameters(BattParameters *bat_param)
{
    
    save_parameters_to_flash(bat_param);
    
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


/*
* 0               bat id	
* 1               sampling time	
* 2             whole cycle flag, cycle type (chg, disc)	
* 3,4            measured capacitance (2bytes)	
* 5,6            set current (2bytes)	
* 7,8            set voltage(2bytes)	
* 9               max temp	
* 10 to 4095 	  measurment data	
*/
void read_measurment_data_from_flash(BattParameters *bat_param)
{
    uint32_t measurment_addr, temperature_addr, addr,i;
    uint8_t data_tab[10];
    
    measurment_addr = check_measurment_next_addr();
    temperature_addr = check_temperature_next_addr();
    
    if(bat_param->current_measurment_cycle == 1)
    {
        printf("Empty Memory \r\n");
    }else
    {
        addr = MEASURMENT_START_ADDR;
        
        read_bytes(addr, &data_tab[0], sizeof(data_tab));
        printf("Battery ID: %02d\r\n",data_tab[BAT_ID]);
        printf("Sampling time: %02dms\r\n",data_tab[SAMPLING_TIME]);

        if ((data_tab[DONE_CYCLE_TYPE] & (1 << CYCLE_TYPE_CHARGING_BIT_POS)) == 0) 
        {
            printf("Cycle type: charging\r\n");
        }

        if ((data_tab[DONE_CYCLE_TYPE] & (1 << CYCLE_TYPE_DISCHARGING_BIT_POS)) == 0) 
        {
            printf("Cycle type: discharging\r\n");
        } 

        // Sprawdzenie bitu COMPLETE_CYCLE_BIT_POS
        if ((data_tab[DONE_CYCLE_TYPE] & (1 << COMPLETE_CYCLE_BIT_POS)) == 0) 
        {
            printf("Complete cycle: true\r\n");
        } else 
        {
            printf("Complete cycle: false\r\n");
        }
        
        if(((data_tab[CAPACITANCE_MSB_POS] << 8) | CAPACITANCE_LSB_POS) == 0xFFFF)
        {
             printf("Measured capacitance: fail\r\n");
        }else
        {
            printf("Measured capacitance: %umAh\r\n", ((data_tab[CAPACITANCE_MSB_POS] << 8) | CAPACITANCE_LSB_POS));
        }
        
        printf("Set current: %umA\r\n", ((data_tab[SET_CURRENT_MSB_POS] << 8) | SET_CURRENT_LSB_POS));
        printf("Set voltage: %uV\r\n", ((data_tab[SET_VOLTAGE_MSB_POS] << 8) | SET_VOLTAGE_LSB_POS));
        
        if(data_tab[MAX_TEMP] == 0xFF)
        {
            printf("Max temp: fail\r\n"); 
        }else
        {
            printf("Max temp: %uC\r\n",data_tab[MAX_TEMP]);
        }
        printf("Voltage data:\r\n");
        
        for (i=addr+MAX_TEMP+1; i<=addr+100; i=i+4)
        {
            read_bytes(i, &data_tab[0], 2);
            if(((data_tab[0] << 8) | data_tab[1]) == 0xFFFF)
            {
                printf("Voltage data: END\r\n");
                break;
            }
            printf("%u\r\n", ((data_tab[0] << 8) | data_tab[1]));           //print data do console
        }
        
        printf("Current data:\r\n");

        for (i=addr+MAX_TEMP+3; i<=addr+100; i=i+4)
        {
            read_bytes(i, &data_tab[0], 2);
            if(((data_tab[0] << 8) | data_tab[1]) == 0xFFFF)
            {
                printf("Current data: END\r\n");
                break;
            }
            printf("%u\r\n", ((data_tab[0] << 8) | data_tab[1]));
        }
        
        addr = TEMP_START_ADDR;
        printf("Temp data:\r\n");
        
        // TODO temperature data print
        /*
        for (i=addr+MAX_TEMP+3; i>=addr+MEASURMENT_DATA_SIZE; i=i+4)
        {
            read_bytes(i, &data_tab[0], 2);
            printf("%u\r\n", ((data_tab[0] << 8) | data_tab[1]));
        }
         * */
    }
    
    
    
    bat_param->current_measurment_cycle;
    bat_param->bat_id; 
}