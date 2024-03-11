#include <xc.h>
#include <stdio.h>
#include "peripherials/UART.h"
#include "main.h"
#include "settings.h"
#include "interrupts.h"
#include "SST25VF.h"
#include "MMSP.h"



#define WEAR_LEVELING_PARAM_START       0x0     // 0
#define WEAR_LEVELING_PARAM_LENGHT      4    

#define WEAR_LEVELING_MEASUREMENT_START 0x10    // 16
#define WEAR_LEVELING_MEASUREMENT_END   0x1F    // 31

#define UNUSED_SPACE_START              0x20    // 32
#define UNUSED_SPACE_END                0x3F    // 63

#define PARAMETERS_DATA_START           0x40    // 64
#define PARAMETERS_DATA_END             0x1000  // 4096

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


/*
 
 */
uint16_t CheckCurrentParamAddress(void)
{
    uint8_t current_param_address[4],i,j,zeroBitCount;
    
    ReadBytes(WEAR_LEVELING_PARAM_START,&current_param_address[0],WEAR_LEVELING_PARAM_LENGHT);
    
    
    for (int i = 0; i < 4; ++i) 
    {
        uint8_t currentByte = current_param_address[i];

        // Sprawdzenie ka?dego bitu w bajcie
        for (int j = 0; j < 8; ++j) 
        {
            if (((currentByte >> j) & 0x01) == 0) 
            {
                zeroBitCount++;
            }
        }
    }
    
}

void SaveParamToFlash (void)
{
    
}