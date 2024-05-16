/* 
 * File:   menu.h
 * Author: RaD
 *
 * Created on 30 listopada 2023, 09:02
 */

#ifndef MENU_H
#define	MENU_H

#define set_mode_edit 1
#define set_mode_display 0


typedef struct
{
    uint8_t set_cycle ,
         batt_max_temp,
         bat_storage_precentage,  selected_mode, cell_count,
         charge_current_2_percent, 
         charge_current_3_percent, 
         charge_current_4_percent, 
         precent_current_flags;


    uint16_t batt_set_voltage, batt_set_current, 
         batt_set_trickle_voltage, batt_set_trickle_current,
         batt_set_min_discharge_voltage,
         charge_current_1, discharge_current_1,
         charge_current_2, discharge_current_2,
         charge_current_3, discharge_current_3,
         charge_current_4, discharge_current_4,
         discharge_current_4_percent,
         discharge_current_3_percent,
         discharge_current_2_percent,
         set_max_time;
   
}BattTypeSettings;



typedef struct 
{

    BattTypeSettings *settings_ptr;
    BattTypeSettings *liion_settings_ptr;
    BattTypeSettings *pb_settings_ptr;
    BattTypeSettings *nimh_settings_ptr;
    
    uint8_t set_cycle, current_cycle, 
            bat_actual_temp, batt_max_temp,
            bat_chem, bat_storage_precentage,  selected_mode, cell_count,
            charge_current_2_percent, 
            charge_current_3_percent, 
            charge_current_4_percent, 
            precent_current_flags;


    uint16_t batt_set_voltage, batt_set_current, 
            batt_actual_voltage, batt_actual_current,
            batt_set_trickle_voltage, batt_set_trickle_current,
            batt_set_min_discharge_voltage,
            batt_capacitance_cycle1,batt_capacitance_cycle2,batt_capacitance_cycle3,batt_capacitance_cycle4,
            charge_current_1, discharge_current_1,
            charge_current_2, discharge_current_2,
            charge_current_3, discharge_current_3,
            charge_current_4, discharge_current_4,
            discharge_current_4_percent,
            discharge_current_3_percent,
            discharge_current_2_percent,
            set_max_time;


    uint32_t current_time;

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
void Options3_Menu(BattParameters *bat_param);
void Options4_Menu(BattParameters *bat_param);
void Options5_Menu(BattParameters *bat_param);
void ChemistryDisplay(BattParameters *bat_param, uint8_t set_mode);
void CellCount(BattParameters *batparam_ptr, uint8_t set_mode);
void ChargerMode(BattParameters *bat_param, uint8_t set_mode);
void CycleSet(BattParameters *batparam_ptr, uint8_t set_mode);
void SetCellVotage(BattParameters *batparam_ptr, uint8_t set_mode);
void MinimumDischargeVoltage(BattParameters *batparam_ptr, uint8_t set_mode);
void TrickleCurrent(BattParameters *batparam_ptr, uint8_t set_mode);
void TrickleVoltage(BattParameters *batparam_ptr, uint8_t set_mode);
void SetMaxTime (BattParameters *batparam_ptr, uint8_t set_mode);
void SetTemp(BattParameters *batparam_ptr, uint8_t set_mode);
void SetChargingCurrent_1(BattParameters *batparam_ptr, uint8_t set_mode);
void SetChargingCurrent_2(BattParameters *batparam_ptr, uint8_t set_mode);
void SetChargingCurrent_3(BattParameters *batparam_ptr, uint8_t set_mode);
void SetChargingCurrent_4(BattParameters *batparam_ptr, uint8_t set_mode);
void SetDischargingCurrent_1(BattParameters *batparam_ptr, uint8_t set_mode);
void SetDischargingCurrent_2(BattParameters *batparam_ptr, uint8_t set_mode);
void SetDischargingCurrent_3(BattParameters *batparam_ptr, uint8_t set_mode);
void SetDischargingCurrent_4(BattParameters *batparam_ptr, uint8_t set_mode);
void switch_between_battery_types(BattParameters *bat_param, uint8_t init);
#endif	/* MENU_H */
