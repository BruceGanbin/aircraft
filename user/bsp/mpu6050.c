#include "mpu6050.h"
#include "includes.h"


unsigned char *(*Single_read)(unsigned char *pBuffer,unsigned char PartAddr,      \
										unsigned char WriteAddr,unsigned char NumByteToWrite);
unsigned char *(*Single_write)(unsigned char *pBuffer,unsigned char PartAddr,     \
										 unsigned char WriteAddr,unsigned char NumByteToWrite);




/**


****/
char MPU6050_swrite(unsigned char WriteAddr,unsigned char Data)
{
  Single_write(&Data,MPU6050_Addr,WriteAddr,1);
  return 1;
}

char MPU6050_sread(unsigned char WriteAddr)
{
  unsigned char Data;
  Single_read(&Data,MPU6050_Addr,WriteAddr,1);
  return Data;
}


void Init_MPU6050(MPU6050Init_Typedef *MPU6050_Config){
  
  unsigned char Rdata[5]={0};
  
  Single_read = MPU6050_Config->Read_Data;
  Single_write= MPU6050_Config->Write_Data;


  //  MPU6050_swrite(PWR_MGMT_1,0x07);
  MPU6050_swrite(PWR_MGMT_1,0x00);
  MPU6050_swrite(SMPLRT_DIV,0x07);
  MPU6050_swrite(CONFIG,0x06);
  MPU6050_swrite(GYRO_CONFIG,0x18);
  MPU6050_swrite(ACCEL_CONFIG,0x01);
}


unsigned char testConnection(void)   //test read ID 
{
	return getDeviceID(); //== 0x
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
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(CONFIG);
	empty&= ~MPU6050_CFG_EXT_SYNC_SET;
	sync<<=MPU6050_CFG_EXT_SYNC_OFFSETBIT;
	MPU6050_swrite(CONFIG,sync|empty);
}

unsigned char getDLPFMode(void)
{
	unsigned char ret=0;
	ret = MPU6050_sread(CONFIG);
	ret&= MPU6050_CFG_DLPF_CFG_BIT;
	return ret>>MPU6050_CFG_DLPF_CFG_OFFSETBIT;
}
void setDLPFMode(unsigned char bandwidth)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(CONFIG);
	empty&= ~MPU6050_CFG_DLPF_CFG_SET;
	
	MPU6050_swrite(CONFIG,bandwidth|empty);
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
	return ret>>MPU6050_ACONFIG_AFS_SEL_BIT;
}
void setFullScaleAccelRange(unsigned char range)
{
	unsigned char empty=0;
	empty = MPU6050_sread(ACCEL_CONFIG);
	empty&= ~MPU6050_ACONFIG_AFS_SEL_SET;
	range <<= MPU6050_ACONFIG_AFS_SEL_OFFSETBIT;
	MPU6050_swrite(CONFIG,range|empty);
}


// PWR_MGMT_1 register
void reset(void)
{
	MPU6050_swrite(PWR_MGMT_1,1<<MPU6050_PWR1_DEVICE_RESET_BIT);
}
unsigned char getSleepEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	ret = empty&~(1<<MPU6050_PWR1_SLEEP_BIT);
	return ret;
}
void setSleepEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	empty|= 1<<MPU6050_PWR1_SLEEP_BIT;
	MPU6050_swrite(PWR_MGMT_1,empty);
}
unsigned char getWakeCycleEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	ret = empt&~(1<<MPU6050_PWR1_CYCLE_BIT);
	return ret;
}
void setWakeCycleEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	empty|= 1<<MPU6050_PWR1_CYCLE_BIT;
	MPU6050_swrite(PWR_MGMT_1,empty);
}
unsigned char getTempSensorEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	ret = empty&~(1<<MPU6050_PWR1_TEMP_DIS_BIT);
	return ret;
}
void setTempSensorEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	empty|= 1<<MPU6050_PWR1_TEMP_DIS_BIT;
	MPU6050_swrite(PWR_MGMT_1,empty);
}
unsigned char getClockSource(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	ret  = empty &~MPU6050_PWR1_CLKSEL_SET;
	return ret;
}
void setClockSource(unsigned char source)
{
	unsigned char empty=0;
	empty = MPU6050_sread(PWR_MGMT_1);
	empty&= ~MPU6050_PWR1_CLKSEL_SET;
	MPU6050_swrite(PWR_MGMT_1,empty|source);
}



