#include "mpu6050.h"
#include "i2c.h"
#include <rtthread.h>

unsigned char *(*Single_read)(unsigned char *pBuffer,unsigned char PartAddr,      \
										unsigned char WriteAddr,unsigned char NumByteToWrite);
unsigned char *(*Single_write)(unsigned char *pBuffer,unsigned char PartAddr,     \
										 unsigned char WriteAddr,unsigned char NumByteToWrite);




/**


****/
char MPU6050_swrite(unsigned char WriteAddr,unsigned char Data)
{
    //  Single_write(&Data,MPU6050_Addr,WriteAddr,1);
    IIC_Write(&Data,MPU6050_Addr,WriteAddr,1);
    return Data;
}

char MPU6050_sread(unsigned char WriteAddr)
{
    unsigned char Data;
  //  Single_read(&Data,MPU6050_Addr,WriteAddr,1);
     IIC_Read(&Data,MPU6050_Addr,WriteAddr,1);
     return Data;
}

void MPU6050_read(unsigned char WriteAddr,unsigned char *pData,unsigned char Num)
{
    unsigned char chk=0;
    chk = IIC_Read(pData,MPU6050_Addr,WriteAddr,Num);
    if(chk != I2C_NOTimeout)
        rt_kprintf("I2C TimerOut \n");
}

//void Init_MPU6050(MPU6050Init_Typedef *MPU6050_Config)
void Init_MPU6050(void)
{
  unsigned char Rdata[5]={0};
  unsigned char MPU_ID=0;
  //  Single_read = MPU6050_Config->Read_Data;
  //  Single_write= MPU6050_Config->Write_Data;

  MPU_ID=getDeviceID();
  if(MPU_ID == 0x68)
      rt_kprintf("MPU6050 init OK!\n");
  else
      rt_kprintf("MPU6050 init ERROR!\n");
  //  MPU6050_swrite(PWR_MGMT_1,0x07);
  MPU6050_swrite(PWR_MGMT_1,0x00);
  MPU6050_swrite(SMPLRT_DIV,0x07);
  MPU6050_swrite(CONFIG,0x06);
  MPU6050_swrite(GYRO_CONFIG,0x18);
  MPU6050_swrite(ACCEL_CONFIG,0x01);

}


unsigned char getDeviceID(void)
{
    unsigned char tmp=0x00;
    MPU6050_read(WHO,&tmp,1);
    return tmp;
}


//CONFIG register
unsigned char getExternalFrameSync(void)
{
	unsigned char ret=0;
	ret = MPU6050_sread(CONFIG);
	ret&= MPU6050_CFG_EXT_SYNC_SET;
	return	ret>>MPU6050_CFG_EXT_SYNC_OFFSETBIT; 
}

void setExternalFrameSync(unsigned char sync)
{
	unsigned char ret=0,temp=0;
	temp = MPU6050_sread(CONFIG);
	temp&= ~MPU6050_CFG_EXT_SYNC_SET;
	sync<<=MPU6050_CFG_EXT_SYNC_OFFSETBIT;
	MPU6050_swrite(CONFIG,sync|temp);
}

unsigned char getDLPFMode(void)
{
	unsigned char ret=0;
	ret = MPU6050_sread(CONFIG);
	ret&= MPU6050_CFG_DLPF_CFG_SET;
	return ret>>MPU6050_CFG_DLPF_CFG_OFFSETBIT;
}
void setDLPFMode(unsigned char bandwidth)
{
	unsigned char ret=0,temp=0;
	temp = MPU6050_sread(CONFIG);
	temp&= ~MPU6050_CFG_DLPF_CFG_SET;
	
	MPU6050_swrite(CONFIG,bandwidth|temp);
}

//GYRO_CONFIG register
unsigned char getFullScaleGyroRange(void)
{
	unsigned char ret=0;
	ret = MPU6050_sread(GYRO_CONFIG);
	return ret>>MPU6050_GCONFIG_FS_SEL_SET;
}
void setFullScaleGyroRange(unsigned char range)
{
	MPU6050_swrite(GYRO_CONFIG,range<<MPU6050_GCONFIG_FS_SEL_SET);
}

