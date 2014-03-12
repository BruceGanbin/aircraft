#ifndef _SENSOR_H
#define _SENSOR_H

#include  "stm32f10x.h"
#include  "stm32f10x_bus.h"
#include  "HMC5883.h"
#include  "mpu6050.h"

typedef struct __SENSOR_TYPEDEF
{
	s16 Ac_X;
	s16 Ac_Y;
	s16 Ac_Z;
	s16 GY_X;
	s16 GY_Y;
	s16 GY_Z;
	s16 Cm_X;
	s16 Cm_Y;
	s16 Cm_Z;
	s16 Temper;
}SensorData_Typedef;


#include "reg.h"
void SensorInit(void);
void getSensorData(SensorData_Typedef *pSensorData,regstaTypedef regsta);














#endif

