#include "atcommand.h"
#include "bt_parse.h"
#include "rtthread.h"
#include <string.h>


///struct _bt_rx_type
//{
//    BT_type type;
//    rt_size_t size;
//    rt_uint8_t buf[AT_BUF_MAX];
//}bt_rx_type;

rt_sem_t  BT_sem;

BT_parameter BT_para;
BT_LINK_ST BT_link_st;

extern rx_buf_type rx_buf;

static void bt_send(char *pdata)
{
    at_send(pdata);
}

unsigned char BT_chklink(char *para,rt_size_t num)
{
    if(num == 12 || num == 13)
    {

    }

    return RT_NULL;
}

unsigned char BT_da_parse(void)
{
return RT_NULL;
}

char *BT_receive(void)
{
    rt_err_t result;
    result = rt_sem_take(BT_sem,RT_WAITING_FOREVER);
    if(result == RT_EOK)
    {
        return rx_buf.uart_rx_buffer;
    }
    else
    {
        rt_kprintf("bt read error~! \n");
        return RT_NULL;
    }
}

char* BT_cmd_send(char *cmd)
{
    char *result = RT_NULL;
    unsigned char num = 0;

    num = strlen(cmd);
    bt_send(cmd);
    result = BT_receive();
    if(num > 2)
    {
        result = result + num;
    }
    return result;

}


char *BT_at_parse(char *para)
{
    unsigned char num=0;
    char *presult;
    
    num = strlen(para);
    presult = BT_cmd_send(para);
    if(num > 2)
    {
        presult = presult + num;
    }
    return presult;
}

int bt_strcmp(char *cmd,char *reply)
{
    char *red;
    int ret;
    red = BT_cmd_send(cmd);
    ret = strcmp(reply,red);
    //    rt_kprintf("reply:%s ,read data:%s \n",reply,red);
    rt_sem_release(BT_sem);
    rt_thread_delay(5);
    return ret;
}

char *bt_strget(char *cmd,char *dat)
{
    char *red;
    char *ret;
    red = BT_cmd_send(cmd);
    ret = strcpy(dat,red);
    rt_sem_release(BT_sem);
    rt_thread_delay(5);
    return ret; 
}

char bt_setting(char *cmd,char *dat)
{
    char *red;
    int ret;
    char temp[15]={0};
    rt_sprintf(temp,"%s%s",cmd,dat);
    bt_send(temp);
    red = BT_receive();
    ret = strncmp("OK",red,2);
    if(ret == 0)
    {
        rt_kprintf("set %s \"%s\" OK\n",cmd,dat);
    }
    rt_sem_release(BT_sem);
    rt_thread_delay(5);
    return ret;
}

//void BT_para_read(void)
void bt_read(void)
{
    char *redat;
    char *pname;
    char *pauth;
    char *ppin;
    char *pdisc;
    char *pled;
    int ret;
    
    memset(BT_para.name,0,BT_NAME_LEN);
    memset(BT_para.pin,0,BT_PIN_LEN);

    ret = bt_strcmp("AT","OK");
    if(ret != 0)
    {
        rt_kprintf("Bluetooth ACK error!\n");
        return;
    }

    // check uart configuration
    ret = bt_strcmp("AT+BAUD?","9600");
    if(ret != 0 )
    {
        rt_kprintf("baudrate error!\n");
        bt_setting("AT+BAUD",BT_BAUD_9600);
    }

    ret = bt_strcmp("AT+CHK?",BT_CHK_NONE);
    if(ret != 0 )
    {
        rt_kprintf("check error!\n");
        bt_setting("AT+CHK",BT_CHK_NONE);
    }

    ret = bt_strcmp("AT+STOP?",BT_STOP_1BIT);
    if(ret != 0 )
    {
        rt_kprintf("stop error!\n");
        bt_setting("AT+STOP",BT_STOP_1BIT);
    }

    // check bluetooth configuration
    ret = bt_strcmp("AT+NAME?",BT_NAME);
    if(ret != 0 )
    {
        rt_kprintf("name error!\n");
        bt_setting("AT+NAME",BT_NAME);
    }

    ret = bt_strcmp("AT+ROLE?",BT_ROLE);
    if(ret != 0 )
    {
        rt_kprintf("ROLE error!\n");
        bt_setting("AT+ROLE",BT_ROLE);
    }

    ret = bt_strcmp("AT+PIN?",BT_PIN);
    if(ret != 0 )
    {
        rt_kprintf("PIN error!\n");
        bt_setting("AT+PIN",BT_PIN);
    }

    ret = bt_strcmp("AT+AUTH?",BT_AUTH);
    if(ret != 0 )
    {
        rt_kprintf("AUTH error!\n");
        bt_setting("AT+PIN",BT_AUTH);
    }

    ret = bt_strcmp("AT+NOTI?",BT_NOTI_Y);
    if(ret != 0 )
    {
        rt_kprintf("Notification error!\n");
        bt_setting("AT+NOTI",BT_NOTI_Y);
    }
    
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(bt_read,send data to bluetooth .)
#endif


void bt_test(char *cmd)
{
    char *dat;

    dat = BT_cmd_send(cmd);
    if(dat == RT_NULL)
    {
        rt_kprintf("read BT at command fiail \n");
        rt_sem_release(BT_sem);
        return;
    }
    rt_kprintf("%s\n",dat);
    rt_sem_release(BT_sem);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(bt_test,send data to bluetooth .)
#endif



void BT_thread_init(void *parameter)
{
    rt_err_t result=RT_EOK;
    int i;

    rt_thread_delay(100);
    bt_read();
    //BT_para_read();
    
    while(1)
    {
        result = rt_sem_take(BT_sem,RT_WAITING_FOREVER);
        while(result == RT_EOK )//&& rx_buf.size > 0)
        {
//            BT_chklink(rx_buf.uart_rx_buffer,rx_buf.size);
            rt_kprintf("data:%s \n",rx_buf.uart_rx_buffer);
            rt_sem_release(BT_sem);
            result = !RT_EOK;
            rt_thread_delay(5);
        }
    }
     
}

void BT_application_init(void)
{
    rt_thread_t BT_thread;    
    BT_sem = rt_sem_create("BT_SEM",0,RT_IPC_FLAG_FIFO);
    RT_ASSERT(BT_sem != RT_NULL);
    

    BT_thread = rt_thread_create("bt_app",
                                 BT_thread_init, RT_NULL,
                                   2048, 8, 20);
    if(BT_thread != RT_NULL)
    {
        rt_thread_startup(BT_thread);
    }
}
