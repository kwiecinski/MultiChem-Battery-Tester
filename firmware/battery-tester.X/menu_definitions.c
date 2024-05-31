      /*****MENU MAP*****
     ********************
     * ---Options 1
     * Batt Chem: Li-Ion / PB / NiMh
     * Cell Count: 1
     * Mode: Only Charging / Chrg/Disc / Chrg/Disc/Stor
     * Cycles: 3

     
     *---Options 2
     * Vol: 4,22 
     * TrickleVol: 2,8V
     * TrickeCurr: 10mA
     * MinDiscVol: 2,80V  
     * StorCap:70% 
  
     *---Options 3
     * Max Time: 00h00m
     * MaxTemp:50C 
     * 
     * 
    
     *---Cycle 1/2 Options
     * ChrgCur 1: 1000mA
     * DiscCur 1: 1000mA
     * ChrgCur 2: 1000mA/%
     * DiscCur 2: 1000mA/%
     
     *---Cycle 3/4 Options
     * ChrgCur 3: 1000mA/%
     * DiscCur 3: 1000mA/%
     * ChrgCur 4: 1000mA/%
     * DiscCur 5: 1000mA/%
     
    */


#include <xc.h>
#include <stdio.h>

#include "LCD-KS0108/KS0108.h"
#include "LCD-KS0108/font5x8.h"
#include "LCD-KS0108/Tahoma11x13.h"
#include "LCD-KS0108/Tekton_Pro_Ext27x28.h"
#include "menu_definitions.h"
#include "menu_navigation.h"

void batState(uint8_t state, uint8_t  battery_number)
{
    /*
        switch(battery_number)
        {
            case battery_1:      GLCD_GotoXY(35,2);  break;
            case battery_2:      GLCD_GotoXY(102,2); break;
        }
    */
        switch(state)
        {
            case state_charging:     GLCD_PrintString("CHRG"); break;
            case state_discharging:  GLCD_PrintString("DISC"); break;
            case state_idle:         GLCD_PrintString("IDLE"); break;
        }
}

void ChemistryDisplay(BattParameters *bat_param, uint8_t set_mode)
{
        GLCD_GotoXY(0, 13);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_PrintString("Batt type:");
        
        if(set_mode == set_mode_edit)
        {
            GLCD_FillRectangle(60, 12, 98, 20, GLCD_Black);
            GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        
        }else if(set_mode == set_mode_display)
        {    
            GLCD_FillRectangle(60, 12, 98, 20, GLCD_White);
            GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        }
        
        
        GLCD_GotoXY(61, 13);
        switch(bat_param->bat_chem)
        {
            case liion:     GLCD_PrintString("Li-Ion");     break;
            case pb:        GLCD_PrintString("PB");         break;
            case nimh:      GLCD_PrintString("Ni-MH");      break;
        }
        
       
}

