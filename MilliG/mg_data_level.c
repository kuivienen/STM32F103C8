/**************************************************************************************************
 MG_DATA_LEVEL.C

 Уровеь данных MilliGanjubus

**************************************************************************************************/


#include <mg_data_level.h>



/**************************************************************************************************
                                    ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
**************************************************************************************************/

static void processReadRangeReq(mg_input * inMsg, mg_output * outMsg);
static void processWriteRangeReq(mg_input * inMsg, mg_output * outMsg);
static void processReadSeriesReq(mg_input * inMsg, mg_output * outMsg);
static void processWriteSeriesReq(mg_input * inMsg, mg_output * outMsg);
static void generateNack(mg_input * inMsg, mg_output * outMsg, uint8_t errCode);
static ok_fail processReadRangeAns(mg_input * inMsg, mg_output * outMsg);
static ok_fail processWriteRangeAns(mg_input * inMsg, mg_output * outMsg);
static ok_fail processReadSeriesAns(mg_input * inMsg, mg_output * outMsg);
static ok_fail processWriteSeriesAns(mg_input * inMsg, mg_output * outMsg);



/**************************************************************************************************
                                        ГЛОБАЛЬНЫЕ ФУНКЦИИ
**************************************************************************************************/

/**************************************************************************************************
Описание:  Обработка слэйвом запроса мастера
Аргументы: Указатель на структуру запроса, указатель на структуру ответа
Возврат:   Нет
Замечания:

   Структура обрабатываемого запроса:
   ----------------------------------
   | G-байт | ... ... data ... ...  |
   ----------------------------------
   |<------------ Stuff ----------->|

**************************************************************************************************/
void MG_ProcessRequest(mg_input * inMsg, mg_output * outMsg)
{
	uint8_t gByte = inMsg->stuff[0];
	uint8_t ackCode = G_GetAckCode(gByte);
	uint8_t fCode = G_GetFcode(gByte);
	
	if (inMsg->expectedAckCode == ackCode)
	{
		switch (fCode)
		{
			case G_FCODE_WRITE_SINGLE_REG:
			case G_FCODE_WRITE_REGS_SERIES:
				processWriteSeriesReq(inMsg, outMsg);
				break;			
			case G_FCODE_WRITE_REGS_RANGE:
				processWriteRangeReq(inMsg, outMsg);
				break;
			case G_FCODE_READ_SINGLE_REG:
			case G_FCODE_READ_REGS_SERIES:
				processReadSeriesReq(inMsg, outMsg);
				break;			
			case G_FCODE_READ_REGS_RANGE:
				processReadRangeReq(inMsg, outMsg);
				break;			
			default:
				generateNack(inMsg, outMsg, G_ERRCODE_WRONG_GBYTE);
		}
	}
}



/**************************************************************************************************
Описание:  Обработка мастером ответа слэйва
Аргументы: Указатель на структуру ответа, указатель на структуру запроса
Возврат:   Нет
Замечания:

   Структура обрабатываемого запроса:
   ----------------------------------
   | G-байт | ... ... data ... ...  |
   ----------------------------------
   |<------------ Stuff ----------->|

**************************************************************************************************/
ok_fail MG_ProcessAnswer(mg_input * inMsg, mg_output * outMsg)
{
	uint8_t ackCode = G_GetAckCode(inMsg->stuff[0]);
	uint8_t inFcode = G_GetFcode(inMsg->stuff[0]);
	uint8_t outFcode = G_GetFcode(outMsg->stuff[0]);	
	
	if ((inMsg->expectedAckCode == ackCode) && (outFcode == inFcode))
	{
		switch (inFcode)
		{
			case G_FCODE_WRITE_SINGLE_REG:
			case G_FCODE_WRITE_REGS_SERIES:
				return processWriteSeriesAns(inMsg, outMsg);
			case G_FCODE_WRITE_REGS_RANGE:
				return processWriteRangeAns(inMsg, outMsg);
			case G_FCODE_READ_SINGLE_REG:
			case G_FCODE_READ_REGS_SERIES:
				return processReadSeriesAns(inMsg, outMsg);
			case G_FCODE_READ_REGS_RANGE:
				return processReadRangeAns(inMsg, outMsg);
			default:
				return RET_FAIL;
		}
	}
	return RET_FAIL;
}



/**************************************************************************************************
                                       ЛОКАЛЬНЫЕ ФУНКЦИИ
**************************************************************************************************/

