#ifndef __MOTOR__

#define __MOTOR__

#include <stm32f10x_conf.h>
#include <stm32f10x.h>

#include "macros.h"
//#include <pwm.h>


/**************************************************************************************************
                                          НАСТРОЙКИ МОТОРОВ
**************************************************************************************************/

#define MOTOR1

	#define MOTOR1_PORT_F		GPIOB
	#define MOTOR1_PORT_B		GPIOB
	#define MOTOR1_PIN_F		GPIO_Pin_2
	#define MOTOR1_PIN_B		GPIO_Pin_3
	
	#define MOTOR1_DIR			0							//	направление вращения начальное
	#define	MOTOR1_ON				0							//	вкл/выкл
	#define MOTOR1_POWER		0							//	начальная мощность
	
#define MOTOR2

	#define MOTOR2_PORT_F		GPIOB
	#define MOTOR2_PORT_B		GPIOB
	#define MOTOR2_PIN_F		GPIO_Pin_4
	#define MOTOR2_PIN_B		GPIO_Pin_14

	#define MOTOR2_DIR			0							//	направление вращения начальное
	#define	MOTOR2_ON				0							//	вкл/выкл
	#define MOTOR2_POWER		0							//	начальная мощность
	
#define MOTOR3

	#define MOTOR3_PORT_F		GPIOB
	#define MOTOR3_PORT_B		GPIOB
	#define MOTOR3_PIN_F		GPIO_Pin_10
	#define MOTOR3_PIN_B		GPIO_Pin_11
	
	#define MOTOR3_DIR			0							//	направление вращения начальное
	#define	MOTOR3_ON				0							//	вкл/выкл
	#define MOTOR3_POWER		0							//	начальная мощность
	
#define MOTOR4

	#define MOTOR4_PORT_F		GPIOB
	#define MOTOR4_PORT_B		GPIOB
	#define MOTOR4_PIN_F		GPIO_Pin_12
	#define MOTOR4_PIN_B		GPIO_Pin_13
	
	#define MOTOR4_DIR			0							//	направление вращения начальное
	#define	MOTOR4_ON				0							//	вкл/выкл
	#define MOTOR4_POWER		0							//	начальная мощность
	
	
										/*	РЕЖИМЫ РАБОТЫ МОТОРОВ	*/	
										
#define FORWARD						0
#define	BACKWARD					1
#define OFF								2
	
/**************************************************************************************************
                                          ТИПЫ ДАННЫХ
**************************************************************************************************/

enum motor_number {First, Second, Third, Fours};

typedef struct
{
	enum motor_number				regs;
	
	GPIO_TypeDef 						*fPort;
	uint16_t 								fPin;
	GPIO_TypeDef 						*bPort;
	uint16_t 								bPin;
	
	uint8_t 								dir;
	uint8_t 								on;
	uint8_t 								power;
} motor_struct;


/**************************************************************************************************
                                         EXTERN-ПЕРЕМЕННЫЕ
**************************************************************************************************/
#ifdef MOTOR1
	extern motor_struct Motor1;
#endif

#ifdef MOTOR2
	extern motor_struct Motor2;
#endif

#ifdef MOTOR3
	extern motor_struct Motor3;
#endif

#ifdef MOTOR4
	extern motor_struct Motor4;
#endif

/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/
void InitMotor( motor_struct * motor );
void SetPinMode( motor_struct * motor, uint8_t runmode );
void RunModeMotor( motor_struct * motor, uint8_t runmode );


#endif	/*	__MOTOR__	*/
