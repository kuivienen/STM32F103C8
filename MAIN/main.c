
#include "stdint.h"
#include "stdbool.h"
#include "stm32f10x.h"


#include <time_service.h>

#include <self_control.h>

#include <pwm.h>

#include <link_slave.h>

//****************** Includes USB *************************************************
#include <hw_config.h>
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
//*********************************************************************************

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
//extern __IO uint8_t Receive_Buffer[64];
//extern __IO  uint32_t Receive_length ;
//extern __IO  uint32_t length ;
//uint8_t Send_Buffer[64];
__IO uint32_t packet_sent=1;
__IO uint32_t packet_receive=1;



/*************************************************************************************************/
int main( void )
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0 );

	InitTimeService();
	
	InitLinkSlave(&Usb1);
	
	InitTimer( &Timer2 );
	InitTimer( &Timer4 );
	InitTimer( &Timer3 );
	
	//InitButtons( &Buttons );
	
	InitMotor( &Motor1 );
	InitMotor( &Motor2 );
	InitMotor( &Motor3 );
	InitMotor( &Motor4 );

	InitIwdg();							//	запуск WatchDog
	
	//*************инициализация USB****************
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
	//**********************************************
	
	while(1)
	{
		//	SwitcDir и AccelerationState должны стоять именно в такой последовательности
		SwitchDir ( &Timer3 );
		AccelerationState ( &Timer3 );
		
		IWDG_ReloadCounter();
		
		if (bDeviceState == CONFIGURED)
    {
      CDC_Receive_DATA();
    }
		ProcessLinkSlave();
		SelfControlFsm();	
		if (bDeviceState == CONFIGURED)
    {
			if (Send_length  != 0)
      {
        if (packet_sent == 1) CDC_Send_DATA ((unsigned char*)Send_Buffer, Send_length);
        Send_length = 0;
			}
		}
	}
}


/*************************************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