/**************************************************************************************************
Описание:  Функция обработки запроса на чтение диапазона регистров
Аргументы: Указатель на структуру запроса, указатель на структуру ответа
Возврат:   Нет
Замечания:

   Структура обрабатываемого запроса:
   ------------------------------------
   | G-байт | StartRegAdr | EndRegAdr |
   ------------------------------------
       0           1            2

   Структура подготавливаемого ответа:
   -----------------------------------------------------------
   | G-байт |  StartRegAdr | EndRegAdr | Data1 | ... | DataN |
   -----------------------------------------------------------
       0           1             2         3            2+N

**************************************************************************************************/
static void processReadRangeReq(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t expectedStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_REG_ADR_SIZE;
	if (inMsg->stuffSize != expectedStuffSize)
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_BYTES_NUM);
		return;	
	}

	mg_reg_addr regAdrStart = inMsg->stuff[1];
	mg_reg_addr regAdrEnd = inMsg->stuff[2];
	uint8_t regsTotal = (uint8_t)(regAdrEnd - regAdrStart + 1);

	if ( (regAdrStart > regAdrEnd) || (regsTotal > MG_MAX_REGS_IN_RANGE) )
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_ADDRESS);
		return;	
	}

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		if (MG_IsRegValid(regAdrStart + i) == false)
		{
			generateNack(inMsg, outMsg, G_ERRCODE_WRONG_ADDRESS);
			return;	
		}
	}

	outMsg->stuff[0] = G_MakeGbyte(G_GetFcode(inMsg->stuff[0]), G_ACK);
	outMsg->stuff[1] = regAdrStart;
	outMsg->stuff[2] = regAdrEnd;
	outMsg->stuffSize = 3;

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		outMsg->stuff[outMsg->stuffSize] = MG_GetReg(regAdrStart + i);
		outMsg->stuffSize++;
	}	
}



/**************************************************************************************************
Описание:  Функция обработки запроса на запись диапазона регистров
Аргументы: Указатель на структуру запроса, указатель на структуру ответа
Возврат:   Нет
Замечания:

   Структура обрабатываемого запроса:
   -----------------------------------------------------------
   | G-байт |  StartRegAdr | EndRegAdr | Data0 | ... | DataN |
   -----------------------------------------------------------
       0            1            2         3            3+N

   Структура подготавливаемого ответа:
   ------------------------------------
   | G-байт | StartRegAdr | EndRegAdr |
   ------------------------------------
       0           1            2

**************************************************************************************************/
static void processWriteRangeReq(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t minStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_REG_ADR_SIZE + MG_MIN_DATA_SIZE;
	if (inMsg->stuffSize < minStuffSize)
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_BYTES_NUM);
		return;	
	}

	mg_reg_addr regAdrStart = inMsg->stuff[1];
	mg_reg_addr regAdrEnd = inMsg->stuff[2];
	uint8_t regsTotal = (uint8_t)(regAdrEnd - regAdrStart + 1);
	
	if ( (regAdrStart > regAdrEnd) || (regsTotal > MG_MAX_REGS_IN_RANGE) )
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_ADDRESS);
		return;
	}

	uint8_t expectedStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_REG_ADR_SIZE + regsTotal;
	if (inMsg->stuffSize != expectedStuffSize)
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_BYTES_NUM);
		return;	
	}

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		if ((MG_IsRegValid(regAdrStart + i) == false) || (MG_IsRegTypeW(regAdrStart + i) == false))
		{
			generateNack(inMsg, outMsg, G_ERRCODE_WRONG_ADDRESS);
			return;	
		}
	}
	
	for (uint8_t i = 0; i < regsTotal; i++)
	{
		MG_SetReg(regAdrStart + i, inMsg->stuff[3 + i]);
	}
	
	outMsg->stuff[0] = G_MakeGbyte(G_GetFcode(inMsg->stuff[0]), G_ACK);
	outMsg->stuff[1] = regAdrStart;
	outMsg->stuff[2] = regAdrEnd;
	outMsg->stuffSize = 3;
}



/**************************************************************************************************
Описание:  Функция обработки запроса на чтение серии регистров
Аргументы: Указатель на структуру запроса, указатель на структуру ответа
Возврат:   Нет
Замечания:

   Структура обрабатываемого запроса:
   ------------------------------------
   | G-байт | RegAdr0 | ... | RegAdrN |
   ------------------------------------
       0         1              1+N

   Структура подготавливаемого ответа:
   -----------------------------------------------------
   | G-байт |  RegAdr0 | Data0 | ... | RegAdrN | DataN |
   -----------------------------------------------------
       0          1        2            1+N*2   1+N*2+1

**************************************************************************************************/
static void processReadSeriesReq(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t minStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE;
	if (inMsg->stuffSize < minStuffSize)
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_BYTES_NUM);
		return;	
	}

	uint8_t regsTotal = inMsg->stuffSize - 1;

	if (regsTotal > MG_MAX_REGS_IN_SERIES)
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_ADDRESS);
		return;	
	}

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		if (MG_IsRegValid(inMsg->stuff[1 + i]) == false)
		{
			generateNack(inMsg, outMsg, G_ERRCODE_WRONG_ADDRESS);
			return;	
		}
	}

	outMsg->stuff[0] = G_MakeGbyte(G_GetFcode(inMsg->stuff[0]), G_ACK);
	outMsg->stuffSize = 1;

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		outMsg->stuff[outMsg->stuffSize] = inMsg->stuff[1 + i];
		outMsg->stuff[outMsg->stuffSize + 1] = MG_GetReg(inMsg->stuff[1 + i]);
		outMsg->stuffSize += 2;
	}
}



