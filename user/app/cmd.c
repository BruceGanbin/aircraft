#include "event_cmd.h"


eventTypedf cmd_proc(void){
	char Cmd_Sta=0;
	u16  Cmd_Num=0;
	char *Cmd_pData;
	cmdTypedef  Cmd_Frame;

	// change the cmd data come from
	Cmd_pData = BT_ReadLData(&Cmd_Sta,&Cmd_Num);
	if(state==LINKLOST){
		Cmd_pData = Usart1_Read(&Cmd_Num,0);
	}
	
	if(Cmd_Num == 6){
	 



	}

	if(Cmd_Sta == LINKOK){
		BT_SendData();
	}
	else Usart1_Send();
}


char * cmd_get(void);    // read dates
char cmd_set(char *);    // set one reg
char *cmd_read(char *);  // read one reg
