#include "motor.h"

/**************************************************************************************************
                                    ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void InitGpioMotor( motor_struct * motor );
void SetPinMode( motor_struct * motor, uint8_t runmode );


/**************************************************************************************************
                                       ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef MOTOR1
	motor_struct Motor1 =
	{
		.regs 				= First,
		
		.fPort				=	MOTOR1_PORT_F,
		.bPort				=	MOTOR1_PORT_B,
		.fPin					=	MOTOR1_PIN_F,
		.bPin					=	MOTOR1_PIN_B,
		
		.targetPower	= 8,
		.currentPower	=	-10,
	};
#endif
	
#ifdef MOTOR2	
	motor_struct Motor2 =
	{
		.regs 			= Second,
		
		.fPort			=	MOTOR2_PORT_F,
		.bPort			=	MOTOR2_PORT_B,
		.fPin				=	MOTOR2_PIN_F,
		.bPin				=	MOTOR2_PIN_B,
		
		.targetPower	= 8,
		.currentPower	=	-10,	
	};
#endif
	
#ifdef MOTOR3	
	motor_struct Motor3 =
	{
		.regs 			= Third,
		
		.fPort			=	MOTOR3_PORT_F,
		.bPort			=	MOTOR3_PORT_B,
		.fPin				=	MOTOR3_PIN_F,
		.bPin				=	MOTOR3_PIN_B,
		
		.targetPower	= 8,
		.currentPower	=	-10,	
	};
#endif	
	
#ifdef MOTOR4	
	motor_struct Motor4 =
	{
		.regs 			= Fours,
		
		.fPort			=	MOTOR4_PORT_F,
		.bPort			=	MOTOR4_PORT_B,
		.fPin				=	MOTOR4_PIN_F,
		.bPin				=	MOTOR4_PIN_B,
		
		.targetPower	= 8,
		.currentPower	=	-10,	
	};
#endif

/**************************************************************************************************
                                        ГЛОБАЛЬНЫЕ ФУНКЦИИ
**************************************************************************************************/


/**************************************************************************************************
Описание:  Инициализация двигателя
Аргументы: Указатель на структуру motor_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/
	
void InitMotor( motor_struct * motor ) {
	InitGpioMotor( motor );
}

/**************************************************************************************************
Описание:  Устанавливает пины разрешения управления логическими микросхемами
Аргументы: Указатель на структуру motor_struct и состояние
Возврат:   Нет
Замечания: 
**************************************************************************************************/

void SetPinMode( motor_struct * motor, uint8_t runmode ){
	switch (runmode){
		case OFF:
				//	устанавливаем разрешения
				GPIO_ResetBits(motor->bPort, motor->bPin);
				GPIO_ResetBits(motor->fPort, motor->fPin);
				break;
		case FORWARD:
				GPIO_ResetBits(motor->bPort, motor->bPin);
				GPIO_SetBits(motor->fPort, motor->fPin);
				break;
		case BACKWARD:
				GPIO_SetBits(motor->bPort, motor->bPin);
				GPIO_ResetBits(motor->fPort, motor->fPin);
				break;
		default:
				FAIL_ASSERT();
		
	}
}

/**************************************************************************************************
Описание:  Задает режим работы двигателя
Аргументы: Указатель на структуру motor_struct
Возврат:   Нет
Замечания: 
**************************************************************************************************/
	
void RunModeMotor( motor_struct * motor, uint8_t runmode ){
	switch (runmode){
		case OFF:
				//	устанавливаем разрешения
				SetPinMode( motor, runmode );
				break;
		case FORWARD:
				SetPinMode( motor, runmode );
				break;
		case BACKWARD:
				SetPinMode( motor, runmode );
				break;
		default:
				FAIL_ASSERT();	
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

void InitGpioMotor( motor_struct * motor ) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//	инициализируем сканирующие порты
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = motor->bPin;	
	GPIO_Init( motor->bPort , &GPIO_InitStructure );	
	
	GPIO_InitStructure.GPIO_Pin = motor->fPin;	
	GPIO_Init( motor->fPort , &GPIO_InitStructure );	
}
