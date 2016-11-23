
#include "stdint.h"
#include "stdbool.h"
#include "stm32f10x.h"


#include <time_service.h>

#include <self_control.h>
#include <uart.h>

#include <pwm.h>
#include <buttons.h>

#include <link_slave.h>


void Blink(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init( GPIOB , &GPIO_InitStructure );
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	Delay(1000);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	Delay(1000);
}

void OutputMCO() {
	GPIO_InitTypeDef GPIO_InitStructure;


	/* Output clock on MCO pin ---------------------------------------------*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// pick one of the clocks to spew
	RCC_MCOConfig(RCC_MCO_SYSCLK); // Put on MCO pin the: System clock selected
}


/*************************************************************************************************/
int main( void )
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0 );

	InitTimeService();
	
	InitLinkSlave(&Uart2);
	
	InitTimer( &Timer1 );
	InitTimer( &Timer4 );
	InitTimer( &Timer3 );
	
	//InitButtons( &Buttons );
	
	InitMotor( &Motor1 );
	InitMotor( &Motor2 );
	InitMotor( &Motor3 );
	InitMotor( &Motor4 );
	//InitPwm(&Pwm1);
	//	SpiConfig(&Spi1Struct);

	//InitMsgProcessor();
	//InteruptConfig(&ext1);

	//OutputMCO();
	//	Delay(5000);
	InitIwdg();
	//Blink();
	//	InitOpticalSensor(&Spi1Struct);
	//	InitPinOUT();
	while(1)
	{
	//	ScanButtons( &Buttons );
	//	ScanChangeButtons( &Buttons );
		
		//	SwitcDir и AccelerationState должны стоять именно в такой последовательности
		SwitchDir ( &Timer3 );
		AccelerationState ( &Timer3 );
		
		IWDG_ReloadCounter();

		ProcessLinkSlave();
		SelfControlFsm();		
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
