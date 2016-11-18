/**************************************************************************************************
 MACROS.H

 �������
   
**************************************************************************************************/

#pragma once


/**************************************************************************************************
                                           �������
**************************************************************************************************/

// ����������� ����������� ������ ����
#define ENTER_CRITICAL_SECTION()    __disable_irq()
#define LEAVE_CRITICAL_SECTION()    __enable_irq()

// ���������� BIT � ADDRESS
#define SETBIT(ADDRESS,BIT) ( (ADDRESS) |= ( 1<<(BIT) ) )

// �������� BIT � ADDRESS
#define CLEARBIT(ADDRESS,BIT) ( (ADDRESS) &= ~( 1<<(BIT) ) )

// ��������� BIT � ADDRESS
#define TESTBIT(ADDRESS,BIT) ( (ADDRESS) & ( 1<<(BIT) ) )

// ������� ���������� ��������� � �������
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)))

// ��������� � ����� ������������ �� ����������� �������
#define FAIL_ASSERT() do{ __disable_irq(); while(1){continue;} }while(0)
