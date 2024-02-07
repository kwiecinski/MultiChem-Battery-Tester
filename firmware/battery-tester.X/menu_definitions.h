/* 
 * File:   menu.h
 * Author: RaD
 *
 * Created on 30 listopada 2023, 09:02
 */

#ifndef MENU_H
#define	MENU_H




typedef struct 
{

    uint8_t set_cycle, current_cycle, 
            bat_actual_temp, bat_max_temp,
            bat_chem, bat_storage_precentage,  selected_mode, cell_count;


    uint16_t batt_set_voltage, batt_set_current, 
            batt_actual_voltage, batt_actual_current,
            batt_set_trickle_voltage, bat_set_trickle_current,
            batt_set_min_discharge_voltage,
            batt_capacitance;

    uint32_t set_time;

    char text[25];

}BattParameters;


enum state 
{
    state_charging,
    state_discharging,
    state_idle
};

enum battery_number 
{
    battery_1,
    battery_2
};

enum select_menu_start
{
    menu,
    start
};

enum select_batt_chemistry
{
    liion,
    pb,
    nimh
};

enum mode
{
    charging,
    charging_discharging,
    charging_discharging_storage
};



void InitBattParameters(BattParameters *bat_param);
void SingleBat_Menu(BattParameters *bat_param);
void Options1_Menu(BattParameters *bat_param);
void Options2_Menu(BattParameters *bat_param);
#endif	/* MENU_H */

