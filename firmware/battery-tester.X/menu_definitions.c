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

void batState(uint8_t state, uint8_t  battery_number )
{
        switch(battery_number)
        {
            case battery_1:      GLCD_GotoXY(35,2);  break;
            case battery_2:      GLCD_GotoXY(102,2); break;
        }
    
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
            GLCD_FillRectangle(30, 30, 127, 39, GLCD_Black);
            GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        
        }else if(set_mode == set_mode_display)
        {    
            GLCD_FillRectangle(30, 30, 127, 39, GLCD_White);
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
void SetChargingCurrent_1(BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "ChrgCur 1:%umA", batparam_ptr->charge_current_1);
    GLCD_PrintString(batparam_ptr->text); 
}
void SetDischargingCurrent_1(BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "DiscCur 1:%umA", batparam_ptr->charge_current_1);
    GLCD_PrintString(batparam_ptr->text); 
}


void SetChargingCurrent_2(BattParameters *batparam_ptr, uint8_t display_type)
{ 
    if(display_type == display_in_mA)
    {
        sprintf(batparam_ptr->text, "ChrgCur 2:%umA", batparam_ptr->charge_current_2);
    }else if(display_type == display_in_percentage)
    {
        sprintf(batparam_ptr->text, "ChrgCur 2:%u%%", batparam_ptr->charge_current_2_percent);
    }
    GLCD_PrintString(batparam_ptr->text); 
}

void SetChargingCurrent_3(BattParameters *batparam_ptr, uint8_t display_type)
{ 
    if(display_type == display_in_mA)
    {
        sprintf(batparam_ptr->text, "ChrgCur 3:%umA", batparam_ptr->charge_current_3);
    }else if(display_type == display_in_percentage)
    {
        sprintf(batparam_ptr->text, "ChrgCur 3:%u%%", batparam_ptr->charge_current_3_percent);
    }
    GLCD_PrintString(batparam_ptr->text); 
}


void SetChargingCurrent_4 (BattParameters *batparam_ptr, uint8_t display_type)
{ 
    if(display_type == display_in_mA)
    {
        sprintf(batparam_ptr->text, "ChrgCur 4:%umA", batparam_ptr->charge_current_4);
    }else if(display_type == display_in_percentage)
    {
        sprintf(batparam_ptr->text, "ChrgCur 4:%u%%", batparam_ptr->charge_current_4_percent);
    }
    GLCD_PrintString(batparam_ptr->text); 
}


void SetDischargingCurrent_2(BattParameters *batparam_ptr, uint8_t display_type)
{ 
    if(display_type == display_in_mA)
    {
        sprintf(batparam_ptr->text, "DiscCur 2:%umA", batparam_ptr->discharge_current_2);
    }else if(display_type == display_in_percentage)
    {
        sprintf(batparam_ptr->text, "DiscCur 2:%u%%", batparam_ptr->discharge_current_2_percent);
    }
    GLCD_PrintString(batparam_ptr->text); 
}

void SetDischargingCurrent_3(BattParameters *batparam_ptr, uint8_t display_type)
{ 
    if(display_type == display_in_mA)
    {
        sprintf(batparam_ptr->text, "DiscCur 3:%umA", batparam_ptr->discharge_current_3);
    }else if(display_type == display_in_percentage)
    {
        sprintf(batparam_ptr->text, "DiscCur 3:%u%%", batparam_ptr->discharge_current_3_percent);
    }
    GLCD_PrintString(batparam_ptr->text); 
}


void SetDischargingCurrent_4 (BattParameters *batparam_ptr, uint8_t display_type)
{ 
    if(display_type == display_in_mA)
    {
        sprintf(batparam_ptr->text, "DiscCur 4:%umA", batparam_ptr->discharge_current_4);
    }else if(display_type == display_in_percentage)
    {
        sprintf(batparam_ptr->text, "DiscCur 4:%u%%", batparam_ptr->discharge_current_4_percent);
    }
    GLCD_PrintString(batparam_ptr->text); 
}

