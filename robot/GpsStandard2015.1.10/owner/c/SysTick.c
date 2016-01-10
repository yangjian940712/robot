#include "all.h"

static __IO uint32_t TimingDelay;
int clk=0;

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
	TimingDelay_Decrement();
    
    if((clk%40)==0)     //尽量让CAN总线负担平均到各个时间段
    {
        SEND_CONVERS();
    }
    else if(clk%40==10)
	{
		S_GYRO_RAD_FUNC();
	}
    else if(clk%40==20)
	{
		S_ENC1_DIS_FUNC();
	}
    else if(clk%40==30)
	{
		S_ENC2_DIS_FUNC();
	}
    
	Encoder_Update();
	GPS_Update();
    
    CAN1->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;//清空发送FIFO
    
    GPS_TO_MASTER();
    
	if(clk==1)
	{
		LED1_on;
	}
	else if(clk==250)
	{
		LED1_off;
        if(Gyro_Error_Flag==0)
        {
            DMA_check();
        }
	}
	else if(clk>=500)
	{
		clk=0;
	}
	clk++;
//     LCD_SetXY(0,0);
//     LCD_PushString("        ");
//     LCD_WriteIntXY(i,0,0);
}
