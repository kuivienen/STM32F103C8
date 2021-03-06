/**************************************************************************************************
 MG_REGS.C

 ������� ������ � ����������:
   MG_GetReg
   MG_SetReg
   MG_IsRegChanged
   MG_IsRegValid

**************************************************************************************************/


#include <mg_regs.h>



/**************************************************************************************************
                                      ���������� ����������
**************************************************************************************************/

mg_reg_value RegsRo[MG_REGS_RO_TOTAL];
mg_reg_value RegsRw[MG_REGS_RW_TOTAL];
bool ChangeFlagsRo[MG_REGS_RO_TOTAL];
bool ChangeFlagsRw[MG_REGS_RW_TOTAL];



/**************************************************************************************************
                                        ���������� �������
**************************************************************************************************/

/**************************************************************************************************
��������:  ���������� �������� ��������� ��������
���������: �������
�������:   ��������
���������: �� ����� ���������� � �������������� ��������� - ������� ���������� �������� ��������
**************************************************************************************************/
mg_reg_value MG_GetReg(mg_reg_addr reg)
{
	if (reg <= MG_MAX_REG_RO_ADDR)
	{
		return RegsRo[reg];
	}
	else
	{	
		return RegsRw[reg - MG_MIN_REG_RW_ADDR];
	}
}



/**************************************************************************************************
��������:  ���������� �������� ��������� ��������
���������: �������, ��������
�������:   ���
���������: �� ����� ���������� � �������������� ��������� - ������� ���������� �������� ��������
           ���������� ����� �������, ������� RO, ������� ����
**************************************************************************************************/
void MG_SetReg(mg_reg_addr reg, mg_reg_value value)
{
	if (reg <= MG_MAX_REG_RO_ADDR)
	{
		RegsRo[reg] = value;
		ChangeFlagsRo[reg] = true;
	}
	else
	{	
		RegsRw[reg - MG_MIN_REG_RW_ADDR] = value;
		ChangeFlagsRw[reg - MG_MIN_REG_RW_ADDR] = true;
	}
}



/**************************************************************************************************
��������:  ���������, ��� �� ������� ������� ����� ��������� ��������
���������: �������
�������:   True, ���� ������� ��� �������
���������:
**************************************************************************************************/
bool MG_IsRegChanged(mg_reg_addr reg)
{
	if (reg <= MG_MAX_REG_RO_ADDR)
	{
		if (ChangeFlagsRo[reg] == true)
		{
			ChangeFlagsRo[reg] = false;
			return true;
		}
	}
	else
	{	
		if (ChangeFlagsRw[reg - MG_MIN_REG_RW_ADDR] == true)
		{
			ChangeFlagsRw[reg - MG_MIN_REG_RW_ADDR] = false;
			return true;
		}
	}
	return false;
}



/**************************************************************************************************
��������:  ��������� ������������� � ������� ��������� ��������
���������: �������
�������:   True, ���� ������� ����������
���������:
**************************************************************************************************/
bool MG_IsRegValid(mg_reg_addr reg)
{
	if ( (reg <= MG_REGS_RO_TOTAL - 1) ||
	     ((reg >= MG_MIN_REG_RW_ADDR) && (reg <= MG_MIN_REG_RW_ADDR + MG_REGS_RW_TOTAL - 1)) )
	{
		return true;
	}
	else
	{
		return false;
	}
}



/**************************************************************************************************
��������:  ��������� ������� �� �������������� � R-���������
���������: �������
�������:   True, ���� ������� ������������ ��� ������
���������:
**************************************************************************************************/
bool MG_IsRegTypeR(mg_reg_addr reg)
{
	return (reg <= MG_MAX_REG_RO_ADDR);
}



/**************************************************************************************************
��������:  ��������� ������� �� �������������� � W-���������
���������: �������
�������:   True, ���� ������� ������������ ��� ������
���������:
**************************************************************************************************/
bool MG_IsRegTypeW(mg_reg_addr reg)
{
	return (reg >= MG_MIN_REG_RW_ADDR);
}
