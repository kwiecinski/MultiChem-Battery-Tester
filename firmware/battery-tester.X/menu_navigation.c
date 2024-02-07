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
    back,
    save_exit
};

enum menu_type
{
    main_menu,
    settings1,
    settings2,
    settings3
        
};

void Handle_Main_MenuNavigation(uint8_t option);
void Handle_Setting1_MenuNavigation(uint8_t option);
void Handle_Setting23_MenuNavigation(uint8_t option);

void Menu(BattParameters *bat_param)
{
    
    static uint8_t menu_change=start, menu_type=main_menu, menu_selection;
          
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
                menu_selection=start;

            }else if (menu_change == menu) 
            {
                Handle_Main_MenuNavigation(menu_change);
                menu_change = start;
                 menu_selection=menu;
            }  
        }else if(menu_type==settings1)
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
        }else if(menu_type==settings2)
        {
            
            if (menu_change == back) 
            {
                Handle_Setting23_MenuNavigation(menu_change);
                menu_change = next;
                menu_selection=back;

            }else if (menu_change == next) 
            {
                Handle_Setting23_MenuNavigation(menu_change);
                menu_change = save_exit;
                menu_selection=next;
            }else if (menu_change == save_exit) 
            {
                Handle_Setting23_MenuNavigation(menu_change);
                menu_change = back;
                menu_selection= save_exit;
            }  
        } 
    }  
    
    if (Button_EventGet(OK_SW)) 
    {
        Button_Event_Reset();
        if(menu_type==main_menu)
        {
            if(menu_selection==menu)
            {
                Options1_Menu(bat_param);
                Handle_Setting1_MenuNavigation(next);
                menu_type = settings1;
            }
        }else if(menu_type==settings1)
        {
            if(menu_selection==next)
            {
                Options2_Menu(bat_param);
                Handle_Setting23_MenuNavigation(next);
                menu_type = settings2;
                
            }else if(menu_selection==back)
            {
                SingleBat_Menu(bat_param);
                Handle_Main_MenuNavigation(start);
                menu_type = main_menu;
            }
        }else if(menu_type==settings2)
        {
            if(menu_selection==next)
            {
                //Options2_Menu(bat_param);
                //Handle_Setting23_MenuNavigation(next);
                menu_type = settings3;
                
            }else if(menu_selection==back)
            {
                Options1_Menu(bat_param);
                Handle_Setting23_MenuNavigation(next);
                menu_type = settings1;
            }
        }
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
    } 
    
    GLCD_Render();
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
        GLCD_FillRectangle(0, 51, 26, 57, GLCD_White);
        GLCD_DrawRectangle(0, 51, 26, 57, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Non_Inverted);
        GLCD_GotoXY(2, 54);
        GLCD_PrintString("NEXT");


        GLCD_FillRectangle(29, 51, 86, 63, GLCD_Black);
        GLCD_SetFont(Font5x8, 5, 8, GLCD_Merge, GLCD_Inverted);
        GLCD_GotoXY(37, 54);
        GLCD_PrintString("SAVE & EXIT");
    }  
    
    GLCD_Render();
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
    
    GLCD_Render();
}