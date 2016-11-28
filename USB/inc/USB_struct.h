
#ifndef __USB_STRUCT__

#define __USB_STRUCT__


#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <macros.h>

#include <time_service.h>
#include <stm32f10x_conf.h>
#include <usb_desc.h>
#include <hw_config.h>

/**************************************************************************************************
                                          Õ¿—“–Œ… »
**************************************************************************************************/
#define RX_BUFFER_SIZE		255   // ≈ÏÍÓÒÚ¸ ÔËÂÏÌÓ„Ó ÍÓÎ¸ˆÂ‚Ó„Ó ·ÛÙÂ‡ (3...255 ·‡ÈÚ)


/**************************************************************************************************
                                          “»œ€ ƒ¿ÕÕ€’
**************************************************************************************************/

typedef struct
{
	uint8_t rxBuf[ RX_BUFFER_SIZE ];
	uint8_t rxCounter;
	uint8_t rxTail;
	uint8_t rxHead;
	uint8_t * txBufPtr;
	uint16_t txCounter;
	
} usb_struct;

/**************************************************************************************************
                                   œ–Œ“Œ“»œ€ √ÀŒ¡¿À‹Õ€’ ‘”Õ ÷»…
**************************************************************************************************/

uint8_t GetByteFromUsb( usb_struct * usb );
bool IsNewDataInUsb( usb_struct * usb );
void SendDataToUsb( usb_struct * usb, uint8_t *buffer, uint16_t size );
void CopyDataFromReceiveToUsb( usb_struct * usb );
void CopyDataFromUsbToSend( usb_struct * usb );

/**************************************************************************************************
                                         EXTERN-œ≈–≈Ã≈ÕÕ€≈
**************************************************************************************************/

extern usb_struct Usb1;

extern __IO uint32_t packet_sent;
extern __IO uint8_t Send_Buffer[VIRTUAL_COM_PORT_DATA_SIZE] ;
extern __IO uint8_t Send_length;
extern __IO uint32_t packet_receive;
extern __IO uint8_t Receive_Buffer[64];
extern __IO uint8_t Receive_length ;

#endif	/*	__USB_STRUCT__	*/