// MOT_THR register
unsigned char getMotionDetectionThreshold(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(MOT_THR);
	ret  = empty;
	return ret;
}
void setMotionDetectionThreshold(unsigned char threshold)
{
	MPU6050_swrite(MOT_THR,threshold);
}

// INT_PIN_CFG register
unsigned char getInterruptMode(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	ret  = empty&(1<<MPU6050_INTCFG_INT_LEVEL_BIT);
	return ret>>MPU6050_INTCFG_INT_LEVEL_BIT;
}
void setInterruptMode(unsigned char mode)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	mode<<=MPU6050_INTCFG_INT_LEVEL_BIT;
	MPU6050_swrite(INT_PIN_CFG,empty|mode);
}
unsigned char  getInterruptDrive(void)
{
	unsigned char ret=0,empty=0;
	empty= MPU6050_sread(INT_PIN_CFG);
	ret  = empty&(1<<MPU6050_INTCFG_INT_OPEN_BIT);
	return ret>>MPU6050_INTCFG_INT_OPEN_BIT;
}
void setInterruptDrive(unsigned char drive)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	drive<<=MPU6050_INTCFG_INT_OPEN_BIT;
	MPU6050_swrite(INT_PIN_CFG,empty|drive);
}
unsigned char getInterruptLatch(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	ret  = empty&(1<<MPU6050_INTCFG_LATCH_INT_EN_BIT);
	return ret>>MPU6050_INTCFG_LATCH_INT_EN_BIT;
}
void setInterruptLatch(unsigned char latch)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	latch<<=MPU6050_INTCFG_LATCH_INT_EN_BIT;
	MPU6050_swrite(INT_PIN_CFG,empty|latch);
}
unsigned char getInterruptLatchClear(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	ret  = empty&(1<<MPU6050_INTCFG_INT_RD_CLEAR_BIT);
	return ret>>MPU6050_INTCFG_INT_RD_CLEAR_BIT;
}
void setInterruptLatchClear(unsigned char clear)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	clear<<=MPU6050_INTCFG_INT_RD_CLEAR_BIT;
	MPU6050_swrite(INT_PIN_CFG,empty|clear);
}
unsigned char getFSyncInterruptLevel(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	ret  = empty&(1<<MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT);
	return ret>>MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT;
}
void setFSyncInterruptLevel(unsigned char level)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	level<<=MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT;
	MPU6050_swrite(INT_PIN_CFG,empty|level);
}
unsigned char getFSyncInterruptEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	ret  = empty&(1<<MPU6050_INTCFG_FSYNC_INT_EN_BIT);
	return ret>>MPU6050_INTCFG_FSYNC_INT_EN_BIT;
}
void setFSyncInterruptEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	enabled<<=MPU6050_INTCFG_FSYNC_INT_EN_BIT
	MPU6050_swrite(INT_PIN_CFG,empty|enabled);
}
unsigned char getI2CBypassEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	ret  = empty&(1<<MPU6050_INTCFG_I2C_BYPASS_EN_BIT);
	return ret>>MPU6050_INTCFG_I2C_BYPASS_EN_BIT;
}
void setI2CBypassEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_PIN_CFG);
	enabled<<=MPU6050_INTCFG_I2C_BYPASS_EN_BIT;
	MPU6050_swrite(INT_PIN_CFG,empty|enabled);
}

