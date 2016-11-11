#include "pwm.h"

/**************************************************************************************************
                                    ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void initTimRcc( timer_struct * timer );
void initTimParams( timer_struct * timer );
void initTimGpio( timer_struct * timer );
void initTimDma( timer_struct * timer);
void initTimNvic( timer_struct * timer );
void tim1IrqHandler( timer_struct * timer );
void tim3IrqHandler( timer_struct * timer );
void tim4IrqHandler( timer_struct * timer );
void timIrqPwmHandler( timer_struct * pwm );
void InitTimPwmParams( timer_struct * pwm );
void initTimPwmNvic( timer_struct * pwm );
void InitTim3Params( timer_struct * timer );


/**************************************************************************************************
                                       ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef TIMER_1
	timer_struct Timer1 =
	{
		.regs 		= TIMER1_REGS,
		
		.ch1Pin 	= TIMER1_PIN_CH1,
		.ch1Port 	= TIMER1_PORT_CH1,

		.ch2Pin 	= TIMER1_PIN_CH2,
		.ch2Port 	= TIMER1_PORT_CH2,
		
		.ch3Pin 	= TIMER1_PIN_CH3,
		.ch3Port 	= TIMER1_PORT_CH3,
		
		.ch4Pin 	= TIMER1_PIN_CH4,
		.ch4Port 	= TIMER1_PORT_CH4,
		
		.freqSig	=	TIMER1_FREQ_SIG,
		
		.irq			=	TIMER1_IRQ,
		
		.pwmChannel1	=	8,
		.pwmChannel2	=	8, 
		
		.pwmMax				=	(uint32_t)TIMER4_FREQ_SIG/TIMER4_FREQ_PWM,
	};
#endif
	
#ifdef TIMER_4
	timer_struct Timer4 =
	{
		.regs 				= TIMER4_REGS,
		
		.ch1Pin 			= TIMER4_PIN_CH1,
		.ch1Port 			= TIMER4_PORT_CH1,

		.ch2Pin 			= TIMER4_PIN_CH2,
		.ch2Port 			= TIMER4_PORT_CH2,
		
		.ch3Pin 			= TIMER4_PIN_CH3,
		.ch3Port 			= TIMER4_PORT_CH3,
		
		.ch4Pin 			= TIMER4_PIN_CH4,
		.ch4Port 			= TIMER4_PORT_CH4,
		
		.freqSig			=	TIMER4_FREQ_SIG,
		
		.irq					=	TIMER4_IRQ,
		
		.pwmChannel1	=	8,
		.pwmChannel2	=	8,
		
		.pwmMax				=	(uint32_t)TIMER4_FREQ_SIG/TIMER4_FREQ_PWM,
	};
#endif
	
#ifdef PWM_1
	timer_struct Pwm1 =
	{
		.regs 		= PWM1_REGS,
		.irq			=	PWM1_IRQ,
	};
#endif
	
#ifdef TIMER_3
	timer_struct Timer3 =
	{
		.regs 		= TIMER3_REGS,
		.irq			=	TIMER3_IRQ,
	};
#endif
	
/**************************************************************************************************
                                        ГЛОБАЛЬНЫЕ ФУНКЦИИ
**************************************************************************************************/


/**************************************************************************************************
Описание:  Инициализация таймеров отвечающих за генерацию прямоуголных имульсов (модуляция)
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void InitTimer( timer_struct * timer )
{
	initTimRcc( timer );
	initTimGpio( timer );
	initTimNvic( timer );
	initTimParams( timer );
}

/**************************************************************************************************
Описание:  Инициализация таймера отвечающего за формирование ШИМ (огибающая)
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void InitPwm ( timer_struct * pwm )
{
	initTimRcc( pwm );
	initTimPwmNvic( pwm );
	InitTimPwmParams( pwm );
}

/**************************************************************************************************
Описание:  Инициализация таймера отвечающего за формирование разгона и торможения двигателей
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void InitTimer3 ( timer_struct * timer )
{
	initTimRcc( timer );
	initTimPwmNvic( timer );
	InitTim3Params( timer );
}

/**************************************************************************************************
                                       ЛОКАЛЬНЫЕ ПЕРЕМЕННЫЕ
**************************************************************************************************/

//	счетчик количества обновлений модулирующих таймеров
volatile int8_t pwmModTim1 = 0;
volatile int8_t pwmModTim4 = 0;

