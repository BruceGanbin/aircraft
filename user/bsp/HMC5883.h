#ifndef  __HMC5883_H__
#define  __HMC5883_H__

// Register
#define HMC5883_CONFIG_A            0x00
#define HMC5883_CONFIG_B            0x01
#define HMC5883_MODE                0x02
#define HMC5883_XOUT_MSB            0x03
#define HMC5883_XOUT_LSB            0x04
#define HMC5883_ZOUT_MSB            0x05
#define HMC5883_ZOUT_LSB            0x06
#define HMC5883_YOUT_MSB            0x07
#define HMC5883_YOUT_LSB            0x08
#define HMC5883_STATUS              0x09
#define HMC5883_IDEN_A              0x0a
#define HMC5883_IDEN_B              0x0b
#define HMC5883_IDEN_C              0x0c

#define HMC5883_ADDRESS             0x3C

//CONFIG_A
#define HMC5883_MA_MASK             0x60
#define HMC5883_DO_MASK             0x1C
#define HMC5883_MS_MASK             0x03
#define HMC5883_MA_OFFSET           5
#define HMC5883_DO_OFFSET           2
#define HMC5883_MS_OFFSET           0
#define HMC5883_SAMPLES_1           0x00
#define HMC5883_SAMPLES_2           0x20
#define HMC5883_SAMPLES_4           0x40
#define HMC5883_SAMPLES_8           0x60

//CONFIG_B
#define HMC5883_GAIN_OFFSET         5

//MODE
#define HMC5883_HS_MASK            0xFC
#define HMC5883_HS_OFFSET          2
#define HMC5883_MODE_MASK          0x03



typedef struct _HMC5883
{
	unsigned char *(*Read_Data)(unsigned char *pBuffer,unsigned char PartAddr,unsigned char WriteAddr,unsigned char NumByteToWrite);
	unsigned char *(*Write_Data)(unsigned char *pBuffer,unsigned char PartAddr,unsigned char WriteAddr,unsigned char NumByteToWrite);
}HMC5883Init_Typedef;



void Init_HMC5883(HMC5883Init_Typedef *HMC5883_Config);

char HMC5883_swrite(unsigned char WriteAddr,unsigned char data);
char HMC5883_sread(unsigned char WriteAddr);


void setSampleAveraged(unsigned char num);
void setOutputRate(unsigned char rate);
unsigned char getOutputRate(void);
void setMeasurementMode(unsigned char mode);
unsigned char getMeasurementMode(void);

void setGain(unsigned char gain);
unsigned char getGain(void);

void setHighSpeed(unsigned char speed);
unsigned char getHighSpeed(void);
void setMode(unsigned char select);
unsigned char getMode(void);

short int getCompassX(void);
short int getCompassZ(void);
short int getCompassY(void);
void getCompass3(short int* cx,short int* cy,short int*cz);

unsigned char getStatus(void);


#endif
