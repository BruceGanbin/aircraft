#include "AT_bluetooth.h"


#include "includes.h"


void BT_SendData(char *pData)
{
  char *pHead;
  u8  Num=0;
  pHead=pData;
  while( *pData!= '\0')
  {
	 pData++;
	 Num++;
  }
  Serial_SendData(pData,Num);
}

char *BT_ReadData(void)
{
  return Serial_ReadData();
}


void (*Serial_SendData)(char *Data,u8 Num)=0;
char *(*Serial_ReadData)(void)=0;


u8 BT_Init(BT_InitTypedef *BT_Config)
{
  char AT_cmd[20]="AT+";
  char Empty[5];
  u16 OverTime=0;
  char *pAT;

  //init BT_SendData();
  Serial_SendData = BT_Config.BT_SendData;
  //init BT_ReadData();
  Serial_ReadData = BT_Config.BT_ReadData;

  // check bluetooth is OK
  BT_SendData("AT");
  while(OverTime>TIMEOUT)
  {
	 if(!strcmp("OK",strncpy(Empty,BT_ReadData(),2))) break;

	 OverTime++;		 
  }
  if(OverTime>TIMEOUT)  return 0;
 
  //set bluetooth name  :"AT+"name
  if(BT_Config->BT_State & Set_BTname)
  {
	 BT_SendData(strcat(AT_cmd,BT_Config->BT_Name));
	 while(OverTime>TIMEOUT)
	 {
		if(!strcmp("OK",strncpy(Empty,BT_ReadData(),2))) break;
		OverTime++;		 
	 }
	 if(OverTime>TIMEOUT)  return 0;
  }

  //set bluetooth Auth  :"AT+"AUTH1:AUTH0
  if(BT_Config->BT_State & Set_BTauth)
  {
	 BT_SendData(strcat(AT_cmd,BT_Config->BT_Auth));
	 while(OverTime>TIMEOUT)
	 {
		if(!strcmp("OK",strncpy(Empty,BT_ReadData(),2))) break;
		OverTime++;		 
	 }
	 if(OverTime>TIMEOUT)  return 0;
  }

  //set bluetooth PIN  :"AT+"PIN***
  if(BT_Config->BT_State & Set_BTPIN)
  {
	 BT_SendData(strcat(AT_cmd,BT_Config->BT_PIN));
	 while(OverTime>TIMEOUT)
	 {
		if(!strcmp("OK",strncpy(Empty,BT_ReadData(),2))) break;
		OverTime++;		 
	 }
	 if(OverTime>TIMEOUT)  return 0;  
  }


  //set bluetooth Baud rate  :"AT+"BAUDx x
  if(BT_Config->BT_State & Set_BTBaud)
  {
	 BT_SendData(strcat(AT_cmd,BT_Config->BT_Baud));
	 while(OverTime>TIMEOUT)
	 {
		if(!strcmp("OK",strncpy(Empty,BT_ReadData(),2))) break;
		OverTime++;		 
	 }
	 if(OverTime>TIMEOUT)  return 0;  
  }

  //set bluetooth CHK  :"AT+"CHK0,1,2
  if(BT_Config->BT_State & Set_BTChk)
  {
	 BT_SendData(strcat(AT_cmd,BT_Config->BT_Chk));
	 while(OverTime>TIMEOUT)
	 {
		if(!strcmp("OK",strncpy(Empty,BT_ReadData(),2))) break;
		OverTime++;		 
	 }
	 if(OverTime>TIMEOUT)  return 0;  
  }

  //set bluetooth StopBit  :"AT+"STOP1,2
  if(BT_Config->BT_State & Set_BTChk)
  {
	 BT_SendData(strcat(AT_cmd,BT_Config->BT_Chk));
	 while(OverTime>TIMEOUT)
	 {
		if(!strcmp("OK",strncpy(Empty,BT_ReadData(),2))) break;
		OverTime++;		 
	 }
	 if(OverTime>TIMEOUT)  return 0;  
  }

  // master or slave
  if(BT_Config->Server&MASTER_S)
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
