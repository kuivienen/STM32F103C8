/**************************************************************************************************
 MG_DATA_LEVEL.H

 Заголовочный файл уровня данных MilliGanjubus

**************************************************************************************************/

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include <ganjubus_common.h>
#include <mg_common.h>
#include <mg_regs.h>
#include <common.h>



/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/

void MG_ProcessRequest(mg_input * inMsg, mg_output * outMsg);
ok_fail MG_ProcessAnswer(mg_input * inMsg, mg_output * outMsg);
