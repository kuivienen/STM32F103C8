//#pragma once

#ifndef __pwm__

#define __pwm__


#include <stm32f10x_conf.h>
#include <stm32f10x.h>

#include "macros.h"
#include <time_service.h>

#include "math.h"

/**************************************************************************************************
                                          НАСТРОЙКИ ТАЙМЕРОВ
**************************************************************************************************/

#define TIMER_1
	#define TIMER1_REGS						TIM1
	
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
	
	#define TIMER1_IRQ						TIM1_UP_IRQn
	
	#define TIMER1_FREQ_SIG				50000
	
	#define TIMER1_FREQ_PWM				3000
	
	
	
#define TIMER_4
	#define TIMER4_REGS						TIM4
	
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
	
	#define TIMER4_IRQ						TIM4_IRQn
	
	#define TIMER4_FREQ_SIG				50000
	
	#define TIMER4_FREQ_PWM				3000



#define PWM_1
	#define PWM1_REGS							TIM2
	#define PWM1_IRQ							TIM2_IRQn


#define TIMER_3
	#define TIMER3_REGS							TIM3
	#define TIMER3_IRQ							TIM3_IRQn
	
/**************************************************************************************************
                                          ТИПЫ ДАННЫХ
**************************************************************************************************/

typedef struct
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
	
	uint8_t					pwmChannel1;
	uint8_t					pwmChannel2;
	
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

#ifdef PWM_1
	extern timer_struct Pwm1;
#endif

#ifdef TIMER_3
	extern timer_struct Timer3;
#endif

/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void InitTimer( timer_struct * timer );
void InitPwm ( timer_struct * pwm );
void InitTimer3 ( timer_struct * timer );

#ifdef TIMER_1
	void TIM1_UP_IRQHandler( void );
#endif

#ifdef TIMER_4
	void TIM4_IRQHandler( void );
#endif

#ifdef PWM_1
	void TIM2_IRQHandler( void );
#endif

#ifdef Timer_3
	void TIM3_IRQHandler( void );
#endif

#endif /*	__pwm__	*/
