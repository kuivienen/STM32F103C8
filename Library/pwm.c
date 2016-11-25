#include "pwm.h"

/**************************************************************************************************
                                    ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void initTimRcc( timer_struct * timer );
void initTimParams( timer_struct * timer );
void initTimGpio( timer_struct * timer );
void initTimNvic( timer_struct * timer );

void tim2IrqHandler( timer_struct * timer );
void tim3IrqHandler( timer_struct * timer );
void tim4IrqHandler( timer_struct * timer );


/**************************************************************************************************
                                       ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef TIMER_2
	timer_struct Timer2 =
	{
		.regs 							= TIMER2_REGS,
		
		.ch1Pin 						= TIMER2_PIN_CH1,
		.ch1Port 						= TIMER2_PORT_CH1,

		.ch2Pin 						= TIMER2_PIN_CH2,
		.ch2Port 						= TIMER2_PORT_CH2,
		
		.ch3Pin 						= TIMER2_PIN_CH3,
		.ch3Port 						= TIMER2_PORT_CH3,
		
		.ch4Pin 						= TIMER2_PIN_CH4,
		.ch4Port 						= TIMER2_PORT_CH4,
		
		.freqSig						=	TIMER2_FREQ_SIG,
		
		.irq								=	TIMER2_IRQ,
		
		.pwmTargetChannel1	=	8,
		.pwmTargetChannel2	=	8,

		.pwmCurrentChannel1 = -10,
		.pwmCurrentChannel2 = -10,
		
		.pwmMax							=	(uint32_t)TIMER2_FREQ_SIG/TIMER2_FREQ_PWM,
		.pwmModTim					=	0,
	};
#endif
	
#ifdef TIMER_4
	timer_struct Timer4 	=
	{
		.regs 							= TIMER4_REGS,
		
		.ch1Pin 						= TIMER4_PIN_CH1,
		.ch1Port 						= TIMER4_PORT_CH1,

		.ch2Pin 						= TIMER4_PIN_CH2,
		.ch2Port 						= TIMER4_PORT_CH2,
		
		.ch3Pin 						= TIMER4_PIN_CH3,
		.ch3Port 						= TIMER4_PORT_CH3,
		
		.ch4Pin 						= TIMER4_PIN_CH4,
		.ch4Port 						= TIMER4_PORT_CH4,
		
		.freqSig						=	TIMER4_FREQ_SIG,
		
		.irq								=	TIMER4_IRQ,
		
		.pwmTargetChannel3	=	8,
		.pwmTargetChannel4	=	8,
		
		.pwmCurrentChannel3 = -10,
		.pwmCurrentChannel4 = -10,
		
		.pwmMax							=	(uint32_t)TIMER4_FREQ_SIG/TIMER4_FREQ_PWM,
		.pwmModTim					=	0,
	};
#endif
	
#ifdef TIMER_3
	timer_struct Timer3 =
	{
		.regs 							= TIMER3_REGS,
		.irq								=	TIMER3_IRQ,
		.channel1						= TIMER3_CH1, 
		.channel2						= TIMER3_CH2,
		.channel3						= TIMER3_CH3,
		.channel4						= TIMER3_CH4,
		.accelerationFlag		=	0,
		.switchDirFlagCH1			=	0,
		.switchDirFlagCH2			=	0,
		.switchDirFlagCH3			=	0,
		.switchDirFlagCH4			=	0,
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
Описание:  Осуществляет увеличение заполнения ШИМ
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void AccelerationState ( timer_struct * timer )
{
	if ( timer->accelerationFlag ) {
		if ( timer->channel1->pwmCurrentChannel1 != timer->channel1->pwmTargetChannel1 ) {
			if ( timer->channel1->pwmCurrentChannel1 > timer->channel1->pwmTargetChannel1 ) --timer->channel1->pwmCurrentChannel1;
			else if ( timer->channel1->pwmCurrentChannel1 < timer->channel1->pwmTargetChannel1 ) ++timer->channel1->pwmCurrentChannel1;
			else timer->switchDirFlagCH1 = 1;
		}
		
		if ( timer->channel2->pwmCurrentChannel2 != timer->channel2->pwmTargetChannel2 ) {
			if ( timer->channel2->pwmCurrentChannel2 > timer->channel2->pwmTargetChannel2 ) --timer->channel2->pwmCurrentChannel2;
			else if ( timer->channel2->pwmCurrentChannel2 < timer->channel2->pwmTargetChannel2 ) ++timer->channel2->pwmCurrentChannel2;
			else timer->switchDirFlagCH2 = 1;
		}
		
		if ( timer->channel3->pwmCurrentChannel3 != timer->channel3->pwmTargetChannel3 ) {
			if ( timer->channel3->pwmCurrentChannel3 > timer->channel3->pwmTargetChannel3 ) --timer->channel3->pwmCurrentChannel3;
			else if ( timer->channel3->pwmCurrentChannel3 < timer->channel3->pwmTargetChannel3 ) ++timer->channel3->pwmCurrentChannel3;
			else timer->switchDirFlagCH3 = 1;
		}
		
		if ( timer->channel4->pwmCurrentChannel4 != timer->channel4->pwmTargetChannel4 ) {
			if ( timer->channel4->pwmCurrentChannel4 > timer->channel4->pwmTargetChannel4 ) --timer->channel4->pwmCurrentChannel4;
			else if ( timer->channel4->pwmCurrentChannel4 < timer->channel4->pwmTargetChannel4 ) ++timer->channel4->pwmCurrentChannel4;
			else timer->switchDirFlagCH4 = 1;
		}	
		timer->accelerationFlag = 0;
	}
}

/**************************************************************************************************
Описание:  Осуществляет переключение каналов управления
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void SwitchDir ( timer_struct * timer )
{
	if ( timer->switchDirFlagCH1 ) {
		if ( timer->channel1->pwmTargetChannel1 > 0 ) SetPinMode ( &Motor1, FORWARD );
		else if ( timer->channel1->pwmTargetChannel1 < 0 ) SetPinMode ( &Motor1, BACKWARD );
		else SetPinMode ( &Motor1, OFF );
		timer->switchDirFlagCH1 = 0;
	}
	
	if ( timer->switchDirFlagCH2) {
		if ( timer->channel2->pwmTargetChannel2 > 0 ) SetPinMode ( &Motor2, FORWARD );
		else if ( timer->channel2->pwmTargetChannel2 < 0 ) SetPinMode ( &Motor2, BACKWARD );
		else SetPinMode ( &Motor2, OFF );
		timer->switchDirFlagCH2 = 0;
	}
	
	if ( timer->switchDirFlagCH3) {
		if ( timer->channel3->pwmTargetChannel3 > 0 ) SetPinMode ( &Motor3, FORWARD );
		else if ( timer->channel3->pwmTargetChannel3 < 0 ) SetPinMode ( &Motor3, BACKWARD );
		else SetPinMode ( &Motor3, OFF );
		timer->switchDirFlagCH3 = 0;
	}
	
	if ( timer->switchDirFlagCH4) {
		if ( timer->channel4->pwmTargetChannel4 > 0 ) SetPinMode ( &Motor4, FORWARD );
		else if ( timer->channel4->pwmTargetChannel4 < 0 ) SetPinMode ( &Motor4, BACKWARD );
		else SetPinMode ( &Motor4, OFF );
		timer->switchDirFlagCH4 = 0;
	}
}

/**************************************************************************************************
Описание:  Осуществляет установку заполнения ШИМ
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void SetPwmTarget ( timer_struct * timer, enum _Channel channel, int8_t data )
{
	switch (channel){
		case FIRST:		
			timer->switchDirFlagCH1 = 1;
			timer->channel1->pwmTargetChannel1 = data;
			break;
		case SECOND:
			timer->switchDirFlagCH2 = 1;			
			timer->channel2->pwmTargetChannel2 = data;
			break;
		case THIRD:	
			timer->switchDirFlagCH3 = 1;
			timer->channel3->pwmTargetChannel3 = data;
			break;
		case FOURS:	
			timer->switchDirFlagCH4 = 1;
			timer->channel4->pwmTargetChannel4 = data;
			break;
		default:
			FAIL_ASSERT();
	}
}

/**************************************************************************************************
Описание:  Устанавливает флаг переключения состояния каналов управления
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void switchDirFlag ( timer_struct * timer, enum _Channel channel )
{
	switch (channel){
			case FIRST:		
				timer->switchDirFlagCH1 = 1;
				break;
			case SECOND:		
				timer->switchDirFlagCH2 = 1;
				break;
			case THIRD:		
				timer->switchDirFlagCH3 = 1;
				break;
			case FOURS:		
				timer->switchDirFlagCH4 = 1;
				break;
			default:
				FAIL_ASSERT();
	}
}

/**************************************************************************************************
                                       ЛОКАЛЬНЫЕ ПЕРЕМЕННЫЕ
**************************************************************************************************/


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
	if ((timer->regs == TIM1) || (timer->regs == TIM4)) {
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
}


