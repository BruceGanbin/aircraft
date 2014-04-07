/**
  ******************************************************************************
  * @file    AT_bluetooth.c
  * @author  Bruce_gan (ckbin008@gmail.com)
  * @version V1.0
  * @date    2014/01/7
  * @brief   the drive to HM-06 bluetooth module 
  *          
  *          
  ******************************************************************************
  **/


#include "AT_bluetooth.h"
#include "includes.h"

//  read data and  send data port
u8 (*Serial_SendData)(char *Data,u32 Num);
char *(*Serial_ReadData)(u16 *Num,u8 swait);


/**
***********************************
@Function : BT_Init()
@Brief    :	module initialization
***********************************/
char BT_Init(BT_InitTypedef *BT_Config)
{

	char OK_buf[5]={0};
	u16 OverTime=0;
	u16 Read_count;
	u8 i;

	//init BT_SendData();
	Serial_SendData = BT_Config->BT_SendData;
	//init BT_ReadData();
	Serial_ReadData = BT_Config->BT_ReadData;
  
  
	//check bluetooth	
	BT_SendData("AT");
	while(OverTime<TIMEOUT)
		{ for(i=0;i<5;i++) OK_buf[i]=0x00;  
			if(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_count),2),2)==0) break;
			OverTime++;		 
		}
	if(OverTime>TIMEOUT)  return 1;
  
	//Set bluetooth name  :"AT+"name
	BT_Set(NAME,BT_Config->BT_Name);
	//set bluetooth Auth  :"AT+"AUTH
	BT_Set(AUTH,BT_Config->BT_Auth);
	//set bluetooth PIN  :"AT+"PIN***
	BT_Set(PIN,BT_Config->BT_PIN);
	//set bluetooth DISC  :"AT+"DISC0,1,2
	BT_Set(DISC,BT_Config->BT_Disc);
	//set bluetooth LED  :"AT+"LED 0,1
	BT_Set(LED,BT_Config->BT_LED);
	
	//  // master or slave

	return 0;
}

/**
***********************************
@Function : BT_Set()
@Brief    :	set the module 
***********************************/
char BT_Set(char *cmd,char *p_value){
  
	u8 len=0;
	u8 cmd_offset=0,i=0;
	u8 Send_Count=0;
	u16 Read_Count=0;
	char AT_cmd[50]={0};
	char OK_buf[5]={0};
	char *pAT=AT_cmd;
	//read value in the module
	len=strlen(cmd);
	BT_SendCmd(cmd);
	pAT=BT_ReadData(&Read_Count);
	cmd_offset=(Read_Count-len-3);
	strncpy(AT_cmd,pAT+(len+3),cmd_offset);

	// check the value and bluetooth value
	if(strcmp(AT_cmd,p_value)!=0)    //different cmd
		{
			for(i=0;i<cmd_offset;i++)	AT_cmd[i]=0;
			strncpy(AT_cmd,cmd,len-1);
			strcat(AT_cmd,p_value);

			Send_Count=BT_SendCmd(AT_cmd);
			while(strncmp("OK",strncpy(OK_buf,BT_ReadData(&Read_Count),2),2)!=0)
			{}

			return 1;
		}

	return 0;
}


/**
***********************************
@Function : BT_SendCmd()
@Brief    :	set AT cmd to the module 
**********************************
   char *BT_ReadCmd(void)
   {

   }  

*/

/**
***********************************
@Function : BT_SendCmd()
@Brief    :	set AT cmd to the module 
***********************************/
static u8 BT_SendCmd(char *pData)
{
	u8 i=0;
	char Cmd_buffer[50]={0};
	u8 Num=0;
	//clean buffer
	do{Cmd_buffer[i]=0x00;}
	while(i++>=50);
	strncpy(Cmd_buffer,"AT+",3);
	// link AT+*pData
	strcat(Cmd_buffer,pData);
	Num=BT_SendData(Cmd_buffer);
	return Num;
}  



/**
***********************************
@Function : BT_SendData()
@Brief    :	send data to module
***********************************/
u8 BT_SendData(char *pData)
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
	return Num;
}


/**
***********************************
@Function : BT_ReadData()
@Brief    :	read data from module
***********************************/
char *BT_ReadData(u16 *Num)
{
  return Serial_ReadData(Num,1);
}

/**
***********************************
@Function : BT_ReadLData()
@Brief    :	read data from module and check link ,if not link return num=0
***********************************/
char *BT_ReadLData(char *Sta_link,u16 *Num){
  char *p;
  static u8 link=0;
  p=Serial_ReadData(Num,0);

  if(*Num == 7){
	 if(link !=1){
		if(strcmp("OK+CONN",p) == 0) {link = 1;*Num=0;}
	 }
  
	 if(link ==1){
		if(strcmp("OK+LOST",p) == 0) {link = 0;*Num=0;}
	 }
  }
  
  *Sta_link = link;
  return p;

}




//u8   BT_MasterInit(BT_MasterTypedef *BT_MasterConfig)
//{


//}


//u8   BT_SlaveInit(BT_SlaveTypedef *BT_SlaveConfig)
//{

//}

//  u8   BT_Set();
//  u8   BT_RState();