// INT_ENABLE register
unsigned char getIntMotionEnabled(void)
{
	unsigned char ret=0,temp=0;
	empty = MPU6050_sread(INT_ENABLE);
	ret  = empty&(1<<MPU6050_INTERRUPT_MOT_BIT);
	return ret>>MPU6050_INTERRUPT_MOT_BIT;
}
void setIntMotionEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_ENABLE);
	enabled<<=MPU6050_INTERRUPT_MOT_BIT;
	MPU6050_swrite(INT_ENABLE,empty|enabled);
}
unsigned char getIntFIFOBufferOverflowEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_ENABLE);
	ret  = empty&(1<<MPU6050_INTERRUPT_FIFO_OFLOW_BIT);
	return ret>>MPU6050_INTERRUPT_FIFO_OFLOW_BIT;
}
void setIntFIFOBufferOverflowEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_ENABLE);
	enabled<<=MPU6050_INTERRUPT_FIFO_OFLOW_BIT
	MPU6050_swrite(INT_ENABLE,empty|enabled);
}
unsigned char getIntI2CMasterEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_ENABLE);
	ret  = empty&(1<<MPU6050_INTERRUPT_I2C_MST_INT_BIT);
	return ret>>MPU6050_INTERRUPT_I2C_MST_INT_BIT;
}
void setIntI2CMasterEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_ENABLE);
	enabled<<=MPU6050_INTERRUPT_I2C_MST_INT_BIT;
	MPU6050_swrite(INT_ENABLE,empty|enabled);
}
unsigned char getIntDataReadyEnabled(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_ENABLE);
	ret  = empty&(1<<MPU6050_INTERRUPT_DATA_RDY_BIT);
	return ret>>MPU6050_INTERRUPT_DATA_RDY_BIT;
}
void setIntDataReadyEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(INT_ENABLE);
	enabled<<=MPU6050_INTERRUPT_DATA_RDY_BIT;
	MPU6050_swrite(INT_ENABLE,empty|enabled);
}

// INT_STATUS register
unsigned char getIntMotionStatus(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_STATUS);
	ret  = empty&(1<<MPU6050_INTERRUPT_MOT_BIT);
	return ret>>MPU6050_INTERRUPT_MOT_BIT;
}
unsigned char getIntFIFOBufferOverflowStatus(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_STATUS);
	ret  = empty&(1<<MPU6050_INTERRUPT_FIFO_OFLOW_BIT);
	return ret>>MPU6050_INTERRUPT_FIFO_OFLOW_BIT;
}
unsigned char getIntI2CMasterStatus(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_STATUS);
	ret   = empty&(1<<MPU6050_INTERRUPT_I2C_MST_INT_BIT);
	return ret>>MPU6050_INTERRUPT_I2C_MST_INT_BIT;
}
unsigned char getIntDataReadyStatus(void)
{
	unsigned char ret=0,empty=0;
	empty = MPU6050_sread(INT_STATUS);
	ret   = empty&(1<<MPU6050_INTERRUPT_DATA_RDY_BIT);
	return ret>>MPU6050_INTERRUPT_DATA_RDY_BIT;
}

// ACCEL_*OUT_* registers
void getAcceleration(short int* x,short int* y,short int* z)
{

	unsigned char Data[6]=0;

	Single_read(Data,MPU6050_Addr,ACCEL_XOUT_H,6);

	x = (Data[0]<<8)|Data[1];
	y = (Data[2]<<8)|Data[3];
	z = (Data[4]<<8)|Data[5];
}
short int getAccelerationX(void)
{
	unsigned char Data[2]=0;
	short int ret;

	Single_read(Data,MPU6050_Addr,ACCEL_XOUT_H,2);
	ret = (Data[0]<<8)|Data[1];

	return ret;
}
short int getAccelerationY(void)
{
	unsigned char Data[2]=0;
	short int ret;

	Single_read(Data,MPU6050_Addr,ACCEL_YOUT_H,2);
	ret = (Data[0]<<8)|Data[1];

	return ret;
}
short int getAccelerationZ(void)
{
	unsigned char Data[2]=0;
	short int ret;

	Single_read(Data,MPU6050_Addr,ACCEL_ZOUT_H,2);
	ret = (Data[0]<<8)|Data[1];

	return ret;
}

