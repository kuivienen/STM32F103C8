
#ifndef __pwm__

#define __pwm__


#include <stm32f10x_conf.h>
#include <stm32f10x.h>

#include "macros.h"
#include <time_service.h>
#include "motor.h"
#include "stdlib.h"

/**************************************************************************************************
                                          НАСТРОЙКИ ТАЙМЕРОВ
**************************************************************************************************/

/**********настройка таймера для первого и второго канала управления*/
#define TIMER_1
	#define TIMER1_REGS						TIM1
	
//*********пины комплиментарных выходов CH***********

#define TIMER1_CH1	
	#define TIMER1_PORT_CH1				GPIOA
	#define TIMER1_PIN_CH1				GPIO_Pin_8

#define TIMER1_CH2	
	#define TIMER1_PORT_CH2				GPIOA
	#define TIMER1_PIN_CH2				GPIO_Pin_9
	
#define TIMER1_CH3
	#define TIMER1_PORT_CH3				GPIOA
	#define TIMER1_PIN_CH3				GPIO_Pin_10
	
#define TIMER1_CH4
	#define TIMER1_PORT_CH4				GPIOA
	#define TIMER1_PIN_CH4				GPIO_Pin_11
	
//--------------------------------------------------

//	флаг прерывания 
	#define TIMER1_IRQ						TIM1_UP_IRQn
//	частота модулирующего сигнала
	#define TIMER1_FREQ_SIG				50000
//	частота огибающей	
	#define TIMER1_FREQ_PWM				3000
	
	
/******настройка таймера для третьего и четвертого канала управления****/	
#define TIMER_4
	#define TIMER4_REGS						TIM4

//*********пины комплиментарных выходов CH***********

#define TIMER4_CH1	
	#define TIMER4_PORT_CH1				GPIOB
	#define TIMER4_PIN_CH1				GPIO_Pin_6

#define TIMER4_CH2	
	#define TIMER4_PORT_CH2				GPIOB
	#define TIMER4_PIN_CH2				GPIO_Pin_7
	
#define TIMER4_CH3
	#define TIMER4_PORT_CH3				GPIOB
	#define TIMER4_PIN_CH3				GPIO_Pin_8
	
#define TIMER4_CH4
	#define TIMER4_PORT_CH4				GPIOB
	#define TIMER4_PIN_CH4				GPIO_Pin_9
	
//--------------------------------------------------

//	флаг прерывания 
	#define TIMER4_IRQ						TIM4_IRQn
//	частота модулирующего сигнала
	#define TIMER4_FREQ_SIG				50000
//	частота огибающей	
	#define TIMER4_FREQ_PWM				3000

/**********настройка таймера для формирования ускорения и торможения*******/	

#define TIMER_3
	#define TIMER3_REGS						TIM3
//	флаг прерывания 
	#define TIMER3_IRQ						TIM3_IRQn
	#define TIMER3_CH1						&Timer1
	#define TIMER3_CH2						&Timer1
	#define TIMER3_CH3						&Timer4
	#define TIMER3_CH4						&Timer4
	
/**************************************************************************************************
                                          ТИПЫ ДАННЫХ
**************************************************************************************************/
//*******структура данных для таймеров************
typedef struct str
{
	TIM_TypeDef * regs;
	
	uint16_t 				ch1Pin;
	GPIO_TypeDef 		* ch1Port;
	
	uint16_t 				ch2Pin;
	GPIO_TypeDef 		* ch2Port;
	
	uint16_t 				ch3Pin;
	GPIO_TypeDef 		* ch3Port;
	
	uint16_t 				ch4Pin;
	GPIO_TypeDef 		* ch4Port;
	uint16_t 				freqSig;
	
	IRQn_Type 			irq;
	
	uint8_t 				pwmMax;
	
	volatile int8_t pwmModTim;
	
	int8_t					pwmTargetChannel1;
	int8_t					pwmTargetChannel2;
	int8_t					pwmTargetChannel3;
	int8_t					pwmTargetChannel4;
	
	int8_t					pwmCurrentChannel1;
	int8_t					pwmCurrentChannel2;
	int8_t					pwmCurrentChannel3;
	int8_t					pwmCurrentChannel4;
	
	struct str  		*channel1;
	struct str   		*channel2;
	struct str  		*channel3;
	struct str   		*channel4;
	
	bool 						accelerationFlag;
	bool						switchDirFlagCH1;	
	bool						switchDirFlagCH2;
	bool						switchDirFlagCH3;
	bool						switchDirFlagCH4;
	
} timer_struct;


/**************************************************************************************************
                                         EXTERN-ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef TIMER_1
	extern timer_struct Timer1;
#endif

#ifdef TIMER_4
	extern timer_struct Timer4;
#endif

#ifdef TIMER_3
	extern timer_struct Timer3;
#endif

//	количество каналов управления
extern enum _Channel {FIRST, SECOND, THIRD, FOURS} channel;

/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void InitTimer( timer_struct * timer );
void AccelerationState ( timer_struct * timer );
void SwitchDir ( timer_struct * timer );
void SetPwmTarget ( timer_struct * timer, enum _Channel channel, int8_t data );
void switchDirFlag ( timer_struct * timer, enum _Channel channel );

#ifdef TIMER_1
	void TIM1_UP_IRQHandler( void );
#endif

#ifdef TIMER_4
	void TIM4_IRQHandler( void );
#endif


#ifdef Timer_3
	void TIM3_IRQHandler( void );
#endif

#endif /*	__pwm__	*/
