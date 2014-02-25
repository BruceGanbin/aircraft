#include "includes.h"




char cmd_proc(regMachTypedef *reg_mach)
{
	
	char Cmd_Sta=0;
	u16  Cmd_Num=0;
	char *Cmd_pData;
	char Cmd_ret;

	u8   tra_Num=0;
	char  *pTransmit;
	cmdTypedef *Cmd_Frame;
	traTypedef Cmd_Transmit;


	// change the cmd data come from
	Cmd_pData = BT_ReadLData(&Cmd_Sta,&Cmd_Num);
	if(Cmd_Sta==LINKLOST){	Cmd_pData = Usart1_Read(&Cmd_Num,0);  }

	// process command
	if(Cmd_Num == 6)
	{
		Cmd_Frame    = (cmdTypedef*)Cmd_pData;
		//		Cmd_Transmit = (traTypedef*)transmit;


		if(Cmd_Frame->MachineNum != reg_mach->machnum)  return 0;

		Cmd_Transmit.MachineNum = Cmd_Frame->MachineNum;
		Cmd_Transmit.Funcode    = Cmd_Frame->Funcode;

		//if right machine number 
		switch(Cmd_Frame->Funcode){

		case '1': //read status reg
			Cmd_ret = cmd_readsta(Cmd_Frame->Reg,&reg_mach->regsta,Cmd_Transmit.data,Cmd_Frame->len);
			tra_Num = 4+Cmd_Frame->len;
			break;
		 
		case 3: //read  data reg 
			Cmd_ret = cmd_readdat(Cmd_Frame->Reg,&reg_mach->regdata,Cmd_Transmit.data,Cmd_Frame->len);
			tra_Num = 4+Cmd_Frame->len;
			break;

		 
		case 5: //set switch state reg
			Cmd_ret =  cmd_setsta(Cmd_Frame->Reg,&reg_mach->regsta,(u8)Cmd_Frame->len,reg_mach->permission);
			tra_Num = 4;
			break;
		
		case 6: //set  data reg
			Cmd_ret =  cmd_setdat(Cmd_Frame->Reg,&reg_mach->regdata,Cmd_Frame->len,reg_mach->permission);
			tra_Num = 4;
			break;

		default:
			break;
		}

		//  transmit

		Cmd_Transmit.len = Cmd_Frame->len;
		pTransmit = (char*)&Cmd_Transmit;

		if(Cmd_Sta == LINKOK){ Usart2_Send(pTransmit,tra_Num); }
		else Usart1_Send(pTransmit,tra_Num);


	}
	else  Cmd_ret = 0;




	return Cmd_ret;
}




char cmd_setsta(u16 reg,regstaTypedef *regsta,u8 sData,regperTypedef permission)    // set one switch reg
{
	
	*((u8*)regsta+reg) = sData;
	return 1;
}


char cmd_setdat(u16 reg,regdatTypedef *regdat,u16 sData,regperTypedef permission)    // set data reg
{

	*((u16*)regdat+reg) = sData;
	return 1;
}



char cmd_readsta(u16 reg,regstaTypedef *regsta,u8 *rData,u16 len)  // read one swi reg
{
	u8 *preg;

	preg = (u8*)regsta+reg;
	memcpy(rData,preg,len);

	return 1;
}



char cmd_readdat(u16 reg,regdatTypedef *regdat,u8 *rData,u16 len)  // read one data reg
{
	u16 *preg;

	preg = (u16*)regdat+reg;
	memcpy(rData,preg,len);

	return 1;
}




void reg_init(regMachTypedef *reg_mach)
{
	u8 i;
	u8 *preg;
	u16 *dreg;
	const  u8 temp[STA_NUM]={
				LED_ST,
				MOTOR_ST,
				SENSOR_ST,
				ORIEN_ST,
	};


	reg_mach->machnum = MACHNUM;
 
	preg = (u8*)&reg_mach->regsta;
	//init switch reg
	for(i=0;i<STA_NUM;i++)
	{
		*preg = temp[i];
		preg++;
	}


	// init data reg
	dreg = (u16*)&reg_mach->regdata;
	for(i=0;i<DATA_NUM;i++)
	{
		*dreg = 0;
		dreg++;
	}
 

}



