#include <xc.h>
#include <stdio.h>
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
#define UPDATE_PARAMETERS 0
#define UPDATE_MEASURMENT 1

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


uint16_t check_current_measurment_offset(void);
uint16_t check_current_temp_offset(void);


#define BAT_ID          0
#define SAMPLING_TIME   1
#define DONE_CYCLE_TYPE 2
#define CAPACITANCE     3
#define SET_CURRENT     5
#define SET_VOLTAGE     7
#define MAX_TEMP        8


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

void save_measurment_to_flash(void)
{
    uint8_t param_tab[10];
    read_bytes(check_current_measurment_offset(), &param_tab[0], sizeof(param_tab));
   
    
    
}


uint16_t check_parameters_offset_position(uint16_t addr, uint8_t lenght);

void memory_and_cycle_positions(BattParameters *bat_param)
{
    uint8_t param_tab[1],offset;
    
    
    bat_param->max_memory_cycle = WEAR_LEVELING_MEASUREMENT_DATA_CYCLES;
    bat_param->current_memory_cycle = check_parameters_offset_position(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH); 
    
    offset = check_current_measurment_offset();
    if(offset == MEASURMENT_START_ADDR)
    {
        bat_param->current_battery_memory_position = 1;
    }else
    {
         //to read bat_id need to read heder of previous cycle so i need to do offset-MEASURMENT_DATA_SIZE
         read_bytes(offset-MEASURMENT_DATA_SIZE, &param_tab[0], sizeof(param_tab));        
         bat_param->current_battery_memory_position = &param_tab[BAT_ID]+1;
         
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
uint16_t check_parameters_offset_position(uint16_t addr, uint8_t lenght)
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
uint16_t check_current_param_offset(void)
{ 
    return (check_parameters_offset_position(WEAR_LEVELING_PARAM_START_ADDR, WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH) * PARAMETERS_DATA_SIZE + PARAMETER_START_ADDR);
}


uint16_t check_current_measurment_offset(void)
{ 
    return (check_parameters_offset_position(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH) * MEASURMENT_DATA_SIZE + MEASURMENT_START_ADDR);
}

uint16_t check_current_temp_offset(void)
{ 
    return (check_parameters_offset_position(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH) * TEMP_DATA_SIZE + TEMP_START_ADDR);
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
    uint8_t current_param_address[4];
    int8_t i, j;
    uint16_t addr,lenght;

    if(wear_leveling_type == UPDATE_PARAMETERS)
    {
        addr = WEAR_LEVELING_PARAM_START_ADDR;
        lenght = WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH;
        if (check_parameters_offset_position(WEAR_LEVELING_PARAM_START_ADDR, WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGTH) >= WEAR_LEVELING_PARAM_CYCLES)
        {
            parameter_erase();
            printf("CLEAR PARAMETER MEMORY! \n\r");
            return;
        }
    }else if (wear_leveling_type == UPDATE_MEASURMENT)
    {
        
        addr = WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR;
        lenght = WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH;
        
        if (check_parameters_offset_position(WEAR_LEVELING_MEASUREMENT_DATA_START_ADDR, WEAR_LEVELING_MEASUREMENT_DATA_CYCLES_BYTES_LENGTH) >= WEAR_LEVELING_MEASUREMENT_DATA_CYCLES)
        {
            measurment_erase();
            printf("CLEAR MEASURMENT MEMORY! \n\r");
            return;
        }
    }
    
    
    
    read_bytes(addr, &current_param_address[0], lenght);
    
    printf("current_param_address tab: \r\n");
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
                printf("i:%d j:%d\r\n",i,j);
                  print_tab(&current_param_address[0],sizeof(current_param_address));
                write_byte(addr + i, current_param_address[i]);    // save new offset position to flash
              
                 
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
void save_param_to_table(uint16_t data, uint8_t length, uint8_t *parameter_position, uint8_t *param_tab)
{
    for (uint8_t j = 0; j < length; j++)
    {
        *(param_tab + *parameter_position + j) = (unsigned char)((data >> (8 * (length - 1 - j))) & 0xFF);
    }
    *parameter_position = *parameter_position + length;
}  

 uint16_t save_table_to_param(uint8_t length, uint8_t *parameter_position, uint8_t *param_tab)
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
    
    printf("Write table: \n\r");
    print_data_tab(&param_tab[0], parameter_position);
    
    update_wear_leveling_static_buffer(UPDATE_PARAMETERS);
    printf("Write to flash, parameter address: %u  \n\r", check_current_param_offset());
    write_byte_table_auto_address_increment(check_current_param_offset(), &param_tab[0], parameter_position);
  
}


void read_parameters_from_flash(BattParameters *bat_param)
{
    uint8_t param_tab[144], parameter_position;

    read_bytes(check_current_param_offset(), &param_tab[0], sizeof(param_tab));
   

    
    printf("Parameter addr read:%u \r\n", check_current_param_offset());
    
    parameter_position = 0;
    bat_param->batt_capacitance_cycle1 = save_table_to_param(sizeof(bat_param->batt_capacitance_cycle1), &parameter_position, &param_tab[0]);
    bat_param->batt_capacitance_cycle2 = save_table_to_param(sizeof(bat_param->batt_capacitance_cycle2), &parameter_position, &param_tab[0]);
    bat_param->batt_capacitance_cycle3 = save_table_to_param(sizeof(bat_param->batt_capacitance_cycle3), &parameter_position, &param_tab[0]);
    bat_param->batt_capacitance_cycle4 = save_table_to_param(sizeof(bat_param->batt_capacitance_cycle4), &parameter_position, &param_tab[0]);
    bat_param->bat_chem = save_table_to_param(sizeof(bat_param->bat_chem), &parameter_position, &param_tab[0]);

    
    
    for(uint8_t i=0; i<3;i++)
    {
        switch(i)
        {
            case 1:  bat_param->settings_ptr = bat_param->liion_settings_ptr; break;
            case 2:  bat_param->settings_ptr = bat_param->pb_settings_ptr; break;
            case 3:  bat_param->settings_ptr = bat_param->nimh_settings_ptr; break;
        }
        
        
        bat_param->settings_ptr->cell_count = save_table_to_param(sizeof(bat_param->settings_ptr->cell_count), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->selected_mode = save_table_to_param(sizeof(bat_param->settings_ptr->selected_mode), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->set_cycle = save_table_to_param(sizeof(bat_param->settings_ptr->set_cycle), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_voltage = save_table_to_param(sizeof(bat_param->settings_ptr->batt_set_voltage), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_min_discharge_voltage = save_table_to_param(sizeof(bat_param->settings_ptr->batt_set_min_discharge_voltage), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_trickle_voltage = save_table_to_param(sizeof(bat_param->settings_ptr->batt_set_trickle_voltage), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_set_trickle_current = save_table_to_param(sizeof(bat_param->settings_ptr->batt_set_trickle_current), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->set_max_time = save_table_to_param(sizeof(bat_param->settings_ptr->set_max_time), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->batt_max_temp = save_table_to_param(sizeof(bat_param->settings_ptr->batt_max_temp), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_1 = save_table_to_param(sizeof(bat_param->settings_ptr->charge_current_1), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_1 = save_table_to_param(sizeof(bat_param->settings_ptr->discharge_current_1), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_2 = save_table_to_param(sizeof(bat_param->settings_ptr->charge_current_2), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_2_percent = save_table_to_param(sizeof(bat_param->settings_ptr->charge_current_2_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_3 = save_table_to_param(sizeof(bat_param->settings_ptr->charge_current_3), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_3_percent = save_table_to_param(sizeof(bat_param->settings_ptr->charge_current_3_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_4 = save_table_to_param(sizeof(bat_param->settings_ptr->charge_current_4), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->charge_current_4_percent = save_table_to_param(sizeof(bat_param->settings_ptr->charge_current_4_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_2 = save_table_to_param(sizeof(bat_param->settings_ptr->discharge_current_2), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_2_percent = save_table_to_param(sizeof(bat_param->settings_ptr->discharge_current_2_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_3 = save_table_to_param(sizeof(bat_param->settings_ptr->discharge_current_3), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_3_percent = save_table_to_param(sizeof(bat_param->settings_ptr->discharge_current_3_percent), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_4 = save_table_to_param(sizeof(bat_param->settings_ptr->discharge_current_4), &parameter_position, &param_tab[0]);
        bat_param->settings_ptr->discharge_current_4_percent = save_table_to_param(sizeof(bat_param->settings_ptr->discharge_current_4_percent), &parameter_position, &param_tab[0]);
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