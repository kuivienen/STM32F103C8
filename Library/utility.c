#include "utility.h"

void Blink(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init( GPIOB , &GPIO_InitStructure );
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	Delay(1000);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	Delay(1000);
}

void OutputMCO(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// pick one of the clocks to spew
	RCC_MCOConfig(RCC_MCO_SYSCLK); // Put on MCO pin the: System clock selected
}

