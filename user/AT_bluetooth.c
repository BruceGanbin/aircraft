#include "AT_bluetooth.h"


#include "includes.h"


void BT_SendData(char *Data)
{
  char *pData;
  u8  Num=0;
  pData=Data;
  while( *Data!= '\0')
  {
	 Data++;
	 Num++;
  }
  Serial_SendData(*Data,Num);
}

void BT_Init(u8 State)
{
  
  if(State==MASTER_S)
  {
	 BT_MasterInit();
  }
  else
  {
	 BT_SlaveInit();
  }
}

  u8   BT_Set();
  u8   BT_RState();
  u8   BT_MasterInit();
  u8   BT_SlaveInit();
  u8   BT_CMD();
