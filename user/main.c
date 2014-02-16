#include "includes.h"
#include "string.h"


void bsp_init(void);



/***************************************************************************************************
**************************************************************************************************
                                          main
**************************************************************************************************
***************************************************************************************************/

int main(void)
{
  regMachTypedef Reg_fly;

  u16 Init_error=0;

  
  u8 ret;

  // config
  Init_error=RCC_Configuration();
  while(!Init_error){}
  NVIC_Configuration();
  GPIO_Configuration();
  Usart_Configuration();
  TIME_Configuration();
  IIC_Configuration();
  DMA_Configuration();
  //
	reg_init(&Reg_fly);
  bsp_init();
  

  while(1)
  {
	ret = cmd_proc(&Reg_fly);
//	if(ret) {  ret=0;}



  }
	
//  return 0;
} 

/***************************************************************************************************
**************************************************************************************************
                                          main
**************************************************************************************************
***************************************************************************************************/

void bsp_init(void){
  MPU6050Init_Typedef MPU6050_Config;
	BT_InitTypedef BT_configure;


  Usart1_Ropen();	
  Usart2_Ropen();	

	//Init  MPU6050
  MPU6050_Config.Read_Data = IIC_ReadPage;
  MPU6050_Config.Write_Data= IIC_WritePage;
 // Init_MPU6050(&MPU6050_Config);

  //Init bluetooth
  BT_configure.BT_ReadData=Usart2_Read;
  BT_configure.BT_SendData=Usart2_Send;
  BT_configure.BT_Name="bruce";
  BT_configure.BT_Auth=AUTH1;
  BT_configure.BT_PIN="222";
  BT_configure.BT_Disc=DISC_DIS_LNK;
  BT_configure.BT_LED=LED_FLASH;
  BT_Init(&BT_configure);


}
