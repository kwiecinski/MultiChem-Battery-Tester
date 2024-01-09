#ifndef IO_MACROS_H_INCLUDED
#define IO_MACROS_H_INCLUDED
/*
||
||  Filename:	 		IO_Macros.h
||  Title: 			    IO manipulation macros
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:		This library contains macros for 
||						easy port manipulation (similar 
||						to Arduino).
||
||	Demo:
|| 1.	#define LED		A, 0		|| 6. 	PinModeToggle(BUTTON);
|| 2.	#define BUTTON	A, 1		|| 7. 	DigitalWrite(LED, LOW);
|| 3.								|| 8. 	DigitalWrite(LED, HIGH);
|| 4. 	PinMode(BUTTON, OUTPUT);	|| 9. 	DigitalLevelToggle(LED);
|| 5. 	PinMode(LED, OUTPUT);		||10.	int a = DigitalRead(BUTTON);
||
*/

//----- I/O Macros -----
//Macros to edit PORT, DDR and PIN
#define PinMode(			x, y)	( 		y 			?	_SET(TRIS, x)	:	_CLEAR(TRIS, x)		)
#define DigitalWrite(		x, y)	( 		y 			?	_SET(LAT, x)	:	_CLEAR(LAT, x)		)
#define DigitalRead(		x)		(						_GET(PORT, x)							)
#define PinModeToggle(		x)		(						_TOGGLE(TRIS, x)							)
#define DigitalLevelToggle(	x)		(						_TOGGLE(LAT, x)						)

//General use bit manipulating commands
#define BitSet(		x, y)			(	x |=	 (1UL<<y)			)
#define BitClear(	x, y)			(	x &=	(~(1UL<<y))			)
#define BitToggle(	x, y)			(	x ^=	 (1UL<<y)			)
#define BitCheck(	x, y)			(	x &		 (1UL<<y)	? 1 : 0	)

//Access PORT, DDR and PIN
#define LAT(	port)				(_LAT(	port))
#define TRIS(	port)				(_TRIS(	port))
#define PORT(	port)				(_PORT(	port))

#define _LAT(	port)				(LAT##	port)
#define _TRIS(	port)				(TRIS##	port)
#define _PORT	port)				(PORT##	port)

#define _SET(	type, port, bit)	(	BitSet(		(type##port),	bit)	)
#define _CLEAR(	type, port, bit)	(	BitClear(	(type##port),	bit)	)
#define _TOGGLE(type, port, bit)	(	BitToggle(	(type##port),	bit)	)
#define _GET(	type, port, bit)	(	BitCheck(	(type##port),	bit)	)



//Definitions
#define Input		1
#define Output		!Input
#define Low			0
#define High		!Low
#define False		0
#define True		!False
//------------------
#endif
