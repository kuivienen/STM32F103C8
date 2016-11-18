/**************************************************************************************************
 MG_COMMON.H

 Заголовочный файл протокола MilliGanjubus

**************************************************************************************************/

#ifndef __MG_COMMON__

#define __MG_COMMON__


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include <time_service.h>
#include <mg_regs.h>
#include <ganjubus_common.h>


/**************************************************************************************************
                                            КОНСТАНТЫ
**************************************************************************************************/

// Размеры буферов
#define MG_IN_MSG_BUF_SIZE 255
#define MG_OUT_BUF_SIZE 255

// Параметры приема
#define MG_MAX_RECEIVE_DELAY 500

// Структура пакета
#define MG_START_BYTE 0xBB
#define MG_OFFSET_START_BYTE 0
#define MG_OFFSET_DEV_ADR 1
#define MG_OFFSET_SIZE 2
#define MG_OFFSET_HEADER_CRC 3
#define MG_OFFSET_STUFF 4

// Параметра пакетов
#define MG_MAX_MSG_SIZE 15
#define MG_HEADER_SIZE 4
#define MG_GBYTE_SIZE 1
#define MG_REG_ADR_SIZE 1
#define MG_CRC_SIZE 1
#define MG_MIN_DATA_SIZE 1
#define MG_MAX_REGS_IN_RANGE (MG_MAX_MSG_SIZE - MG_HEADER_SIZE - MG_GBYTE_SIZE - MG_REG_ADR_SIZE * 2)
#define MG_MAX_REGS_IN_SERIES ((MG_MAX_MSG_SIZE - MG_HEADER_SIZE - MG_GBYTE_SIZE) / 2)
#define MG_MIN_REQ_SIZE (MG_HEADER_SIZE + MG_GBYTE_SIZE + MG_MIN_DATA_SIZE + MG_CRC_SIZE)
#define MG_MAX_REQ_SIZE MG_MAX_MSG_SIZE



/**************************************************************************************************
                                             ТИПЫ ДАННЫХ
**************************************************************************************************/

typedef struct
{
	uint8_t joint[MG_IN_MSG_BUF_SIZE];
	uint8_t jointSize;
	uint8_t * stuff;
	uint8_t stuffSize;
	uint8_t counter;
	soft_timer timer;
	uint8_t devAdr;
	uint8_t expectedAckCode;
	mg_reg_addr (*getRegAdr)(mg_reg_addr slaveReg, uint8_t devAdr);
} mg_input;



typedef struct
{
	uint8_t joint[MG_OUT_BUF_SIZE];
	uint8_t jointSize;
	uint8_t * stuff;	
	uint8_t stuffSize;
	uint8_t devAdr;
} mg_output;



/**************************************************************************************************
                                   ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/

void MG_InitInputStruct(mg_input * msg, uint8_t ackCode);
void MG_InitOutputStruct(mg_output * msg);
void MG_StartWriteReq(mg_output * outMsg, uint8_t devAdr);
void MG_AddRegToWriteReq(mg_output * outMsg, mg_reg_addr adr, mg_reg_value value);
void MG_StartReadReq(mg_output * outMsg, uint8_t devAdr);
void MG_AddRegToReadReq(mg_output * outMsg, mg_reg_addr adr);
bool MG_IsWriteReqFull(mg_output * outMsg);
bool MG_IsReadReqFull(mg_output * outMsg);
void MG_SetRegRemapFunc(mg_input * msg, mg_reg_addr (*getRegAdr)(mg_reg_addr slaveReg, uint8_t devAdr));
void MG_SetInputDevAdr(mg_input * msg, uint8_t devAdr);
void MG_SetOutputDevAdr(mg_output * msg, uint8_t devAdr);
uint8_t * MG_GetOutBufPtr(mg_output * msg);
uint8_t MG_GetOutBufSize(mg_output * msg);
uint8_t MG_GetOutputDevAdr(mg_output * msg);

#endif		/* __MG_COMMON__ */

