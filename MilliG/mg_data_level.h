/**************************************************************************************************
 MG_DATA_LEVEL.H

 ������������ ���� ������ ������ MilliGanjubus

**************************************************************************************************/

#ifndef __MG_DATA_LEVEL__

#define __MG_DATA_LEVEL__


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include <ganjubus_common.h>
#include <mg_common.h>
#include <mg_regs.h>
#include <common.h>



/**************************************************************************************************
                                   ��������� ���������� �������
**************************************************************************************************/

void MG_ProcessRequest(mg_input * inMsg, mg_output * outMsg);
ok_fail MG_ProcessAnswer(mg_input * inMsg, mg_output * outMsg);

#endif		/* __MG_DATA_LEVEL__ */

