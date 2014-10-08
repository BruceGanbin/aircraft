#include "atcommand.h"
#include "rtthread.h"
#include <string.h>
//#include "includes.h"

struct rx_msg
{
    rt_size_t size;
    rx_type   type;
};

static rt_mq_t rx_mq,tx_mq;

rx_buf_type rx_buf={0};
//char uart_rx_buffer[AT_BUF_MAX];
//static char uart_tx_buffer[64];

rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg msg;
    msg.size = size;
    msg.type = READING;
    rt_mq_send(rx_mq, &msg, sizeof(struct rx_msg));
    return RT_EOK;
}

void at_send(char *pdata)
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


void parse_at_buf(char *buf,unsigned int num)
{

return ;
}

extern rt_sem_t  BT_sem;

static void at_thread(void* parameter)
{
    struct rx_msg msg;
    at_msg send_msg;
    int count = 0;
    rt_device_t device;
    rt_err_t result = RT_EOK;
    rt_err_t result_sem = RT_EOK;
    rt_uint8_t *kpt;
    static rx_type rx_st = EMPTY;
    
    
    device = rt_device_find("uart2");
    if (device!= RT_NULL)
    {
        rt_device_set_rx_indicate(device, uart_input);
        rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
    }

    rt_sem_release(BT_sem);
    //    rt_thread_delay(5);

    while(1)
    {
        result_sem = rt_sem_take(BT_sem,RT_WAITING_FOREVER);
        if(result_sem == RT_EOK)
        {
            memset(rx_buf.uart_rx_buffer,0,AT_BUF_MAX);
//          rt_kprintf("clean uart_buff \n");
            rx_buf.size = 0;
        }

        while(result_sem == RT_EOK)
        {
            result = rt_mq_recv(rx_mq, &msg, sizeof(struct rx_msg),20);
            if (result == -RT_ETIMEOUT)
            {
                rt_uint8_t length_rx = 0;
                length_rx = rt_device_read(device, 0, rx_buf.uart_rx_buffer,AT_BUF_MAX);
                if(length_rx > 0)
                {
//                  rt_kprintf("BT_Receive Data: %s\n",rx_buf.uart_rx_buffer);
                    rx_buf.size = length_rx;
                    rt_sem_release(BT_sem);
                    result_sem = !RT_EOK;
                }
            }
            if(result == RT_EOK)
            {
                rt_uint8_t rx_length;
            }
        }
    }
}

void at_application_init(void)
{

    rx_mq = rt_mq_create("btrx_mq", sizeof(struct rx_msg), 5, RT_IPC_FLAG_FIFO);
    RT_ASSERT(rx_mq != RT_NULL);

    rt_thread_t thread = rt_thread_create("at_cmd",
                                          at_thread, RT_NULL,
                                          1024, 25, 10);

    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
}

