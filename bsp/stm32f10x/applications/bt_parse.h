#include "stm32f10x.h"
#include <rthw.h>

#define BT_NAME_LEN        12
#define BT_PIN_LEN         12
#define BT_DISC_LEN        10
#define BT_LED_LEN         10

#define BT_BAUD_1200       "1" 
#define BT_BAUD_2400       "2" 
#define BT_BAUD_4800       "3" 
#define BT_BAUD_9600       "4" 
#define BT_BAUD_19200      "5" 
#define BT_BAUD_38400      "6" 
#define BT_BAUD_57600      "7" 
#define BT_BAUD_115200     "8" 
#define BT_BAUD_230400     "9" 
#define BT_BAUD_460800     "A" 
#define BT_BAUD_921600     "B" 
#define BT_BAUD_1382400    "C" 
                            
#define BT_CHK_NONE        "0"
#define BT_CHK_ODD         "1"
#define BT_CHK_EVEN        "2"

#define BT_STOP_1BIT       "1"
#define BT_STOP_2BIT       "2"

#define BT_ROLE_MASTER     "M"
#define BT_ROLE_SLAVE      "S"

#define BT_AUTH_Y          "1"
#define BT_AUTH_N          "0"

#define BT_DISC_FIN_LINK   "0"
#define BT_DISC_ONLY_FIN   "1"
#define BT_DISC_ONLY_LINK  "2"

#define BT_NOTI_N          "0"
#define BT_NOTI_Y          "1"

#define BT_LINK_UP          1
#define BT_LINK_DOWN        0
/* BT configuration*/
#define BT_NAME            "minifly"

#define BT_ROLE            BT_ROLE_SLAVE

#define BT_PIN             "222"

#define BT_AUTH            BT_AUTH_Y

typedef struct _BT_UART
{
    char baud;
    char check;
    char stop;
}BT_uartyped;

typedef struct _ATARTTRTYP
{
    char name[BT_NAME_LEN];
    char role;
    char auth;
    char disc;
    char pin[BT_PIN_LEN];
    BT_uartyped BT_uart;
}BT_parameter;

typedef enum
{
    BTdata,
    BTcommand,
}sendtypedef;

typedef enum
{
    LINK_UP,
    LINK_DOWN
}BT_LINK_ST;

//int BT_thread_init(void);
void BT_application_init(void);

unsigned char BT_chklink(void);

void BT_send(char *pdata);
