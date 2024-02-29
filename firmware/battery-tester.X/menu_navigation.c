#include <xc.h>
#include <stdio.h>

#include "LCD-KS0108/KS0108.h"
#include "LCD-KS0108/font5x8.h"
#include "LCD-KS0108/Tahoma11x13.h"
#include "LCD-KS0108/Tekton_Pro_Ext27x28.h"
#include "menu_definitions.h"
#include "menu_navigation.h"
#include "button.h"

#define MAX_CELL_COUNT 5
#define MAX_CYCLE_COUNT 4
#define MAX_CELL_VOLTAGE 430
#define MIN_CELL_VOLTAGE 410
#define MIN_MIN_DISCHARGE_VOLTAGE 250
#define MAX_MIN_DISCHARGE_VOLTAGE 320
#define MIN_TRICKLE_VOLTAGE 180
#define MAX_TRICKLE_VOLTAGE 250
#define MIN_TRICKLE_CURRENT 5
#define MAX_TRICKLE_CURRENT 50
#define MAX_TEMP            45
#define MIN_TEMP            5
#define MAX_TIME            60*48
#define MIN_TIME            60

#define MAX_CHARGE_CURRENT      3000
#define MAX_DISCHARGE_CURRENT   3000
#define MIN_CHARGE_CURRENT      10
#define MIN_DISCHARGE_CURRENT   10


#define MAX_PRECENT_CHARGE_VAL         100
#define MIN_PRECENT_CHARGE_VAL         1
#define MAX_PRECENT_DISCHARGE_VAL         300
#define MIN_PRECENT_DISCHARGE_VAL         5

enum settings1_navigation
{
    next,
    back,
    save_exit,
    disable_start_next_menu
};

enum settings_horizontal_navigation
{
    start_navigation,
    param_1,
    param_2,
    param_3,
    param_4,
    menu_start_next,
    end_navigation,
       
};

enum menu_type
{
    main_menu,
    settings1,
    settings2,
    settings3,
    settings4,
    settings5
        
};

void Handle_Main_MenuNavigation(uint8_t option);
void Handle_Setting1_MenuNavigation(uint8_t option);
void Handle_Setting23_MenuNavigation(uint8_t option);
void Handle_Setting5_MenuNavigation(uint8_t option);
/*
             if(Button_EventGet(UP_SW))
            {
                 param_pos++;
            }else if(Button_EventGet(DOWN_SW))
            {
                 param_pos--;
            }

            if( param_pos==0)
            {
                 param_pos = 4;
                 
            }else if(param_pos==5)
            {
                 param_pos = 1; 
                 
            }else if(param_pos>6)
            {
                 param_pos=1;
            }


            if(param_pos == 1)
            {
                bat_param->cell_count = 1;
                bat_param->bat_chem = liion;
                bat_param->selected_mode = charging;
                   bat_param->set_cycle = 1;
                ChemistryDisplay(bat_param, set_mode_edit);
                CellCount(bat_param, set_mode_edit);
                ChargerMode(bat_param, set_mode_edit);
                CycleSet(bat_param, set_mode_display);
                
            }else if (param_pos == 2)
            {
                bat_param->cell_count = 2;
                bat_param->bat_chem = pb;
                bat_param->selected_mode = charging_discharging;
                bat_param->set_cycle = 2;
           
                ChemistryDisplay(bat_param, set_mode_edit);
                CellCount(bat_param, set_mode_edit);
                ChargerMode(bat_param, set_mode_edit);
                CycleSet(bat_param, set_mode_edit);
                
            }else if (param_pos == 3)
            {
                bat_param->cell_count = 3;
                bat_param->bat_chem = nimh;
                bat_param->selected_mode = charging_discharging_storage;
                
                bat_param->set_cycle = 3;
                ChemistryDisplay(bat_param, set_mode_edit);
                CellCount(bat_param, set_mode_edit);
                ChargerMode(bat_param, set_mode_edit);
                CycleSet(bat_param, set_mode_edit);
                
                  
                
            }else if (param_pos == 4)
            {
                bat_param->cell_count = 4;
                ChemistryDisplay(bat_param, set_mode_display);
                CellCount(bat_param, set_mode_display);
                ChargerMode(bat_param, set_mode_display);
                CycleSet(bat_param, set_mode_display);
            }
            
 
 
 */
