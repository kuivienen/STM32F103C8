#pragma once

#define MG_MIN_REG_RO_ADDR 0x00
#define MG_MAX_REG_RO_ADDR 0x7F
#define MG_MIN_REG_RW_ADDR 0x80
#define MG_MAX_REG_RW_ADDR 0xFE
#define MG_REG_NULL 0xFF


#define MG_REGS_RO_TOTAL 0x0F
// ----------------------------------------

#define RO_DEVICE_ID							0x00	//	ID ���������� ��� �������� �����������������
#define RO_DEVICE_ADDRESS						0x01	//	����� ����������
#define RO_CPU_TEMP								0x02	//	����������� ����������
#define RO_DEVICE_COMMAND						0x03	//	���������� ��������� ����������, ������������ ����� ���������� �������, ����� ������ ����������
#define RO_CURRENT_DEVICE_TEMP_C_L				0x04	//	������� ����������� �������� � ��*10, ������� �������� �������
#define RO_CURRENT_DEVICE_TEMP_C_H				0x05	//	������� ����������� �������� � ��*10, ������� �������� �������
#define RO_CURRENT_DEVICE_TEMP_L				0x06	//	������� ����������� �������� � ��, ������� �������� �������
#define RO_CURRENT_DEVICE_TEMP_H				0x07	//	������� ����������� �������� � ��, ������� �������� �������
#define RO_DEVICE_TEMP_25_L						0x08	//	����������� 25�C � ��, ������� �������� �������
#define RO_DEVICE_TEMP_25_H						0x09	//	����������� 25�C � ��, ������� �������� �������
#define RO_DELTA_TEMP							0x0A	//	��/��
#define RO_TARGET_DEVICE_TEMP_C_L				0x0B	//	�������� ����������� �������� � ��*10, ������� �������� �������
#define RO_TARGET_DEVICE_TEMP_C_H				0x0C	//	�������� ����������� �������� � ��*10, ������� �������� �������
#define RO_TARGET_DEVICE_TEMP_L					0x0D	//	�������� ����������� �������� � ��, ������� �������� �������
#define RO_TARGET_DEVICE_TEMP_H					0x0E	//	�������� ����������� �������� � ��, ������� �������� �������




#define	MG_REGS_RW_TOTAL 0x07
// ----------------------------------------
#define RW_DEVICE_COMMAND						0x80	//	������� ������
#define RW_DEVICE_ADDRESS						0x81	//	���������� ������ ������ ����������
#define RW_DEVICE_TEMP_25_L						0x82	//	����������� 25�C � ��, ������� �������� �������
#define RW_DEVICE_TEMP_25_H						0x83	//	����������� 25�C � ��, ������� �������� �������
#define RW_DELTA_TEMP							0x84	//	��/��
#define RW_TARGET_DEVICE_TEMP_C_L				0x85	//	�������� ����������� �������� � ��*10, ������� �������� �������
#define RW_TARGET_DEVICE_TEMP_C_H				0x86	//	�������� ����������� �������� � ��*10, ������� �������� �������



