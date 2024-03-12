#include <xc.h>
#include <stdio.h>
#include "peripherials/UART.h"
#include "main.h"
#include "settings.h"
#include "interrupts.h"
#include "SST25VF.h"
#include "MMSP.h"
#include "memory.h"


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

#define CAPACITANCE_CYCLE1_ADDR         0x01    // 1
#define CAPACITANCE_CYCLE1_LENGTH       2       // Used bytes: 2

#define CAPACITANCE_CYCLE2_ADDR         0x03    // 3
#define CAPACITANCE_CYCLE2_LENGTH       2       // Used bytes: 2

#define CAPACITANCE_CYCLE3_ADDR         0x05    // 5
#define CAPACITANCE_CYCLE3_LENGTH       2       // Used bytes: 2

#define CAPACITANCE_CYCLE4_ADDR         0x07    // 7
#define CAPACITANCE_CYCLE4_LENGTH       2       // Used bytes: 2

#define BATT_TYPE_ADDR                  0x09    // 9
#define BATT_TYPE_LENGTH                1       // Used bytes: 1

#define CELL_COUNT_ADDR_LI_ION             0x10    // 16
#define CELL_COUNT_ADDR_PB                 0x33    // 51
#define CELL_COUNT_ADDR_NI_MH              0x5D    // 93
#define CELL_COUNT_LENGTH                  1       // Used bytes: 1

#define MODE_ADDR_LI_ION                   0x11    // 17
#define MODE_ADDR_PB                       0x34    // 52
#define MODE_ADDR_NI_MH                    0x5E    // 94
#define MODE_LENGTH                        1       // Used bytes: 1

#define CYCLE_ADDR_LI_ION                  0x12    // 18
#define CYCLE_ADDR_PB                      0x35    // 53
#define CYCLE_ADDR_NI_MH                   0x5F    // 95
#define CYCLE_LENGTH                       1       // Used bytes: 1

#define CELL_VOLTAGE_ADDR_LI_ION           0x13    // 19
#define CELL_VOLTAGE_ADDR_PB               0x37    // 55
#define CELL_VOLTAGE_ADDR_NI_MH            0x61    // 97
#define CELL_VOLTAGE_LENGTH                2       // Used bytes: 2

#define MIN_DISCHARGE_VOLTAGE_ADDR_LI_ION  0x15    // 21
#define MIN_DISCHARGE_VOLTAGE_ADDR_PB      0x39    // 57
#define MIN_DISCHARGE_VOLTAGE_ADDR_NI_MH   0x63    // 99
#define MIN_DISCHARGE_VOLTAGE_LENGTH       2       // Used bytes: 2

#define TRICKLE_VOLTAGE_ADDR_LI_ION        0x17    // 23
#define TRICKLE_VOLTAGE_ADDR_PB            0x3B    // 59
#define TRICKLE_VOLTAGE_ADDR_NI_MH         0x65    // 101
#define TRICKLE_VOLTAGE_LENGTH             2       // Used bytes: 2

#define TRICKLE_CURRENT_ADDR_LI_ION        0x19    // 25
#define TRICKLE_CURRENT_ADDR_PB            0x3D    // 61
#define TRICKLE_CURRENT_ADDR_NI_MH         0x67    // 103
#define TRICKLE_CURRENT_LENGTH             2       // Used bytes: 2

#define MAX_TIME_ADDR_LI_ION               0x1B    // 27
#define MAX_TIME_ADDR_PB                   0x3F    // 63
#define MAX_TIME_ADDR_NI_MH                0x69    // 105
#define MAX_TIME_LENGTH                    2       // Used bytes: 2

#define TEMP_ADDR_LI_ION                   0x1C    // 28
#define TEMP_ADDR_PB                       0x40    // 64
#define TEMP_ADDR_NI_MH                    0x6A    // 106
#define TEMP_LENGTH                        1       // Used bytes: 1

#define CHRG_CUR1_ADDR_LI_ION              0x1D    // 29
#define CHRG_CUR1_ADDR_PB                  0x42    // 66
#define CHRG_CUR1_ADDR_NI_MH               0x6C    // 108
#define CHRG_CUR1_LENGTH                   2       // Used bytes: 2

#define DISC_CUR1_ADDR_LI_ION             0x1F    // 31
#define DISC_CUR1_ADDR_PB                 0x44    // 68
#define DISC_CUR1_ADDR_NI_MH              0x6E    // 110
#define DISC_CUR1_LENGTH                  2       // Used bytes: 2

#define CHRG_CUR2_MILLI_ADDR_LI_ION       0x1E    // 30
#define CHRG_CUR2_MILLI_ADDR_PB           0x46    // 70
#define CHRG_CUR2_MILLI_ADDR_NI_MH        0x70    // 112
#define CHRG_CUR2_MILLI_LENGTH            2       // Used bytes: 2

#define DISC_CUR2_MILLI_ADDR_LI_ION       0x20    // 32
#define DISC_CUR2_MILLI_ADDR_PB           0x48    // 72
#define DISC_CUR2_MILLI_ADDR_NI_MH        0x74    // 114
#define DISC_CUR2_MILLI_LENGTH            2       // Used bytes: 2

#define CHRG_CUR2_PERCENT_ADDR_LI_ION     0x22    // 34
#define CHRG_CUR2_PERCENT_ADDR_PB         0x4A    // 74
#define CHRG_CUR2_PERCENT_ADDR_NI_MH      0x76    // 116
#define CHRG_CUR2_PERCENT_LENGTH          2       // Used bytes: 2

#define DISC_CUR2_PERCENT_ADDR_LI_ION     0x24    // 36
#define DISC_CUR2_PERCENT_ADDR_PB         0x4C    // 76
#define DISC_CUR2_PERCENT_ADDR_NI_MH      0x78    // 118
#define DISC_CUR2_PERCENT_LENGTH          2       // Used bytes: 2

#define CHRG_CUR3_MILLI_ADDR_LI_ION       0x26    // 38
#define CHRG_CUR3_MILLI_ADDR_PB           0x4E    // 78
#define CHRG_CUR3_MILLI_ADDR_NI_MH        0x7A    // 120
#define CHRG_CUR3_MILLI_LENGTH            2       // Used bytes: 2

#define DISC_CUR3_MILLI_ADDR_LI_ION       0x28    // 40
#define DISC_CUR3_MILLI_ADDR_PB           0x50    // 80
#define DISC_CUR3_MILLI_ADDR_NI_MH        0x7C    // 122
#define DISC_CUR3_MILLI_LENGTH            2       // Used bytes: 2

#define CHRG_CUR3_PERCENT_ADDR_LI_ION     0x2A    // 42
#define CHRG_CUR3_PERCENT_ADDR_PB         0x52    // 82
#define CHRG_CUR3_PERCENT_ADDR_NI_MH      0x7E    // 124
#define CHRG_CUR3_PERCENT_LENGTH          2       // Used bytes: 2

#define DISC_CUR3_PERCENT_ADDR_LI_ION     0x2C    // 44
#define DISC_CUR3_PERCENT_ADDR_PB         0x54    // 84
#define DISC_CUR3_PERCENT_ADDR_NI_MH      0x80    // 126
#define DISC_CUR3_PERCENT_LENGTH          2       // Used bytes: 2

#define CHRG_CUR4_MILLI_ADDR_LI_ION       0x2E    // 46
#define CHRG_CUR4_MILLI_ADDR_PB           0x56    // 86
#define CHRG_CUR4_MILLI_ADDR_NI_MH        0x82    // 128
#define CHRG_CUR4_MILLI_LENGTH            2       // Used bytes: 2

#define DISC_CUR4_MILLI_ADDR_LI_ION       0x30    // 48
#define DISC_CUR4_MILLI_ADDR_PB           0x58    // 88
#define DISC_CUR4_MILLI_ADDR_NI_MH        0x84    // 130
#define DISC_CUR4_MILLI_LENGTH            2       // Used bytes: 2

#define CHRG_CUR4_PERCENT_ADDR_LI_ION     0x32    // 50
#define CHRG_CUR4_PERCENT_ADDR_PB         0x5A    // 90
#define CHRG_CUR4_PERCENT_ADDR_NI_MH      0x86    // 132
#define CHRG_CUR4_PERCENT_LENGTH          2       // Used bytes: 2

