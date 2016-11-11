#ifndef __BUTTONS__

#define __BUTTONS__

#include <stm32f10x_conf.h>
#include <stm32f10x.h>
#include "time_service.h"
#include <stdbool.h>
#include "motor.h"

/**************************************************************************************************
                                          НАСТРОЙКИ КНОПОК
**************************************************************************************************/

#define BUTTONS
	
	#define BUTTON_LINES						8

	#define BUTTON_PORTS						{GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA, GPIOA}
	#define BUTTON_PINS							{GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_12}
	
/**************************************************************************************************
                                   МАСКА СОСТОЯНИЙ ТУМБЛЕРОВ
**************************************************************************************************/
	
	
#define MOTOR1_MASK			0x02 << 6	
#define MOTOR1_BACKWARD		0x01 << 6
#define MOTOR1_FORWARD		0x02 << 6
#define MOTOR1_OFF			0x03 << 6

#define MOTOR2_MASK			0x02 << 4	
#define MOTOR2_BACKWARD		0x01 << 4
#define MOTOR2_FORWARD		0x02 << 4
#define MOTOR2_OFF			0x03 << 4
	
#define MOTOR3_MASK			0x02 << 2
#define MOTOR3_BACKWARD		0x01 << 2
#define MOTOR3_FORWARD		0x02 << 2
#define MOTOR3_OFF			0x03 << 2
	
#define MOTOR4_MASK			0x02
#define MOTOR4_BACKWARD		0x01
#define MOTOR4_FORWARD		0x02
#define MOTOR4_OFF			0x03
	
/**************************************************************************************************
                                          ТИПЫ ДАННЫХ
**************************************************************************************************/

typedef struct
{
	uint8_t						countLines;
	
	GPIO_TypeDef 			* buttonPorts[BUTTON_LINES];
	uint16_t 						buttonPins[BUTTON_LINES];
	
	uint8_t							lastState;
	uint8_t							currentState;
	
} button_struct;

/**************************************************************************************************
                                         EXTERN-ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef BUTTONS
	extern button_struct Buttons;
#endif

/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void InitButtons( button_struct * buttons );
void ScanButtons( button_struct * buttons );
bool ChangeButtons( button_struct * buttons );
void ScanChangeButtons( button_struct * buttons );

#endif	/*	__BUTTONS__	*/
