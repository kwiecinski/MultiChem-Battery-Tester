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
#define LENGHT      1

#define PARAMETER_SECTOR_ADDR                       0x0
#define WEAR_LEVELING_PARAM_START_ADDR              0x0     
#define WEAR_LEVELING_PARAM_CYCLES                  27      // 1 Sector size (4096B) / Size of parameters table (144B)
#define WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGHT     4       // value is up ronded from WEAR_LEVELING_PARAM_CYCLES/8 
#define PARAMETERS_DATA_SIZE                        144

#define MEASURMENT_DATA_SECTOR_ADDR                           0x40    //64
#define WEAR_LEVELING_MEASURMENT_DATA_START_ADDR              0X10     // 0
#define WEAR_LEVELING_MEASURMENT_DATA_CYCLES                  32      // 1 Sector size (4096B) / Size of parameters table (144B)
#define WEAR_LEVELING_MEASURMENT_DATA_CYCLES_BYTES_LENGHT     4       // value is up ronded from WEAR_LEVELING_PARAM_CYCLES/8 
#define MEASURMENT_DATA_SIZE                                  16384

/********************************************************************************/
#define DATA_COPY_LENGHT 64


void ParameterSector_CopyEraseRestore(void)
{
    uint8_t tab[DATA_COPY_LENGHT];
    ReadBytes(MEASURMENT_DATA_SECTOR_ADDR-1,&tab[0],DATA_COPY_LENGHT);
    for (uint8_t i = 0; i < WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGHT; ++i)
    {
         tab[WEAR_LEVELING_PARAM_START_ADDR + i] = 0xFF;
    }
    SectorErase(PARAMETER_SECTOR_ADDR);
    
    
}

uint16_t CheckOffsetPosition(void)
{
    uint8_t current_param_address[4],zeroBitCount;
    int8_t i,j;
    
    ReadBytes(WEAR_LEVELING_PARAM_START_ADDR,&current_param_address[0],WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGHT);
    
    // Check how many zeros are in bytes are in Wear Leveling Status Buffer
    zeroBitCount = 0;
    for (i = 0; i < WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGHT; ++i) 
    {
        for (j = 7; j >= 0; --j) 
        {
            if (((current_param_address[i] >> j) & 0x01) == 0) 
            {
                zeroBitCount++;
            }
        }
    }
    return zeroBitCount; 
      
}

/*
 * @brief Checks the current parameter offset.
 * @return The calculated current parameter offset.
 */

uint16_t CheckCurrentParamOffset(void)
{ 
    return  (CheckOffsetPosition()*PARAMETERS_DATA_SIZE + PARAMETERS_DATA_SIZE);
}
/*
 * @brief Saves parameter to flash memory.
 *
 * This function is responsible for saving parameters to flash memory. If the wear leveling
 * parameter cycles threshold is reached, it clears the memory and restores it. It then
 * searches for the first bit that is set to 1 (MSB) in the current parameter address array.
 * Once found, it changes that bit to 0 and writes the updated parameter to the flash memory.
 */

void SaveParamToFlash(void)
{
    uint8_t current_param_address[4];
    int8_t i,j;

    if(CheckOffsetPosition()>=WEAR_LEVELING_PARAM_CYCLES)
    {
        ParameterSector_CopyEraseRestore();
        printf("CLEAR MEMORY! \n\r");
        return;
    }
    ReadBytes(WEAR_LEVELING_PARAM_START_ADDR,&current_param_address[0],WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGHT);
    
    for (i = 0; i < WEAR_LEVELING_PARAM_CYCLES_BYTES_LENGHT; ++i) 
    {
        // Checking for first bit that is 1 (MSB)
        for (j = 7; j >= 0; --j) 
        {
            if (((current_param_address[i] >> j) & 0x01) == 1) 
            {
                // Found bit 1, change it to 0
               current_param_address[i] &= ~(1 << j);
               WriteByte(WEAR_LEVELING_PARAM_START_ADDR+i, current_param_address[i]);    // save new offset posistion to flash
               
               return; // End for after finding first 1 bit
            }
        }
    }
}

void RepresentValueInBinary (uint8_t value)
{
    for (int8_t j = 7; j >= 0; j--) 
    {
        printf("%d", (value & (1 << j)) ? 1 : 0);
    }
}

/**
 * @brief Saves parameter data to a table.
 *
 * @param lenght: The length of the data.
 * @param data: The data to be saved.
 * @param parameter_position: The position of the parameter in the table.
 * @param param_tab: The table where the parameter data will be saved.
 */
void SaveParamToTable( uint8_t lenght, uint8_t data, uint8_t *parameter_position, uint8_t *param_tab)
{
    for(uint8_t j=0; j<=lenght; j++)
    {
        *(param_tab + *parameter_position +j) = (uint8_t)data>>(8*j);
    }
}  
    

void SaveParametersToFlash (BattParameters *bat_param)
{
    uint16_t address_offset;
    address_offset = CheckCurrentParamOffset();
    
    uint8_t param_tab[144], parameter_position;
    
   parameter_position=0;
   SaveParamToTable(bat_param->batt_capacitance_cycle1, sizeof(bat_param->batt_capacitance_cycle1), &parameter_position, &param_tab[0]);
   SaveParamToTable(bat_param->batt_capacitance_cycle2, sizeof(bat_param->batt_capacitance_cycle2), &parameter_position, &param_tab[0]);
   SaveParamToTable(bat_param->batt_capacitance_cycle3, sizeof(bat_param->batt_capacitance_cycle3), &parameter_position, &param_tab[0]);
   SaveParamToTable(bat_param->batt_capacitance_cycle4, sizeof(bat_param->batt_capacitance_cycle4), &parameter_position, &param_tab[0]);
   SaveParamToTable(bat_param->bat_chem, sizeof(bat_param->bat_chem), &parameter_position, &param_tab[0]);
   SaveParamToTable(bat_param->cell_count, sizeof(bat_param->cell_count), &parameter_position, &param_tab[0]);
   SaveParamToTable(bat_param->selected_mode, sizeof(bat_param->selected_mode), &parameter_position, &param_tab[0]);
   SaveParamToTable(bat_param->set_cycle, sizeof(bat_param->set_cycle), &parameter_position, &param_tab[0]);

  

    
}