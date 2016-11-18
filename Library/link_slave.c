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
static uart_struct * LinkUart;

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
void InitLinkSlave(uart_struct * uart)
{
	LinkUart = uart;
//	LinkUart->baud = XBEE_BAUDRATE;
	LinkUart->txCallback = onMsgTransmited;
	InitUart(LinkUart);
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
				while (IsNewDataInUart(LinkUart) == true)
				{
					if (MG_ComposeMsg(GetByteFromUart(LinkUart), &Request) == RET_DONE)
					{
	//					Led1On();
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
					SendDataToUart(LinkUart, MG_GetOutBufPtr(&Answer), MG_GetOutBufSize(&Answer));
					MsgTransmited = false;
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
��������:  ���������� �� ������ UART ��� ���������� �������� ���������
���������: ���
�������:   ���
���������:
**************************************************************************************************/
static void onMsgTransmited(void)
{
	MsgTransmited = true;
}



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



/**************************************************************************************************
��������:  ������������� ������ DE
���������: ���
�������:   ���
���������:
**************************************************************************************************/

/*static void initGpio(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = PIN_PRI_RS485_DE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(PORT_PRI_RS485_DE, &GPIO_InitStructure);  
}*/




/**************************************************************************************************
��������:  ���������� �������� �� ����� RS-485
���������: ���
�������:   ���
���������:
**************************************************************************************************/
/*static void enableTransmit(void)
{	
	if (LinkUart == &PRI_RS485_UART)
	{	
		GPIO_SetBits(PORT_PRI_RS485_DE, PIN_PRI_RS485_DE);  
	}
}*/



/**************************************************************************************************
��������:  ���������� �������� �� ����� RS-485
���������: ���
�������:   ���
���������:
**************************************************************************************************/
/*static void disableTransmit(void)
{
	if (LinkUart == &PRI_RS485_UART)
	{	
		GPIO_ResetBits(PORT_PRI_RS485_DE, PIN_PRI_RS485_DE);  
	}
}*/
