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


/////////////////////////////////////////////////////////////////////////////////

typedef struct 
{
uint8_t set_cycle, current_cycle, temp, bat_chem, bat_storage_precentage, bat_max_temp, bat_actual_temp;
uint16_t batt_set_voltage, batt_set_current, batt_actual_voltage, batt_actual_current,
        batt_set_trickle_voltage, bat_set_trickle_current,
        batt_set_min_discharge_voltage,
        
        batt_capacitance;
         
uint32_t set_time;

char text[15];

}BattParameters;

/////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////

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

void ChemistryDisplay(uint8_t chem_type)
{
        switch(chem_type)
        {
            case liion:     GLCD_PrintString("Li-Ion"); break;
            case pb:        GLCD_PrintString("PB"); break;
            case nimh:      GLCD_PrintString("Ni-MH"); break;
        }
}

void ChargerMode(uint8_t charge_mode)
{
    
        switch(charge_mode)
        {
            case charging:                          GLCD_PrintString("Charging"); break;
            case charging_discharging:              GLCD_PrintString("Charg/Disch"); break;
            case charging_discharging_storage:      GLCD_PrintString("Charg/Disch/Stor"); break;
        }
}
/////////////////////////////////////////////////////////////////////////////////
void VoltageDisplay (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Vol:%u.%02u/%u.%02uV", batparam_ptr->batt_set_voltage/100, batparam_ptr->batt_set_voltage%100, batparam_ptr->batt_actual_voltage/100, batparam_ptr->batt_actual_voltage%100);
    GLCD_PrintString(batparam_ptr->text); 
}

void CurrentDisplay (BattParameters *batparam_ptr)
{    
    sprintf(batparam_ptr->text, "Cur:%04u/%04umA", batparam_ptr->batt_set_current,  batparam_ptr->batt_actual_current);
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

void TimeDisplay (BattParameters *batparam_ptr)
{
    sprintf(batparam_ptr->text, "%02uh%02um%02us", (unsigned int)(batparam_ptr->set_time/3600),(unsigned int)((batparam_ptr->set_time%3600)/60),(unsigned int)(batparam_ptr->set_time%60));
    GLCD_PrintString(batparam_ptr->text); 
}


void TempDisplay (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Temp:%02uC", batparam_ptr->temp);
    GLCD_PrintString(batparam_ptr->text); 
}

/////////////////////////////////////////////////////////////////////////////////



void SingleBat_Menu(void) 
{

    BattParameters bat_param;
    
    bat_param.batt_actual_current=1234;
    bat_param.batt_actual_voltage=418;
    bat_param.batt_capacitance=1526;
    bat_param.batt_set_current=1200;
    bat_param.batt_set_voltage=425;
    bat_param.temp=32;
    bat_param.set_time=35681;
    bat_param.current_cycle=3;
    bat_param.set_cycle=4;

                
    GLCD_Clear();
 
    
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    batState(state_idle, battery_1);
   

    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
 
   
    GLCD_FillRectangle(0, 51, 32, 63, GLCD_Black);
    GLCD_GotoXY(2, 54);
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
    GLCD_PrintString("START");
    GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
    GLCD_GotoXY(37, 54);
    GLCD_PrintString("MENU");
    GLCD_DrawRectangle(35, 51, 61, 63, GLCD_Black);
    
    
    GLCD_GotoXY(0, 13);
    VoltageDisplay(&bat_param);
    GLCD_GotoXY(0, 22);
    CurrentDisplay(&bat_param);
    GLCD_GotoXY(0, 31);
    CapacitanceDisplay(&bat_param);
    GLCD_GotoXY(0, 40);
    CycleDisplay(&bat_param);
    GLCD_GotoXY(74, 0);
    TimeDisplay(&bat_param);
    GLCD_GotoXY(78, 40);
    TempDisplay(&bat_param);
    
    //GLCD_DrawLine(66, 31, 66, 47, GLCD_Black);
       
       
    GLCD_Render();
}

void Options1_Menu(void)
{
    
     /*
     *  ---Options 1
     * Batt Chem: Li-Ion / PB / NiMh
     * Cell Count: 1
     * Mode: Only Charging / Chrg/Disc / Chrg/Disc/Stor
     * Cycles: 3
     */ 
         
    GLCD_Clear();
    
    
    GLCD_GotoXY(2, 2);
    GLCD_PrintString("BAT 1");
    GLCD_DrawRectangle(0, 0, 31, 10, GLCD_Black);
  
    GLCD_GotoXY(35,2); 
    GLCD_PrintString("Options 1");
    
    GLCD_GotoXY(0, 13);
    ChemistryDisplay()
    GLCD_GotoXY(0, 22);
    CurrentDisplay(&bat_param);
    GLCD_GotoXY(0, 31);
    CapacitanceDisplay(&bat_param);
    GLCD_GotoXY(0, 40);
    
    
    
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

