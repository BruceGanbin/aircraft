#include "sensor.h"



void SensorInit(void)
{
  MPU6050Init_Typedef MPU6050_Config;
  HMC5883Init_Typedef HMC5883_Config;

	//Init  MPU6050
  MPU6050_Config.Read_Data = IIC_ReadPage;
  MPU6050_Config.Write_Data= IIC_WritePage;
  Init_MPU6050(&MPU6050_Config);

  HMC5883_Config.Read_Data = IIC_ReadPage;
  HMC5883_Config.Write_Data= IIC_WritePage;
  Init_HMC5883(HMC5883_Config);
}

void getSensorData(SensorData_Typedef *pSensorData,regstaTypedef sta)
{
	short int Data[10];


getMotion6(&Data[0],&Data[1],&Data[2],&Data[3],&Data[4],&Data[5],&Data[6]);

 getCompass3(&Data[7],&Data[8],&Data[9]);

}


