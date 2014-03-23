#include "includes.h"
#include "cmd.h"



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
	if(Cmd_Num >= 6)
	{

		 Cmd_Frame    = (cmdTypedef*)Cmd_pData;
	


		if(Cmd_Frame->MachineNum != reg_mach->machnum)  return 0;

		Cmd_Transmit.MachineNum = Cmd_Frame->MachineNum;
		Cmd_Transmit.Funcode    = Cmd_Frame->Funcode;

		//if right machine number 
		switch(Cmd_Frame->Funcode){

		case CMD_READSTA: //read status reg
			Cmd_ret = cmd_readsta(Cmd_Frame->Reg,&reg_mach->regsta,Cmd_Transmit.data,Cmd_Frame->len_data.len);
			tra_Num = 4+Cmd_Frame->len_data.len;
			break;
		 
		case CMD_READDATA: //read  data reg 
			Cmd_ret = cmd_readdat(Cmd_Frame->Reg,&reg_mach->regdata,Cmd_Transmit.data,Cmd_Frame->len_data.len);
			tra_Num = 4+Cmd_Frame->len_data.len;
			break;

		 
		case CMD_SETSTA: //set switch state reg
			Cmd_ret =  cmd_setsta(Cmd_Frame->Reg,&reg_mach->regsta,(u8)Cmd_Frame->len_data.len,reg_mach->permission);
			tra_Num = 4;
			break;
		
		case CMD_SETDATA: //set  data reg
			Cmd_ret =  cmd_setdat(Cmd_Frame->Reg,&reg_mach->regdata,Cmd_Frame->len_data.data,Cmd_Num-4,reg_mach->permission);
			tra_Num = 4;
			break;

		default:
			break;
		}

		//  transmit

		Cmd_Transmit.len = Cmd_Frame->len_data.len;
		pTransmit = (char*)&Cmd_Transmit;

		if(Cmd_Sta == LINKOK){ Usart2_Send(pTransmit,tra_Num); }
		else Usart1_Send(pTransmit,tra_Num);


	}
	else  Cmd_ret = 0;




	return Cmd_ret;
}




char cmd_setsta(u16 reg,regstaTypedef *regsta,u8 sData,regperTypedef permission)    // set one switch reg
{
	switch(reg)
	{
	case REG_LED : 	 regsta->led_sta = sData;
		break;
	case REG_MOTOR:  regsta->motor_sta = sData;
		break;
	case REG_SENSOR: regsta->sensor_sta = sData;
		break;
	default:      return 0;
	
	}

	return 1;
}


char cmd_setdat(u16 reg,regdatTypedef *regdat,u8 *sData,u16 len,regperTypedef permission)    // set data reg
{
	u8 *pData;
	u8 i=0;
	switch(reg)
	{
/*	case REG_LED :    
		break;
	case REG_MOTOR:    	
		break;   
	case REG_SENSOR:     (pData =(u16 *)&regdat->sensor_da;
		break; */
	case REG_PID:          pData =(u8 *)&regdat->reg_PID ; //= *sData;
											  for(i=0;i<len;i++)
			                     {
															pData = sData;
															pData++;
														  sData++;
														}
			break;										
	default:   return 0;
		
	}

	return 1;
}



char cmd_readsta(u16 reg,regstaTypedef *regsta,u8 *rData,u16 len)  // read one swi reg
{
	u8 *preg;
	switch(reg)
	{
	case REG_LED :    	preg = (u8*)&regsta->led_sta;
		break;
	case REG_MOTOR:     preg = (u8*)&regsta->motor_sta;
		break;
	case REG_SENSOR:	  preg = (u8*)&regsta->sensor_sta;
		break;
	default:    return 0;
		
	}	

	memcpy(rData,preg,len);

	return 1;
}



char cmd_readdat(u16 reg,regdatTypedef *regdat,u8 *rData,u16 len)  // read one data reg
{
	u16 *preg;
	switch(reg)
	{
/*	case REG_LED :  	preg = (u16*)regdat->led;
		break;
	case REG_MOTOR:     preg = (u16*)regdat->motor;
		break;  */
	case REG_SENSOR:    preg = (u16*)&regdat->sensor_da;
		break;
	default:
		break;
	}

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



