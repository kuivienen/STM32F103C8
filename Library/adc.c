

#include <adc.h>

/**************************************************************************************************
                                    ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void InitAdcRcc(ADC_struct * adc);
void InitAdcGpio(ADC_struct *adc);
void InitAdcParams(ADC_struct *adc);

/**************************************************************************************************
                                 ИНИЦИАЛИЗАЦИЯ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
**************************************************************************************************/

#ifdef ADC_1
/*	ADC_struct ADC1_ =
	{
		.regs 							= ADC1_REGS,
		.usingChannel 			= ADC1_USING_CHAN,

		.pin 								= ADC1_PIN,
		.port 							= ADC1_PORT,

		.contConvMode 			= ADC1_CONT_CONV_MODE,
		.dataAlign 					= ADC1_ALIGN,
		.extTrigConv 				= ADC1_EXT_TRIG_CONV,
		.mode 							= ADC1_MODE,
		.scanConvMode 			= ADC1_SCAN_MODE,
		.sampleTime 				= ADC1_SAMP_TIME,
		.dma 								= ADC1_DMA,
	};*/
#endif


/**************************************************************************************************
Описание:  Включение тактирования ADC
Аргументы: Указатель на структуру ADC_struct
Возврат:   Нет
Замечания:
**************************************************************************************************/
void InitAdcRcc(ADC_struct * adc)
{
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	if (adc->regs == ADC1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	}

	else if (adc->regs == ADC2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	}

	else if (adc->regs == ADC3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	}

	else
	{
		#ifdef  USE_FULL_ASSERT
			assert_failed( (uint8_t *)__FILE__, __LINE__ );
		#endif
	}
}


/**************************************************************************************************
Описание:  Настройка портов для работы в режиме аналогового входа
Аргументы: Указатель на структуру ADC_struct
Возврат:   Нет
Замечания:
**************************************************************************************************/
void InitAdcGpio(ADC_struct *adc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t count = (adc->temp) ? (adc->countChannel - 1) : adc->countChannel;

	for (int i=0; i<count; ++i)
	{
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Pin   = adc->pin[i];
		GPIO_Init(adc->port[i], &GPIO_InitStructure);
	}
}

/**************************************************************************************************
Описание:  Настройка АЦП
Аргументы: Указатель на структуру ADC_struct
Возврат:   Нет
Замечания:
**************************************************************************************************/
void InitAdcParams(ADC_struct *adc)
{
	ADC_InitTypeDef ADC_InitStructure;

	ADC_InitStructure.ADC_Mode = adc->mode;
	ADC_InitStructure.ADC_ScanConvMode = adc->scanConvMode;
	ADC_InitStructure.ADC_ContinuousConvMode = adc->contConvMode;
	ADC_InitStructure.ADC_ExternalTrigConv = adc->extTrigConv;
	ADC_InitStructure.ADC_DataAlign = adc->dataAlign;
	ADC_InitStructure.ADC_NbrOfChannel = adc->countChannel;

	ADC_Init (adc->regs, &ADC_InitStructure);

	if (adc->temp)
	{
		ADC_TempSensorVrefintCmd(ENABLE);
	}

	for (int i = 0; i < adc->countChannel; ++i)
	{
		ADC_RegularChannelConfig(adc->regs, adc->usingChannel[i], adc->sequenceChannel[i], adc->sampleTime);
	}


	ADC_Cmd (adc->regs, ENABLE);

	ADC_ResetCalibration(adc->regs);
	while(ADC_GetResetCalibrationStatus(adc->regs));
	ADC_StartCalibration(adc->regs);
	while(ADC_GetCalibrationStatus(adc->regs));

	if (adc->dma)
	{
		ADC_DMACmd(adc->regs, ENABLE);
	}

	ADC_Cmd (adc->regs,ENABLE);
}

/**************************************************************************************************
Описание:  Инициализация АЦП
Аргументы: Указатель на структуру ADC_struct
Возврат:   Нет
Замечания:
**************************************************************************************************/

void InitAdc(ADC_struct * adc)
{
	InitAdcRcc(adc);
	if (adc->temp)
	{
		InitAdcGpio(adc);
	}
	InitAdcParams(adc);
}

