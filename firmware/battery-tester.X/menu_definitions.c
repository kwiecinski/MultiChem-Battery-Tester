#include <xc.h>
#include <stdio.h>

#include "LCD-KS0108/KS0108.h"
#include "LCD-KS0108/font5x8.h"
#include "LCD-KS0108/Tahoma11x13.h"
#include "LCD-KS0108/Tekton_Pro_Ext27x28.h"

typedef struct 
{
uint8_t set_cycle, current_cycle, temp;
uint16_t batt_set_voltage, batt_set_current, batt_capacitance,
         batt_actual_voltage, batt_actual_current;
uint32_t time;

char text[15];

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

void change_start_menu(void)
{
    
}

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
    sprintf(batparam_ptr->text, "%02uh%02um%02us", (unsigned int)(batparam_ptr->time/3600),(unsigned int)((batparam_ptr->time%3600)/60),(unsigned int)(batparam_ptr->time%60));
    GLCD_PrintString(batparam_ptr->text); 
}


void TempDisplay (BattParameters *batparam_ptr)
{ 
    sprintf(batparam_ptr->text, "Temp:%02uC", batparam_ptr->temp);
    GLCD_PrintString(batparam_ptr->text); 
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


void OneBatMenu(void) 
{

    BattParameters bat_param;
    
    bat_param.batt_actual_current=1234;
    bat_param.batt_actual_voltage=418;
    bat_param.batt_capacitance=1526;
    bat_param.batt_set_current=1200;
    bat_param.batt_set_voltage=425;
    bat_param.temp=32;
    bat_param.time=35681;
    bat_param.current_cycle=3;
    bat_param.set_cycle=4;

            
    const char cycle[] = "Cycle:2/4";
    const char temp[] = "Temp:35C";
    const char time[] = "Time:1h35m";
    
    
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
    GLCD_GotoXY(68, 31);
    TimeDisplay(&bat_param);
    GLCD_GotoXY(68, 40);
    TempDisplay(&bat_param);
    
    GLCD_DrawLine(66, 31, 66, 47, GLCD_Black);
       
       
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