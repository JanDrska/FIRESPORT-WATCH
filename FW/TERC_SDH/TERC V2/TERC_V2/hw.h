 /*
 * hw.h
 *
 *  Created on: 4. 12. 2024
 *  Author: Ing. Jan Drska
 */

#ifndef HW_H_
#define HW_H_

//#define TEST

#define LED_R		F,5 // chyba na DPS.... propojeno na reset... opraveno propojkou na F5
#define LED_Y		A,3
#define LED_G		A,2		

#define DE_RE		A,7

#define OUT0		C,0
#define OUT1		C,1
#define OUT2		C,2
#define OUT3		C,3
#define RELAY		F,4
#define CS			F,3

#define IN0			D,1
#define IN1			D,2
#define IN2			D,3
#define IN3			D,4
#define IN4			D,5

// Definition of I/O names
#define TARGET_L_FULL	IN0
#define TARGET_L_LOW	IN1
#define TARGET_R_FULL	IN2
#define TARGET_R_LOW	IN3
#define BATTERY_VOLTAGE	IN4

#define VALVE	RELAY
#define TARGET_L_LIGHT	OUT0
#define TARGET_R_LIGHT	OUT3

#endif /* HW_H_ */
