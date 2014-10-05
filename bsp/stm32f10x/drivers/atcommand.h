#include "stm32f10x.h"
#include <rthw.h>

#define AT_BUF_MAX        64

typedef enum
{
    READING,
    FINISH,
    EMPTY
}rx_type;

typedef struct _BT_MSG
{
    rx_type type;
    char data[64];
}at_msg;

typedef struct _RX_BUF_TYPE
{
    char uart_rx_buffer[AT_BUF_MAX];
    rt_size_t size;
}rx_buf_type;

void at_send(char *pdata);
void at_application_init(void);

