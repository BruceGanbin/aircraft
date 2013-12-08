#include "AT_bluetooth.h"


#include "includes.h"




u8 (*Serial_SendData)(char *Data,u32 Num);
char *(*Serial_ReadData)(u16 *Num);

/**
char *BT_GetCmd(void)
{

}  

*****/


void BT_SendCmd(char *pData)
{
  u8 i=0;
  char Cmd_buffer[50];
  char *pCmd;
  pCmd=Cmd_buffer;
  //clean buffer
  do{*pCmd=0,pCmd++;}while(i++>=50);
  strncpy(Cmd_buffer,"AT+",3);
  strcat(Cmd_buffer,pData);
  BT_SendData(Cmd_buffer);
}  

/**

**********/
void BT_SendData(char *pData)
{
  char *pHead;
  u8  Num=0;
  pHead=pData;
  while( *pHead!= '\0')
  {
	 pHead++;
	 Num++;
  }
  Serial_SendData(pData,Num);
}

/**
******/
char *BT_ReadData(u16 *Num)
{
  return Serial_ReadData(Num);
}




char BT_Init(BT_InitTypedef *BT_Config)
{
  char AT_cmd[50]={0};
  char OK_buf[5]={0};
  //  char Empty[5]={0};
  u16 OverTime=0;
//  int Count=0;
  char *pAT=AT_cmd;
  u16 Read_count;
  u8 i;

  //init BT_SendData();
  Serial_SendData = BT_Config->BT_SendData;
  //init BT_ReadData();
  Serial_ReadData = BT_Config->BT_ReadData;

	
  BT_SendData("AT");
  while(OverTime<TIMEOUT)
  { for(i=0;i<5;i++) OK_buf[i]=0x00;  
    if(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_count),2),2)==0) break;
    OverTime++;		 
  }
  if(OverTime>TIMEOUT)  return 0;
  
  
  //set bluetooth name  :"AT+"name
  BT_SendCmd("NAME?");
  pAT=BT_ReadData(&Read_count);
  strncpy(AT_cmd,pAT+8,(Read_count-8));
  if(strncmp(AT_cmd,BT_Config->BT_Name,Read_count-8)!=0)
  {
	 for(i=0;i<Read_count;i++)	AT_cmd[i]=0;
	 strncpy(AT_cmd,"NAME",4);
	 strcat(AT_cmd,BT_Config->BT_Name);

	 BT_SendCmd(AT_cmd);
	 while(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_count),2),2)!=0) 
	 {  }
  }


  //set bluetooth Auth  :"AT+"AUTH
  BT_SendCmd("AUTH?");
  pAT=BT_ReadData(&Read_count);
  strncpy(AT_cmd,pAT+8,(Read_count-8));
  if(strncmp(AT_cmd,&BT_Config->BT_Auth,Read_count-8)!=0)
  {
	 for(i=0;i<Read_count;i++)	AT_cmd[i]=0;
	 strncpy(AT_cmd,"AUTH",4);
	 strcat(AT_cmd,&BT_Config->BT_Auth);

	 BT_SendCmd(AT_cmd);
	 while(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_count),2),2)!=0) 
	 {  }
  }

  //set bluetooth PIN  :"AT+"PIN***
  BT_SendCmd("PIN?");
  pAT=BT_ReadData(&Read_count);
  strncpy(AT_cmd,pAT+7,(Read_count-7));
  if(strncmp(AT_cmd,&BT_Config->BT_Auth,Read_count-7)!=0)
  {
	 for(i=0;i<Read_count;i++)	AT_cmd[i]=0;
	 strncpy(AT_cmd,"PIN",3);
	 strcat(AT_cmd,&BT_Config->BT_Auth);

	 BT_SendCmd(AT_cmd);
	 while(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_count),2),2)!=0) 
	 {  }
  }


//  //set bluetooth Baud rate  :"AT+"BAUDx x


//  //set bluetooth CHK  :"AT+"CHK0,1,2
  BT_SendCmd("CHK?");
  pAT=BT_ReadData(&Read_count);
  strncpy(AT_cmd,pAT+7,(Read_count-7));
  if(strncmp(AT_cmd,&BT_Config->BT_Chk,Read_count-7)!=0)
  {
	 for(i=0;i<Read_count;i++)	AT_cmd[i]=0;
	 strncpy(AT_cmd,"CHK",3);
	 strcat(AT_cmd,&BT_Config->BT_Chk);

	 BT_SendCmd(AT_cmd);
	 while(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_count),2),2)!=0) 
	 {  }
  }

//  //set bluetooth StopBit  :"AT+"STOP1,2
  BT_SendCmd("STOP?");
  pAT=BT_ReadData(&Read_count);
  strncpy(AT_cmd,pAT+8,(Read_count-8));
  if(strncmp(AT_cmd,&BT_Config->BT_StopBit,Read_count-8)!=0)
  {
	 for(i=0;i<Read_count;i++)	AT_cmd[i]=0;
	 strncpy(AT_cmd,"STOP",4);
	 strcat(AT_cmd,&BT_Config->BT_StopBit);

	 BT_SendCmd(AT_cmd);
	 while(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_count),2),2)!=0) 
	 {  }
  }

//  // master or slave
 
  return 1;
}


//u8   BT_MasterInit(BT_MasterTypedef *BT_MasterConfig)
//{


//}


//u8   BT_SlaveInit(BT_SlaveTypedef *BT_SlaveConfig)
//{

//}

//  u8   BT_Set();
//  u8   BT_RState();