// TEMP_OUT_* registers
short int getTemperature(void)
{
	unsigned char Data[2]=0;
	short int ret;

	Single_read(Data,MPU6050_Addr,TEMP_OUT_H,2);
	ret = (Data[0]<<8)|Data[1];

	return ret;
}

// GYRO_*OUT_* registers
void getgyroscope(short int* x,short int* y,short int* z)
{
	unsigned char Data[6]=0;

	Single_read(Data,MPU6050_Addr,GYRO_XOUT_H,6);

	x = (Data[0]<<8)|Data[1];
	y = (Data[2]<<8)|Data[3];
	z = (Data[4]<<8)|Data[5];
}
short int getgyroscopeX(void)
{
	unsigned char Data[2]=0;
	short int ret;

	Single_read(Data,MPU6050_Addr,GYRO_XOUT_H,2);
	ret = (Data[0]<<8)|Data[1];

	return ret;
}
short int getgyroscopeY(void)
{
	unsigned char Data[2]=0;
	short int ret;

	Single_read(Data,MPU6050_Addr,GYRO_YOUT_H,2);
	ret = (Data[0]<<8)|Data[1];

	return ret;
}
short int tgetgyroscopeZ(void)
{
	unsigned char Data[2]=0;
	short int ret;

	Single_read(Data,MPU6050_Addr,GYRO_ZOUT_H,2);
	ret = (Data[0]<<8)|Data[1];

	return ret;
}

void getMotion6(short int* ax,short int* ay,short int* az,short int* gx,short int* gy,short int* gz,short int* temp)
{
	unsigned char Data[14]=0;

	Single_read(Data,MPU6050_Addr,GYRO_XOUT_H,14);

	ax = (Data[0]<<8)|Data[1];
	ay = (Data[2]<<8)|Data[3];
	az = (Data[4]<<8)|Data[5];

	temp = (Data[6]<<8)|Data[7];

	gx = (Data[8]<<8)|Data[9];
	gy = (Data[10]<<8)|Data[11];
	gz = (Data[12]<<8)|Data[13];
}


// SIGNAL_PATH_RESET register
void resetGyroscopePath(void)
{
	unsigned char empty=0;
	empty = MPU6050_sread(SIGNAL_PATH_RESET);
	empty|= 1<<MPU6050_PATHRESET_GYRO_RESET_BIT;
	MPU6050_swrite(INT_ENABLE,empty);
}
void resetAccelerometerPath(void)
{
	unsigned char empty=0;
	empty = MPU6050_sread(SIGNAL_PATH_RESET);
	empty|= 1<<MPU6050_PATHRESET_ACCEL_RESET_BIT;
	MPU6050_swrite(INT_ENABLE,empty);
}
void resetTemperaturePath(void)
{
	unsigned char empty=0;
	empty = MPU6050_sread(SIGNAL_PATH_RESET);
	empty|= 1<<MPU6050_PATHRESET_TEMP_RESET_BIT;
	MPU6050_swrite(INT_ENABLE,empty);
}

// MOT_DETECT_CTRL register
unsigned char getAccelerometerPowerOnDelay(void)
{
	unsigned char empty=0;
	temp = MPU6050_sread(MOT_DETECT_CTRL);
	return temp>>MPU6050_DETECT_ACCEL_ON_DELAY_OFFSET;
}
void setAccelerometerPowerOnDelay(unsigned char delay)
{
	unsigned char empty=0;
	empty = MPU6050_sread(MOT_DETECT_CTRL);
	empty&= ~MPU6050_DETECT_ACCEL_ON_DELAY_SET;
	MPU6050_swrite(INT_ENABLE,empty|(delay<<MPU6050_DETECT_ACCEL_ON_DELAY_OFFSET));
}

