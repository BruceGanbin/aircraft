#include "atbluetooth.h"
#include "rtthread.h"
#include <string.h>
//#include "includes.h"

static lk_st;

struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};

typedef enum
{
    BTdata,
    BTcommand,
}sendtypedef;

static rt_mq_t rx_mq;

static char uart_rx_buffer[64];
static char uart_tx_buffer[64];

rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{

    struct rx_msg msg;
    msg.dev = dev;
    msg.size = size;

    rt_mq_send(rx_mq, &msg, sizeof(struct rx_msg));
    return RT_EOK;
}

static bt_send(char *pdata)
{
    rt_device_t write_device = RT_NULL;
    unsigned int num = 0;
    write_device = rt_device_find("uart2");
    if(write_device == RT_NULL)
    {
        rt_kprintf("open uart error! \n");
        return;
    }
    rt_device_open(write_device, RT_DEVICE_OFLAG_RDWR);
    num = strlen(pdata);
    rt_device_write(write_device, 0, pdata,num);
}

void bt_thread(void* parameter)
{
    struct rx_msg msg;
    int count = 0;
    rt_device_t device;
    rt_err_t result = RT_EOK;
    
    device = rt_device_find("uart2");
    if (device!= RT_NULL)
    {
        rt_device_set_rx_indicate(device, uart_input);
        rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
    }

    while(1)
    {
        result = rt_mq_recv(rx_mq, &msg, sizeof(struct rx_msg), 50);
        if (result == -RT_ETIMEOUT)
        {
            rt_kprintf("timeout count:%d\n", ++count);
        }
        if(result == RT_EOK)
        {
            rt_uint32_t rx_length;
            rx_length = (sizeof(uart_rx_buffer) - 1) > msg.size ? msg.size : sizeof(uart_rx_buffer) - 1;
            rx_length = rt_device_read(msg.dev, 0, &uart_rx_buffer[0],rx_length);
            uart_rx_buffer[rx_length] = RT_NULL;
        }
    }
}

char bt_application_init(void)
{

    rt_thread_t thread = rt_thread_create("Bluetooth",
                                          bt_thread, RT_NULL,
                                          1024, 25, 7);

    if(thread != RT_NULL)
        rt_thread_startup(thread);
}

char bt_setattri(btattr_typedef bt_attri);
btattr_typedef bt_getattri(void);
char bt_link(void);

unsigned char bt_getdata(char *pdata);
void bt_senddata(char *pdata,unsigned char num);