//ACCEL_CONFIG register
unsigned char getFullScaleAccelRange(void)
{
	unsigned char ret=0;
	ret = MPU6050_sread(ACCEL_CONFIG);
	return ret>>MPU6050_ACONFIG_AFS_SEL_SET;
}
void setFullScaleAccelRange(unsigned char range)
{
	unsigned char temp=0;
	temp = MPU6050_sread(ACCEL_CONFIG);
	temp&= ~MPU6050_ACONFIG_AFS_SEL_SET;
	range <<= MPU6050_ACONFIG_AFS_SEL_OFFSETBIT;
	MPU6050_swrite(CONFIG,range|temp);
}


// PWR_MGMT_1 register
void reset(void)
{
	MPU6050_swrite(PWR_MGMT_1,1<<MPU6050_PWR1_DEVICE_RESET_BIT);
}

unsigned char getSleepEnabled(void)
{
	unsigned char ret=0,temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	ret = temp&~(1<<MPU6050_PWR1_SLEEP_BIT);
	return ret;
}
void setSleepEnabled(unsigned char enabled)
{
	unsigned char temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	temp|= 1<<MPU6050_PWR1_SLEEP_BIT;
	MPU6050_swrite(PWR_MGMT_1,temp);
}
unsigned char getWakeCycleEnabled(void)
{
	unsigned char ret=0,temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	ret = temp&~(1<<MPU6050_PWR1_CYCLE_BIT);
	return ret;
}
void setWakeCycleEnabled(unsigned char enabled)
{
	unsigned char temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	temp|= 1<<MPU6050_PWR1_CYCLE_BIT;
	MPU6050_swrite(PWR_MGMT_1,temp);
}
unsigned char getTempSensorEnabled(void)
{
	unsigned char ret=0,temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	ret = temp&~(1<<MPU6050_PWR1_TEMP_DIS_BIT);
	return ret;
}
void setTempSensorEnabled(unsigned char enabled)
{
	unsigned char temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	temp|= 1<<MPU6050_PWR1_TEMP_DIS_BIT;
	MPU6050_swrite(PWR_MGMT_1,temp);
}
unsigned char getClockSource(void)
{
	unsigned char ret=0,temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	ret  = temp &~MPU6050_PWR1_CLKSEL_SET;
	return ret;
}
void setClockSource(unsigned char source)
{
	unsigned char ret=0,temp=0;
	temp = MPU6050_sread(PWR_MGMT_1);
	temp&= ~MPU6050_PWR1_CLKSEL_SET;
	MPU6050_swrite(PWR_MGMT_1,temp|source);
}



// MOT_THR register
unsigned char getMotionDetectionThreshold(void);
void setMotionDetectionThreshold(unsigned char threshold);

// INT_PIN_CFG register
unsigned char getInterruptMode(void);
void setInterruptMode(unsigned char mode);
unsigned char  getInterruptDrive(void);
void setInterruptDrive(unsigned char drive);
unsigned char getInterruptLatch(void);
void setInterruptLatch(unsigned char latch);
unsigned char getInterruptLatchClear(void);
void setInterruptLatchClear(unsigned char clear);
unsigned char getFSyncInterruptLevel(void);
void setFSyncInterruptLevel(unsigned char level);
unsigned char getFSyncInterruptEnabled(void);
void setFSyncInterruptEnabled(unsigned char enabled);
unsigned char getI2CBypassEnabled(void);
void setI2CBypassEnabled(unsigned char enabled);

// INT_ENABLE register
unsigned char getIntMotionEnabled(void);
void setIntMotionEnabled(unsigned char enabled);
unsigned char getIntFIFOBufferOverflowEnabled(void);
void setIntFIFOBufferOverflowEnabled(unsigned char enabled);
unsigned char getIntI2CMasterEnabled(void);
void setIntI2CMasterEnabled(unsigned char enabled);
unsigned char getIntDataReadyEnabled(void);
//void setIntDataReadyEnabled(bool enabled);

// INT_STATUS register
unsigned char getIntMotionStatus();
unsigned char getIntFIFOBufferOverflowStatus();
unsigned char getIntI2CMasterStatus();
unsigned char getIntDataReadyStatus();

// ACCEL_*OUT_* registers
void getAcceleration(short int* x,short int* y,short int* z);
short int getAccelerationX(void);
short int getAccelerationY(void);
short int getAccelerationZ(void);

// TEMP_OUT_* registers
short int getTemperature(void);