/*-----------------------------------------------------------------------------*/

void VoltageDisplay (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Vol:%u.%02u/%u.%02uV", batparam_ptr->batt_set_voltage/100, batparam_ptr->batt_set_voltage%100, batparam_ptr->batt_actual_voltage/100, batparam_ptr->batt_actual_voltage%100);
    GLCD_PrintString(batparam_ptr->text); 
}

void CurrentDisplay (BattParameters *batparam_ptr)
{    
    sprintf(batparam_ptr->text, "Cur:%u/%umA", batparam_ptr->batt_set_current,  batparam_ptr->batt_actual_current);
    GLCD_PrintString(batparam_ptr->text); 
}
/*-----------------------------------------------------------------------------*/
void CellCount(BattParameters *batparam_ptr, uint8_t set_mode)
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
        sprintf(batparam_ptr->text, "%u", batparam_ptr->cell_count);
        GLCD_PrintString(batparam_ptr->text); 
    
    
}

void CapacitanceDisplay (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Cap:%04umAh", batparam_ptr->batt_capacitance);
    GLCD_PrintString(batparam_ptr->text); 
}

void CycleDisplay (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Cycle:%u/%u", batparam_ptr->current_cycle,batparam_ptr->set_cycle);
    GLCD_PrintString(batparam_ptr->text); 
}
void CycleSet (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Cycle:%u",batparam_ptr->set_cycle);
    GLCD_PrintString(batparam_ptr->text); 
}
/*-----------------------------------------------------------------------------*/
void TimeDisplay (BattParameters *batparam_ptr)
{
    sprintf(batparam_ptr->text, "%02uh%02um%02us", (unsigned int)(batparam_ptr->current_time/3600),(unsigned int)((batparam_ptr->current_time%3600)/60),(unsigned int)(batparam_ptr->current_time%60));
    GLCD_PrintString(batparam_ptr->text); 
}

void SetMaxTime (BattParameters *batparam_ptr)
{
    sprintf(batparam_ptr->text, "Max Time: %02uh%02um", (unsigned int)(batparam_ptr->set_time/3600),(unsigned int)((batparam_ptr->set_time%3600)/60));
    GLCD_PrintString(batparam_ptr->text); 
}
/*-----------------------------------------------------------------------------*/
void TempDisplay (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Temp:%02uC", batparam_ptr->bat_actual_temp);
    GLCD_PrintString(batparam_ptr->text); 
}

void SetTemp(BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Temp:%02uC", batparam_ptr->bat_max_temp);
    GLCD_PrintString(batparam_ptr->text); 
}
/*-----------------------------------------------------------------------------*/
void TrickleVoltage(BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "TrickleVol:%u.%02uV", batparam_ptr->batt_set_trickle_voltage/100, batparam_ptr->batt_set_trickle_voltage%100);
    GLCD_PrintString(batparam_ptr->text); 
}
void TrickleCurrent(BattParameters *batparam_ptr)
{     
    sprintf(batparam_ptr->text, "TrickleCur:%umA", batparam_ptr->bat_set_trickle_current);
    GLCD_PrintString(batparam_ptr->text); 
}
void SetCellVotage(BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Cell Volta:%u.%02uV", batparam_ptr->batt_set_voltage/100, batparam_ptr->batt_set_voltage%100);
    GLCD_PrintString(batparam_ptr->text); 
}
void MinimumDischargeVoltage(BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "MinDiscVol:%u.%02uV", batparam_ptr->batt_set_min_discharge_voltage/100, batparam_ptr->batt_set_min_discharge_voltage%100);
    GLCD_PrintString(batparam_ptr->text); 
}


/////////////////////////////////////////////////////////////////////////////////




