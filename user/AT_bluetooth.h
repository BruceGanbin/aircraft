#ifndef __AT_BLUETOOTH_H
#define __AT_BLUETOOTH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;

#define BAUD_1200             '1'
#define BAUD_2400             '2'
#define BAUD_4800             '3'
#define BAUD_9600             '4'
#define BAUD_19200            '5'
#define BAUD_38400            '6'
#define BAUD_57600            '7'
#define BAUD_115200           '8'
#define BAUD_230400           '9'
#define BAUD_460800           'A'
#define BAUD_921600           'B'
#define BAUD_1382400          'C'


typedef struct BLUETOOTH_INIT_
{
  u8 BT_Baud;
  u8 BT_CHK;
  u8 BT_StopBit;
  u8 *BT_Name;
  u8 BT_AUTH;
  u8 *BT_PIN;


  void (*Serial_SendData)(u8 *Data,u16 Num);
}BT_InitTypedef;

  void BT_SendData(char *Data);
  void BT_Init();
  u8   BT_Set();
  u8   BT_RState();
  u8   BT_MasterInit();
  u8   BT_SlaveInit();
  u8   BT_CMD();
  


#ifdef __cplusplus
}
#endif

#endif