void ChargerMode(BattParameters *bat_param, uint8_t set_mode)
{
    
    GLCD_GotoXY(0, 31);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("Mode:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(30, 30, 127, 38, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(30, 30, 127, 38, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }


    GLCD_GotoXY(31, 31);
    switch(bat_param->selected_mode)
    {
        case charging:                          GLCD_PrintString("Charging"); break;
        case charging_discharging:              GLCD_PrintString("Charg/Disch"); break;
        case charging_discharging_storage:      GLCD_PrintString("Charg/Disch/Stor"); break;
    }
  
}
/////////////////////////////////////////////////////////////////////////////////

/*-----------------------------------------------------------------------------*/
void SetChargingCurrent_1(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 13);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("ChrgCur 1:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 12, 102, 20, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 12, 102, 20, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 13);
    sprintf(bat_param->text, "%umA", bat_param->charge_current_1);
    GLCD_PrintString(bat_param->text); 
}
void SetDischargingCurrent_1(BattParameters *bat_param, uint8_t set_mode)
{ 
    
    GLCD_GotoXY(0, 22);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("DiscCur 1:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 21, 102, 29, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 21, 102, 29, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 22);
    sprintf(bat_param->text, "%umA", bat_param->discharge_current_1);
    GLCD_PrintString(bat_param->text); 
}


void SetChargingCurrent_2(BattParameters *bat_param, uint8_t set_mode)
{ 
    
    GLCD_GotoXY(0, 31);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("ChrgCur 2:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 30, 102, 38, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 30, 102, 38, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 31);
    if(CHECK_CHRG2_CURRENT_SET)
    {
        sprintf(bat_param->text, "%umA", bat_param->charge_current_2);
    }else
    {
        sprintf(bat_param->text, "%u%%", bat_param->charge_current_2_percent);
    }
    GLCD_PrintString(bat_param->text); 
}

void SetChargingCurrent_3(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 13);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("ChrgCur 3:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 12, 102, 20, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 12, 102, 20, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 13);
    if(CHECK_CHRG3_CURRENT_SET)
    {
        sprintf(bat_param->text, "%umA", bat_param->charge_current_3);
    }else
    {
        sprintf(bat_param->text, "%u%%", bat_param->charge_current_3_percent);
    }
    GLCD_PrintString(bat_param->text); 
}


void SetChargingCurrent_4 (BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 31);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("ChrgCur 4:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 30, 102, 38, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 30, 102, 38, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 31);
    if(CHECK_CHRG4_CURRENT_SET)
    {
        sprintf(bat_param->text, "%umA", bat_param->charge_current_4);
    }else
    {
        sprintf(bat_param->text, "%u%%", bat_param->charge_current_4_percent);
    }
    GLCD_PrintString(bat_param->text); 
}


void SetDischargingCurrent_2(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 40);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("DiscCur 2:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 39, 102, 48, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 39, 102, 48, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 40);
    if(CHECK_DISCHRG2_CURRENT_SET)
    {
        sprintf(bat_param->text, "%umA", bat_param->discharge_current_2);
    }else
    {
        sprintf(bat_param->text, "%u%%", bat_param->discharge_current_2_percent);
    }
    GLCD_PrintString(bat_param->text); 
}

void SetDischargingCurrent_3(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 22);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("DiscCur 3:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 21, 102, 29, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 21, 102, 29, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 22);
    if(CHECK_DISCHRG3_CURRENT_SET)
    {
        sprintf(bat_param->text, "%umA", bat_param->discharge_current_3);
    }else
    {
        sprintf(bat_param->text, "%u%%", bat_param->discharge_current_3_percent);
    }
    GLCD_PrintString(bat_param->text); 
}


void SetDischargingCurrent_4 (BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 40);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("DiscCur 4:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(65, 39, 102, 48, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(65, 39, 102, 48, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(66, 40);
    if(CHECK_DISCHRG4_CURRENT_SET)
    {
        sprintf(bat_param->text, "%umA", bat_param->discharge_current_4);
    }else
    {
        sprintf(bat_param->text, "%u%%", bat_param->discharge_current_4_percent);
    }
    GLCD_PrintString(bat_param->text); 
}

/*-----------------------------------------------------------------------------*/

void VoltageDisplay (BattParameters *bat_param)
{ 
    sprintf(bat_param->text, "Vol:%u.%02u/%u.%02uV", bat_param->batt_set_voltage/100, bat_param->batt_set_voltage%100, bat_param->batt_actual_voltage/100, bat_param->batt_actual_voltage%100);
    GLCD_PrintString(bat_param->text); 
}

void CurrentDisplay (BattParameters *bat_param)
{    
    sprintf(bat_param->text, "Cur:%u/%umA", bat_param->batt_set_current,  bat_param->batt_actual_current);
    GLCD_PrintString(bat_param->text); 
}
/*-----------------------------------------------------------------------------*/
void CellCount(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 22);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("Cell Count:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(66, 21, 72, 29, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(66, 21, 72, 29, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(67, 22);
    sprintf(bat_param->text, "%u", bat_param->cell_count);
    GLCD_PrintString(bat_param->text);  
}

void CapacitanceDisplay (BattParameters *bat_param)
{ 
     
    switch(bat_param->set_cycle)
    {
        case 1:     sprintf(bat_param->text, "Cap:%04umAh", bat_param->batt_capacitance_cycle1);  break;
        case 2:     sprintf(bat_param->text, "Cap:%04u/%04umAh", bat_param->batt_capacitance_cycle1,bat_param->batt_capacitance_cycle2);  break;
        case 3:     sprintf(bat_param->text, "C:%04u/%04u/%04umAh", bat_param->batt_capacitance_cycle1,bat_param->batt_capacitance_cycle2,bat_param->batt_capacitance_cycle3);     break;
        case 4:     sprintf(bat_param->text, "C:%04u/%04u/%04u/%04u", bat_param->batt_capacitance_cycle1,bat_param->batt_capacitance_cycle2,bat_param->batt_capacitance_cycle3,bat_param->batt_capacitance_cycle4);     break;
    }
    GLCD_PrintString(bat_param->text); 
}

void CycleDisplay (BattParameters *bat_param)
{ 
    sprintf(bat_param->text, "Cycle:%u/%u", bat_param->current_cycle,bat_param->set_cycle);
    GLCD_PrintString(bat_param->text); 
}
void CycleSet(BattParameters *bat_param, uint8_t set_mode)
{   
    GLCD_GotoXY(0, 40);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("Cycle:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(35, 39, 41, 48, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(35, 39, 41, 48, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(36, 40);
    sprintf(bat_param->text, "%u",bat_param->set_cycle);
    GLCD_PrintString(bat_param->text); 
}
/*-----------------------------------------------------------------------------*/
void TimeDisplay (BattParameters *bat_param)
{
    sprintf(bat_param->text, "%02uh%02um%02us", (unsigned int)(bat_param->current_time/3600),(unsigned int)((bat_param->current_time%3600)/60),(unsigned int)(bat_param->current_time%60));
    GLCD_PrintString(bat_param->text); 
}

void SetMaxTime (BattParameters *bat_param, uint8_t set_mode)
{
    GLCD_GotoXY(0, 13);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("Max Time:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(53, 12, 90, 20, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(53, 12, 90, 20, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(54, 13);
    sprintf(bat_param->text, "%02uh%02um", bat_param->set_max_time/60, bat_param->set_max_time%60);
    GLCD_PrintString(bat_param->text); 
}
/*-----------------------------------------------------------------------------*/
void TempDisplay (BattParameters *bat_param)
{ 
    sprintf(bat_param->text, "T:%02uC", bat_param->bat_actual_temp);
    GLCD_PrintString(bat_param->text); 
}

void MemoryDisplay (BattParameters *bat_param)
{ 
    GLCD_GotoXY(73, 45);
    sprintf(bat_param->text, "BAT:%02d", bat_param->bat_id);
    GLCD_PrintString(bat_param->text); 
    GLCD_GotoXY(73, 54);
    sprintf(bat_param->text,"CYC:%02d/%02d", bat_param->current_measurment_cycle, bat_param->max_memory_cycle);
    GLCD_PrintString(bat_param->text); 
}


void SetTemp(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 22);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("Temp:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(29, 21, 48, 29, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(29, 21, 48, 29, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(30, 22);
    sprintf(bat_param->text, "%uC", bat_param->batt_max_temp);
    GLCD_PrintString(bat_param->text);  
}
/*-----------------------------------------------------------------------------*/
void TrickleVoltage(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 31);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("TrickleVol:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(66, 30, 97, 38, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(66, 30, 97, 38, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(67, 31);
    sprintf(bat_param->text, "%u.%02uV", bat_param->batt_set_trickle_voltage/100, bat_param->batt_set_trickle_voltage%100);
    GLCD_PrintString(bat_param->text); 
}
void TrickleCurrent(BattParameters *bat_param, uint8_t set_mode)
{     
    GLCD_GotoXY(0, 40);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("TrickleCur:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(66, 39, 97, 48, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(66, 39, 97, 48, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(67, 40);
    sprintf(bat_param->text, "%umA", bat_param->batt_set_trickle_current);
    GLCD_PrintString(bat_param->text); 
}
void SetCellVotage(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 13);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("Cell Volta:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(66, 12, 97, 20, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(66, 12, 97, 20, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(67, 13);
    sprintf(bat_param->text, "%u.%02uV", bat_param->batt_set_voltage/100, bat_param->batt_set_voltage%100);
    GLCD_PrintString(bat_param->text); 
}
void MinimumDischargeVoltage(BattParameters *bat_param, uint8_t set_mode)
{ 
    GLCD_GotoXY(0, 22);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("MinDiscVol:");

    if(set_mode == set_mode_edit)
    {
        GLCD_FillRectangle(66, 21, 97, 29, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);

    }else if(set_mode == set_mode_display)
    {    
        GLCD_FillRectangle(66, 21, 97, 29, GLCD_White);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    }

    GLCD_GotoXY(67, 22);
    sprintf(bat_param->text, "%u.%02uV", bat_param->batt_set_min_discharge_voltage/100, bat_param->batt_set_min_discharge_voltage%100);
    GLCD_PrintString(bat_param->text); 
}


/////////////////////////////////////////////////////////////////////////////////




void SingleBat_Menu(BattParameters *bat_param) 
{
    GLCD_Clear();
 
    GLCD_DrawRectangle(0, 0, 26, 10, GLCD_Black);
    GLCD_GotoXY(2, 2);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    batState(state_idle, battery_1);
   

    //GLCD_GotoXY(2, 2);
    //GLCD_PrintString("BAT 1");
    //GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
 
    GLCD_GotoXY(0, 13);
    VoltageDisplay(bat_param);
    GLCD_GotoXY(0, 22);
    CurrentDisplay(bat_param);
    GLCD_GotoXY(0, 31);
    CapacitanceDisplay(bat_param);
    GLCD_GotoXY(0, 40);
    CycleDisplay(bat_param);
    GLCD_GotoXY(74, 0);
    TimeDisplay(bat_param);
    GLCD_GotoXY(98, 13);
    TempDisplay(bat_param);

    MemoryDisplay(bat_param);
    
}

void Options5_Menu(BattParameters *bat_param)
{
    
     /*
     *---Cycle 1/2 Options
     * ChrgCur 1: 1000mA
     * DiscCur 1: 1000mA
     * ChrgCur 2: 1000mA/%
     * DiscCur 2: 1000mA/%
     
     */  
    
    GLCD_Clear();
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
  
    GLCD_GotoXY(35,2); 
    GLCD_PrintString("Options 5");
    
    GLCD_GotoXY(0, 13);
    SetChargingCurrent_3(bat_param, set_mode_display);     //ChrgCur 3: 
    GLCD_GotoXY(0, 22);
    SetDischargingCurrent_3(bat_param, set_mode_display);  //DiscCur 3:
    GLCD_GotoXY(0, 31);
    SetChargingCurrent_4(bat_param, set_mode_display);     //ChrgCur 4:
    GLCD_GotoXY(0, 40);
    SetDischargingCurrent_4(bat_param, set_mode_display);  //DiscCur 4:
 
   
}

void Options4_Menu(BattParameters *bat_param)
{
    
     /*
     *---Cycle 1/2 Options
     * ChrgCur 1: 1000mA
     * DiscCur 1: 1000mA
     * ChrgCur 2: 1000mA/%
     * DiscCur 2: 1000mA/%
     
     */  
    
    GLCD_Clear();
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
  
    GLCD_GotoXY(35,2); 
    GLCD_PrintString("Options 4");
    
    SetChargingCurrent_1(bat_param, set_mode_display);                    //ChrgCur 1: 
    SetDischargingCurrent_1(bat_param, set_mode_display);                 //DiscCur 1:
    SetChargingCurrent_2(bat_param, set_mode_display);     //ChrgCur 2:
    SetDischargingCurrent_2(bat_param, set_mode_display);  //DiscCur 2:
 
     
}


void Options3_Menu(BattParameters *bat_param)
{
    
   /*---Options 3
     * Max Time: 00h00m
     * MaxTemp:50C 
     * 
     *
    */  
    
    GLCD_Clear();
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
  
    GLCD_GotoXY(35,2); 
    GLCD_PrintString("Options 3");
 
    SetMaxTime(bat_param, set_mode_display);           //Max Time: 00h00m 
    SetTemp(bat_param, set_mode_display);          //MaxTemp:50C 
  
}



void Options2_Menu(BattParameters *bat_param)
{  
     /*
     *---Options 2
     * Vol: 4,22 
     * TrickleVol: 2,8V
     * TrickeCurr: 10mA
     * MinDiscVol: 2,80V
     */  
    
    GLCD_Clear();
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
  
    GLCD_GotoXY(35,2); 
    GLCD_PrintString("Options 2");
    
    SetCellVotage(bat_param,set_mode_display);             // Vol: 
    MinimumDischargeVoltage(bat_param, set_mode_display);  // MinDiscVol:
    TrickleVoltage(bat_param, set_mode_display);           // TrickleVol:
    TrickleCurrent(bat_param, set_mode_display);           // TrickeCurr: 
}

void Options1_Menu(BattParameters *bat_param)
{
    
     /*
     *  ---Options 1
     * Batt Chem: Li-Ion / PB / NiMh
     * Cell Count: 1
     * Mode: Only Charging / Chrg/Disc / Chrg/Disc/Stor
     * Cycles: 3
     */ 
         
    GLCD_Clear();
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
  
    GLCD_GotoXY(35,2); 
    GLCD_PrintString("Options 1");
    
    ChemistryDisplay(bat_param,set_mode_display);        // Batt Chem: Li-Ion / PB / NiMh
    CellCount(bat_param, set_mode_display);              // Cell Count: 1/2/3/4
    ChargerMode(bat_param, set_mode_display);            // Mode: Only Charging / Chrg/Disc / Chrg/Disc/Stor
    CycleSet(bat_param, set_mode_display);               // Cycles: 1-4   
}


void Test_function (void)
{
         GLCD_Clear();
     
     for(uint8_t i=1; i<=8; i++)
     {
         GLCD_DrawLine(0, 8*i, 127,8*i, GLCD_Black);
     }


    GLCD_FillRectangle(0, 0, 50, 63, GLCD_Black);
    GLCD_GotoXY(0, 0);
    GLCD_SetFont(Tahoma11x13, 11, 13, GLCD_Merge, GLCD_Inverted);
    GLCD_PrintString("P");
    GLCD_GotoXY(12, 4);
    GLCD_PrintString("P");
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
    GLCD_GotoXY(23, 4);
    GLCD_PrintString("U");
    GLCD_GotoXY(32, 0);
    GLCD_PrintString("K");
    GLCD_SetFont(Tahoma11x13, 11, 13, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(105, 0);
    GLCD_PrintString("P");


    GLCD_GotoXY(65, 33);
    GLCD_SetFont(Tahoma11x13, 11, 13, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_PrintString("TEST");
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(65, 55);
    // GLCD_PrintString("test");


    GLCD_GotoXY(0, 25);
    GLCD_SetFont(Tekton_Pro_Ext27x28, 27, 28, GLCD_Merge, GLCD_Inverted);
    GLCD_PrintString("N");
    GLCD_GotoXY(28, 29);
    GLCD_PrintString("R");
    GLCD_SetFont(Tekton_Pro_Ext27x28, 27, 28, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(60, 0);
    GLCD_PrintString("W]");

    GLCD_GotoXY(100, 4 * 8);
    GLCD_PrintString("N"); 
    
    GLCD_Render(); 
}
/*

void TwoBatMenu(void)
{
    const char voltage[] = "Vol:3,35V";
    const char current[] = "Cur:230mA";
    const char capacity[] = "Cap:540mAh"; //Capacity Level
    const char cycle[] = "Cycle:2/4";
    
     GLCD_GotoXY(2, 2);

    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);

    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);

    GLCD_DrawLine(63, 0, 63, 63, GLCD_Black);
    GLCD_GotoXY(67, 2);
    GLCD_PrintString("BAT 2");
    GLCD_DrawRectangle(65, 0, 98, 10, GLCD_Black);

    GLCD_GotoXY(0, 13);
    GLCD_PrintString(voltage);
    GLCD_GotoXY(0, 22);
    GLCD_PrintString(current);
    GLCD_GotoXY(0, 31);
    GLCD_PrintString(capacity);
    GLCD_GotoXY(0, 40);
    GLCD_PrintString(cycle);

    GLCD_FillRectangle(0, 51, 32, 63, GLCD_Black);
    GLCD_GotoXY(2, 54);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
    GLCD_PrintString("START");
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);

    //GLCD_DrawRectangle(0, 51, 32,63, GLCD_Black);

    GLCD_GotoXY(37, 54);
    GLCD_PrintString("MENU");
    GLCD_DrawRectangle(35, 51, 61, 63, GLCD_Black);

    GLCD_GotoXY(66, 13);
    GLCD_PrintString(voltage);
    GLCD_GotoXY(66, 22);
    GLCD_PrintString(current);
    GLCD_GotoXY(66, 31);
    GLCD_PrintString(capacity);
    GLCD_GotoXY(66, 40);
    GLCD_PrintString(cycle);
    GLCD_GotoXY(2 + 66, 54);
    GLCD_PrintString("START");

    GLCD_DrawRectangle(66, 51, 32 + 66, 63, GLCD_Black);
    GLCD_GotoXY(37 + 66, 54);
    GLCD_PrintString("MENU");
    GLCD_DrawRectangle(35 + 66, 51, 61 + 66, 63, GLCD_Black);
    
    batState(state_idle, battery_1);
    batState(state_idle, battery_2);
}

*/
void InitBattParameters (BattParameters *bat_param)
{
    
    bat_param->set_cycle = 1;
    bat_param->current_cycle = 1;
    bat_param->bat_actual_temp = 20;
    bat_param->batt_max_temp = 35;
    bat_param->bat_chem = liion;
    bat_param->bat_storage_precentage = 70;
    bat_param->selected_mode = charging_discharging;
    bat_param->cell_count = 1;
    bat_param->set_max_time = 30;
    bat_param->charge_current_1=1100;
    bat_param->charge_current_2=1200;
    bat_param->charge_current_3=1300;
    bat_param->charge_current_4=1400;
    bat_param->batt_set_voltage = 420;
    bat_param->batt_set_current = 1000;
    bat_param->batt_actual_voltage = 370;
    bat_param->batt_actual_current = 430;
    bat_param->batt_set_trickle_voltage = 270;
    bat_param->batt_set_trickle_current = 30;
    bat_param->batt_set_min_discharge_voltage = 270;
    bat_param->batt_capacitance_cycle1 = 1000;
    bat_param->batt_capacitance_cycle2 = 1200;
    bat_param->batt_capacitance_cycle3 = 1300;
    bat_param->batt_capacitance_cycle4 = 1402;
    
    
    
    for(uint8_t i=0; i<3; i++)
    {
        if(i==0)
        {   
            bat_param->settings_ptr = bat_param->liion_settings_ptr;   

        }else if(i==1)
        {
            bat_param->settings_ptr = bat_param->pb_settings_ptr;

        }else if(i==2)
        {
            bat_param->settings_ptr = bat_param->nimh_settings_ptr;
        }

        bat_param->settings_ptr->batt_max_temp = bat_param->batt_max_temp;
        bat_param->settings_ptr->bat_storage_precentage = bat_param->bat_storage_precentage;
        bat_param->settings_ptr->batt_set_current = bat_param->batt_set_current;
        bat_param->settings_ptr->batt_set_min_discharge_voltage = bat_param->batt_set_min_discharge_voltage;
        bat_param->settings_ptr->batt_set_trickle_current = bat_param->batt_set_trickle_current;
        bat_param->settings_ptr->batt_set_trickle_voltage = bat_param->batt_set_trickle_voltage;
        bat_param->settings_ptr->batt_set_voltage = bat_param->batt_set_voltage;
        bat_param->settings_ptr->cell_count = bat_param->cell_count;
        bat_param->settings_ptr->charge_current_1 = bat_param->charge_current_1;
        bat_param->settings_ptr->charge_current_2 = bat_param->charge_current_2;
        bat_param->settings_ptr->charge_current_3 = bat_param->charge_current_3;
        bat_param->settings_ptr->charge_current_4 = bat_param->charge_current_4;
        bat_param->settings_ptr->charge_current_2_percent = bat_param->charge_current_2_percent;
        bat_param->settings_ptr->charge_current_3_percent = bat_param->charge_current_3_percent;
        bat_param->settings_ptr->charge_current_4_percent = bat_param->charge_current_4_percent;
        bat_param->settings_ptr->selected_mode = bat_param->selected_mode;
        bat_param->settings_ptr->set_cycle = bat_param->set_cycle;
        bat_param->settings_ptr->set_max_time = bat_param->set_max_time;
    }

    
    
}

