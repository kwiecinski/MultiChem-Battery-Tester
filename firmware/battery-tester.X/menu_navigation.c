#include <xc.h>
#include <stdio.h>

#include "LCD-KS0108/KS0108.h"
#include "LCD-KS0108/font5x8.h"
#include "LCD-KS0108/Tahoma11x13.h"
#include "LCD-KS0108/Tekton_Pro_Ext27x28.h"
#include "menu_definitions.h"
#include "menu_navigation.h"
#include "button.h"

void DisplayMenuOption(uint8_t option);


void Menu(BattParameters *bat_param)
{
    
    static uint8_t menu_change=0;
          
    Button_Update();
    if (Button_EventGet(LEFT_SW) || Button_EventGet(RIGHT_SW)) 
    {
        Button_Event_Reset();

        if (menu_change == start) 
        {
            DisplayMenuOption(start);
            menu_change = menu;
            
        }else if (menu_change == menu) 
        {
            DisplayMenuOption(menu);
            menu_change = start;
        }  
    }  
    
    if (Button_EventGet(OK_SW)) 
    {
        
    }
}



void DisplayMenuOption(uint8_t option)
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