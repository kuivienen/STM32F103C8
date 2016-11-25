/**************************************************************************************************
 LINK_SLAVE.H

 Заголовочный файл модуля связи

**************************************************************************************************/

#ifndef __LINK_SLAVE__

#define __LINK_SLAVE__


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include <milli_ganjubus.h>
#include <common.h>
#include <time_service.h>
#include <USB_struct.h>



/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/

void InitLinkSlave( usb_struct * usb );
void ResetLinkSlave(void);
void ProcessLinkSlave(void);

#endif	/*	__LINK_SLAVE__	*/