void SingleBat_Menu(BattParameters *bat_param) 
{
    GLCD_Clear();
 
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    batState(state_idle, battery_1);
   

    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
 
   /*
    GLCD_FillRectangle(0, 51, 32, 63, GLCD_Black);
    GLCD_GotoXY(2, 54);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
    GLCD_PrintString("START");
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(37, 54);
    GLCD_PrintString("MENU");
    GLCD_DrawRectangle(35, 51, 61, 63, GLCD_Black);
    */
    
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
    GLCD_GotoXY(78, 40);
    TempDisplay(bat_param);
    
    //GLCD_DrawLine(66, 31, 66, 47, GLCD_Black);
       
       
    GLCD_Render();
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
    SetChargingCurrent_3(bat_param, display_in_mA);     //ChrgCur 3: 
    GLCD_GotoXY(0, 22);
    SetDischargingCurrent_3(bat_param, display_in_mA);  //DiscCur 3:
    GLCD_GotoXY(0, 31);
    SetChargingCurrent_4(bat_param, display_in_mA);     //ChrgCur 4:
    GLCD_GotoXY(0, 40);
    SetDischargingCurrent_4(bat_param, display_in_mA);  //DiscCur 4:
 
    GLCD_Render(); 
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
    
    GLCD_GotoXY(0, 13);
    SetChargingCurrent_1(bat_param);                    //ChrgCur 1: 
    GLCD_GotoXY(0, 22);
    SetDischargingCurrent_1(bat_param);                 //DiscCur 1:
    GLCD_GotoXY(0, 31);
    SetChargingCurrent_2(bat_param, display_in_mA);     //ChrgCur 2:
    GLCD_GotoXY(0, 40);
    SetDischargingCurrent_2(bat_param, display_in_mA);  //DiscCur 2:
 
    GLCD_Render(); 
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
    
    GLCD_GotoXY(0, 13);
    SetMaxTime(bat_param);           //Max Time: 00h00m 
    GLCD_GotoXY(0, 22);
    SetTemp(bat_param);          //MaxTemp:50C 
 
    GLCD_Render(); 
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
    
    GLCD_GotoXY(0, 13);
    SetCellVotage(bat_param);           //Vol: 
    GLCD_GotoXY(0, 22);
    MinimumDischargeVoltage(bat_param); //MinDiscVol:
    GLCD_GotoXY(0, 31);
    TrickleVoltage(bat_param);          //TrickleVol:
    GLCD_GotoXY(0, 40);
    TrickleCurrent(bat_param);          //TrickeCurr: 
 
    GLCD_Render(); 
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
    
    ChemistryDisplay(bat_param,set_mode_display);          //Batt Chem: Li-Ion / PB / NiMh
    CellCount(bat_param, set_mode_display);      //Cell Count: 1/2/3/4
    ChargerMode(bat_param, set_mode_display);     //Mode: Only Charging / Chrg/Disc / Chrg/Disc/Stor
    GLCD_GotoXY(0, 40);
    CycleSet(bat_param);       //Cycles: 1-4
 
    GLCD_Render(); 
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


void InitBattParameters (BattParameters *bat_param)
{
        bat_param->set_cycle = 1;
        bat_param->current_cycle = 1;
        bat_param->bat_actual_temp = 20;
        bat_param->bat_max_temp = 35;
        bat_param->bat_chem = liion;
        bat_param->bat_storage_precentage = 70;
        bat_param->selected_mode = charging_discharging;
        bat_param->cell_count = 1;
        
        bat_param->charge_current_1=1234;
        bat_param->charge_current_1=2234;
        bat_param->charge_current_1=3234;
        bat_param->charge_current_1=4234;
        bat_param->batt_set_voltage = 420;
        bat_param->batt_set_current = 1000;
        bat_param->batt_actual_voltage = 370;
        bat_param->batt_actual_current = 430;
        bat_param->batt_set_trickle_voltage = 270;
        bat_param->bat_set_trickle_current = 30;
        bat_param->batt_set_min_discharge_voltage = 270;
        bat_param->batt_capacitance = 1000;
}

