#ifndef XC_MEMORY_H
#define	XC_MEMORY_H


#include "menu_definitions.h"


#define LOG_MEASURMENT 0
#define INIT_LOG_MEASURMENT 1
#define UPDATE_PARAMETERS 0
#define UPDATE_MEASURMENT 1

typedef struct 
{
     uint8_t  temp_pos,  measured_data[4], measured_temp[16], 
                    measurment_sampling_time,temp_sampling_time;
     uint16_t data_pos;
     uint32_t memory_offset_temp, memory_offset_data;

}memory_data;

void memory_and_cycle_positions(BattParameters *bat_param);
void check_if_any_changes_in_parameters(BattParameters *bat_param);
void read_parameters_from_flash(BattParameters *bat_param);
void save_parameters_to_flash(BattParameters *bat_param);

void save_measurment_start_header_to_flash(BattParameters *bat_param, uint8_t charger_state);
void save_measurment_end_header_to_flash(BattParameters *bat_param);
void save_measurment_data_to_flash(BattParameters *bat_param, memory_data *memory, uint8_t init);
void read_measurment_data_from_flash(BattParameters *bat_param);
void update_wear_leveling_static_buffer(uint8_t wear_leveling_type);
uint16_t check_measurment_next_addr(void);

#endif	