void Menu(BattParameters *bat_param)
{
    
    static uint8_t menu_change=start, menu_type=main_menu, menu_selection, menu_init=0, 
                   menu_pos=0, param_pos=menu_start_next, switch_param;
    uint16_t val;
          
    Button_Update();
    
    if (Button_EventGet(UP_SW) || Button_EventGet(DOWN_SW)) 
    {
        if(menu_type==settings1)
        {
            
            if(Button_EventGet(UP_SW))
            {
                 param_pos--;
            }else if(Button_EventGet(DOWN_SW))
            {
                 param_pos++;
            }
            
            if(param_pos==start_navigation)
            {
                 param_pos = menu_start_next;  
                 
            }else if(param_pos>=end_navigation)
            {
                 param_pos = param_1;  
            }
            
            if(param_pos == param_1)
            {
                Handle_Setting1_MenuNavigation(disable_start_next_menu);
                ChemistryDisplay(bat_param, set_mode_edit);
                CellCount(bat_param, set_mode_display);
                ChargerMode(bat_param, set_mode_display);
                CycleSet(bat_param, set_mode_display);
                
            }else if(param_pos == param_2)
            {
                Handle_Setting1_MenuNavigation(disable_start_next_menu);
                ChemistryDisplay(bat_param, set_mode_display);
                CellCount(bat_param, set_mode_edit);
                ChargerMode(bat_param, set_mode_display);
                CycleSet(bat_param, set_mode_display);
                
            }else if(param_pos == param_3)
            {
                Handle_Setting1_MenuNavigation(disable_start_next_menu);
                ChemistryDisplay(bat_param, set_mode_display);
                CellCount(bat_param, set_mode_display);
                ChargerMode(bat_param, set_mode_edit);
                CycleSet(bat_param, set_mode_display);
                
            }else if(param_pos == param_4)
            {
                Handle_Setting1_MenuNavigation(disable_start_next_menu);
                ChemistryDisplay(bat_param, set_mode_display);
                CellCount(bat_param, set_mode_display);
                ChargerMode(bat_param, set_mode_display);
                CycleSet(bat_param, set_mode_edit);
                
            }else if(param_pos == menu_start_next)
            {
                ChemistryDisplay(bat_param, set_mode_display);
                CellCount(bat_param, set_mode_display);
                Handle_Setting1_MenuNavigation(menu_selection);
                ChargerMode(bat_param, set_mode_display);
                CycleSet(bat_param, set_mode_display);
            }       
        }else if(menu_type==settings2)
        {
             if(Button_EventGet(UP_SW))
            {
                 param_pos--;
            }else if(Button_EventGet(DOWN_SW))
            {
                 param_pos++;
            }
            
            if(param_pos==start_navigation)
            {
                 param_pos = menu_start_next;  
                 
            }else if(param_pos>=end_navigation)
            {
                 param_pos = param_1;  
            }
             
            if(param_pos == param_1)
            {
                SetCellVotage(bat_param, set_mode_edit);
                MinimumDischargeVoltage(bat_param, set_mode_display);
                TrickleCurrent(bat_param, set_mode_display);
                TrickleVoltage(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
            }else if(param_pos == param_2)
            {
                SetCellVotage(bat_param, set_mode_display);
                MinimumDischargeVoltage(bat_param, set_mode_edit);
                TrickleCurrent(bat_param, set_mode_display);
                TrickleVoltage(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
            }else if(param_pos == param_3)
            {
                SetCellVotage(bat_param, set_mode_display);
                MinimumDischargeVoltage(bat_param, set_mode_display);
                TrickleCurrent(bat_param, set_mode_display);
                TrickleVoltage(bat_param, set_mode_edit);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
            }else if(param_pos == param_4)
            {
                SetCellVotage(bat_param, set_mode_display);
                MinimumDischargeVoltage(bat_param, set_mode_display);
                TrickleCurrent(bat_param, set_mode_edit);
                TrickleVoltage(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
            }else if(param_pos == menu_start_next)
            {
                SetCellVotage(bat_param, set_mode_display);
                MinimumDischargeVoltage(bat_param, set_mode_display);
                TrickleCurrent(bat_param, set_mode_display);
                TrickleVoltage(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(menu_selection);
            }    
        }else if(menu_type==settings3)
        {
            
            if(Button_EventGet(UP_SW))
            {
                 param_pos--;
            }else if(Button_EventGet(DOWN_SW))
            {
                 param_pos++;
            }
            
            if(param_pos == param_3)
            {
                param_pos = menu_start_next;
            }else if(param_pos == param_4)
            {
                param_pos = param_2; 
            }
            
            
            if(param_pos==start_navigation)
            {
                 param_pos = menu_start_next;  
                 
            }else if(param_pos>=end_navigation)
            {
                 param_pos = param_1;  
            }
                         
            if(param_pos == param_1)
            {
                SetMaxTime (bat_param, set_mode_edit);
                SetTemp(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
            }else if(param_pos == param_2)
            {
                SetMaxTime (bat_param, set_mode_display);
                SetTemp(bat_param, set_mode_edit);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
            }else if(param_pos == menu_start_next)
            {
                SetMaxTime (bat_param, set_mode_display);
                SetTemp(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(menu_selection);
            }     
        }else if(menu_type==settings4)
        {
            
            if(Button_EventGet(UP_SW))
            {
                 param_pos--;
            }else if(Button_EventGet(DOWN_SW))
            {
                 param_pos++;
            }

            if(param_pos==start_navigation)
            {
                 param_pos = menu_start_next;  
                 
            }else if(param_pos>=end_navigation)
            {
                 param_pos = param_1;  
            }
                         
            if(param_pos == param_1)
            {
                SetChargingCurrent_1(bat_param, set_mode_edit);
                SetDischargingCurrent_1(bat_param, set_mode_display);
                SetChargingCurrent_2(bat_param, set_mode_display);
                SetDischargingCurrent_2(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == param_2)
            {
                SetChargingCurrent_1(bat_param, set_mode_display);
                SetDischargingCurrent_1(bat_param, set_mode_edit);
                SetChargingCurrent_2(bat_param, set_mode_display);
                SetDischargingCurrent_2(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == param_3)
            {
                SetChargingCurrent_1(bat_param, set_mode_display);
                SetDischargingCurrent_1(bat_param, set_mode_display);
                SetChargingCurrent_2(bat_param, set_mode_edit);
                SetDischargingCurrent_2(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == param_4)
            {
                SetChargingCurrent_1(bat_param, set_mode_display);
                SetDischargingCurrent_1(bat_param, set_mode_display);
                SetChargingCurrent_2(bat_param, set_mode_display);
                SetDischargingCurrent_2(bat_param, set_mode_edit);
                Handle_Setting23_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == menu_start_next)
            {
                SetChargingCurrent_1(bat_param, set_mode_display);
                SetDischargingCurrent_1(bat_param, set_mode_display);
                SetChargingCurrent_2(bat_param, set_mode_display);
                SetDischargingCurrent_2(bat_param, set_mode_display);
                Handle_Setting23_MenuNavigation(menu_selection);
            }    
        }
        else if(menu_type==settings5)
        {
            
            if(Button_EventGet(UP_SW))
            {
                 param_pos--;
            }else if(Button_EventGet(DOWN_SW))
            {
                 param_pos++;
            }

            if(param_pos==start_navigation)
            {
                 param_pos = menu_start_next;  
                 
            }else if(param_pos>=end_navigation)
            {
                 param_pos = param_1;  
            }
                         
            if(param_pos == param_1)
            {
                SetChargingCurrent_3(bat_param, set_mode_edit);
                SetDischargingCurrent_3(bat_param, set_mode_display);
                SetChargingCurrent_4(bat_param, set_mode_display);
                SetDischargingCurrent_4(bat_param, set_mode_display);
                Handle_Setting5_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == param_2)
            {
                SetChargingCurrent_3(bat_param, set_mode_display);
                SetDischargingCurrent_3(bat_param, set_mode_edit);
                SetChargingCurrent_4(bat_param, set_mode_display);
                SetDischargingCurrent_4(bat_param, set_mode_display);
                Handle_Setting5_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == param_3)
            {
                SetChargingCurrent_3(bat_param, set_mode_display);
                SetDischargingCurrent_3(bat_param, set_mode_display);
                SetChargingCurrent_4(bat_param, set_mode_edit);
                SetDischargingCurrent_4(bat_param, set_mode_display);
                Handle_Setting5_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == param_4)
            {
                SetChargingCurrent_3(bat_param, set_mode_display);
                SetDischargingCurrent_3(bat_param, set_mode_display);
                SetChargingCurrent_4(bat_param, set_mode_display);
                SetDischargingCurrent_4(bat_param, set_mode_edit);
                Handle_Setting5_MenuNavigation(disable_start_next_menu);
                
            }else if(param_pos == menu_start_next)
            {
                SetChargingCurrent_3(bat_param, set_mode_display);
                SetDischargingCurrent_3(bat_param, set_mode_display);
                SetChargingCurrent_4(bat_param, set_mode_display);
                SetDischargingCurrent_4(bat_param, set_mode_display);
                Handle_Setting5_MenuNavigation(menu_selection);
            }    
        }
        GLCD_Render();
        Button_Event_Reset();  
        
    }// if (Button_EventGet(UP_SW) || Button_EventGet(DOWN_SW)) 
    
    
    if (Button_EventGet(LEFT_SW) || Button_EventGet(RIGHT_SW) || menu_init==0 ) 
    {
        menu_init=1;
        /************************************************************************/
        if(menu_type==main_menu)
        {
            if (menu_change == start) 
            {
                Handle_Main_MenuNavigation(menu_change);
                menu_change = menu;
                menu_selection=start;

            }else if (menu_change == menu) 
            {
                Handle_Main_MenuNavigation(menu_change);
                menu_change = start;
                menu_selection=menu;
            }  
       /************************************************************************/
        }else if(menu_type==settings1)
        {
            if(param_pos == menu_start_next)
            {
           
                if (menu_change == save_exit) 
                {
                    Handle_Setting1_MenuNavigation(menu_change);
                    menu_change = next;
                    menu_selection=save_exit;

                }else if (menu_change == next) 
                {
                    Handle_Setting1_MenuNavigation(menu_change);
                    menu_change = save_exit;
                    menu_selection=next;
                }  

            }else if(param_pos == param_1)
            {

                  switch(bat_param->bat_chem)
                {
                    case liion:     switch_param=1;      break;
                    case pb:        switch_param=2;      break;
                    case nimh:      switch_param=3;      break;
                }  
                
                if(Button_EventGet(RIGHT_SW))
                {
                     switch_param++;
                }else if(Button_EventGet(LEFT_SW))
                {
                     switch_param--;
                }

                if(switch_param==0)
                {
                     switch_param = 3;  

                }else if(switch_param>=4)
                {
                     switch_param = 1;  
                }
            

                switch(switch_param)
                {
                    case 1:     bat_param->bat_chem = liion; break;
                    case 2:     bat_param->bat_chem = pb;    break;
                    case 3:     bat_param->bat_chem = nimh;  break;
                }       

                ChemistryDisplay(bat_param, set_mode_edit);  
                
            }else if(param_pos == param_2)
            {

                if(Button_EventGet(RIGHT_SW))
                {
                     bat_param->cell_count++;
                }else if(Button_EventGet(LEFT_SW))
                {
                     bat_param->cell_count--;
                }

                if(bat_param->cell_count==0)
                {
                     bat_param->cell_count = MAX_CELL_COUNT;  

                }else if(bat_param->cell_count>=MAX_CELL_COUNT+1)
                {
                     bat_param->cell_count = 1;  
                }

                CellCount(bat_param, set_mode_edit);
                
            }else if(param_pos == param_3)
            {

                switch(bat_param->selected_mode)
                {
                    case charging:                      switch_param=1;      break;
                    case charging_discharging:          switch_param=2;      break;
                    case charging_discharging_storage:  switch_param=3;      break;
                }  
                
                if(Button_EventGet(RIGHT_SW))
                {
                     switch_param++;
                }else if(Button_EventGet(LEFT_SW))
                {
                     switch_param--;
                }

                if(switch_param==0)
                {
                     switch_param = 3;  

                }else if(switch_param>=4)
                {
                     switch_param = 1;  
                }

                switch(switch_param)
                {
                    case 1:     bat_param->selected_mode =  charging;                        break;
                    case 2:     bat_param->selected_mode =  charging_discharging;            break;
                    case 3:     bat_param->selected_mode =  charging_discharging_storage;    break;
                }       

                ChargerMode(bat_param, set_mode_edit);
                
            }else if(param_pos == param_4)
            {

                if(Button_EventGet(RIGHT_SW))
                {
                     bat_param->set_cycle++;
                }else if(Button_EventGet(LEFT_SW))
                {
                     bat_param->set_cycle--;
                }

                if(bat_param->set_cycle==0)
                {
                     bat_param->set_cycle = MAX_CYCLE_COUNT;  

                }else if(bat_param->set_cycle>=MAX_CYCLE_COUNT+1)
                {
                     bat_param->set_cycle = 1;  
                }
                
                CycleSet(bat_param, set_mode_edit);
            }
      /************************************************************************/  
        }else if(menu_type==settings2)
        {
            if(param_pos == menu_start_next)
            {
                if(Button_EventGet(LEFT_SW))
                {
                     menu_pos--;
                }else if(Button_EventGet(RIGHT_SW))
                {
                     menu_pos++;
                }
            
                if(menu_pos==0)
                {
                    menu_pos = 3;
                }else if(menu_pos==4)
                {
                    menu_pos = 1; 
                }else if(menu_pos>4)
                {
                    menu_pos=2;
                }
                
                switch(menu_pos)
                {
                    case 1: menu_change = back;         break;
                    case 2: menu_change = next;         break;
                    case 3: menu_change = save_exit;    break;
                }

                if (menu_change == back) 
                {
                    Handle_Setting23_MenuNavigation(menu_change); 
                    menu_selection=back;
                }else if (menu_change == next) 
                {
                    Handle_Setting23_MenuNavigation(menu_change);
                    menu_selection=next;   
                }else if (menu_change == save_exit) 
                {
                    Handle_Setting23_MenuNavigation(menu_change);
                    menu_selection= save_exit;
                }  
            }else if(param_pos == param_1)
            {
            
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->batt_set_voltage=bat_param->batt_set_voltage+5;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->batt_set_voltage=bat_param->batt_set_voltage-5;
                }

                if(bat_param->batt_set_voltage<MIN_CELL_VOLTAGE-1)
                {
                    bat_param->batt_set_voltage = MAX_CELL_VOLTAGE;  

                }else if(bat_param->batt_set_voltage>MAX_CELL_VOLTAGE+1)
                {
                    bat_param->batt_set_voltage = MIN_CELL_VOLTAGE;  
                }
                
                 SetCellVotage(bat_param, set_mode_edit);
            }else if(param_pos == param_2)
            {
            
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->batt_set_min_discharge_voltage=bat_param->batt_set_min_discharge_voltage+5;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->batt_set_min_discharge_voltage=bat_param->batt_set_min_discharge_voltage-5;
                }

                if(bat_param->batt_set_min_discharge_voltage<MIN_MIN_DISCHARGE_VOLTAGE-1)
                {
                    bat_param->batt_set_min_discharge_voltage = MAX_MIN_DISCHARGE_VOLTAGE;  

                }else if(bat_param->batt_set_min_discharge_voltage>MAX_MIN_DISCHARGE_VOLTAGE+1)
                {
                    bat_param->batt_set_min_discharge_voltage = MIN_MIN_DISCHARGE_VOLTAGE;  
                }
                
                 MinimumDischargeVoltage(bat_param, set_mode_edit);
                 
            }else if(param_pos == param_3)
            {
            
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->batt_set_trickle_voltage=bat_param->batt_set_trickle_voltage+5;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->batt_set_trickle_voltage=bat_param->batt_set_trickle_voltage-5;
                }

                if(bat_param->batt_set_trickle_voltage<MIN_TRICKLE_VOLTAGE-1)
                {
                    bat_param->batt_set_trickle_voltage = MAX_TRICKLE_VOLTAGE;  

                }else if(bat_param->batt_set_trickle_voltage>MAX_TRICKLE_VOLTAGE+1)
                {
                    bat_param->batt_set_trickle_voltage = MIN_TRICKLE_VOLTAGE;  
                }
                
                 TrickleVoltage(bat_param, set_mode_edit);
                 
            }else if(param_pos == param_4)
            {
            
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->bat_set_trickle_current=bat_param->bat_set_trickle_current+5;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->bat_set_trickle_current=bat_param->bat_set_trickle_current-5;
                }

                if(bat_param->bat_set_trickle_current<MIN_TRICKLE_CURRENT-1)
                {
                    bat_param->bat_set_trickle_current = MAX_TRICKLE_CURRENT;  

                }else if(bat_param->bat_set_trickle_current>MAX_TRICKLE_CURRENT+1)
                {
                    bat_param->bat_set_trickle_current = MIN_TRICKLE_CURRENT;  
                }
                TrickleCurrent(bat_param, set_mode_edit);
            }
      /************************************************************************/
        }else if(menu_type==settings3)
        {
            if(param_pos == menu_start_next)
            {
                if(Button_EventGet(LEFT_SW))
                {
                     menu_pos--;
                }else if(Button_EventGet(RIGHT_SW))
                {
                     menu_pos++;
                }

                if(menu_pos==0)
                {
                    menu_pos = 3;
                }else if(menu_pos==4)
                {
                    menu_pos = 1; 
                }else if(menu_pos>4)
                {
                    menu_pos=2;
                }

                switch(menu_pos)
                {
                    case 1: menu_change = back;         break;
                    case 2: menu_change = next;         break;
                    case 3: menu_change = save_exit;    break;
                }

                if (menu_change == back) 
                {
                    Handle_Setting23_MenuNavigation(menu_change); 
                    menu_selection=back;
                }else if (menu_change == next) 
                {
                    Handle_Setting23_MenuNavigation(menu_change);
                    menu_selection=next;   
                }else if (menu_change == save_exit) 
                {
                    Handle_Setting23_MenuNavigation(menu_change);
                    menu_selection= save_exit;
                }  
            
            
            }else if(param_pos == param_1)
            {
            
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->set_time=bat_param->set_time+30;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->set_time=bat_param->set_time-30;
                }

                if(bat_param->set_time<MIN_TIME)
                {
                    bat_param->set_time = MAX_TIME;  

                }else if(bat_param->set_time>MAX_TIME)
                {
                    bat_param->set_time = MIN_TIME;  
                }
                
                 SetMaxTime(bat_param, set_mode_edit);
            }else if(param_pos == param_2)
            {
            
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->bat_max_temp=bat_param->bat_max_temp+5;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->bat_max_temp=bat_param->bat_max_temp-5;
                }

                if(bat_param->bat_max_temp<MIN_TEMP-1)
                {
                    bat_param->bat_max_temp = MAX_TEMP;  

                }else if(bat_param->bat_max_temp>MAX_TEMP+1)
                {
                    bat_param->bat_max_temp = MIN_TEMP;  
                }
                
                 SetTemp(bat_param, set_mode_edit);
                 
            }
      /************************************************************************/
        }else if(menu_type==settings4)
        { 
            if(param_pos == menu_start_next)
            {
            
                if(Button_EventGet(LEFT_SW))
                {
                     menu_pos--;
                }else if(Button_EventGet(RIGHT_SW))
                {
                     menu_pos++;
                }
            
                if(menu_pos==0)
                {
                    menu_pos = 3;
                }else if(menu_pos==4)
                {
                    menu_pos = 1; 
                }else if(menu_pos>4)
                {
                    menu_pos=2;
                }
                
                switch(menu_pos)
                {
                    case 1: menu_change = back;         break;
                    case 2: menu_change = next;         break;
                    case 3: menu_change = save_exit;    break;
                }

                if (menu_change == back) 
                {
                    Handle_Setting23_MenuNavigation(menu_change); 
                    menu_selection=back;
                }else if (menu_change == next) 
                {
                    Handle_Setting23_MenuNavigation(menu_change);
                    menu_selection=next;   
                }else if (menu_change == save_exit) 
                {
                    Handle_Setting23_MenuNavigation(menu_change);
                    menu_selection= save_exit;
                } 
            }

            if(param_pos == param_1)
            {
                
                if(bat_param->charge_current_1>=1000)
                {
                    val=100;
                }else if(bat_param->charge_current_1>=100)
                {
                    val=50;
                }else if(bat_param->charge_current_1<100)
                {
                    val=10;
                }
                
                
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->charge_current_1=bat_param->charge_current_1+val;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->charge_current_1=bat_param->charge_current_1-val;
                }

                if(bat_param->charge_current_1<MIN_CHARGE_CURRENT)
                {
                    bat_param->charge_current_1 = MAX_CHARGE_CURRENT;  

                }else if(bat_param->charge_current_1>MAX_CHARGE_CURRENT)
                {
                    bat_param->charge_current_1 = MIN_CHARGE_CURRENT;  
                }
                
                 SetChargingCurrent_1(bat_param, set_mode_edit);
            }
            
            if(param_pos == param_2)
            {
                
                if(bat_param->discharge_current_1>=1000)
                {
                    val=100;
                }else if(bat_param->discharge_current_1>=100)
                {
                    val=50;
                }else if(bat_param->discharge_current_1<100)
                {
                    val=10;
                }
                
                
                if(Button_EventGet(RIGHT_SW))
                {
                    bat_param->discharge_current_1=bat_param->discharge_current_1+val;
                }else if(Button_EventGet(LEFT_SW))
                {
                    bat_param->discharge_current_1=bat_param->discharge_current_1-val;
                }

                if(bat_param->discharge_current_1<MIN_CHARGE_CURRENT)
                {
                    bat_param->discharge_current_1 = MAX_CHARGE_CURRENT;  

                }else if(bat_param->discharge_current_1>MAX_CHARGE_CURRENT)
                {
                    bat_param->discharge_current_1 = MIN_CHARGE_CURRENT;  
                }
                
                 SetDischargingCurrent_1(bat_param, set_mode_edit);
            }
            if(param_pos == param_3)
            {
                if(CHECK_CHRG2_CURRENT_SET)
                {
                     if(bat_param->charge_current_2>=1000)
                    {
                        val=100;
                    }else if(bat_param->charge_current_2>=100)
                    {
                        val=50;
                    }else
                    {
                        val=10;
                    }
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->charge_current_2=bat_param->charge_current_2+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->charge_current_2=bat_param->charge_current_2-val;
                    }

                    if(bat_param->charge_current_2<MIN_CHARGE_CURRENT)
                    {
                        bat_param->charge_current_2 = MAX_CHARGE_CURRENT;  

                    }else if(bat_param->charge_current_2>MAX_CHARGE_CURRENT)
                    {
                        bat_param->charge_current_2 = MIN_CHARGE_CURRENT;  
                    }    
                }else
                {
                    if(bat_param->charge_current_2_percent>=50)
                    {
                        val=10;
                    }else if(bat_param->charge_current_2_percent>=10)
                    {
                        val=5;
                    }else 
                    {
                        val=1;
                    }
                    
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->charge_current_2_percent=bat_param->charge_current_2_percent+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->charge_current_2_percent=bat_param->charge_current_2_percent-val;
                    }

                    if(bat_param->charge_current_2_percent<MIN_PRECENT_CHARGE_VAL)
                    {
                        bat_param->charge_current_2_percent = MAX_PRECENT_CHARGE_VAL;  

                    }else if(bat_param->charge_current_2_percent>MAX_PRECENT_CHARGE_VAL)
                    {
                        bat_param->charge_current_2_percent = MIN_PRECENT_CHARGE_VAL;  
                    }      
                }
                SetChargingCurrent_2(bat_param, set_mode_edit);
            } if(param_pos == param_4)
            {
                if(CHECK_DISCHRG2_CURRENT_SET)
                {
                     if(bat_param->discharge_current_2>=1000)
                    {
                        val=100;
                    }else if(bat_param->discharge_current_2>=100)
                    {
                        val=50;
                    }else
                    {
                        val=10;
                    }
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->discharge_current_2=bat_param->discharge_current_2+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->discharge_current_2=bat_param->discharge_current_2-val;
                    }

                    if(bat_param->discharge_current_2<MIN_DISCHARGE_CURRENT)
                    {
                        bat_param->discharge_current_2 = MAX_DISCHARGE_CURRENT;  

                    }else if(bat_param->discharge_current_2>MAX_DISCHARGE_CURRENT)
                    {
                        bat_param->discharge_current_2 = MIN_DISCHARGE_CURRENT;  
                    }    
                }else
                {
                    if(bat_param->discharge_current_2_percent>=50)
                    {
                        val=10;
                    }else if(bat_param->discharge_current_2_percent>=10)
                    {
                        val=5;
                    }else 
                    {
                        val=1;
                    }
                    
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->discharge_current_2_percent=bat_param->discharge_current_2_percent+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->discharge_current_2_percent=bat_param->discharge_current_2_percent-val;
                    }

                    if(bat_param->discharge_current_2_percent<MIN_PRECENT_DISCHARGE_VAL)
                    {
                        bat_param->discharge_current_2_percent = MAX_PRECENT_DISCHARGE_VAL;  

                    }else if(bat_param->discharge_current_2_percent>MAX_PRECENT_DISCHARGE_VAL)
                    {
                        bat_param->discharge_current_2_percent = MIN_PRECENT_DISCHARGE_VAL;  
                    }      
                }
                SetDischargingCurrent_2(bat_param, set_mode_edit);
            }
      /************************************************************************/
        }else if(menu_type==settings5)
        {
            if(param_pos == menu_start_next)
            {
                if (menu_change == back) 
                {
                    Handle_Setting5_MenuNavigation(menu_change); 
                    menu_selection=back;  
                    menu_change = save_exit;
                }else if (menu_change == save_exit) 
                {
                    Handle_Setting5_MenuNavigation(menu_change);
                    menu_selection = save_exit;
                    menu_change = back;
                }  
            }
            if(param_pos == param_1)
            {
                if(CHECK_CHRG3_CURRENT_SET)
                {
                     if(bat_param->charge_current_3>=1000)
                    {
                        val=100;
                    }else if(bat_param->charge_current_3>=100)
                    {
                        val=50;
                    }else
                    {
                        val=10;
                    }
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->charge_current_3=bat_param->charge_current_3+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->charge_current_3=bat_param->charge_current_3-val;
                    }

                    if(bat_param->charge_current_3<MIN_CHARGE_CURRENT)
                    {
                        bat_param->charge_current_3 = MAX_CHARGE_CURRENT;  

                    }else if(bat_param->charge_current_3>MAX_CHARGE_CURRENT)
                    {
                        bat_param->charge_current_3 = MIN_CHARGE_CURRENT;  
                    }    
                }else
                {
                    if(bat_param->charge_current_3_percent>=50)
                    {
                        val=10;
                    }else if(bat_param->charge_current_3_percent>=10)
                    {
                        val=5;
                    }else 
                    {
                        val=1;
                    }
                    
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->charge_current_3_percent=bat_param->charge_current_3_percent+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->charge_current_3_percent=bat_param->charge_current_3_percent-val;
                    }

                    if(bat_param->charge_current_3_percent<MIN_PRECENT_CHARGE_VAL)
                    {
                        bat_param->charge_current_3_percent = MAX_PRECENT_CHARGE_VAL;  

                    }else if(bat_param->charge_current_3_percent>MAX_PRECENT_CHARGE_VAL)
                    {
                        bat_param->charge_current_3_percent = MIN_PRECENT_CHARGE_VAL;  
                    }      
                }
                SetChargingCurrent_3(bat_param, set_mode_edit);
            } if(param_pos == param_2)
            {
                if(CHECK_DISCHRG3_CURRENT_SET)
                {
                     if(bat_param->discharge_current_3>=1000)
                    {
                        val=100;
                    }else if(bat_param->discharge_current_3>=100)
                    {
                        val=50;
                    }else
                    {
                        val=10;
                    }
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->discharge_current_3=bat_param->discharge_current_3+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->discharge_current_3=bat_param->discharge_current_3-val;
                    }

                    if(bat_param->discharge_current_3<MIN_DISCHARGE_CURRENT)
                    {
                        bat_param->discharge_current_3 = MAX_DISCHARGE_CURRENT;  

                    }else if(bat_param->discharge_current_3>MAX_DISCHARGE_CURRENT)
                    {
                        bat_param->discharge_current_3 = MIN_DISCHARGE_CURRENT;  
                    }    
                }else
                {
                    if(bat_param->discharge_current_3_percent>=50)
                    {
                        val=10;
                    }else if(bat_param->discharge_current_3_percent>=10)
                    {
                        val=5;
                    }else 
                    {
                        val=1;
                    }
                    
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->discharge_current_3_percent=bat_param->discharge_current_3_percent+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->discharge_current_3_percent=bat_param->discharge_current_3_percent-val;
                    }

                    if(bat_param->discharge_current_3_percent<MIN_PRECENT_DISCHARGE_VAL)
                    {
                        bat_param->discharge_current_3_percent = MAX_PRECENT_DISCHARGE_VAL;  

                    }else if(bat_param->discharge_current_3_percent>MAX_PRECENT_DISCHARGE_VAL)
                    {
                        bat_param->discharge_current_3_percent = MIN_PRECENT_DISCHARGE_VAL;  
                    }      
                }
                SetDischargingCurrent_3(bat_param, set_mode_edit); 
            }
            if(param_pos == param_3)
            {
                if(CHECK_CHRG4_CURRENT_SET)
                {
                     if(bat_param->charge_current_4>=1000)
                    {
                        val=100;
                    }else if(bat_param->charge_current_4>=100)
                    {
                        val=50;
                    }else
                    {
                        val=10;
                    }
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->charge_current_4=bat_param->charge_current_4+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->charge_current_4=bat_param->charge_current_4-val;
                    }

                    if(bat_param->charge_current_4<MIN_CHARGE_CURRENT)
                    {
                        bat_param->charge_current_4 = MAX_CHARGE_CURRENT;  

                    }else if(bat_param->charge_current_4>MAX_CHARGE_CURRENT)
                    {
                        bat_param->charge_current_4 = MIN_CHARGE_CURRENT;  
                    }    
                }else
                {
                    if(bat_param->charge_current_4_percent>=50)
                    {
                        val=10;
                    }else if(bat_param->charge_current_4_percent>=10)
                    {
                        val=5;
                    }else 
                    {
                        val=1;
                    }
                    
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->charge_current_4_percent=bat_param->charge_current_4_percent+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->charge_current_4_percent=bat_param->charge_current_4_percent-val;
                    }

                    if(bat_param->charge_current_4_percent<MIN_PRECENT_CHARGE_VAL)
                    {
                        bat_param->charge_current_4_percent = MAX_PRECENT_CHARGE_VAL;  

                    }else if(bat_param->charge_current_4_percent>MAX_PRECENT_CHARGE_VAL)
                    {
                        bat_param->charge_current_4_percent = MIN_PRECENT_CHARGE_VAL;  
                    }      
                }
                SetChargingCurrent_4(bat_param, set_mode_edit);
            }if(param_pos == param_4)
            {
                if(CHECK_DISCHRG4_CURRENT_SET)
                {
                     if(bat_param->discharge_current_4>=1000)
                    {
                        val=100;
                    }else if(bat_param->discharge_current_4>=100)
                    {
                        val=50;
                    }else
                    {
                        val=10;
                    }
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->discharge_current_4=bat_param->discharge_current_4+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->discharge_current_4=bat_param->discharge_current_4-val;
                    }

                    if(bat_param->discharge_current_4<MIN_DISCHARGE_CURRENT)
                    {
                        bat_param->discharge_current_4 = MAX_DISCHARGE_CURRENT;  

                    }else if(bat_param->discharge_current_4>MAX_DISCHARGE_CURRENT)
                    {
                        bat_param->discharge_current_4 = MIN_DISCHARGE_CURRENT;  
                    }    
                }else
                {
                    if(bat_param->discharge_current_4_percent>=50)
                    {
                        val=10;
                    }else if(bat_param->discharge_current_4_percent>=10)
                    {
                        val=5;
                    }else 
                    {
                        val=1;
                    }
                    
                    if(Button_EventGet(RIGHT_SW))
                    {
                        bat_param->discharge_current_4_percent=bat_param->discharge_current_4_percent+val;
                    }else if(Button_EventGet(LEFT_SW))
                    {
                        bat_param->discharge_current_4_percent=bat_param->discharge_current_4_percent-val;
                    }

                    if(bat_param->discharge_current_4_percent<MIN_PRECENT_DISCHARGE_VAL)
                    {
                        bat_param->discharge_current_4_percent = MAX_PRECENT_DISCHARGE_VAL;  

                    }else if(bat_param->discharge_current_4_percent>MAX_PRECENT_DISCHARGE_VAL)
                    {
                        bat_param->discharge_current_4_percent = MIN_PRECENT_DISCHARGE_VAL;  
                    }      
                }
                
                SetDischargingCurrent_4(bat_param, set_mode_edit);
            }
        } 
        GLCD_Render();
        Button_Event_Reset();
    }  
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/    
    if (Button_EventGet(OK_SW)) 
    {
        Button_Event_Reset();
    /////MAIN MENU////////////////////////////////////////////////////
        if(menu_type==main_menu)
        {
            if(menu_selection==menu)
            {
                menu_type = settings1;
                menu_change = menu;
                Options1_Menu(bat_param);
                menu_init=0;
            }
    /////SETTINGS 1////////////////////////////////////////////////////
        }else if(menu_type==settings1 && param_pos == menu_start_next)
        {
            if(menu_selection==next)
            {
                menu_type = settings2;
                menu_change = next;
                menu_init=0;
                menu_pos=2;
                Options2_Menu(bat_param);
                
                
            }else if(menu_selection==save_exit)
            {
                menu_type = main_menu;
                menu_change = start;
                SingleBat_Menu(bat_param);
                menu_init=0;
                
                
            }
    /////SETTINGS 2////////////////////////////////////////////////////
        }else if(menu_type==settings2)
        {
            if(menu_selection==next)
            {
                Options3_Menu(bat_param);
                menu_type = settings3;
                menu_init=0;
                menu_pos=2;
                
                
            }else if(menu_selection==back)
            {
                menu_type = settings1;
                Options1_Menu(bat_param);
                menu_change = save_exit;
                menu_init=0;
              
            }
            else if(menu_selection==save_exit)
            {
                menu_type = main_menu;
                SingleBat_Menu(bat_param);
                menu_change = start;
                menu_init=0;
            }
    /////SETTINGS 3////////////////////////////////////////////////////
        }else if(menu_type==settings3)
        {
            if(menu_selection==next)
            {
                Options4_Menu(bat_param);
                menu_type = settings4;
                menu_init=0;
                menu_pos=2;
                
                
            }else if(menu_selection==back)
            {
                menu_type = settings2;
                Options2_Menu(bat_param);
                menu_init=0;
                menu_pos=1;
            }
            else if(menu_selection==save_exit)
            {
                menu_type = main_menu;
                SingleBat_Menu(bat_param);
                menu_change = start;
                menu_init=0;
            }
            
            
/////SETTINGS 4////////////////////////////////////////////////////
        }else if(menu_type==settings4)
        {
            if(param_pos == menu_start_next)
            {
                if(menu_selection==next )
                {
                    Options5_Menu(bat_param);
                    menu_type = settings5;
                    menu_init=0;
                    menu_change = save_exit;


                }else if(menu_selection==back)
                {
                    menu_type = settings3;
                    Options3_Menu(bat_param);
                    menu_init=0;
                    menu_pos=1;
                }
                else if(menu_selection==save_exit)
                {
                    menu_type = main_menu;
                    SingleBat_Menu(bat_param);
                    menu_change = start;
                    menu_init=0;
                }
            }else if(param_pos == param_3)
            {
                if(CHECK_CHRG2_CURRENT_SET)
                {
                    SET_CHRG2_PRECENT;
                }else
                {
                    SET_CHRG2_CURRENT;
                }
                
                SetChargingCurrent_2(bat_param, set_mode_edit);
                
                
            }else if(param_pos == param_4)
            {
               
                if(CHECK_DISCHRG2_CURRENT_SET)
                {
                    SET_DISCHRG2_PRECENT;
                }else
                {
                    SET_DISCHRG2_CURRENT;
                }
                SetDischargingCurrent_2(bat_param, set_mode_edit);
                
            }
        
        
        
        
            
            
            
/////SETTINGS 5////////////////////////////////////////////////////
        }else if(menu_type==settings5)
        {
            if(param_pos == menu_start_next)
            {
                if(menu_selection==back)
                {
                    menu_type = settings4;
                    menu_change = save_exit;
                    Options4_Menu(bat_param);
                    menu_init=0;
                    menu_pos=1;
                }
                else if(menu_selection==save_exit)
                {
                    menu_type = main_menu;
                    menu_change = start;
                    SingleBat_Menu(bat_param);
                    menu_init=0;
                }
            }else if(param_pos == param_1)
            {
                if(CHECK_CHRG3_CURRENT_SET)
                {
                    SET_CHRG3_PRECENT;
                }else
                {
                    SET_CHRG3_CURRENT;
                }
                
                SetChargingCurrent_3(bat_param, set_mode_edit);
                
                
            }else if(param_pos == param_2)
            {
               
                if(CHECK_DISCHRG3_CURRENT_SET)
                {
                    SET_DISCHRG3_PRECENT;
                }else
                {
                    SET_DISCHRG3_CURRENT;
                }
                SetDischargingCurrent_3(bat_param, set_mode_edit);
                
            }else if(param_pos == param_3)
            {
                if(CHECK_CHRG4_CURRENT_SET)
                {
                    SET_CHRG4_PRECENT;
                }else
                {
                    SET_CHRG4_CURRENT;
                }
                
                SetChargingCurrent_4(bat_param, set_mode_edit);
                
                
            }else if(param_pos == param_4)
            {
               
                if(CHECK_DISCHRG4_CURRENT_SET)
                {
                    SET_DISCHRG4_PRECENT;
                }else
                {
                    SET_DISCHRG4_CURRENT;
                }
                SetDischargingCurrent_4(bat_param, set_mode_edit);
            }
        }
          GLCD_Render();
    } 
}


