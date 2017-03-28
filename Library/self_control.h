#ifndef __SELF_CONTROL__

#define __SELF_CONTROL__

#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <time_service.h>
#include <mg_regs.h>
#include <pwm.h>


void InitSelfControlFsm( void );
void SelfControlFsm( void );
void InitPowerControlPins( void );
void EncoderPowerOn( void );
void EncoderPowerOff( void );
void VideoPowerOn( void );
void VideoPowerOff( void );
void LedOn( void );
void LedOff( void );
bool IsInSleepMode( void );
void InitIwdg( void );

#endif	/*	__SELF_CONTROL__	*/