//	количество каналов управления
enum _Channel {First, Second, Third, Fours} channel;


/**************************************************************************************************
                                       ЛОКАЛЬНЫЕ ФУНКЦИИ
**************************************************************************************************/



/**************************************************************************************************
Описание:  Инициализация тактирования таймера
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void initTimRcc( timer_struct * timer )
{
	//	включаем тактирование таймера
	if ( timer->regs  == TIM1 ) {
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
	}
	
	else if ( timer->regs  == TIM2 ) {
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
	}
	
	else if ( timer->regs  == TIM3 ) {
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
	}
	
	else if ( timer->regs  == TIM4 ) {
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
	}
	
	else {
		FAIL_ASSERT();
	}
}

/**************************************************************************************************
Описание:  Инициализация выходных CH пинов таймера
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void initTimGpio( timer_struct * timer )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = timer->ch1Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( timer->ch1Port , &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = timer->ch2Pin;
	GPIO_Init( timer->ch2Port , &GPIO_InitStructure );	

	GPIO_InitStructure.GPIO_Pin = timer->ch3Pin;
	GPIO_Init( timer->ch3Port , &GPIO_InitStructure );	

	GPIO_InitStructure.GPIO_Pin = timer->ch4Pin;
	GPIO_Init( timer->ch4Port , &GPIO_InitStructure );
}


/**************************************************************************************************
Описание:  Инициализация параметров таймера для модуляции
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void initTimParams( timer_struct * timer )
{
  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);
  base_timer.TIM_Prescaler = SystemCoreClock / (1000000 + 1);		//	тик равен ста наносекундам
  base_timer.TIM_Period = 10;																		//	период равен 20мкс или частота 50кГц
	base_timer.TIM_ClockDivision = 0;
  base_timer.TIM_CounterMode = TIM_CounterMode_CenterAligned1;

  TIM_TimeBaseInit(timer->regs, &base_timer);

  TIM_OCInitTypeDef OC_InitStruct;
  TIM_OCStructInit(&OC_InitStruct);

	//	указываем для комплиментарных выходов разные значения в регистр сравнения для deadtime
  OC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  OC_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	OC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	OC_InitStruct.TIM_Pulse = 6;

	TIM_OC1Init(timer->regs, &OC_InitStruct);
	TIM_OC3Init(timer->regs, &OC_InitStruct);
	
	OC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	OC_InitStruct.TIM_Pulse = 5;
	
	TIM_OC2Init(timer->regs, &OC_InitStruct);
	TIM_OC4Init(timer->regs, &OC_InitStruct);
	
	if (timer->regs == TIM1)
	{
		TIM_CtrlPWMOutputs(timer->regs, ENABLE);
	}
	
	TIM_Cmd(timer->regs, ENABLE);	 
}

/**************************************************************************************************
Описание:  Инициализация параметров таймера для огибающей
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void InitTimPwmParams( timer_struct * pwm )
{
	TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);
  base_timer.TIM_Prescaler = SystemCoreClock / (312500 + 1);		//	тик равен ста наносекундам
  base_timer.TIM_Period = 100;																	//	период равен 20мкс или частота 50кГц
	base_timer.TIM_ClockDivision = 0;
  base_timer.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_OCInitTypeDef OC_InitStruct;
  TIM_OCStructInit(&OC_InitStruct);
	
	OC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//  OC_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
  OC_InitStruct.TIM_Pulse = 20;
//  OC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

  TIM_OC1Init(pwm->regs, &OC_InitStruct);

  TIM_TimeBaseInit(pwm->regs, &base_timer);
	
	TIM_Cmd(pwm->regs, ENABLE);
}

/**************************************************************************************************
Описание:  Инициализация параметров таймера для формирования ускорения
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void InitTim3Params( timer_struct * timer )
{
	TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);
  base_timer.TIM_Prescaler = SystemCoreClock / (1000 + 1);		//	тик равен ста наносекундам
  base_timer.TIM_Period = 100;																//	период равен 20мкс или частота 50кГц
	base_timer.TIM_ClockDivision = 0;
  base_timer.TIM_CounterMode = TIM_CounterMode_Up;
	
  TIM_TimeBaseInit(timer->regs, &base_timer);
	
	TIM_Cmd(timer->regs, ENABLE);
}


/**************************************************************************************************
Описание:  Инициализация прерывания таймера по переполнению
Аргументы: Указатель на структуру Timer
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void initTimNvic( timer_struct * timer )
{
	NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = timer->irq;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(timer->regs, TIM_IT_Update, ENABLE);

	
	NVIC_EnableIRQ(timer->irq);
}

/**************************************************************************************************
Описание:  Инициализация прерывания таймера по совпадению 
Аргументы: Указатель на структуру Timer
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void initTimPwmNvic( timer_struct * pwm )
{
	NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = pwm->irq;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(pwm->regs, TIM_IT_CC1, ENABLE);

	
	NVIC_EnableIRQ(pwm->irq);
}

/**************************************************************************************************
Описание:  Включает выходы сравнения
Аргументы: Указатель на структуру Timer и номер канала
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void enableChannel(timer_struct * timer, enum _Channel channel)
{
	switch (channel){
		case First:		
			timer->regs->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
			break;
		case Second:		
			timer->regs->CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E;
			break;
		case Third:		
			timer->regs->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
			break;
		case Fours:		
			timer->regs->CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E;
			break;
		default:
			FAIL_ASSERT();
	}
}

/**************************************************************************************************
Описание:  Отключает выходы сравнения
Аргументы: Указатель на структуру Timer и номер канала
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void disableChannel(timer_struct * timer, enum _Channel channel)
{
	switch (channel){
		case First:		
			timer->regs->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E);
			break;
		case Second:		
			timer->regs->CCER &= ~(TIM_CCER_CC3E | TIM_CCER_CC4E);
			break;
		case Third:		
			timer->regs->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E);
			break;
		case Fours:		
			timer->regs->CCER &= ~(TIM_CCER_CC3E | TIM_CCER_CC4E);
			break;
		default:
			FAIL_ASSERT();
	}
}

/**************************************************************************************************
Описание:  Прерываение от TIM TIMER1
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
#ifdef TIMER_1
void TIM1_UP_IRQHandler( void )
{
	tim1IrqHandler( &Timer1 );
}
#endif


void tim1IrqHandler( timer_struct * timer ) {
	TIM_ClearITPendingBit( timer->regs, TIM_IT_Update );
	++pwmModTim1;
	
	if (pwmModTim1 == timer->pwmMax*2) {
		enableChannel( timer, First );
		enableChannel( timer, Second );
		pwmModTim1 = 0;
	}
	
	if (pwmModTim1 == timer->pwmChannel1*2) {
		disableChannel( timer, First );
	}
	
	if (pwmModTim1 == timer->pwmChannel2*2) {
		disableChannel( timer, Second );
	}
}

/**************************************************************************************************
Описание:  Прерываение от TIM TIMER4
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
#ifdef TIMER_4
void TIM4_IRQHandler( void )
{
	tim4IrqHandler( &Timer4 );
}
#endif

void tim4IrqHandler( timer_struct * timer ) {
	if (TIM_GetITStatus(timer->regs, TIM_IT_Update)) {
		TIM_ClearITPendingBit( timer->regs, TIM_IT_Update );
		++pwmModTim4;
		
		if (pwmModTim4 == timer->pwmMax*2) {
			enableChannel( timer, Third );
			enableChannel( timer, Fours );
			pwmModTim4 = 0;
		}
		
		if (pwmModTim4 == timer->pwmChannel1*2) {
			disableChannel( timer, Third );
		}
		
		if (pwmModTim4 == timer->pwmChannel2*2) {
			disableChannel( timer, Fours );
		}
	}
}


/**************************************************************************************************
Описание:  Прерываение от TIM PWM1
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
#ifdef PWM_1
void TIM2_IRQHandler( void )
{
	timIrqPwmHandler( &Pwm1 );
}
#endif

void timIrqPwmHandler( timer_struct * pwm ){
	if (TIM_GetITStatus(pwm->regs, TIM_IT_CC1)){
		TIM_SetCounter(Timer1.regs, 0);
		TIM_ClearITPendingBit(pwm->regs, TIM_IT_CC1);
	}
}

/**************************************************************************************************
Описание:  Прерываение от TIM3
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
#ifdef TIMER_3
void TIM3_IRQHandler( void )
{
	timIrqPwmHandler( &Timer3 );
}
#endif

void tim3IrqHandler( timer_struct * timer ){
	if (TIM_GetITStatus(timer->regs, TIM_IT_Update)){
		TIM_ClearITPendingBit(timer->regs, TIM_IT_Update);
	}
}
