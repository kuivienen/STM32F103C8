/**************************************************************************************************
 UART.C

 �����������: ������� �����

 ������ � UART��� ��� STM32F10x
 
 �������������:
  - ����� ������ ��������� � uart.h
  - ������� ������� �������������, ��������: InitUart( Uart1 );
  - ��������� ����� �� UART �����, ��������, ���:
		while ( IsNewDataInUart( &Uart1 ) )
		{
			data[i] = GetByteFromUart( &Uart1 );
			i++;
		}
  - ���������� ���:
		uint8_t msg[] = {'H', 'e', 'l', 'l', 'o'};
		SendDataToUart( &Uart3, msg, 5 );
  - ��� ���� EnableUartReceive, DisableUartReceive, ������ �� ��������� �������� � ����������� 
    ������������ ��������� ������
	
**************************************************************************************************/


#include "uart.h"



/**************************************************************************************************
                                    ��������� ��������� �������
**************************************************************************************************/
static void initUartRcc( uart_struct * uart );
static void initUartGpio( uart_struct * uart );
static void initUartNvic( uart_struct * uart );
static void initUartParams( uart_struct * uart );
static void uartIrqHandler( uart_struct * uart );
void UartDelay(uint32_t count);



/**************************************************************************************************
                                       ���������� ����������
**************************************************************************************************/
#ifdef UART_1
	uart_struct Uart1 =
	{
		.regs = UART1_REGS,
		.irqn = UART1_IRQN,
		.rxPin = UART1_RX_PIN,
		.rxPort = UART1_RX_PORT,
		.txPin = UART1_TX_PIN,
		.txPort = UART1_TX_PORT,
		.baud = UART1_BAUDRATE,
		.preemptionPriority = UART1_PREEMPTION_PRIORITY,
		.subPriority = UART1_SUB_PRIORITY,
		.txCallback = UART1_TX_CALLBACK,
		.rxOvrHandler = UART1_RX_OVR_HANDLER
	};
#endif

#ifdef UART_2
	uart_struct Uart2 = 
	{
		.regs = UART2_REGS,
		.irqn = UART2_IRQN,
		.rxPin = UART2_RX_PIN,
		.rxPort = UART2_RX_PORT,
		.txPin = UART2_TX_PIN,
		.txPort = UART2_TX_PORT,
		.rtsPin = UART2_RTS_PIN,
		.rtsPort = UART2_RTS_PORT,
		.baud = UART2_BAUDRATE,
		.preemptionPriority = UART2_PREEMPTION_PRIORITY,
		.subPriority = UART2_SUB_PRIORITY,
		.txCallback = UART2_TX_CALLBACK,
		.rxOvrHandler = UART2_RX_OVR_HANDLER,
		.rxCallback = UART2_RX_CALLBACK
	};
#endif

#ifdef UART_3
	uart_struct Uart3 =
	{
		.regs = UART3_REGS,
		.irqn = UART3_IRQN,
		.rxPin = UART3_RX_PIN,
		.rxPort = UART3_RX_PORT,	
		.txPin = UART3_TX_PIN,
		.txPort = UART3_TX_PORT,	
		.baud = UART3_BAUDRATE,
		.preemptionPriority = UART3_PREEMPTION_PRIORITY,
		.subPriority = UART3_SUB_PRIORITY,
		.txCallback = UART3_TX_CALLBACK,
		.rxOvrHandler = UART3_RX_OVR_HANDLER	
	};
#endif

#ifdef UART_4
	uart_struct Uart4 = 
	{
		.regs = UART4_REGS,
		.irqn = UART4_IRQN,
		.rxPin = UART4_RX_PIN,
		.rxPort = UART4_RX_PORT,
		.txPin = UART4_TX_PIN,
		.txPort = UART4_TX_PORT,
		.baud = UART4_BAUDRATE,
		.preemptionPriority = UART4_PREEMPTION_PRIORITY,
		.subPriority = UART4_SUB_PRIORITY,
		.txCallback = UART4_TX_CALLBACK,
		.rxOvrHandler = UART4_RX_OVR_HANDLER
	};
#endif

