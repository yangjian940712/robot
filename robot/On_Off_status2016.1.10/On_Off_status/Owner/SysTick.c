#include "all.h"

static __IO uint32_t TimingDelay;

void SysTick_init(void)
{
	if (SysTick_Config(SystemCoreClock / 500))   //Setup SysTick Timer for 1 ms interrupts
	{ 
		/* Capture error */ 
		while (1);
	}
}

void delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	
	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  static int i;
	TimingDelay_Decrement();
// 	SEND_CONVERS();
// 	Encoder_Update();
// 	GPS_Update();
// 	if(i%20==0)
// 	{
// 		S_GYRO_RAD_FUNC();
// 		S_ENC1_DIS_FUNC();
// // S_ENC2_DIS_FUNC();
// 	}
	if(i==1)
	{
		LED1_on;
	}
	else if(i==250)
	{
		LED1_off;
// 	DMA_check();
	}
	else if(i>=500)
	{
		i=0;
//		S_SWITCH_FUNC();
	}
	i++;
// 	GPS_TO_MASTER();
}
