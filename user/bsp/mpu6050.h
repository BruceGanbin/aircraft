#ifndef  __MPU6050_H__
#define  __MPU6050_H__

#include "stm32f10x.h"
//MPU6050 Regist Map********************

#define SELF_TEST_X            0x0D
#define SELF_TEST_Y            0x0E
#define SELF_TEST_Z            0x0F
#define SELF_TEST_A            0x10
#define SMPLRT_DIV             0x19

#define CONFIG                 0x1A
#define GYRO_CONFIG            0x1B
#define ACCEL_CONFIG           0x1C

#define MOT_THR                0x1F
#define FIFO_EN                0x23

#define I2C_MST_CTRL           0x24
#define I2C_SLV0_ADDR          0x25
#define I2C_SLV0_REG           0x26
#define I2C_SLV0_CTRL          0x27
#define I2C_SLV1_ADDR          0x28
#define I2C_SLV1_REG           0x29
#define I2C_SLV1_CTRL          0x2A
#define I2C_SLV2_ADDR          0x2B
#define I2C_SLV2_REG           0x2C
#define I2C_SLV2_CTRL          0x2D
#define I2C_SLV3_ADDR          0x2E
#define I2C_SLV3_REG           0x3F
#define I2C_SLV3_CTRL          0x30
#define I2C_SLV4_ADDR          0x31
#define I2C_SLV4_REG           0x32
#define I2C_SLV4_DO            0x33
#define I2C_SLV4_CTRL          0x34
#define I2C_SLV4_DI            0x35
#define I2C_MST_STATUS         0x36
#define I2C_SLV0_DO            0x63
#define I2C_SLV1_DO            0x64
#define I2C_SLV2_DO            0x65
#define I2C_SLV3_DO            0x66
#define I2C_MST_DELAY_CTRL     0x67

//interrupt configure
#define INT_PIN_CFG            0x37
#define INT_ENABLE             0x38
#define INT_STATUS             0x3A
//accelerate & gyro  value
#define ACCEL_XOUT_H           0x3B
#define ACCEL_XOUT_L           0x3C
#define ACCEL_YOUT_H           0x3D
#define ACCEL_YOUT_L           0x3E
#define ACCEL_ZOUT_H           0x3F
#define ACCEL_ZOUT_L           0x40
#define TEMP_OUT_H             0x41
#define TEMP_OUT_L             0x42
#define GYRO_XOUT_H            0x43
#define GYRO_XOUT_L            0x44
#define GYRO_YOUT_H            0x45
#define GYRO_YOUT_L            0x46
#define GYRO_ZOUT_H            0x47
#define GYRO_ZOUT_L            0x48

#define EXT_SENS_DATA_00       0x49
#define EXT_SENS_DATA_01       0x4A
#define EXT_SENS_DATA_02       0x4B
#define EXT_SENS_DATA_03       0x4C
#define EXT_SENS_DATA_04       0x4D
#define EXT_SENS_DATA_05       0x4E
#define EXT_SENS_DATA_06       0x4F
#define EXT_SENS_DATA_07       0x50
#define EXT_SENS_DATA_08       0x51       
#define EXT_SENS_DATA_09       0x52
#define EXT_SENS_DATA_10       0x53
#define EXT_SENS_DATA_11       0x54
#define EXT_SENS_DATA_12       0x55
#define EXT_SENS_DATA_13       0x56
#define EXT_SENS_DATA_14       0x57
#define EXT_SENS_DATA_15       0x58
#define EXT_SENS_DATA_16       0x59
#define EXT_SENS_DATA_17       0x5A
#define EXT_SENS_DATA_18       0x5B
#define EXT_SENS_DATA_19       0x5C
#define EXT_SENS_DATA_20       0x5D
#define EXT_SENS_DATA_21       0x5E
#define EXT_SENS_DATA_22       0x5F
#define EXT_SENS_DATA_23       0x60
// misc
#define SIGNAL_PATH_RESET      0x68
#define MOT_DETECT_CTRL        0x69
#define USER_CTRL              0x6A
#define PWR_MGMT_1             0x6B
#define PWR_MGMT_2             0x6C
#define FIFO_COUNTH            0x72
#define FIFO_COUNTL            0x73
#define FIFO_R_W               0x74
#define WHO                    0x75