// GYRO_*OUT_* registers
void getgyroscope(short int* x,short int* y,short int* z);
short int getgyroscopeX(void);
short int getgyroscopeY(void);
short int tgetgyroscopeZ(void);

void getMotion6(short int* ax,short int* ay,short int* az,short int* gx,short int* gy,short int* gz);

//void Read_MPU6050AG(MPU6050AGVALUE_Typedef *pAg_value);


// SIGNAL_PATH_RESET register
void resetGyroscopePath(void);
void resetAccelerometerPath(void);
void resetTemperaturePath(void);

// MOT_DETECT_CTRL register
unsigned char getAccelerometerPowerOnDelay(void);
void setAccelerometerPowerOnDelay(unsigned char delay);

// MOT_DETECT_CTRL register
unsigned char getAccelerometerPowerOnDelay(void);
void setAccelerometerPowerOnDelay(unsigned char delay);
unsigned char getFreefallDetectionCounterDecrement(void);
void setFreefallDetectionCounterDecrement(unsigned char decrement);
unsigned char getMotionDetectionCounterDecrement(void);
void setMotionDetectionCounterDecrement(unsigned decrement);

// USER_CTRL register
unsigned char getFIFOEnabled(void);
void setFIFOEnabled(unsigned char enabled);
unsigned char getI2CMasterModeEnabled(void);
void setI2CMasterModeEnabled(unsigned char enabled);
void switchSPIEnabled(unsigned char enabled);
void resetFIFO(void);
void resetI2CMaster(void);
void resetSensors(void);

/**

****/
void Read_MPU6050AG(MPU6050AGVALUE_Typedef *pAg_value)
{
    unsigned char tmp[14]={0};

    MPU6050_read(ACCEL_XOUT_H,tmp,14);
    pAg_value->AccelX=(tmp[0]<<8)|tmp[1];
    pAg_value->AccelY=(tmp[2]<<8)|tmp[3];
    pAg_value->AccelZ=(tmp[4]<<8)|tmp[5];
    
    pAg_value->TEMPER=(tmp[6]<<8)|tmp[7];

    pAg_value->GYROX=(tmp[8]<<8)|tmp[9];
    pAg_value->GYROY=(tmp[10]<<8)|tmp[11];
    pAg_value->GYROZ=(tmp[12]<<8)|tmp[13];
}

void rd_simpu(void)
{
    MPU6050AGVALUE_Typedef Senser_Data={0};
    unsigned char tmp[6]={0};

    MPU6050_read(ACCEL_XOUT_H,&tmp[0],1);
    MPU6050_read(ACCEL_XOUT_L,&tmp[1],1);
    MPU6050_read(ACCEL_YOUT_H,&tmp[2],1);
    MPU6050_read(ACCEL_YOUT_L,&tmp[3],1);
    MPU6050_read(ACCEL_ZOUT_H,&tmp[4],1);
    MPU6050_read(ACCEL_ZOUT_L,&tmp[5],1);

    Senser_Data.AccelX=(tmp[0]<<8)|tmp[1];
    Senser_Data.AccelY=(tmp[2]<<8)|tmp[3];
    Senser_Data.AccelZ=(tmp[4]<<8)|tmp[5];

    rt_kprintf("Accel_X :%d   ,Accel_Y :%d   ,Accel_Z :%d\n",Senser_Data.AccelX,Senser_Data.AccelY,Senser_Data.AccelZ);

}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(rd_simpu, read mpu6050 data in single reg .)
#endif


void rd_mpu(void)
{
    MPU6050AGVALUE_Typedef Senser_Data;

    Read_MPU6050AG(&Senser_Data);
    
    rt_kprintf("Accel_X :%d   ,Accel_Y :%d   ,Accel_Z :%d\n",Senser_Data.AccelX,Senser_Data.AccelY,Senser_Data.AccelZ);
    rt_kprintf("Gyro_X :%d    ,Gyro :%d      ,Gyro :%d\n"   ,Senser_Data.GYROX, Senser_Data.GYROY, Senser_Data.GYROZ);
    rt_kprintf("Temperature :%d \n",Senser_Data.TEMPER);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(rd_mpu, read mpu6050 data .)
#endif

/*
void Set_MPU6050_REG(unsigned char reg,unsigned char *pdata){

}

unsigned char Read_MPU6050_REG(unsigned char reg);
*/
