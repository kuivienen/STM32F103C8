#ifndef __UART__

#define __UART__


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <macros.h>

#include <time_service.h>
#include <stm32f10x_conf.h>

/**************************************************************************************************
                                          НАСТРОЙКИ
**************************************************************************************************/
#define RX_BUFFER_SIZE		255   // Емкость приемного кольцевого буфера (3...255 байт)

// #define UART_1
// 	#define UART1_REGS USART1                        // Структура в Standard Peripherals Library 
// 	#define UART1_IRQN USART1_IRQn                   // Номер прерывания
// 	#define UART1_RX_PIN GPIO_Pin_11                 // Номер пина RX
// 	#define UART1_RX_PORT GPIOB                      // Порт, которому принадлежит пин RX
// 	#define UART1_TX_PIN GPIO_Pin_10                 // Номер пина TX
// 	#define UART1_TX_PORT GPIOB                      // Порт, которому принадлежит пин TX
// 	#define UART1_BAUDRATE 9600                      // Скорость передачи данных
// 	#define UART1_PREEMPTION_PRIORITY 0              // Приоритет прерывания (preemption)
// 	#define UART1_SUB_PRIORITY 3                     // Приоритет прерывания (sub)
// 	#define UART1_TX_CALLBACK TxCallbackDefault      // Указатель на функцию, вызывается после окончания отправки буфера
// 	#define UART1_RX_OVR_HANDLER RxOvrHandlerDefault // Указатель на функцию, вызывается при переполении приемного буфера


 #define UART_2
 	#define UART2_REGS 									USART2
 	#define UART2_IRQN 									USART2_IRQn
 	#define UART2_RX_PIN 								GPIO_Pin_3
 	#define UART2_RX_PORT 							GPIOA
 	#define UART2_TX_PIN 								GPIO_Pin_2
 	#define UART2_TX_PORT 							GPIOA
	#define UART2_RTS_PIN 							GPIO_Pin_1
 	#define UART2_RTS_PORT 							GPIOA
 	#define UART2_BAUDRATE 							9600  
 	#define UART2_PREEMPTION_PRIORITY 	0
 	#define UART2_SUB_PRIORITY 					3
 	#define UART2_TX_CALLBACK 					TxCallbackDefault
 	#define UART2_RX_OVR_HANDLER 				RxOvrHandlerDefault
	#define UART2_RX_CALLBACK 					RxCallbackDefault
	
//#define UART_3
//	#define UART3_REGS USART3
//	#define UART3_IRQN USART3_IRQn
//	#define UART3_RX_PIN GPIO_Pin_11
//	#define UART3_RX_PORT GPIOB
//	#define UART3_TX_PIN GPIO_Pin_10
//	#define UART3_TX_PORT GPIOB
//	#define UART3_BAUDRATE 57600
//	#define UART3_PREEMPTION_PRIORITY 0
//	#define UART3_SUB_PRIORITY 3
//	#define UART3_TX_CALLBACK AnsTransmited
 //	#define UART3_RX_OVR_HANDLER RxOvrHandlerDefault
	
// #define UART_4
// 	#define UART4_REGS UART4
// 	#define UART4_IRQN UART4_IRQn
// 	#define UART4_RX_PIN GPIO_Pin_11
// 	#define UART4_RX_PORT GPIOB
// 	#define UART4_TX_PIN GPIO_Pin_10
// 	#define UART4_TX_PORT GPIOB
// 	#define UART4_BAUDRATE 9600
// 	#define UART4_PREEMPTION_PRIORITY 0
// 	#define UART4_SUB_PRIORITY 3
// 	#define UART4_TX_CALLBACK TxCallbackDefault
// 	#define UART4_RX_OVR_HANDLER RxOvrHandlerDefault
	
// #define UART_5
// 	#define UART5_REGS UART5
// 	#define UART5_IRQN UART5_IRQn
// 	#define UART5_RX_PIN GPIO_Pin_11
// 	#define UART5_RX_PORT GPIOB
// 	#define UART5_TX_PIN GPIO_Pin_10
// 	#define UART5_TX_PORT GPIOB
// 	#define UART5_BAUDRATE 9600
// 	#define UART5_PREEMPTION_PRIORITY 0
// 	#define UART5_SUB_PRIORITY 3
// 	#define UART5_TX_CALLBACK TxCallbackDefault
// 	#define UART5_RX_OVR_HANDLER RxOvrHandlerDefault



/**************************************************************************************************
                                          ТИПЫ ДАННЫХ
**************************************************************************************************/

typedef void (*tx_callback)( void );
typedef void (*rx_ovr_handler)( void );

typedef void (*rx_callback)(void);

typedef struct
{
	USART_TypeDef * regs;

	IRQn_Type irqn;
	uint8_t preemptionPriority;
	uint8_t subPriority;
	
	uint16_t rxPin;
	uint16_t txPin;
	uint16_t rtsPin;
	GPIO_TypeDef * rxPort;
	GPIO_TypeDef * txPort;
	GPIO_TypeDef * rtsPort;
	uint32_t baud;
	
	uint8_t rxBuf[ RX_BUFFER_SIZE ];
	uint8_t rxCounter;
	uint8_t rxTail;
	uint8_t rxHead;
	uint8_t * txBufPtr;
	uint16_t txCounter;
	
	tx_callback txCallback;
	rx_ovr_handler rxOvrHandler;
	rx_callback rxCallback;
} uart_struct;



/**************************************************************************************************
                                         EXTERN-ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef UART_1
	extern uart_struct Uart1;
#endif

#ifdef UART_2
	extern uart_struct Uart2;
#endif

#ifdef UART_3
	extern uart_struct Uart3;
#endif

#ifdef UART_4
	extern uart_struct Uart4;
#endif

#ifdef UART_5
	extern uart_struct Uart5;
#endif



/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void InitUart( uart_struct * uart );
uint8_t GetByteFromUart( uart_struct * uart );
bool IsNewDataInUart( uart_struct * uart );
void SendDataToUart( uart_struct * uart, uint8_t *buffer, uint16_t size );
void EnableUartReceive( uart_struct * uart );
void DisableUartReceive( uart_struct * uart );
void TxCallbackDefault( void );
void RxOvrHandlerDefault( void );
void RxCallbackDefault(void);

#ifdef UART_1
	void USART1_IRQHandler( void );
#endif

#ifdef UART_2
	void USART2_IRQHandler( void );
#endif

#ifdef UART_3
	void USART3_IRQHandler( void );
#endif

#ifdef UART_4
	void UART4_IRQHandler( void );
#endif

#ifdef UART_5
	void UART5_IRQHandler( void );
#endif

#endif	/*	__UART__	*/
