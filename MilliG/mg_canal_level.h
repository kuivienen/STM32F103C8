/**************************************************************************************************
 MG_CANAL_LEVEL.H

 Заголовочный файл канального уровня MilliGanjubus

**************************************************************************************************/

#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include <ganjubus_common.h>
#include <mg_common.h>
#include <crc.h>
#include <time_service.h>
#include <common.h>



/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/

done_inprogress MG_ComposeMsg(uint8_t dataByte, mg_input * msg);
void MG_WrapMsg(mg_output * msg);
bool MG_IsAnsNeeded(mg_input * msg);
