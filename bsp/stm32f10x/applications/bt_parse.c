#include "atcommand.h"
#include "bt_parse.h"
#include "rtthread.h"
#include <string.h>
#include "mpu6050.h"

rt_sem_t  BT_sem;

BT_parameter BT_para;
BT_LINK_ST BT_link_st;

extern rx_buf_type rx_buf;
static unsigned char  link_st = BT_LINK_DOWN;

#define BT_PARSE_LEN                6
#define BT_PADATA_LEN               4
/******************************************************************
|  8bit     |    8bit    |   8bit   |   8bit   |   8bit    |    8bit    |
   start       function      data1      data2      data3        data4 

Start              0xA5

function           code       data
Busy               0xF0       0xFF 
(input)
Ctrl Gas           0x10     
Ctrl Pitch         0x11
Ctrl Rool          0x12
Ctrl Yaw           0x13

(read)
Ret  ACC_X         0x20
Ret  ACC_Y         0x21
Ret  ACC_Z         0x22
Ret  GYR_X         0x23
Ret  GYR_Y         0x24
Ret  GYR_Z         0x25
Ret  TEMP          0x26
Ret  Imu_Rool      0x27
Ret  Imu_pitch     0x28
Ret  Imu_yaw       0x29

(setting)
Set  X_P           0x40
Set  Y_P           0x41
Set  Z_P           0x42
Set  X_I           0x43
Set  Y_I           0x44
Set  Z_I           0x45


******************************************************************/

void BT_send(char *pdata)
{
    at_send(pdata);
}
/*
check BT link status , >0 link up ,=0 link down
 */
//unsigned char BT_chklink(char *dat,rt_size_t num)
unsigned char bt_chklink(void)
{
    if(link_st == BT_LINK_UP)
        rt_kprintf("CHK BT_link up\n");
    else
        rt_kprintf("CHK BT_link down\n");
    return link_st;
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(bt_chklink,check BT link stauts.)
#endif

unsigned char BT_parse(char *dat,rt_size_t num)
{
    int ret;
    char *head;
    unsigned char temp[BT_PADATA_LEN]={0};
    char rec[BT_PARSE_LEN]={0};
    char *srec = &rec[2];
    MPU6050value_typedef agdata;

    head = dat;
    rec[0] = 0x5A;
    /* check link status */
    if(num == 7)
    {
        ret = strcmp(head,"OK+CONN");
        if(ret == 0)
        {
            link_st = BT_LINK_UP;
            rt_kprintf("link up\n");
            return 1;
        }
        ret = strcmp(head,"OK+LOST");
        if(ret == 0)
        {
            link_st = BT_LINK_DOWN;
            rt_kprintf("link down\n");
            return 1;
        }    
    }

    get_MPU6050(&agdata);
    
    /* receive is data  */
    if(link_st == BT_LINK_UP)
    {
        if(num != BT_PARSE_LEN)
            return 0;

        if(*dat != 0xA5)
            return 0;

        dat++;
        strncpy(temp,dat+1,BT_PADATA_LEN);
        rec[1] = *dat;
        
        switch(*dat)
        {
            /*  input  */
        case 0x10:
            break;
        case 0x11:
            break;
        case 0x12:
            break;
        case 0x13:
            break;

            /* read data */
        case 0x20:
            memcpy(srec,&agdata.AccelX,2);
            break;
        case 0x21:
            memcpy(srec,&agdata.AccelY,2);
            break;
        case 0x22:
            memcpy(srec,&agdata.AccelZ,2);
            break;
        case 0x23:
            memcpy(srec,&agdata.GYROX,2);
            break;
        case 0x24:
            memcpy(srec,&agdata.GYROY,2);
            break;
        case 0x25:
            memcpy(srec,&agdata.GYROZ,2);
            break;
        case 0x26:
            memcpy(srec,&agdata.TEMPER,2);
            break;
        case 0x27:
            break;
        case 0x28:
            break;
        case 0x29:
            break;

            /* setting */
        case 0x40:
            break;
        case 0x41:
            break;
        case 0x42:
            break;
        case 0x43:
            break;
        case 0x44:
            break;
        case 0x45:
            break;

        default:
            break;
        }
    }
    else  /* receive is command */
    {

    }

    BT_send(rec);
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
    BT_send(cmd);
    result = BT_receive();
    if(num > 2)
    {
        result = result + num;
    }
    return result;

}

//char *BT_at_parse(char *para)
//{
//    unsigned char num=0;
//    char *presult;
//
//    num = strlen(para);
//    presult = BT_cmd_send(para);
//    if(num > 2)
//    {
//        presult = presult + num;
//    }
//    return presult;
//}

int bt_rcmdcmp(char *cmd,char *reply)
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
    BT_send(temp);
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

void BT_para_read(void)
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

    ret = bt_rcmdcmp("AT","OK");
    if(ret != 0)
    {
        rt_kprintf("Bluetooth ACK error!\n");
        return;
    }

    /* check uart configuration */
    ret = bt_rcmdcmp("AT+BAUD?","9600");
    if(ret != 0 )
    {
        rt_kprintf("baudrate error!\n");
        bt_setting("AT+BAUD",BT_BAUD_9600);
    }

    ret = bt_rcmdcmp("AT+CHK?",BT_CHK_NONE);
    if(ret != 0 )
    {
        rt_kprintf("check error!\n");
        bt_setting("AT+CHK",BT_CHK_NONE);
    }

    ret = bt_rcmdcmp("AT+STOP?",BT_STOP_1BIT);
    if(ret != 0 )
    {
        rt_kprintf("stop error!\n");
        bt_setting("AT+STOP",BT_STOP_1BIT);
    }

    /* check bluetooth configuration */
    ret = bt_rcmdcmp("AT+NAME?",BT_NAME);
    if(ret != 0 )
    {
        rt_kprintf("name error!\n");
        bt_setting("AT+NAME",BT_NAME);
    }

    ret = bt_rcmdcmp("AT+ROLE?",BT_ROLE);
    if(ret != 0 )
    {
        rt_kprintf("ROLE error!\n");
        bt_setting("AT+ROLE",BT_ROLE);
    }

    ret = bt_rcmdcmp("AT+PIN?",BT_PIN);
    if(ret != 0 )
    {
        rt_kprintf("PIN error!\n");
        bt_setting("AT+PIN",BT_PIN);
    }

    ret = bt_rcmdcmp("AT+AUTH?",BT_AUTH);
    if(ret != 0 )
    {
        rt_kprintf("AUTH error!\n");
        bt_setting("AT+PIN",BT_AUTH);
    }

    ret = bt_rcmdcmp("AT+NOTI?",BT_NOTI_Y);
    if(ret != 0 )
    {
        rt_kprintf("Notification error!\n");
        bt_setting("AT+NOTI",BT_NOTI_Y);
    }
    rt_kprintf("BT init config OK!\n");    
}

void bt_read(void)
{
    BT_para_read();
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
FINSH_FUNCTION_EXPORT(bt_test,send AT cmd to bluetooth receive data.)
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
//            rt_kprintf("data:%s \n",rx_buf.uart_rx_buffer);
            BT_parse(rx_buf.uart_rx_buffer,rx_buf.size);
            rt_sem_release(BT_sem);
            result = !RT_EOK;
            rt_thread_delay(1);
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
                                     1048, 10, 5);

    if(BT_thread != RT_NULL)
    {
        rt_thread_startup(BT_thread);
    }
}
