/**************************************************************************************************
 COMMON.H

 ������������ ���� ������ COMMON
**************************************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>


/**************************************************************************************************
                                            �����������
**************************************************************************************************/



/**************************************************************************************************
                                          ���� ������
**************************************************************************************************/

typedef enum done_inprogress_enum {RET_DONE, RET_IN_PROGRESS} done_inprogress;
typedef enum ok_fail_enum {RET_OK, RET_FAIL} ok_fail;

typedef done_inprogress (*done_inprogress_func)(void);
typedef void (*callback)( void );


/**************************************************************************************************
                                   ��������� ���������� �������
**************************************************************************************************/

void InitIwdg(void);
void SysErrorHandler(void);



/**************************************************************************************************
                                     ���������� INLINE-�������
**************************************************************************************************/

/**************************************************************************************************
��������:  ���������� ������� ���� 16-������� �����
���������: 16-������ �����
�������:   ������� ����
���������:
**************************************************************************************************/
static inline uint8_t GetH(uint16_t data)
{
    return (uint8_t)(data >> 8);
}



/**************************************************************************************************
��������:  ���������� ������� ���� 16-������� �����
���������: 16-������ �����
�������:   ������� ����
���������:
**************************************************************************************************/
static inline uint8_t GetL(uint16_t data)
{
    return (uint8_t)(data & 0x00FF);
}



/**************************************************************************************************
��������:  ���������� 16-������ �����
���������: ��������� �� ������� (������) ����
�������:   16-������ �����
���������:
**************************************************************************************************/
static inline uint16_t Get16(uint8_t * ptr)
{
	return ( (uint16_t)(*ptr) | ((uint16_t)(*(ptr + 1)) << 8) );
}



/**************************************************************************************************
��������:  ��������������� ������������� int16_t � int8_t
���������: �������� � int16_t
�������:   �������� � int8_t
���������:
**************************************************************************************************/
static inline int8_t RescaleInt16ToInt8(int16_t value)
{
	return (int8_t)(value * INT8_MAX / INT16_MAX);
}



/**************************************************************************************************
��������:  ��������������� ������������� int16_t � int8_t
���������: �������� � int16_t
�������:   �������� � int8_t
���������:
**************************************************************************************************/
static inline uint8_t RescaleUint16ToUint8(uint16_t value)
{
	return (uint8_t)(value * UINT8_MAX / UINT16_MAX);
}



/**************************************************************************************************
��������:  ��������������� ������������� int8_t � int16_t
���������: �������� � int8_t
�������:   �������� � int16_t
���������:
**************************************************************************************************/
static inline int16_t RescaleInt8ToInt16(int8_t value)
{
	return (int16_t)(value * INT16_MAX / INT8_MAX);
}

