#include "sensor.h"
#include "includes.h"



void SensorInit(void)
{
  MPU6050Init_Typedef MPU6050_Config;
  HMC5883Init_Typedef HMC5883_Config;

	//Init  MPU6050
  MPU6050_Config.Read_Data = IIC_ReadPage;
  MPU6050_Config.Write_Data= IIC_WritePage;
  Init_MPU6050(&MPU6050_Config);
//	delayms(100);
  HMC5883_Config.Read_Data = IIC_ReadPage;
  HMC5883_Config.Write_Data= IIC_WritePage;
  Init_HMC5883(&HMC5883_Config);
}

void getSensorData(SensorData_Typedef *pSensorData,regstaTypedef sta)
{
//	short int Data[10];
	u8 i;
	s16 *pData;
	*pData = pSensorData->Ac_X;

	if(sta.sensor_sta==0)
	{
		for(i=0;i<10;i++)
		{
			*pData = 0;
			pData++;
		}
		return;
	}
	getMotion6(&pSensorData->Ac_X,&pSensorData->Ac_Y,&pSensorData->Ac_Z,   \
							&pSensorData->GY_X,&pSensorData->GY_Y,&pSensorData->GY_Z,  \
							&pSensorData->Temper);
	
	getCompass3(&pSensorData->Cm_X,&pSensorData->Cm_Y,&pSensorData->Cm_Z);

}


