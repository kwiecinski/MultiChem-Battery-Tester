#ifndef KS0108_SETTINGS_H_INCLUDED
#define KS0108_SETTINGS_H_INCLUDED
/*
||
||  Filename:	 		KS0108_Settings.h
||  Title: 			    KS0108 Driver Settings
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:
||	Settings for the KS0108 driver. 
||
*/

//----- Configuration -------------//
//Chip Enable Pin
#define GLCD_ACTIVE_LOW		1

//GLCD pins					PORT, PIN
#define GLCD_D0				E, 0
#define GLCD_D1				E, 1
#define GLCD_D2				E, 2
#define GLCD_D3				E, 3
#define GLCD_D4				E, 4
#define GLCD_D5				E, 5
#define GLCD_D6				E, 6
#define GLCD_D7				E, 7

#define GLCD_DI				D, 4
#define GLCD_RW				D, 3
#define GLCD_EN				D, 7
#define GLCD_CS1			D, 0
#define GLCD_CS2			D, 1
#define GLCD_RST			D, 2
//---------------------------------//
#endif