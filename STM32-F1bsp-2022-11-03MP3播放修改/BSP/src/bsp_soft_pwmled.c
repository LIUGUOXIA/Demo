#include "bsp_soft_pwmled.h"
#include "bsp_led.h"
static void  PwmLed_ON();
static void  PwmLed_OFF();

PwmLed_t  PwmLedTable[LED_NUM] = 
{
	100,//hz 10ms
	20,
	0,//10个档位(占空比)   就是1ms进入中断 进10次
	0,//调节占空比
	6000, //1.5s灭到亮，1.5S从亮到灭
	0,
	0,
	PwmLed_ON,
	PwmLed_OFF,
};


void PwmLed_1msMainfunction(void)
{
	
	int i = 0;
	for(i=0;i<LED_NUM;i++)
	{
	
		
	if(PwmLedTable[i].duty_time_cnt++>=(PwmLedTable[i].duty))
	{
		PwmLedTable[i].pwmledoff();
	}
	else
	{
		PwmLedTable[i].pwmledon();
		
	}

	if(PwmLedTable[i].duty_time_cnt>=(PwmLedTable[i].duty_gear))
	{
		PwmLedTable[i].duty_time_cnt = 0;

	}
	else 
	{


	}
	#if 0
	switch(PwmLedTable[i].state)
	{
		
		case 0 :	if(PwmLedTable[i].peroid_cnt++>=(PwmLedTable[i].peroid)/(2*PwmLedTable[i].duty_gear))
			{
				   PwmLedTable[i].peroid_cnt = 0;
				   PwmLedTable[i].duty++;
				if(PwmLedTable[i].duty>PwmLedTable[i].duty_gear)
				{
				//	PwmLedTable[i].duty = 0;
					PwmLedTable[i].state = 1;
					
				}
				
			}break;
			
			
		case 1: if(PwmLedTable[i].peroid_cnt++>=(PwmLedTable[i].peroid)/(2*PwmLedTable[i].duty_gear))
			{
				   PwmLedTable[i].peroid_cnt = 0;
				   PwmLedTable[i].duty--;
				if(PwmLedTable[i].duty<=0)
				{
				//	PwmLedTable[i].duty = 0;
					PwmLedTable[i].state = 0;
					
				}
				
			}break;
			
		
		
		
	}
	#endif 
		
		
		
		
			
	}
	
}









void PwmLed_SetDuty(Pwmlednum_ennum ledpos,uint16_t duty)
{
	
	
	if(PwmLedTable[ledpos].duty++>PwmLedTable[ledpos].duty_gear)
	{
		PwmLedTable[ledpos].duty = 0;
		
	}
	
}









static void PwmLed_ON()
{
	

       LED1_ON;
	
}

static void  PwmLed_OFF()
{
	

        LED1_OFF;
	
}
