#ifndef __MG_REG_NAMES__

#define __MG_REG_NAMES__


#define MG_MIN_REG_RO_ADDR 0x00
#define MG_MAX_REG_RO_ADDR 0x7F
#define MG_MIN_REG_RW_ADDR 0x80
#define MG_MAX_REG_RW_ADDR 0xFE
#define MG_REG_NULL 0xFF


#define MG_REGS_RO_TOTAL 0x03
// ----------------------------------------

#define RO_DEVICE_ID								0x00	//	ID устройства для проверки работоспособности
#define RO_DEVISE_BUTTON_STATE			0x01	//	текущее состояние кнопок переключения каналов
#define RO_DEVICE_POWER							0X02	//	текущая длительность огибающей




#define	MG_REGS_RW_TOTAL 0x02
// ----------------------------------------
#define RW_DEVISE_BUTTON_STATE			0x80	//	запись состояния кнопок переключения каналов
#define RW_DEVICE_POWER							0X81	//	запись длительности огибающей
#endif		/* __MG_REG_NAMES__ */

