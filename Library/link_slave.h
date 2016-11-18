/**************************************************************************************************
 LINK_SLAVE.H

 ������������ ���� ������ �����

**************************************************************************************************/

#ifndef __LINK_SLAVE__

#define __LINK_SLAVE__


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include <milli_ganjubus.h>
#include <common.h>
#include <uart.h>
#include <time_service.h>



/**************************************************************************************************
                                   ��������� ���������� �������
**************************************************************************************************/

void InitLinkSlave(uart_struct * uart);
void ResetLinkSlave(void);
void ProcessLinkSlave(void);

#endif	/*	__LINK_SLAVE__	*/
