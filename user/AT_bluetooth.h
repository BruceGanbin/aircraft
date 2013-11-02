#ifndef __AT_BLUETOOTH_H
#define __AT_BLUETOOTH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;


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
