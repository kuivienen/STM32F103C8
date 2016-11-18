#pragma once

#define MG_MIN_REG_RO_ADDR 0x00
#define MG_MAX_REG_RO_ADDR 0x7F
#define MG_MIN_REG_RW_ADDR 0x80
#define MG_MAX_REG_RW_ADDR 0xFE
#define MG_REG_NULL 0xFF


#define MG_REGS_RO_TOTAL 0x0F
// ----------------------------------------

#define RO_DEVICE_ID							0x00	//	ID устройства для проверки работоспособности
#define RO_DEVICE_ADDRESS						0x01	//	адрес устройства
#define RO_CPU_TEMP								0x02	//	температура процессора
#define RO_DEVICE_COMMAND						0x03	//	внутреннее состояние устройства, выставляется после выполнения команды, после чтения обнуляется
#define RO_CURRENT_DEVICE_TEMP_C_L				0x04	//	текущая температура пластины в °С*10, младшие значащие разряды
#define RO_CURRENT_DEVICE_TEMP_C_H				0x05	//	текущая температура пластины в °С*10, старшие значащие разряды
#define RO_CURRENT_DEVICE_TEMP_L				0x06	//	текущая температура пластины в мВ, младшие значащие разряды
#define RO_CURRENT_DEVICE_TEMP_H				0x07	//	текущая температура пластины в мВ, старшие значащие разряды
#define RO_DEVICE_TEMP_25_L						0x08	//	температура 25°C в мВ, младшие значащие разряды
#define RO_DEVICE_TEMP_25_H						0x09	//	температура 25°C в мВ, старшие значащие разряды
#define RO_DELTA_TEMP							0x0A	//	мВ/°С
#define RO_TARGET_DEVICE_TEMP_C_L				0x0B	//	заданная температура пластины в °С*10, младшие значащие разряды
#define RO_TARGET_DEVICE_TEMP_C_H				0x0C	//	заданная температура пластины в °С*10, старшие значащие разряды
#define RO_TARGET_DEVICE_TEMP_L					0x0D	//	заданная температура пластины в мВ, младшие значащие разряды
#define RO_TARGET_DEVICE_TEMP_H					0x0E	//	заданная температура пластины в мВ, старшие значащие разряды




#define	MG_REGS_RW_TOTAL 0x07
// ----------------------------------------
#define RW_DEVICE_COMMAND						0x80	//	Регистр команд
#define RW_DEVICE_ADDRESS						0x81	//	назначение нового адреса устройству
#define RW_DEVICE_TEMP_25_L						0x82	//	температура 25°C в мВ, младшие значащие разряды
#define RW_DEVICE_TEMP_25_H						0x83	//	температура 25°C в мВ, старшие значащие разряды
#define RW_DELTA_TEMP							0x84	//	мВ/°С
#define RW_TARGET_DEVICE_TEMP_C_L				0x85	//	заданная температура пластины в °С*10, младшие значащие разряды
#define RW_TARGET_DEVICE_TEMP_C_H				0x86	//	заданная температура пластины в °С*10, старшие значащие разряды