/**************************************************************************************************
Описание:  Инициализация параметров таймера для модуляции
Аргументы: Указатель на структуру timer_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void initTimParams( timer_struct * timer )
{
	if ((timer->regs == TIM1) || (timer->regs == TIM4)) {
		TIM_TimeBaseInitTypeDef base_timer;
		TIM_TimeBaseStructInit(&base_timer);
		base_timer.TIM_Prescaler = SystemCoreClock / 1000000 - 1;		//	тик равен ста наносекундам
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
	else if (timer->regs == TIM3){
		TIM_TimeBaseInitTypeDef base_timer;
		TIM_TimeBaseStructInit(&base_timer);
		base_timer.TIM_Prescaler = SystemCoreClock / 2000 - 1;			//	тик равен ста наносекундам
		base_timer.TIM_Period = 500;																//	период равен 20мкс или частота 50кГц
		base_timer.TIM_ClockDivision = 0;
		base_timer.TIM_CounterMode = TIM_CounterMode_Up;
		
		TIM_TimeBaseInit(timer->regs, &base_timer);
		
		TIM_Cmd(timer->regs, ENABLE);
	}
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
Описание:  Включает выходы сравнения
Аргументы: Указатель на структуру Timer и номер канала
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void enableChannel( timer_struct * timer, enum _Channel channel )
{
	switch (channel){
		case FIRST:		
			timer->regs->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
			break;
		case SECOND:		
			timer->regs->CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E;
			break;
		case THIRD:		
			timer->regs->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
			break;
		case FOURS:		
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

void disableChannel( timer_struct * timer, enum _Channel channel )
{
	switch (channel){
		case FIRST:		
			timer->regs->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E);
			break;
		case SECOND:		
			timer->regs->CCER &= ~(TIM_CCER_CC3E | TIM_CCER_CC4E);
			break;
		case THIRD:		
			timer->regs->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E);
			break;
		case FOURS:		
			timer->regs->CCER &= ~(TIM_CCER_CC3E | TIM_CCER_CC4E);
			break;
		default:
			FAIL_ASSERT();
	}
}

/**************************************************************************************************
Описание:  Прерываение от TIM TIMER2
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
#ifdef TIMER_2
void TIM2_IRQHandler( void )
{
	tim2IrqHandler( &Timer2 );
}
#endif


void tim2IrqHandler( timer_struct * timer ) {
	if (TIM_GetITStatus(timer->regs, TIM_IT_Update)) {
		TIM_ClearITPendingBit( timer->regs, TIM_IT_Update );
		++timer->pwmModTim;
		
		if (timer->pwmModTim == timer->pwmMax*2) {
			if ( timer->pwmCurrentChannel1 != 0 ) enableChannel( timer, FIRST );
			if ( timer->pwmCurrentChannel2 != 0 ) enableChannel( timer, SECOND );
			timer->pwmModTim = 0;
		}
		
		if (timer->pwmModTim == abs(timer->pwmCurrentChannel1)*2) {
			disableChannel( timer, FIRST );
		}
		
		if (timer->pwmModTim == abs(timer->pwmCurrentChannel2)*2) {
			disableChannel( timer, SECOND );
		}
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
		++timer->pwmModTim;
		
		if (timer->pwmModTim == timer->pwmMax*2) {
			enableChannel( timer, THIRD );
			enableChannel( timer, FOURS );
			timer->pwmModTim = 0;
		}
		
		if (timer->pwmModTim == abs(timer->pwmCurrentChannel3)*2) {
			disableChannel( timer, THIRD );
		}
		
		if (timer->pwmModTim == abs(timer->pwmCurrentChannel4)*2) {
			disableChannel( timer, FOURS );
		}
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
	tim3IrqHandler( &Timer3 );
}
#endif

void tim3IrqHandler( timer_struct * timer ){
	if (TIM_GetITStatus(timer->regs, TIM_IT_Update)){
		TIM_ClearITPendingBit(timer->regs, TIM_IT_Update);
		timer->accelerationFlag = 1;
	}
}
