#ifndef __EVENT_CMD_H
#define  _EVENT_CMD_H

#ifdef __cplusplus
extern "C" {
#endif
#include "includes.h"


  typedef struct _EVENT_CONTROL{

	 u8 chpowsta;    // power switch

	 u16 chattitX;   // atticude
	 u16 chattitY;
	 u16 chattitZ;

	 u16 chdirectX;
	 u16 chdirectY;
	 u16 chdirectZ;

	 pidk_Typedef chpid;

  }eventTypedef;


  /**
	  to analysis receive cmd data
  ***/
#pragma pack(1)
  typedef struct _CMD_TYPEDEF{
	 u8  MachineNum;
	 u8  Funcode;
	 u16 Reg;
	 u16 Data;
  }cmdTypedef;
#pragma pack()




  eventTypedf cmd_proc(void);
  char * cmd_get(void);    // read dates
  char cmd_set(char *);    // set one reg
  char *cmd_read(char *);  // read one reg

#ifdef __cplusplus
}
#endif

#endif
