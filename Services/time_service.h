/**************************************************************************************************
 TIMER_SERVICE.H

 Заголовочный файл службы времени
   
**************************************************************************************************/

#ifndef __TIME_SERVICE__

#define __TIME_SERVICE__


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include <macros.h>



/**************************************************************************************************
                                          ТИПЫ ДАННЫХ
**************************************************************************************************/

typedef uint32_t system_time;


typedef struct
{
	system_time time;
	system_time delay;
} soft_timer;



/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/

void InitTimeService(void);
void SysTick_Handler(void);
system_time GetSystemTime(void);
void Delay(system_time delayTime);



/**************************************************************************************************
                                     ГЛОБАЛЬНЫЕ INLINE-ФУНКЦИИ
**************************************************************************************************/

/**************************************************************************************************
Описание:  Запуск программного таймера
Аргументы: Указатель на структуру таймера, задержка
Возврат:   Нет
Замечания: 
**************************************************************************************************/
static inline void StartSoftTimer(soft_timer * timer, system_time delay)
{
	timer->time = GetSystemTime();
	timer->delay = delay;
}



/**************************************************************************************************
Описание:  Проверка программного таймера
Аргументы: Указатель на структуру таймера
Возврат:   True, если таймер сработал
Замечания: 
**************************************************************************************************/
static inline bool CheckSoftTimer(soft_timer * timer)
{
	return ((system_time)(GetSystemTime() - timer->time) >= timer->delay);
}

#endif	/*	__TIME_SERVICE__	*/
