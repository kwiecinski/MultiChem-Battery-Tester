#include <xc.h>
#include <stdio.h>

#include "LCD-KS0108/KS0108.h"
#include "LCD-KS0108/font5x8.h"
#include "LCD-KS0108/Tahoma11x13.h"
#include "LCD-KS0108/Tekton_Pro_Ext27x28.h"
#include "menu_definitions.h"
#include "menu_navigation.h"
#include "button.h"

enum settings1_navigation
{
    next,
    save_exit
};

enum menu_type
{
    main_menu,
    settings1,
    settings2
        
};

void Handle_Main_MenuNavigation(uint8_t option);
void Handle_Setting1_MenuNavigation(uint8_t option);

void Menu(BattParameters *bat_param)
{
    
    static uint8_t menu_change=start, menu_type=main_menu;
          
    Button_Update();
    if (Button_EventGet(LEFT_SW) || Button_EventGet(RIGHT_SW)) 
    {
        Button_Event_Reset();

       
        if(menu_type==main_menu)
        {
            if (menu_change == start) 
            {
                Handle_Main_MenuNavigation(menu_change);
                menu_change = menu;

            }else if (menu_change == menu) 
            {
                Handle_Main_MenuNavigation(menu_change);
                menu_change = start;
            }  
        }else if(menu_type==settings1)
        {
            if (menu_change == save_exit) 
            {
                Handle_Setting1_MenuNavigation(menu_change);
                menu_change = menu;

            }else if (menu_change == save_exit) 
            {
                Handle_Setting1_MenuNavigation(menu_change);
                menu_change = next;
            }  
        }
        
        
    }  
    
    if (Button_EventGet(OK_SW)) 
    {
        Button_Event_Reset();
        
        if(menu_change == !menu)
        {
            Options1_Menu(bat_param);
            Handle_Setting1_MenuNavigation(next);
            menu_type = settings1;
        }
    }
    
    
    
}


void Handle_Setting1_MenuNavigation(uint8_t option)
{     
    if (option == next) 
    {
        GLCD_FillRectangle(0, 51, 32, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("NEXT");

        GLCD_FillRectangle(35, 51, 106, 63, GLCD_White);
        GLCD_DrawRectangle(35, 51, 106, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(37, 54);
        GLCD_PrintString("SAVE & EXIT");

        GLCD_Render();

    }else if (option == save_exit) 
    {

        GLCD_FillRectangle(0, 51, 32, 63, GLCD_White);
        GLCD_DrawRectangle(0, 51, 32, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("NEXT");


        GLCD_FillRectangle(35, 51, 106, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(37, 54);
        GLCD_PrintString("SAVE & EXIT");
        GLCD_Render();
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

        GLCD_Render();

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
        GLCD_Render();
    }  
}