#ifdef UART_5
	uart_struct Uart5 = 
	{
		.regs = UART5_REGS,
		.irqn = UART5_IRQN,
		.rxPin = UART5_RX_PIN,
		.rxPort = UART5_RX_PORT,
		.txPin = UART5_TX_PIN,
		.txPort = UART5_TX_PORT,
		.baud = UART5_BAUDRATE,
		.preemptionPriority = UART5_PREEMPTION_PRIORITY,
		.subPriority = UART5_SUB_PRIORITY,
		.txCallback = UART5_TX_CALLBACK,
		.rxOvrHandler = UART5_RX_OVR_HANDLER
	};
#endif



/**************************************************************************************************
                                        ���������� �������
**************************************************************************************************/


/**************************************************************************************************
��������:  ������������� ������ UART
���������: ��������� �� ��������� UART
�������:   ���
���������: 
**************************************************************************************************/
void InitUart( uart_struct * uart )
{
	initUartRcc( uart );
	initUartGpio( uart );
	initUartParams( uart );
	initUartNvic( uart );

  USART_Cmd( uart->regs, ENABLE );
	USART_ITConfig( uart->regs, USART_IT_RXNE, ENABLE );
	USART_ITConfig( uart->regs, USART_IT_TC, ENABLE );
	
	uart->rxCounter = 0;
	uart->rxTail = 0;
	uart->rxHead = 0;	
}



/**************************************************************************************************
��������:  ����������� �� USART1
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
#ifdef UART_1
void USART1_IRQHandler( void )
{
	uartIrqHandler( &Uart1 );
}
#endif



/**************************************************************************************************
��������:  ����������� �� USART2
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
#ifdef UART_2
void USART2_IRQHandler( void )
{
	uartIrqHandler( &Uart2 );
}
#endif



/**************************************************************************************************
��������:  ����������� �� USART3
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
#ifdef UART_3
void USART3_IRQHandler( void )
{
	uartIrqHandler( &Uart3 );
}
#endif



/**************************************************************************************************
��������:  ����������� �� UART4
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
#ifdef UART_4
void UART4_IRQHandler( void )
{
	uartIrqHandler( &Uart4 );
}
#endif



/**************************************************************************************************
��������:  ����������� �� UART5
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
#ifdef UART_5
void USART5_IRQHandler( void )
{
	uartIrqHandler( &Uart5 );
}
#endif



/**************************************************************************************************
��������:  ���������� ��������� ���� �� ������ ��������� UART
���������: ��������� �� ��������� UART
�������:   ���� ������
���������: �������� ������� �������� ���� 
**************************************************************************************************/
uint8_t GetByteFromUart( uart_struct * uart )
{
	uint8_t dataByte;

	dataByte = uart->rxBuf[ uart->rxTail ]; 
	uart->rxTail++;
	uart->rxTail %= RX_BUFFER_SIZE;
		
	ENTER_CRITICAL_SECTION();
	
		uart->rxCounter--;
		
	LEAVE_CRITICAL_SECTION();	
		
	return dataByte; 	
}



/**************************************************************************************************
��������:  ���������� ��������� ������ ���������
���������: ��������� �� ��������� UART
�������:   true, ���� ���� �������������� �����
���������: 
**************************************************************************************************/
bool IsNewDataInUart( uart_struct * uart )
{
	if ( uart->rxCounter )
	{
		return true;
	}
	else
	{
		return false;
	}
}



/**************************************************************************************************
��������:  ������������� �������� �������� ��������� �� UART
���������: ��������� �� ��������� UART, ��������� �� ������, �����
�������:   ���
���������: 
**************************************************************************************************/
void SendDataToUart( uart_struct * uart, uint8_t *buffer, uint16_t size )
{
	uart->txBufPtr = buffer;
	uart->txCounter = size - 1;
	GPIO_SetBits(uart->rtsPort, uart->rtsPin);
	USART_SendData( uart->regs, *buffer );
}

void UartDelay(uint32_t count)
{
	uint32_t i=0;
	while(i<count) i++;
}

/**************************************************************************************************
��������:  ���������� ������ �� UART
���������: ��������� �� ��������� UART, ��������� �� ������, �����
�������:   ���
���������: 
**************************************************************************************************/
void EnableUartReceive( uart_struct * uart )
{
	uart->regs->CR1 |= USART_CR1_RE;
	uart->rxCounter = 0;
	uart->rxTail = 0;
	uart->rxHead = 0;	
}