/*****
#define	SMPL	0x15
#define DLPF	0x16
#define INT_C	0x17
#define INT_S	0x1A
#define	TMP_H	0x1B
#define	TMP_L	0x1C
#define	GX_H	0x3B
#define	GX_L	0x3C
#define	GY_H	0x3D
#define	GY_L	0x3E
#define GZ_H	0x3F
#define GZ_L	0x40
#define PWR_M	0x3E
**/
#define MPU6050_TC_PWR_MODE_BIT             7
#define MPU6050_TC_OFFSET_BIT               6
#define MPU6050_TC_OFFSET_LENGTH            6
#define MPU6050_TC_OTP_BNK_VLD_BIT          0

#define MPU6050_VDDIO_LEVEL_VLOGIC          0
#define MPU6050_VDDIO_LEVEL_VDD             1

#define MPU6050_CFG_EXT_SYNC_SET            7<<3
#define MPU6050_CFG_EXT_SYNC_OFFSETBIT      3
#define MPU6050_CFG_DLPF_CFG_SET            3<<0
#define MPU6050_CFG_DLPF_CFG_OFFSETBIT      0

#define MPU6050_EXT_SYNC_DISABLED           0x0
#define MPU6050_EXT_SYNC_TEMP_OUT_L         0x1
#define MPU6050_EXT_SYNC_GYRO_XOUT_L        0x2
#define MPU6050_EXT_SYNC_GYRO_YOUT_L        0x3
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L        0x4
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L       0x5
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L       0x6
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L       0x7

#define MPU6050_DLPF_BW_256         0x00
#define MPU6050_DLPF_BW_188         0x01
#define MPU6050_DLPF_BW_98          0x02
#define MPU6050_DLPF_BW_42          0x03
#define MPU6050_DLPF_BW_20          0x04
#define MPU6050_DLPF_BW_10          0x05
#define MPU6050_DLPF_BW_5           0x06

#define MPU6050_GCONFIG_FS_SEL_SET          3<<3
#define MPU6050_GCONFIG_FS_SEL_OFFSETBIT    3

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

#define MPU6050_ACONFIG_XA_ST_BIT           7
#define MPU6050_ACONFIG_YA_ST_BIT           6
#define MPU6050_ACONFIG_ZA_ST_BIT           5
#define MPU6050_ACONFIG_AFS_SEL_SET         3<<3
#define MPU6050_ACONFIG_AFS_SEL_OFFSETBIT   3


#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_DHPF_RESET          0x00
#define MPU6050_DHPF_5              0x01
#define MPU6050_DHPF_2P5            0x02
#define MPU6050_DHPF_1P25           0x03
#define MPU6050_DHPF_0P63           0x04
#define MPU6050_DHPF_HOLD           0x07

#define MPU6050_TEMP_FIFO_EN_BIT    7
#define MPU6050_XG_FIFO_EN_BIT      6
#define MPU6050_YG_FIFO_EN_BIT      5
#define MPU6050_ZG_FIFO_EN_BIT      4
#define MPU6050_ACCEL_FIFO_EN_BIT   3
#define MPU6050_SLV2_FIFO_EN_BIT    2
#define MPU6050_SLV1_FIFO_EN_BIT    1
#define MPU6050_SLV0_FIFO_EN_BIT    0

#define MPU6050_MULT_MST_EN_BIT     7
#define MPU6050_WAIT_FOR_ES_BIT     6
#define MPU6050_SLV_3_FIFO_EN_BIT   5
#define MPU6050_I2C_MST_P_NSR_BIT   4
#define MPU6050_I2C_MST_CLK_BIT     3
#define MPU6050_I2C_MST_CLK_LENGTH  4

