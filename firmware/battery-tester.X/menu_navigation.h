/* 
 * File:   menu_navigation.h
 * Author: RaD
 *
 * Created on 24 stycznia 2024, 13:18
 */

#ifndef MENU_NAVIGATION_H
#define	MENU_NAVIGATION_H

#define PRECENT_CURRENT_CHRG2_POS        0
#define PRECENT_CURRENT_DISCHRG2_POS     1
#define PRECENT_CURRENT_CHRG3_POS        2
#define PRECENT_CURRENT_DISCHRG3_POS     3
#define PRECENT_CURRENT_CHRG4_POS        4
#define PRECENT_CURRENT_DISCHRG4_POS     5

 #define SET_CHRG2_PRECENT      bat_param->precent_current_flags &= ~(1 << PRECENT_CURRENT_CHRG2_POS);      // zero bit
 #define SET_CHRG2_CURRENT      bat_param->precent_current_flags |= (1 << PRECENT_CURRENT_CHRG2_POS);       //set bit
 #define SET_DISCHRG2_PRECENT   bat_param->precent_current_flags &= ~(1 << PRECENT_CURRENT_DISCHRG2_POS);      // zero bit
 #define SET_DISCHRG2_CURRENT   bat_param->precent_current_flags |= (1 << PRECENT_CURRENT_DISCHRG2_POS);       //set bit
 #define SET_CHRG3_PRECENT      bat_param->precent_current_flags &= ~(1 << PRECENT_CURRENT_CHRG3_POS);      // zero bit
 #define SET_CHRG3_CURRENT      bat_param->precent_current_flags |= (1 << PRECENT_CURRENT_CHRG3_POS);       //set bit
 #define SET_DISCHRG3_PRECENT   bat_param->precent_current_flags &= ~(1 << PRECENT_CURRENT_DISCHRG3_POS);      // zero bit
 #define SET_DISCHRG3_CURRENT   bat_param->precent_current_flags |= (1 << PRECENT_CURRENT_DISCHRG3_POS);       //set bit
 #define SET_CHRG4_PRECENT      bat_param->precent_current_flags &= ~(1 << PRECENT_CURRENT_CHRG4_POS);      // zero bit
 #define SET_CHRG4_CURRENT      bat_param->precent_current_flags |= (1 << PRECENT_CURRENT_CHRG4_POS);       //set bit
 #define SET_DISCHRG4_PRECENT   bat_param->precent_current_flags &= ~(1 << PRECENT_CURRENT_DISCHRG4_POS);      // zero bit
 #define SET_DISCHRG4_CURRENT   bat_param->precent_current_flags |= (1 << PRECENT_CURRENT_DISCHRG4_POS);       //set bit

#define CHECK_CHRG2_CURRENT_SET             bat_param->precent_current_flags & (1 << PRECENT_CURRENT_CHRG2_POS)
#define CHECK_DISCHRG2_CURRENT_SET          bat_param->precent_current_flags & (1 << PRECENT_CURRENT_DISCHRG2_POS)
#define CHECK_CHRG3_CURRENT_SET             bat_param->precent_current_flags & (1 << PRECENT_CURRENT_CHRG3_POS)
#define CHECK_DISCHRG3_CURRENT_SET          bat_param->precent_current_flags & (1 << PRECENT_CURRENT_DISCHRG3_POS) 
#define CHECK_CHRG4_CURRENT_SET             bat_param->precent_current_flags & (1 << PRECENT_CURRENT_CHRG4_POS)
#define CHECK_DISCHRG4_CURRENT_SET          bat_param->precent_current_flags & (1 << PRECENT_CURRENT_DISCHRG4_POS) 



void Menu(BattParameters *bat_param);
void switch_between_battery_types(BattParameters *bat_param);


#endif	/* MENU_NAVIGATION_H */

