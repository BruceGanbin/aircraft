#ifndef _CMD_H
#define _CMD_H

#ifdef __cplusplus
extern "C" {
#endif
#include "includes.h"

// command 
#define CMD_READSTA         1
#define CMD_READDATA        3
#define CMD_SETSTA          5
#define CMD_SETDATA         6

// reg num
#define REG_LED             1
#define REG_MOTOR           2
#define REG_SENSOR          3
#define REG_PID             4



	typedef struct _EVENT_CONTROL{

		u8 chpowsta;    // power switch

		u16 chattitX;   // atticude
		u16 chattitY;
		u16 chattitZ;

		u16 chdirectX;
		u16 chdirectY;
		u16 chdirectZ;

		//	 pidk_Typedef chpid;

	}eventTypedef;


	/**
	   to analysis receive cmd data
	***/
#pragma pack(1)
	typedef struct _CMD_TYPEDEF{
		u8  MachineNum;
		u8  Funcode;
		u16 Reg;
		union 
		{u16 len;
		 u8  data[10];	
		}len_data;
	}cmdTypedef;



	typedef struct _CMD_TRASMIT{
		u8  MachineNum;
		u8  Funcode;
		u16 len;
		u8 data[10];
	}traTypedef;
	
#pragma pack()

//#include "reg.h"
	void reg_init(regMachTypedef *reg_mach);

	char cmd_proc(regMachTypedef *reg_mach);

	char cmd_setsta(u16 reg,regstaTypedef *regsta,u8 sData,regperTypedef permission);    // set one switch reg
	char cmd_setdat(u16 reg,regdatTypedef *regdat,u8 *sData,u16 len,regperTypedef permission);    // set data reg

	char cmd_readsta(u16 reg,regstaTypedef *regsta,u8 *rData,u16 len);  // read one swi reg
	char cmd_readdat(u16 reg,regdatTypedef *regdat,u8 *rData,u16 len);  // read one data reg

	//  u16 * cmd_get(u16 reg,u8 len);    // read dates

#ifdef __cplusplus
}
#endif

#endif
