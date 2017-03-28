#ifndef __MG_REG_NAMES__
#define __MG_REG_NAMES__


#define MG_MIN_REG_RO_ADDR 	0x00
#define MG_MAX_REG_RO_ADDR 	0x7F
#define MG_MIN_REG_RW_ADDR	0x80
#define MG_MAX_REG_RW_ADDR	0xFE
#define MG_REG_NULL 				0xFF


#define MG_REGS_RO_TOTAL 0x05
// ----------------------------------------

#define MG_REG_RO_DEVICE_ID									0x00	//	ID ���������� ��� �������� �����������������
#define MG_REG_RO_DEVICE_POWER_CH1					0X01	//	������� ������������ ���������
#define MG_REG_RO_DEVICE_POWER_CH2					0X02	//	������� ������������ ���������
#define MG_REG_RO_DEVICE_POWER_CH3					0X03	//	������� ������������ ���������
#define MG_REG_RO_DEVICE_POWER_CH4					0X04	//	������� ������������ ���������




#define	MG_REGS_RW_TOTAL 0x04
// ----------------------------------------
#define MG_REG_RW_DEVICE_POWER_CH1					0X80	//	������ ������������ ���������
#define MG_REG_RW_DEVICE_POWER_CH2					0X81	//	������ ������������ ���������
#define MG_REG_RW_DEVICE_POWER_CH3					0X82	//	������ ������������ ���������
#define MG_REG_RW_DEVICE_POWER_CH4					0X83	//	������ ������������ ���������

#endif		/* __MG_REG_NAMES__ */
