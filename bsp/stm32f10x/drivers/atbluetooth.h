#include "stm32f10x.h"
#include <rthw.h>





typedef struct _BTARTTRTYP
{
    char name[20];
    char auth[10];
    char pin[10];
    char disc[10];
    char led[10];
    //    serial_configure;
}btattr_typedef;



char bt_init(void);
char bt_setattri(btattr_typedef bt_attri);
btattr_typedef bt_getattri(void);
char bt_link(void);

unsigned char bt_getdata(char *pdata);
void bt_senddata(char *pdata,unsigned char num);
