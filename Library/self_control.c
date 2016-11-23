
#include <self_control.h>

//static void sleepSwitcher( void );


enum self_control_states
{
	START_TIMER,
	WAIT_FOR_TIMER,
	EXECUTE
} SelfControlState;



/**************************************************************************************************
��������:  ������������� ��������� �������� ������ ������������
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
void InitSelfControlFsm( void )
{
	SelfControlState = START_TIMER;
}



/**************************************************************************************************
��������:  ���������� ��������� �������� ������ ������������
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
void SelfControlFsm( void )
{	
	if ( MG_IsRegChanged( MG_REG_RW_DEVICE_BUTTON_STATE ))
	{

	}
	if ( MG_IsRegChanged( MG_REG_RW_DEVICE_POWER ))
	{
		Timer3.pwmTargetChannel1 = MG_GetReg( MG_REG_RW_DEVICE_POWER );
	}
}

/**************************************************************************************************
��������:  ������������� ����������� �������
���������: ���
�������:   ���
���������: 
**************************************************************************************************/
void InitIwdg( void )
{
	/*IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	// ��������� �� ����� T (c):
	// LSI = 40000 (��)
	// reload = LSI * T / prescaler
	// ��� 250��: prescaler = 32; reload = 313
	IWDG_SetPrescaler( IWDG_Prescaler_32 );
	IWDG_SetReload( 500 );
    
	IWDG_ReloadCounter();
	IWDG_Enable();*/
}


