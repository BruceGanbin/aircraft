#include "HMC5883.h"


static unsigned char *(*Single_read)(unsigned char *pBuffer,unsigned char PartAddr,      \
										unsigned char WriteAddr,unsigned char NumByteToWrite);
static unsigned char *(*Single_write)(unsigned char *pBuffer,unsigned char PartAddr,     \
										 unsigned char WriteAddr,unsigned char NumByteToWrite);


char HMC5883_swrite(unsigned char WriteAddr,unsigned char data)
{
	Single_write(&data,HMC5883_ADDRESS,WriteAddr,1);
	return 1;
}
char HMC5883_sread(unsigned char WriteAddr)
{
	unsigned char data;
	Single_read(&data,HMC5883_ADDRESS,WriteAddr,1);
	return data;
}

void Init_HMC5883(HMC5883Init_Typedef *HMC5883_Config)
{
	Single_read = HMC5883_Config->Read_Data;
  Single_write= HMC5883_Config->Write_Data;
	HMC5883_swrite(HMC5883_CONFIG_A,0x70);
  HMC5883_swrite(HMC5883_CONFIG_B,0x00);
	HMC5883_swrite(HMC5883_MODE,0x00);
}

//config register A
void setSampleAveraged(unsigned char num)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_CONFIG_A);
	value&=~HMC5883_MA_MASK;
	HMC5883_swrite(HMC5883_CONFIG_A,value|num);
}
void setOutputRate(unsigned char rate)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_CONFIG_A);
	value&=~HMC5883_DO_MASK;
	HMC5883_swrite(HMC5883_CONFIG_A,value|rate);
}
unsigned char getOutputRate(void)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_CONFIG_A);
	value&=HMC5883_DO_MASK;
	return value;
}
void setMeasurementMode(unsigned char mode)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_CONFIG_A);
	value&=~HMC5883_MS_MASK;
	HMC5883_swrite(HMC5883_CONFIG_A,value|mode);
}
unsigned char getMeasurementMode(void)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_CONFIG_A);
	value|=HMC5883_MS_MASK;
	return value;
}
//config register B
void setGain(unsigned char gain)
{
	gain <<= HMC5883_GAIN_OFFSET;
	HMC5883_swrite(HMC5883_CONFIG_A,gain);
}
unsigned char getGain(void)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_CONFIG_B);
	return value>>HMC5883_GAIN_OFFSET;
}
//mode register
void setHighSpeed(unsigned char speed)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_MODE);
	value&=~HMC5883_HS_MASK;
	speed <<= HMC5883_HS_OFFSET;
	HMC5883_swrite(HMC5883_CONFIG_A,value|speed);
}
unsigned char getHighSpeed(void)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_MODE);
	value|=HMC5883_HS_MASK;
	return value>>HMC5883_HS_OFFSET;
}
void setMode(unsigned char select)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_MODE);
	value&=~HMC5883_MODE_MASK;
	HMC5883_swrite(HMC5883_CONFIG_A,value|select);
}
unsigned char getMode(void)
{
	unsigned char value=0;
	value=HMC5883_sread(HMC5883_MODE);
	value|=HMC5883_MODE_MASK;
	return value;
}
//
short int getCompassX(void)
{
	short int ret=0;
	ret =HMC5883_sread(HMC5883_XOUT_MSB)<<8;
	ret|=HMC5883_sread(HMC5883_XOUT_LSB);
	return ret;
}
short int getCompassZ(void)
{
	short int ret=0;
	ret =HMC5883_sread(HMC5883_ZOUT_MSB)<<8;
	ret|=HMC5883_sread(HMC5883_ZOUT_LSB);
	return ret;
}
short int getCompassY(void)
{
	short int ret=0;
	ret =HMC5883_sread(HMC5883_YOUT_MSB)<<8;
	ret|=HMC5883_sread(HMC5883_YOUT_LSB);
	return ret;
}
void getCompass3(short int* cx,short int* cy,short int*cz)
{
	unsigned char data[6]={0};
	
	Single_read(data,HMC5883_ADDRESS,HMC5883_XOUT_MSB,6);
	*cx = (data[0]<<8)|data[1];
	*cz = (data[2]<<8)|data[3];
	*cy = (data[4]<<8)|data[5];
}

unsigned char getStatus(void)
{
	return HMC5883_sread(HMC5883_STATUS);
}