#define DISC_CUR4_PERCENT_ADDR_LI_ION     0x34    // 52
#define DISC_CUR4_PERCENT_ADDR_PB         0x5C    // 92
#define DISC_CUR4_PERCENT_ADDR_NI_MH      0x88    // 134
#define DISC_CUR4_PERCENT_LENGTH          2       // Used bytes: 2

void ParameterSector_CopyEraseRestore(void)
{
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
 Function return current offset address for parameters
 */
uint16_t CheckCurrentParamOffset(void)
{ 
    return  (CheckOffsetPosition()*PARAMETERS_DATA_SIZE + PARAMETERS_DATA_SIZE);
}

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

void SaveParametersToFlash (void)
{
    uint16_t address_offset;
    address_offset = CheckCurrentParamOffset();
   
const uint8_t parameters_address[40][2] = 
{
    {CAPACITANCE_CYCLE1_ADDR, CAPACITANCE_CYCLE1_LENGTH},
    {CAPACITANCE_CYCLE2_ADDR, CAPACITANCE_CYCLE2_LENGTH},
    {CAPACITANCE_CYCLE3_ADDR, CAPACITANCE_CYCLE3_LENGTH},
    {CAPACITANCE_CYCLE4_ADDR, CAPACITANCE_CYCLE4_LENGTH},
    {BATT_TYPE_ADDR, BATT_TYPE_LENGTH},
    {CELL_COUNT_ADDR_LI_ION, CELL_COUNT_LENGTH},
    {MODE_ADDR_LI_ION, MODE_LENGTH},
    {CYCLE_ADDR_LI_ION, CYCLE_LENGTH},
    {CELL_VOLTAGE_ADDR_LI_ION, CELL_VOLTAGE_LENGTH},
    {MIN_DISCHARGE_VOLTAGE_ADDR_LI_ION, MIN_DISCHARGE_VOLTAGE_LENGTH},
    {TRICKLE_VOLTAGE_ADDR_LI_ION, TRICKLE_VOLTAGE_LENGTH},
    {TRICKLE_CURRENT_ADDR_LI_ION, TRICKLE_CURRENT_LENGTH},
    {MAX_TIME_ADDR_LI_ION, MAX_TIME_LENGTH},
    {TEMP_ADDR_LI_ION, TEMP_LENGTH},
    {CHRG_CUR1_ADDR_LI_ION, CHRG_CUR1_LENGTH},
    {DISC_CUR1_ADDR_LI_ION, DISC_CUR1_LENGTH},
    {CHRG_CUR2_MILLI_ADDR_LI_ION, CHRG_CUR2_MILLI_LENGTH},
    {DISC_CUR2_MILLI_ADDR_LI_ION, DISC_CUR2_MILLI_LENGTH},
    {CHRG_CUR2_PERCENT_ADDR_LI_ION, CHRG_CUR2_PERCENT_LENGTH},
    {DISC_CUR2_PERCENT_ADDR_LI_ION, DISC_CUR2_PERCENT_LENGTH},
    {CHRG_CUR3_MILLI_ADDR_LI_ION, CHRG_CUR3_MILLI_LENGTH},
    {DISC_CUR3_MILLI_ADDR_LI_ION, DISC_CUR3_MILLI_LENGTH},
    {CHRG_CUR3_PERCENT_ADDR_LI_ION, CHRG_CUR3_PERCENT_LENGTH},
    {DISC_CUR3_PERCENT_ADDR_LI_ION, DISC_CUR3_PERCENT_LENGTH},
    {CHRG_CUR4_MILLI_ADDR_LI_ION, CHRG_CUR4_MILLI_LENGTH},
    {DISC_CUR4_MILLI_ADDR_LI_ION, DISC_CUR4_MILLI_LENGTH},
    {CHRG_CUR4_PERCENT_ADDR_LI_ION, CHRG_CUR4_PERCENT_LENGTH},
    {DISC_CUR4_PERCENT_ADDR_LI_ION, DISC_CUR4_PERCENT_LENGTH},
};
    
    
}