#define MPU6050_CLOCK_DIV_348       0x0
#define MPU6050_CLOCK_DIV_333       0x1
#define MPU6050_CLOCK_DIV_320       0x2
#define MPU6050_CLOCK_DIV_308       0x3
#define MPU6050_CLOCK_DIV_296       0x4
#define MPU6050_CLOCK_DIV_286       0x5
#define MPU6050_CLOCK_DIV_276       0x6
#define MPU6050_CLOCK_DIV_267       0x7
#define MPU6050_CLOCK_DIV_258       0x8
#define MPU6050_CLOCK_DIV_500       0x9
#define MPU6050_CLOCK_DIV_471       0xA
#define MPU6050_CLOCK_DIV_444       0xB
#define MPU6050_CLOCK_DIV_421       0xC
#define MPU6050_CLOCK_DIV_400       0xD
#define MPU6050_CLOCK_DIV_381       0xE
#define MPU6050_CLOCK_DIV_364       0xF

#define MPU6050_I2C_SLV_RW_BIT      7
#define MPU6050_I2C_SLV_ADDR_BIT    6
#define MPU6050_I2C_SLV_ADDR_LENGTH 7
#define MPU6050_I2C_SLV_EN_BIT      7
#define MPU6050_I2C_SLV_BYTE_SW_BIT 6
#define MPU6050_I2C_SLV_REG_DIS_BIT 5
#define MPU6050_I2C_SLV_GRP_BIT     4
#define MPU6050_I2C_SLV_LEN_BIT     3
#define MPU6050_I2C_SLV_LEN_LENGTH  4

#define MPU6050_I2C_SLV4_RW_BIT         7
#define MPU6050_I2C_SLV4_ADDR_BIT       6
#define MPU6050_I2C_SLV4_ADDR_LENGTH    7
#define MPU6050_I2C_SLV4_EN_BIT         7
#define MPU6050_I2C_SLV4_INT_EN_BIT     6
#define MPU6050_I2C_SLV4_REG_DIS_BIT    5
#define MPU6050_I2C_SLV4_MST_DLY_BIT    4
#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5

#define MPU6050_MST_PASS_THROUGH_BIT    7
#define MPU6050_MST_I2C_SLV4_DONE_BIT   6
#define MPU6050_MST_I2C_LOST_ARB_BIT    5
#define MPU6050_MST_I2C_SLV4_NACK_BIT   4
#define MPU6050_MST_I2C_SLV3_NACK_BIT   3
#define MPU6050_MST_I2C_SLV2_NACK_BIT   2
#define MPU6050_MST_I2C_SLV1_NACK_BIT   1
#define MPU6050_MST_I2C_SLV0_NACK_BIT   0

#define MPU6050_INTCFG_INT_LEVEL_BIT        7
#define MPU6050_INTCFG_INT_OPEN_BIT         6
#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1
#define MPU6050_INTCFG_CLKOUT_EN_BIT        0

#define MPU6050_INTMODE_ACTIVEHIGH  0x00
#define MPU6050_INTMODE_ACTIVELOW   0x01

#define MPU6050_INTDRV_PUSHPULL     0x00
#define MPU6050_INTDRV_OPENDRAIN    0x01

#define MPU6050_INTLATCH_50USPULSE  0x00
#define MPU6050_INTLATCH_WAITCLEAR  0x01

#define MPU6050_INTCLEAR_STATUSREAD 0x00
#define MPU6050_INTCLEAR_ANYREAD    0x01

#define MPU6050_INTERRUPT_FF_BIT            7
#define MPU6050_INTERRUPT_MOT_BIT           6
#define MPU6050_INTERRUPT_ZMOT_BIT          5
#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2
#define MPU6050_INTERRUPT_DMP_INT_BIT       1
#define MPU6050_INTERRUPT_DATA_RDY_BIT      0

// TODO: figure out what these actually do
// UMPL source code is not very obivous
#define MPU6050_DMPINT_5_BIT            5
#define MPU6050_DMPINT_4_BIT            4
#define MPU6050_DMPINT_3_BIT            3
#define MPU6050_DMPINT_2_BIT            2
#define MPU6050_DMPINT_1_BIT            1
#define MPU6050_DMPINT_0_BIT            0