void Handle_Setting23_MenuNavigation(uint8_t option)
{     
    if (option == back) 
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("BACK");

        GLCD_FillRectangle(29, 51, 29+26, 63, GLCD_White);
        GLCD_DrawRectangle(29, 51, 29+26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2+29, 54);
        GLCD_PrintString("NEXT");
        
        GLCD_FillRectangle(29+26+3, 51, 29+26+3+57, 63, GLCD_White);
        GLCD_DrawRectangle(29+26+3, 51, 29+26+3+57, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(29+26+5, 54);
        GLCD_PrintString("SAVE&EXIT");
        
    }else if (option == next) 
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("BACK");


        GLCD_FillRectangle(29, 51, 29+26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(2+29, 54);
        GLCD_PrintString("NEXT");

        GLCD_FillRectangle(29+26+3, 51, 29+26+3+57, 63, GLCD_White);
        GLCD_DrawRectangle(29+26+3, 51, 29+26+3+57, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(29+26+5, 54);
        GLCD_PrintString("SAVE&EXIT");

  
    }else if (option == save_exit) 
    {

        GLCD_FillRectangle(0, 51, 26, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("BACK");

        GLCD_FillRectangle(29, 51, 29+26, 63, GLCD_White);
        GLCD_DrawRectangle(29, 51, 29+26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2+29, 54);
        GLCD_PrintString("NEXT");
        
        GLCD_FillRectangle(29+26+3, 51, 29+26+3+57, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(29+26+5, 54);
        GLCD_PrintString("SAVE&EXIT");
    }else if (option == disable_start_next_menu) 
    {

        GLCD_FillRectangle(0, 51, 26, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("BACK");

        GLCD_FillRectangle(29, 51, 29+26, 63, GLCD_White);
        GLCD_DrawRectangle(29, 51, 29+26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2+29, 54);
        GLCD_PrintString("NEXT");
        
        GLCD_FillRectangle(29+26+3, 51, 29+26+3+57, 63, GLCD_White);
        GLCD_DrawRectangle(29+26+3, 51, 29+26+3+57, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(29+26+5, 54);
        GLCD_PrintString("SAVE&EXIT");
    } 
    
   
}

void Handle_Setting1_MenuNavigation(uint8_t option)
{     
    if (option == next) 
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("NEXT");

        GLCD_FillRectangle(29, 51, 86, 63, GLCD_White);
        GLCD_DrawRectangle(29, 51, 86, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(31, 54);
        GLCD_PrintString("SAVE&EXIT");
        
    }else if (option == save_exit) 
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("NEXT");


        GLCD_FillRectangle(29, 51, 86, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(31, 54);
        GLCD_PrintString("SAVE&EXIT");
    }else if (option == disable_start_next_menu)
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("NEXT");
        
        GLCD_FillRectangle(29, 51, 86, 63, GLCD_White);
        GLCD_DrawRectangle(29, 51, 86, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(31, 54);
        GLCD_PrintString("SAVE&EXIT");
           
    } 
}

void Handle_Setting5_MenuNavigation(uint8_t option)
{     
    if (option == back) 
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("BACK");

        GLCD_FillRectangle(29, 51, 86, 63, GLCD_White);
        GLCD_DrawRectangle(29, 51, 86, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(31, 54);
        GLCD_PrintString("SAVE&EXIT");
        
    }else if (option == save_exit) 
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("BACK");


        GLCD_FillRectangle(29, 51, 86, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(31, 54);
        GLCD_PrintString("SAVE&EXIT");
    }else if (option == disable_start_next_menu) 
    {
        GLCD_FillRectangle(0, 51, 26, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("BACK");

        GLCD_FillRectangle(29, 51, 86, 63, GLCD_White);
        GLCD_DrawRectangle(29, 51, 86, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(31, 54);
        GLCD_PrintString("SAVE&EXIT");
    }
    
   
}



void Handle_Main_MenuNavigation(uint8_t option)
{   
    if (option == start) 
    {
        GLCD_FillRectangle(0, 51, 32, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("START");

        GLCD_FillRectangle(35, 51, 61, 63, GLCD_White);
        GLCD_DrawRectangle(35, 51, 61, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(37, 54);
        GLCD_PrintString("MENU");

    } else if (option == menu) 
    {

        GLCD_FillRectangle(0, 51, 32, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 32, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("START");


        GLCD_FillRectangle(35, 51, 61, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(37, 54);
        GLCD_PrintString("MENU");  
    }   
}