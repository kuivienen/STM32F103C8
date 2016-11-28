/**************************************************************************************************
 LINK_SLAVE.C

 ������ �����-����� �� ���������� UART
 
**************************************************************************************************/

#include <link_slave.h>



/**************************************************************************************************
                                           ���������
**************************************************************************************************/
//#warning Setup connection delay for debug
#define MAX_CONNECTION_DELAY 750
#define DEVICE_ADDRESS 0x01



/**************************************************************************************************
                                    ��������� ��������� �������
**************************************************************************************************/

static void onMsgTransmited(void);
static void lostLinkHandler(void);
//static void initGpio(void);
//static void enableTransmit(void);
//static void disableTransmit(void);



/**************************************************************************************************
                                       ���������� ����������
**************************************************************************************************/

static bool MsgTransmited;
static usb_struct * LinkUsb;

static mg_output Answer;
static mg_input Request;

static enum states
{
	COMPOSE_REQUEST,
	PROCESS_REQUEST,
	SEND_ANSWER,
	WAIT_FOR_TRANSMIT
} State = COMPOSE_REQUEST;



/**************************************************************************************************
                                        ���������� �������
**************************************************************************************************/

/**************************************************************************************************
��������:  ������������� ������ ������-�����
���������: ��������� �� ��������� UART
�������:   ���
���������:
**************************************************************************************************/
void InitLinkSlave(usb_struct * usb)
{
	LinkUsb = usb;
//	LinkUsb->baud = XBEE_BAUDRATE;
//	LinkUsb->txCallback = onMsgTransmited;
//	InitUsb(LinkUsb);
	MsgTransmited = false;
	
	MG_InitInputStruct(&Request, G_REQ);
	MG_InitOutputStruct(&Answer);
	MG_SetInputDevAdr(&Request, DEVICE_ADDRESS);
	MG_SetOutputDevAdr(&Answer, DEVICE_ADDRESS);

//	initGpio();
//	disableTransmit();
}



/**************************************************************************************************
��������:  ����� ������ ������-����� � ��������� ���������
���������: ���
�������:   ���
���������:
**************************************************************************************************/
void ResetLinkSlave(void)
{
	State = COMPOSE_REQUEST;
}



/**************************************************************************************************
��������:  ���������� ��������� �������� ������ ������-�����
���������: ���
�������:   ���
���������:
**************************************************************************************************/
void ProcessLinkSlave(void)
{
	static soft_timer lostLinkTimer;

	if (CheckSoftTimer(&lostLinkTimer) == true )
	{
		lostLinkHandler();
		StartSoftTimer(&lostLinkTimer, MAX_CONNECTION_DELAY);
	}

	switch (State)
	{
		//-----------------------------------------------------------
		case COMPOSE_REQUEST:
			{
	//			Led1Off();
				while (IsNewDataInUsb(LinkUsb) == true)
				{
//					CDC_Send_DATA((unsigned char*)Receive_Buffer, Receive_length);
					if (MG_ComposeMsg(GetByteFromUsb(LinkUsb), &Request) == RET_DONE)
					{
						StartSoftTimer(&lostLinkTimer, MAX_CONNECTION_DELAY);
						State = PROCESS_REQUEST;
					}
				}
				break;
			}
		//-----------------------------------------------------------
		case PROCESS_REQUEST:
			{
				MG_ProcessRequest(&Request, &Answer);
				State = SEND_ANSWER;
				break;
			}
		//-----------------------------------------------------------
		case SEND_ANSWER:
			{
				if (MG_IsAnsNeeded(&Request))
				{
					MG_WrapMsg(&Answer);
	//				enableTransmit();
					SendDataToUsb(LinkUsb, MG_GetOutBufPtr(&Answer), MG_GetOutBufSize(&Answer));
					CopyDataFromUsbToSend( &Usb1 );
					MsgTransmited = false;
					MsgTransmited = true;
					State = WAIT_FOR_TRANSMIT;
				}
				else
				{
					State = COMPOSE_REQUEST;
				}
				break;
			}
		//-----------------------------------------------------------
		case WAIT_FOR_TRANSMIT:
			{
				if (MsgTransmited == true)
				{
	//				disableTransmit();
					State = COMPOSE_REQUEST;
				}
				break;
			}
		//-----------------------------------------------------------
		default:
			{
				FAIL_ASSERT();
			}
	}
}



/**************************************************************************************************
                                       ��������� �������
**************************************************************************************************/





/**************************************************************************************************
��������:  ���������� ������ �����
���������: ���
�������:   ���
���������:
**************************************************************************************************/
static void lostLinkHandler(void)
{
//	#warning Lost connection handler here
	
/*	MG_SetReg(MG_REG_RW_SLAVE_LEFT_TRACK_SPEED, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_RIGHT_TRACK_SPEED, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_MOT_LINK1_SPEED, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_MOT_LINK2_SPEED, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_LIFTER_LINK1_SPEED, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_LIFTER_LINK2_SPEED, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_CAMOUFLAGE_STATE, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_TVK_BUTTON, 0x00);
	MG_SetReg(MG_REG_RW_SLAVE_LIFTER_LINK2_GOTO_INIT, 0x00);*/
}