#define MPU6050_MOTION_MOT_XNEG_BIT     7
#define MPU6050_MOTION_MOT_XPOS_BIT     6
#define MPU6050_MOTION_MOT_YNEG_BIT     5
#define MPU6050_MOTION_MOT_YPOS_BIT     4
#define MPU6050_MOTION_MOT_ZNEG_BIT     3
#define MPU6050_MOTION_MOT_ZPOS_BIT     2
#define MPU6050_MOTION_MOT_ZRMOT_BIT    0

#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

#define MPU6050_PATHRESET_GYRO_RESET_BIT    2
#define MPU6050_PATHRESET_ACCEL_RESET_BIT   1
#define MPU6050_PATHRESET_TEMP_RESET_BIT    0

#define MPU6050_DETECT_ACCEL_ON_DELAY_SET       3<<4
#define MPU6050_DETECT_ACCEL_ON_DELAY_OFFSET    4
#define MPU6050_DETECT_FF_COUNT_BIT             3
#define MPU6050_DETECT_FF_COUNT_LENGTH          2
#define MPU6050_DETECT_MOT_COUNT_BIT            1
#define MPU6050_DETECT_MOT_COUNT_LENGTH         2

#define MPU6050_DETECT_DECREMENT_RESET  0x0
#define MPU6050_DETECT_DECREMENT_1      0x1
#define MPU6050_DETECT_DECREMENT_2      0x2
#define MPU6050_DETECT_DECREMENT_4      0x3

#define MPU6050_USERCTRL_DMP_EN_BIT             7
#define MPU6050_USERCTRL_FIFO_EN_BIT            6
#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
#define MPU6050_USERCTRL_DMP_RESET_BIT          3
#define MPU6050_USERCTRL_FIFO_RESET_BIT         2
#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0

#define MPU6050_PWR1_DEVICE_RESET_BIT   7
#define MPU6050_PWR1_SLEEP_BIT          6
#define MPU6050_PWR1_CYCLE_BIT          5
#define MPU6050_PWR1_TEMP_DIS_BIT       3
#define MPU6050_PWR1_CLKSEL_SET         3<<0
#define MPU6050_PWR1_CLKSEL_OFFSETBIT   0

#define MPU6050_CLOCK_INTERNAL          0x00
#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_CLOCK_PLL_EXT32K        0x04
#define MPU6050_CLOCK_PLL_EXT19M        0x05
#define MPU6050_CLOCK_KEEP_RESET        0x07

#define MPU6050_PWR2_LP_WAKE_CTRL_BIT       7
#define MPU6050_PWR2_LP_WAKE_CTRL_LENGTH    2
#define MPU6050_PWR2_STBY_XA_BIT            5
#define MPU6050_PWR2_STBY_YA_BIT            4
#define MPU6050_PWR2_STBY_ZA_BIT            3
#define MPU6050_PWR2_STBY_XG_BIT            2
#define MPU6050_PWR2_STBY_YG_BIT            1
#define MPU6050_PWR2_STBY_ZG_BIT            0

#define MPU6050_WAKE_FREQ_1P25      0x0
#define MPU6050_WAKE_FREQ_2P5       0x1
#define MPU6050_WAKE_FREQ_5         0x2
#define MPU6050_WAKE_FREQ_10        0x3

#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6
#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5
#define MPU6050_BANKSEL_MEM_SEL_BIT         4
#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5

#define MPU6050_WHO_AM_I_SET        0x7E
#define MPU6050_WHO_AM_I_LENGTH     1

#define MPU6050_DMP_MEMORY_BANKS        8
#define MPU6050_DMP_MEMORY_BANK_SIZE    256
#define MPU6050_DMP_MEMORY_CHUNK_SIZE   16



//****************************

#define	MPU6050_Addr   0xD0

typedef struct _MPU6050{

	unsigned char *(*Read_Data)(unsigned char *pBuffer,unsigned char PartAddr,        \
								unsigned char WriteAddr,unsigned char NumByteToWrite);
	unsigned char *(*Write_Data)(unsigned char *pBuffer,unsigned char PartAddr,       \
								 unsigned char WriteAddr,unsigned char NumByteToWrite);

}MPU6050Init_Typedef;

