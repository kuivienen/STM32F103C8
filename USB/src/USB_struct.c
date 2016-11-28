
#include <USB_struct.h>

/**************************************************************************************************
                                    ��������� ��������� �������
**************************************************************************************************/

__IO uint8_t Receive_Buffer[64];
__IO uint8_t Receive_length;
__IO uint8_t Send_Buffer[VIRTUAL_COM_PORT_DATA_SIZE] ;
__IO uint8_t Send_length;


/**************************************************************************************************
                                       ���������� ����������
**************************************************************************************************/

usb_struct Usb1 =
{
	.rxCounter	= 0,
	.rxTail			=	0,
	.rxHead			=	0,
	.txCounter	= 0,
};

/**************************************************************************************************
                                        ���������� �������
**************************************************************************************************/

/**************************************************************************************************
��������:  ���������� ��������� ���� �� ������ ��������� UART
���������: ��������� �� ��������� UART
�������:   ���� ������
���������: �������� ������� �������� ���� 
**************************************************************************************************/
uint8_t GetByteFromUsb( usb_struct * usb )
{
	uint8_t dataByte;

	dataByte = usb->rxBuf[ usb->rxTail ]; 
	usb->rxTail++;
	usb->rxTail %= RX_BUFFER_SIZE;
		
	ENTER_CRITICAL_SECTION();
	
		usb->rxCounter--;
		
	LEAVE_CRITICAL_SECTION();	
		
	return dataByte; 	
}



/**************************************************************************************************
��������:  ���������� ��������� ������ ���������
���������: ��������� �� ��������� UART
�������:   true, ���� ���� �������������� �����
���������: 
**************************************************************************************************/
bool IsNewDataInUsb( usb_struct * usb )
{
	if ( usb->rxCounter )
	{
		return true;
	}
	else
	{
		return false;
	}
}



/**************************************************************************************************
��������:  ������������� �������� �������� ��������� �� UART
���������: ��������� �� ��������� UART, ��������� �� ������, �����
�������:   ���
���������: 
**************************************************************************************************/
void SendDataToUsb( usb_struct * usb, uint8_t *buffer, uint16_t size )
{
	usb->txBufPtr = buffer;
	usb->txCounter = size;
}

void CopyDataFromReceiveToUsb( usb_struct * usb )
{
	if( Receive_length > 0 )
	{
 		if ( usb->rxCounter < RX_BUFFER_SIZE  )
 		{ 
			for (uint8_t i = 0; i < Receive_length; ++i)
			{
				usb->rxBuf[ usb->rxHead ] = Receive_Buffer[i];
				usb->rxHead++;
				usb->rxHead %= RX_BUFFER_SIZE;
				usb->rxCounter++;
			}
 		}
 		else
 		{
			usb->rxCounter = 0;
			usb->rxTail = 0;
			usb->rxHead = 0;
 		}
		Receive_length = 0;
	}
}

void CopyDataFromUsbToSend( usb_struct * usb )
{
 	while ( usb->txCounter )	
 	{
		usb->txCounter--;
		Send_Buffer[Send_length] = *(usb->txBufPtr++);
		Send_length++;
 	}
}

/**************************************************************************************************
                                       ��������� �������
**************************************************************************************************/