/**************************************************************************************************
��������:  ���������� ������ �� UART
���������: ��������� �� ��������� UART, ��������� �� ������, �����
�������:   ���
���������: 
**************************************************************************************************/
void DisableUartReceive( uart_struct * uart )
{
	uart->regs->CR1 &= ~USART_CR1_RE;
}



/**************************************************************************************************
��������:  ��������, ���������� �� ��������� ����� ��������� �������� �� UART
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
void TxCallbackDefault( void )
{

}



/**************************************************************************************************
��������:  ��������, ���������� �� ��������� ��� ������������ ��������� ������
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
void RxOvrHandlerDefault( void )
{

}

void RxCallbackDefault(void)
{
	
}

/**************************************************************************************************
                                       ��������� �������
**************************************************************************************************/



/**************************************************************************************************
��������:  ������������� ������������ UART
���������: ��������� �� ��������� USART_TypeDef
�������:   ���
���������: 
**************************************************************************************************/
static void initUartRcc( uart_struct * uart )
{
	if ( uart->regs  == USART1)
	{
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
	}
	else if ( uart->regs  == USART2)
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );	  
	}
	else if ( uart->regs  == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	}    
	else if ( uart->regs  == UART4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	}    
	else if ( uart->regs  == UART5)
	{ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	}
	else
	{
#ifdef  USE_FULL_ASSERT
		assert_failed( (uint8_t *)__FILE__, __LINE__ );
#endif
	}
}



/**************************************************************************************************
��������:  ������������� ������ UART
���������: ��������� �� ��������� UART
�������:   ���
���������: 
**************************************************************************************************/
static void initUartGpio( uart_struct * uart )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//������������� RX	
	GPIO_InitStructure.GPIO_Pin = uart->rxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init( uart->rxPort , &GPIO_InitStructure );

	//������������� TX
	GPIO_InitStructure.GPIO_Pin = uart->txPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init( uart->txPort , &GPIO_InitStructure );
	
		//������������� RTS
	GPIO_InitStructure.GPIO_Pin = uart->rtsPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init( uart->rtsPort , &GPIO_InitStructure );
	GPIO_ResetBits(uart->rtsPort, uart->rtsPin);
}



/**************************************************************************************************
��������:  ������������� ���������� UART
���������: ��������� �� ��������� UART
�������:   ���
���������: 
**************************************************************************************************/
static void initUartNvic( uart_struct * uart )
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = uart->irqn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = uart->preemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = uart->subPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	NVIC_EnableIRQ( uart->irqn );
}



/**************************************************************************************************
��������:  ������������� ���������� UART
���������: ��������� �� ��������� UART
�������:   ���
���������: 
**************************************************************************************************/
static void initUartParams( uart_struct * uart )
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = uart->baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init( uart->regs, &USART_InitStructure );
}



/**************************************************************************************************
��������:  ���������� ����������: ���������� ��������� ����� � ������, �������� ������ �����
���������: ��������� �� ��������� UART
�������:   ���
���������: 
**************************************************************************************************/
static void uartIrqHandler( uart_struct * uart )
{
	uint8_t dataByte;
	
	//-----------------------------------------------------------
	if( uart->regs->SR & USART_FLAG_RXNE )
	{
		dataByte = uart->regs->DR ;
		
 		if ( uart->rxCounter < RX_BUFFER_SIZE  )
 		{ 
			uart->rxBuf[ uart->rxHead ] = dataByte;
			uart->rxHead++;
			uart->rxHead %= RX_BUFFER_SIZE;
 			uart->rxCounter++;
 		}
 		else
 		{
			uart->rxCounter = 0;
			uart->rxTail = 0;
			uart->rxHead = 0;
			uart->rxOvrHandler();
 		}

		USART_ClearFlag( uart->regs, USART_FLAG_RXNE );
	}
	
	//-----------------------------------------------------------
	
	if( uart->regs->SR & USART_FLAG_TC )
	{
 		if ( uart->txCounter )
 		{
 			uart->txBufPtr++;
 			uart->txCounter--;
 			USART_SendData( uart->regs, *(uart->txBufPtr) );
 		}
 		else
 		{
 			UartDelay(1000);
			GPIO_ResetBits(uart->rtsPort, uart->rtsPin);
			uart->txCallback();
 		}
		USART_ClearFlag( uart->regs, USART_FLAG_TC );		
	}	
}