/*

typedef struct _MPU6050_AG_VALUE{

	unsigned int AccelX;
	unsigned int AccelY;
	unsigned int AccelZ;
	unsigned int GYROX;
	unsigned int GYROY;
	unsigned int GYROZ;
}MPU6050AGVALUE_Typedef;
*/


void Init_MPU6050(MPU6050Init_Typedef *MPU6050_Config);
unsigned char testConnection(void);   //test read ID 

char MPU6050_swrite(unsigned char WriteAddr,unsigned char Data);
char MPU6050_sread(unsigned char WriteAddr);

//CONFIG register
unsigned char getExternalFrameSync(void);
void setExternalFrameSync(unsigned char sync);
unsigned char getDLPFMode(void);
void setDLPFMode(unsigned char bandwidth);

//GYRO_CONFIG register
unsigned char getFullScaleGyroRange(void);
void setFullScaleGyroRange(unsigned char range);

//ACCEL_CONFIG register
unsigned char getFullScaleAccelRange(void);
void setFullScaleAccelRange(unsigned char range);

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
void setIntDataReadyEnabled(unsigned char enabled);

// INT_STATUS register
unsigned char getIntMotionStatus(void);
unsigned char getIntFIFOBufferOverflowStatus(void);
unsigned char getIntI2CMasterStatus(void);
unsigned char getIntDataReadyStatus(void);

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

void getMotion6(short int* ax,short int* ay,short int* az,short int* gx,short int* gy,short int* gz,short int* temp);

//void Read_MPU6050AG(MPU6050AGVALUE_Typedef *pAg_value);


// SIGNAL_PATH_RESET register
void resetGyroscopePath(void);
void resetAccelerometerPath(void);
void resetTemperaturePath(void);


// MOT_DETECT_CTRL register
unsigned char getAccelerometerPowerOnDelay(void);
void setAccelerometerPowerOnDelay(unsigned char delay);
/*
unsigned char getFreefallDetectionCounterDecrement(void);
void setFreefallDetectionCounterDecrement(unsigned char decrement);
unsigned char getMotionDetectionCounterDecrement(void);
void setMotionDetectionCounterDecrement(unsigned decrement);
*/
// USER_CTRL register
unsigned char getFIFOEnabled(void);
void setFIFOEnabled(unsigned char enabled);
unsigned char getI2CMasterModeEnabled(void);
void setI2CMasterModeEnabled(unsigned char enabled);
//void switchSPIEnabled(unsigned char enabled);
void resetFIFO(void);
void resetI2CMaster(void);
void resetSensors(void);

// PWR_MGMT_1 register
void reset(void);
unsigned char getSleepEnabled(void);
void setSleepEnabled(unsigned char enabled);
unsigned char getWakeCycleEnabled(void);
void setWakeCycleEnabled(unsigned char enabled);
unsigned char getTempSensorEnabled(void);
void setTempSensorEnabled(unsigned char enabled);
unsigned char getClockSource(void);
void setClockSource(unsigned char source);

// PWR_MGMT_2 register
unsigned char getWakeFrequency(void);
void setWakeFrequency(unsigned char frequency);
unsigned char getStandbyXAccelEnabled(void);
void setStandbyXAccelEnabled(unsigned char enabled);
unsigned char getStandbyYAccelEnabled(void);
void setStandbyYAccelEnabled(unsigned char enabled);
unsigned char getStandbyZAccelEnabled(void);
void setStandbyZAccelEnabled(unsigned char enabled);
unsigned char getStandbyXGyroEnabled(void);
void setStandbyXGyroEnabled(unsigned char enabled);
unsigned char getStandbyYGyroEnabled(void);
void setStandbyYGyroEnabled(unsingned char enabled);
unsigned char getStandbyZGyroEnabled(void);
void setStandbyZGyroEnabled(unsigned char enabled);

// FIFO_COUNT_* registers
short int getFIFOCount(void);

// FIFO_R_W register
unsigned char getFIFOByte(void);
void setFIFOByte(unsigned char data);

// WHO_AM_I register
unsigned char getDeviceID(void);
void setDeviceID(unsigned char id);

#endif
