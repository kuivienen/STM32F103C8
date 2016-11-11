#include "buttons.h"

/**************************************************************************************************
                                    ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/

void initButtonGpio( button_struct * key );
//void initKeyExti( button_struct * key );
//void keyIrqHandler( button_struct * key );

/**************************************************************************************************
                                       ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef BUTTONS
	button_struct Buttons =
	{
		.countLines				=	BUTTON_LINES,
		.buttonPorts 			= BUTTON_PORTS,
		.buttonPins 			= BUTTON_PINS,
	};
#endif
	
	
/**************************************************************************************************
                                       ЛОКАЛЬНЫЕ ПЕРЕМЕННЫЕ
**************************************************************************************************/

	
/**************************************************************************************************
                                        ГЛОБАЛЬНЫЕ ФУНКЦИИ
**************************************************************************************************/


/**************************************************************************************************
Описание:  Инициализация кнопок
Аргументы: Указатель на структуру button
Возврат:   Нет
Замечания: 
**************************************************************************************************/
	
void InitButtons( button_struct * button )
{
	initButtonGpio( button );
}

/**************************************************************************************************
Описание:  Возвращает true, если состояние переключателей изменилось
Аргументы: Указатель на структуру button
Возврат:   Нет
Замечания: 
**************************************************************************************************/

bool ChangeButtons( button_struct * buttons ) {
	if (buttons->lastState != buttons->currentState) return true;
	else return false;
}

/**************************************************************************************************
Описание:  Определяем текущее сосстояние кнопок
Аргументы: Указатель на структуру button
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void ScanButtons( button_struct * buttons ) {
	buttons->currentState = 0;
	for (uint8_t i; i < buttons->countLines; ++i) {
		buttons->currentState |= (GPIO_ReadInputDataBit( buttons->buttonPorts[i], buttons->buttonPins[i] ) << (buttons->countLines - i-1));
	}
}

/**************************************************************************************************
Описание:  Определяем действие при изменении кнопок
Аргументы: Указатель на структуру button
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void ScanChangeButtons( button_struct * buttons ) {
	if (ChangeButtons( buttons ))
	{
		uint8_t temp = buttons->lastState & ~(buttons->currentState);
		
		switch (temp & MOTOR1_MASK)	{
			case MOTOR1_BACKWARD:
				RunModeMotor( &Motor1, BACKWARD );
				break;
			case MOTOR1_FORWARD:
				RunModeMotor( &Motor1, FORWARD );
				break;
			case MOTOR1_OFF:
				RunModeMotor( &Motor1, OFF );
				break;
			default:
				FAIL_ASSERT();
		}
		
		switch (temp & MOTOR2_MASK)	{
			case MOTOR2_BACKWARD:
				RunModeMotor( &Motor2, BACKWARD );
				break;
			case MOTOR2_FORWARD:
				RunModeMotor( &Motor2, FORWARD );
				break;
			case MOTOR2_OFF:
				RunModeMotor( &Motor2, OFF );
				break;
			default:
				FAIL_ASSERT();
		}
		
		switch (temp & MOTOR3_MASK)	{
			case MOTOR3_BACKWARD:
				RunModeMotor( &Motor3, BACKWARD );
				break;
			case MOTOR3_FORWARD:
				RunModeMotor( &Motor3, FORWARD );
				break;
			case MOTOR3_OFF:
				RunModeMotor( &Motor3, OFF );
				break;
			default:
				FAIL_ASSERT();
		}
		
		switch (temp & MOTOR4_MASK)	{
			case MOTOR4_BACKWARD:
				RunModeMotor( &Motor4, BACKWARD );
				break;
			case MOTOR4_FORWARD:
				RunModeMotor( &Motor4, FORWARD );
				break;
			case MOTOR4_OFF:
				RunModeMotor( &Motor4, OFF );
				break;
			default:
				FAIL_ASSERT();
		}
		
	}
}

/**************************************************************************************************
                                       ЛОКАЛЬНЫЕ ФУНКЦИИ
**************************************************************************************************/

/**************************************************************************************************
Описание:  Инициализация выходных пинов клавиатуры
Аргументы: Указатель на структуру Keyboard
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void initButtonGpio( button_struct * button )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//	инициализируем сканирующие порты
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	for (uint8_t i=0; i < button->countLines; ++i)
	{
		GPIO_InitStructure.GPIO_Pin = button->buttonPins[i];	
		GPIO_Init( button->buttonPorts[i] , &GPIO_InitStructure );
	}		
}

