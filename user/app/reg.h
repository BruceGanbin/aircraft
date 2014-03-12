#ifndef _REG_H
#define _REG_H

#include  "stm32f10x.h"
//#include "sensor.h"
// CONFIG-------------------------

#define MACHNUM                '1'


// init status reg
#define LED_ST                 LED_EN
#define MOTOR_ST               0
#define SENSOR_ST              0
#define ORIEN_ST               0



// STATUS---------------
#define LED4_OPEN                 1<<0
#define LED3_OPEN                 1<<1
#define LED2_OPEN                 1<<2
#define LED1_OPEN                 1<<3
#define LED_EN                    1<<7

#define MOTOR4_OPEN               1<<0
#define MOTOR3_OPEN               1<<1
#define MOTOR2_OPEN               1<<2
#define MOTOR1_OPEN               1<<3
#define MOTOR_EN                  1<<7

#define SENSOR_AG_OPEN            1<<3
#define SENSOR_COM_OPEN           1<<7

#define OR_NHOLD                  1<<7
#define OR_UP_DOWN                1<<6
#define OR_FRONT_REAR             1<<5
#define OR_LEFT_RIGHT             1<<4







#define STA_NUM                4
// status reg------------------------------
typedef struct _REG_STATUS{
	u8 led_sta;
	u8 motor_sta;
	u8 sensor_sta;
	u8 orien_sta;
	   
}regstaTypedef;
/******
         |  bit7  |  bit6  |  bit5  |  bit4  |  bit3  |  bit2  |  bit1  |  bit0  |
led_sta  |reserive|



******/

typedef struct _REG_PER{
	u8 usr;
		
}regperTypedef;

#define DATA_NUM               9
// data reg------------------------------
//typedef struct {}SensorData_Typedef;
#include  <sensor.h>
typedef struct _REG_DATA{
  
	SensorData_Typedef sensor_da;
	 
}regdatTypedef;

//pidk data reg
typedef struct _PIDK_TYPEDEF{
	u16 kp;
	u16 ki;
	u16 kd;
}pidk_Typedef;




// all reg  -----------------------------
typedef struct _REG_MACH{

	u8 machnum;
	regstaTypedef regsta;
	regperTypedef permission;
	regdatTypedef regdata;
	pidk_Typedef reg_PID;

}regMachTypedef;




#endif