// USER_CTRL register
unsigned char getFIFOEnabled(void)
{
	unsigned char empty=0,ret=0;
	empty = MPU6050_sread(USER_CTRL);
	ret   = empty&(1<<MPU6050_USERCTRL_FIFO_EN_BIT);
	return ret>>MPU6050_USERCTRL_FIFO_EN_BIT;
}
void setFIFOEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(USER_CTRL);
	empty = empty|(1<<MPU6050_USERCTRL_FIFO_EN_BIT);
	MPU6050_swrite(USER_CTRL,empty);
}
unsigned char getI2CMasterModeEnabled(void)
{
	unsigned char empty=0,ret=0;
	empty = MPU6050_sread(USER_CTRL);
	ret   = empty&(1<<MPU6050_USERCTRL_I2C_MST_EN_BIT);
	return ret>>MPU6050_USERCTRL_I2C_MST_EN_BIT;
}
void setI2CMasterModeEnabled(unsigned char enabled)
{
	unsigned char empty=0;
	empty = MPU6050_sread(USER_CTRL);
	empty = empty|(1<<MPU6050_USERCTRL_I2C_MST_EN_BIT);
	MPU6050_swrite(USER_CTRL,empty);
}
//void switchSPIEnabled(unsigned char enabled);
void resetFIFO(void)
{
	unsigned char empty=0;
	empty = MPU6050_sread(USER_CTRL);
	empty = empty|(1<<MPU6050_USERCTRL_FIFO_RESET_BIT);
	MPU6050_swrite(USER_CTRL,empty);
}
void resetI2CMaster(void)
{
	unsigned char empty=0;
	empty = MPU6050_sread(USER_CTRL);
	empty = empty|(1<<MPU6050_USERCTRL_I2C_MST_RESET_BIT);
	MPU6050_swrite(USER_CTRL,empty);
}
void resetSensors(void)
{
	unsigned char empty=0;
	empty = MPU6050_sread(USER_CTRL);
	empty = empty|(1<<MPU6050_USERCTRL_SIG_COND_RESET_BIT);
	MPU6050_swrite(USER_CTRL,empty);
}

// WHO_AM_I register
unsigned char getDeviceID(void)
{
	unsigned char empty=0,ret=0;
	ret = MPU6050_sread(WHO_AM_I);
	return ret>>1;
}
void setDeviceID(unsigned char id)
{

	MPU6050_swrite(WHO_AM_I,id<<1);
}

/**


void Read_MPU6050AG(MPU6050AGVALUE_Typedef *pAg_value){
  unsigned char tData[12]={0};
  
  tData[0]=MPU6050_sread(ACCEL_XOUT_H);
  tData[1]=MPU6050_sread(ACCEL_XOUT_L);
  tData[2]=MPU6050_sread(ACCEL_YOUT_H);
  tData[3]=MPU6050_sread(ACCEL_YOUT_L);
  tData[4]=MPU6050_sread(ACCEL_ZOUT_H);
  tData[5]=MPU6050_sread(ACCEL_ZOUT_L);

  tData[6]=MPU6050_sread(GYRO_XOUT_H);
  tData[7]=MPU6050_sread(GYRO_XOUT_L);
  tData[8]=MPU6050_sread(GYRO_YOUT_H);
  tData[9]=MPU6050_sread(GYRO_YOUT_L);
  tData[10]=MPU6050_sread(GYRO_ZOUT_H);
  tData[11]=MPU6050_sread(GYRO_ZOUT_L);

  pAg_value->AccelX = (tData[0]<<8)|tData[1];
  pAg_value->AccelY = (tData[2]<<8)|tData[3];
  pAg_value->AccelZ = (tData[4]<<8)|tData[5];
  pAg_value->GYROX  = (tData[6]<<8)|tData[7];
  pAg_value->GYROY  = (tData[8]<<8)|tData[9];
  pAg_value->GYROZ  = (tData[10]<<8)|tData[11];
}

***/

/*
void Set_MPU6050_REG(unsigned char reg,unsigned char *pdata){

}

unsigned char Read_MPU6050_REG(unsigned char reg);
*/