/**************************************************************************************************
Описание:  Функция обработки запроса на запись серии регистров
Аргументы: Указатель на структуру запроса, указатель на структуру ответа
Возврат:   Нет
Замечания:

   Структура обрабатываемого запроса:
   -----------------------------------------------------
   | G-байт |  RegAdr0 | Data0 | ... | RegAdrN | DataN |
   -----------------------------------------------------
       0          1        2            1+N*2   1+N*2+1

   Структура подготавливаемого ответа:
   ------------------------------------
   | G-байт | RegAdr0 | ... | RegAdrN |
   ------------------------------------
       0         1              1+N

**************************************************************************************************/
static void processWriteSeriesReq(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t minStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_MIN_DATA_SIZE;
	if (inMsg->stuffSize < minStuffSize)
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_BYTES_NUM);
		return;	
	}

	uint8_t regsTotal = (inMsg->stuffSize - 1) / 2;

	uint8_t expectedStuffSize = MG_GBYTE_SIZE + regsTotal * 2;
	if (inMsg->stuffSize != expectedStuffSize)
	{
		generateNack(inMsg, outMsg, G_ERRCODE_WRONG_BYTES_NUM);
		return;
	}

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		if ((MG_IsRegValid(inMsg->stuff[1 + i * 2]) == false) || (MG_IsRegTypeW(inMsg->stuff[1 + i * 2]) == false))
		{
			generateNack(inMsg, outMsg, G_ERRCODE_WRONG_ADDRESS);
			return;	
		}
	}

	outMsg->stuff[0] = G_MakeGbyte(G_GetFcode(inMsg->stuff[0]), G_ACK);
	outMsg->stuffSize = 1;

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		MG_SetReg(inMsg->stuff[1 + i * 2], inMsg->stuff[1 + i * 2 + 1]);
		outMsg->stuff[outMsg->stuffSize] = inMsg->stuff[1 + i * 2];
		outMsg->stuffSize++;
	}
}



/**************************************************************************************************
Описание:  Функция подготовки ответа NACK
Аргументы: Указатель на структуру запроса, указатель на структуру ответа
Возврат:   Нет
Замечания:

   Структура подготавливаемого ответа:
   --------------------
   | G-байт | ErrCode |
   --------------------
       0         1

**************************************************************************************************/
static void generateNack(mg_input * inMsg, mg_output * outMsg, uint8_t errCode)
{
	outMsg->stuff[0] = G_MakeGbyte(G_GetFcode(inMsg->stuff[0]), G_NACK);
	outMsg->stuff[1] = errCode;
	outMsg->stuffSize = 2;
}



/**************************************************************************************************
Описание:  Функция обработки ответа на запрос чтения диапазона регистров
Аргументы: Указатель на структуру ответа, указатель на структуру запроса
Возврат:   OK/FAIL
Замечания:

   Структура обрабатываемого ответа:
   -----------------------------------------------------------
   | G-байт |  StartRegAdr | EndRegAdr | Data1 | ... | DataN |
   -----------------------------------------------------------
       0           1             2         3            2+N

   Структура переданного запроса:
   ------------------------------------
   | G-байт | StartRegAdr | EndRegAdr |
   ------------------------------------
       0           1            2

**************************************************************************************************/
static ok_fail processReadRangeAns(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t minStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_REG_ADR_SIZE + MG_MIN_DATA_SIZE;
	if (inMsg->stuffSize < minStuffSize)
	{
		return RET_FAIL;
	}	
	
	if ( (inMsg->stuff[1] != outMsg->stuff[1]) || (inMsg->stuff[2] != outMsg->stuff[2]) )
	{
		return RET_FAIL;
	}
	
	mg_reg_addr regAdrStart = inMsg->stuff[1];
	mg_reg_addr regAdrEnd = inMsg->stuff[2];
	uint8_t regsTotal = (uint8_t)(regAdrEnd - regAdrStart + 1);

	uint8_t expectedStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_REG_ADR_SIZE + regsTotal;
	if (inMsg->stuffSize != expectedStuffSize)
	{
		return RET_FAIL;
	}

	for (uint8_t i = 0; i < regsTotal; i++)
	{
		mg_reg_addr selfRegAdr = inMsg->getRegAdr(regAdrStart + i, inMsg->devAdr);
		if (MG_IsRegValid(selfRegAdr) == true)
		{
			MG_SetReg(selfRegAdr, inMsg->stuff[3 + i]);
		}
	}
	
	return RET_OK;
}



/**************************************************************************************************
Описание:  Функция обработки ответа на запрос записи диапазона регистров
Аргументы: Указатель на структуру ответа, указатель на структуру запроса
Возврат:   OK/FAIL
Замечания:

   Структура обрабатываемого ответа:
   ------------------------------------
   | G-байт | StartRegAdr | EndRegAdr |
   ------------------------------------
       0           1            2

   Структура переданного запроса:
   -----------------------------------------------------------
   | G-байт |  StartRegAdr | EndRegAdr | Data1 | ... | DataN |
   -----------------------------------------------------------
       0           1             2         3            2+N

**************************************************************************************************/
static ok_fail processWriteRangeAns(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t expectedStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_REG_ADR_SIZE;
	if (inMsg->stuffSize != expectedStuffSize)
	{
		return RET_FAIL;
	}

	if ( (inMsg->stuff[1] != outMsg->stuff[1]) || (inMsg->stuff[2] != outMsg->stuff[2]) )
	{
		return RET_FAIL;
	}
	return RET_OK;
}



/**************************************************************************************************
Описание:  Функция обработки ответа на запрос чтения серии регистров
Аргументы: Указатель на структуру ответа, указатель на структуру запроса
Возврат:   OK/FAIL
Замечания:

   Структура обрабатываемого ответа:
   -----------------------------------------------------
   | G-байт |  RegAdr0 | Data0 | ... | RegAdrN | DataN |
   -----------------------------------------------------
       0          1        2            1+N*2   1+N*2+1

   Структура переданного запроса:
   ------------------------------------
   | G-байт | RegAdr0 | ... | RegAdrN |
   ------------------------------------
       0         1              1+N

**************************************************************************************************/
static ok_fail processReadSeriesAns(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t minStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE + MG_MIN_DATA_SIZE;
	if (inMsg->stuffSize < minStuffSize)
	{
		return RET_FAIL;
	}

	uint8_t ansRegsTotal = (inMsg->stuffSize - 1) / 2;
	uint8_t reqRegsTotal = (outMsg->stuffSize - 1);

	if (ansRegsTotal != reqRegsTotal)
	{
		return RET_FAIL;
	}

	uint8_t expectedStuffSize = MG_GBYTE_SIZE + ansRegsTotal * 2;
	if (inMsg->stuffSize != expectedStuffSize)
	{
		return RET_FAIL;
	}

	for (uint8_t i = 0; i < ansRegsTotal; i++)
	{
		if (inMsg->stuff[1 + i * 2] != outMsg->stuff[1 + i])
		{
			return RET_FAIL;
		}
	}

	for (uint8_t i = 0; i < ansRegsTotal; i++)
	{
		mg_reg_addr selfRegAdr = inMsg->getRegAdr(inMsg->stuff[1 + i * 2], inMsg->devAdr);
		if (MG_IsRegValid(selfRegAdr) == true)
		{
			MG_SetReg(selfRegAdr, inMsg->stuff[1 + i * 2 + 1]);
		}
	}

	return RET_OK;
}



/**************************************************************************************************
Описание:  Функция обработки ответа на запрос записи диапазона регистров
Аргументы: Указатель на структуру ответа, указатель на структуру запроса
Возврат:   OK/FAIL
Замечания:

   Структура обрабатываемого ответа:
   ------------------------------------
   | G-байт | RegAdr0 | ... | RegAdrN |
   ------------------------------------
       0         1              1+N

   Структура переданного запроса:
   -----------------------------------------------------
   | G-байт |  RegAdr0 | Data0 | ... | RegAdrN | DataN |
   -----------------------------------------------------
       0          1        2            1+N*2   1+N*2+1

**************************************************************************************************/
static ok_fail processWriteSeriesAns(mg_input * inMsg, mg_output * outMsg)
{
	const uint8_t minStuffSize = MG_GBYTE_SIZE + MG_REG_ADR_SIZE;
	if (inMsg->stuffSize < minStuffSize)
	{
		return RET_FAIL;
	}

	uint8_t ansRegsTotal = (inMsg->stuffSize - 1);
	uint8_t reqRegsTotal = (outMsg->stuffSize - 1)/ 2;

	if (ansRegsTotal != reqRegsTotal)
	{
		return RET_FAIL;
	}

	for (uint8_t i = 0; i < ansRegsTotal; i++)
	{
		if (inMsg->stuff[1 + i] != outMsg->stuff[1 + i * 2])
		{
			return RET_FAIL;
		}
	}

	return RET_OK;
}
