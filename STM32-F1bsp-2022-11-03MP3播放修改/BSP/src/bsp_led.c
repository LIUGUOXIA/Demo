/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2015-10-11 armfly  正式发布
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
	STM32-V6 开发板的LED指示灯是由74HC574驱动的。不是用CPU的IO直接驱动。
	74HC574的驱动程序为 : bsp_ext_io.c
*/

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
#define TASK_CYCLE_TIME             10
#define MS20_TIME_COUNT             (20/TASK_CYCLE_TIME)
#define MS50_TIME_COUNT             (50/TASK_CYCLE_TIME)
#define MS200_TIME_COUNT            (200/TASK_CYCLE_TIME)
#define MS500_TIME_COUNT            (500/TASK_CYCLE_TIME)
#define S1_TIME_COUNT               (1000/TASK_CYCLE_TIME)
#define S2_TIME_COUNT               (2000/TASK_CYCLE_TIME)
#define S5_TIME_COUNT               (5000/TASK_CYCLE_TIME)


#define DUTY      				10000

#define USE_RED_EN     			1
#define USE_GREEN_EN   			1
#define USE_BLUE_EN    			1


static 	Breath_T Red_Breathing;
static 	Breath_T Green_Breathing; 
static	Breath_T Blue_Breathing;

static void Breath_lampVarInit(void);
static void bsp_redBreath(uint32_t duty);
static void bsp_greenBreath(uint32_t duty);
static void bsp_blueBreath(uint32_t duty);

static void LED_Detect_Work(void);
static void LED_Start_Work(void);
static void LED_Doing_Work(void);
static void LED_End_Work(void);
static void LED_Warning_Work(void);
static uint8_t s_ucLedData[LED_BYTE_MAX_NUM] = {0};
/*
*********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/

const LedHdlCfg_Struct c_stLedHdlCfgArray[LED_MODE_MAX_NUM] = 
{
    {LED_BYTE_IDX0,LED_BIT_0,LED_Detect_Work},
    {LED_BYTE_IDX0,LED_BIT_1,LED_Start_Work},
    {LED_BYTE_IDX0,LED_BIT_2,LED_Doing_Work}, 
    {LED_BYTE_IDX0,LED_BIT_3,LED_End_Work},
    {LED_BYTE_IDX0,LED_BIT_4,LED_Warning_Work}
};


/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void LED_GetDataBuff(uint8_t *buff)
{
	memcpy(buff,s_ucLedData,LED_BYTE_MAX_NUM);
}	



/*
*********************************************************************************************************
*	函 数 名: Led_GPIO_Init
*	功能说明: LED的GPIO初始化
*	形    参:  _no : 
*	返 回 值: 
*********************************************************************************************************
*/

void Led_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
}

/*
**********************************************************************************************
*函 数 名：Breath_lampVarInit
*功能说明：呼吸灯变量初始化
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
static void Breath_lampVarInit(void)
{
	#if USE_RED_EN == 1
		Red_Breathing.Duty_ratio = 0;
		Red_Breathing.flag = 0;
		Red_Breathing.count = 0;
		Red_Breathing.Time = 4000;
	
	  Red_Breathing.PwmOut_Led = bsp_redBreath;
	#endif

		#if USE_GREEN_EN == 1
		Green_Breathing.Duty_ratio = 0;
		Green_Breathing.flag = 0;
		Green_Breathing.count = 0;
		Green_Breathing.Time = 4000;
    Green_Breathing.PwmOut_Led = bsp_greenBreath;
	#endif
	
		#if USE_BLUE_EN == 1
		Blue_Breathing.Duty_ratio = 0;
		Blue_Breathing.flag = 0;
		Blue_Breathing.count = 0;
	
		Blue_Breathing.Time = 4000;
		Blue_Breathing.PwmOut_Led = bsp_blueBreath;
	#endif

}

/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/

//4S 占空比需要从0增加到10000  在10ms任务中dis变换400次，那么每一次dis变换的值是多少25;
static uint8_t Breathing_lamp(Breath_T *pLED)             //此文件其他函数调用呼吸灯的接口
{
    uint8_t ret = 0;
		if(pLED->flag == 0)
		{
			pLED->Duty_ratio += (DUTY/(pLED->Time/TASK_CYCLE_TIME));
	  }
		
		//bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_5, TIM3, 2, 20000, pLED->Duty_ratio);
		if(pLED->Duty_ratio>= DUTY)
		{
			pLED->flag =1;
		}
		if(pLED->flag == 1)
		{	
				pLED->Duty_ratio -= (DUTY/(pLED->Time/TASK_CYCLE_TIME));
		}
	
		//bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_5, TIM3, 2, 20000, pLED->Duty_ratio);
		if(pLED->Duty_ratio == 0)
		{

			pLED->flag =0;
			ret = 1;
		}
		
		pLED->PwmOut_Led(pLED->Duty_ratio);
   return ret;

}


/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
void LED_SetStatus(LED_Mode_Enum Led_Mode,uint8_t status)      //其他模块调用接口
{
	LED_Mode_Enum Led_ModeVal;
	uint8_t ByteIdx,BitOffset;
	for(Led_ModeVal = 0u; Led_ModeVal < LED_MODE_MAX_NUM; Led_ModeVal++)
    {
        if(Led_Mode == Led_ModeVal)
        {
            ByteIdx = c_stLedHdlCfgArray[Led_ModeVal].ucByteIdx;
            BitOffset = c_stLedHdlCfgArray[Led_ModeVal].ucBitOffset;
            if(status == 1)
            {
                LIB_SET_UINT8_BIT(s_ucLedData[ByteIdx], BitOffset);  
            }
            else
            {
                LIB_RESET_UINT8_BIT(s_ucLedData[ByteIdx], BitOffset);  
            }
        }
    }

}

/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
void Led_5msMainFunction(void)           //任务调用接口  一直轮询5ms轮询一次，当轮询收到给外部调用的接口有信号时执行内部处理
{
	uint8_t ByteIdx,BitOffset;
	LED_Mode_Enum Led_ModeVal;
	for(Led_ModeVal = 0u; Led_ModeVal < LED_MODE_MAX_NUM; Led_ModeVal++)
	{
			ByteIdx = c_stLedHdlCfgArray[Led_ModeVal].ucByteIdx;
			BitOffset = c_stLedHdlCfgArray[Led_ModeVal].ucBitOffset;
	 
			if(1 == LIB_GET_UINT8_BIT(s_ucLedData[ByteIdx], BitOffset))
			{
							 c_stLedHdlCfgArray[Led_ModeVal].LedHandleFun();
							 break;
			}
			else
			{

			}
	}
	if(Led_ModeVal == LED_MODE_MAX_NUM)
	{

	LED1_OFF;
    LED2_OFF;
    LED3_OFF;
		
	}
}

/*
**********************************************************************************************
*函 数 名：Breathing_lamp_10msMainFunction
*功能说明：呼吸灯10毫秒测试任务
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
void Breathing_lamp_10msMainFunction(void)      //任务调用接口  一直轮询10ms轮询一次，
{
		Breathing_lamp(&Red_Breathing);
		Breathing_lamp(&Green_Breathing);
		Breathing_lamp(&Blue_Breathing);
		
}


/*
*********************************************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
*********************************************************************************************************
*/
static void LED_Detect_Work(void)
{
	static uint16_t  ucCount = 0;
	LED1_TOGGLE;
	if(ucCount ++ > S1_TIME_COUNT)
	{
	    ucCount = 0;
	    LED1_OFF;
	    LED2_ON;
	}
	
}
/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
static void LED_Start_Work(void)
{
		Breathing_lamp(&Green_Breathing);
	
}
/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
static void LED_Doing_Work(void)
{
	Breathing_lamp(&Blue_Breathing);
}
/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
static void  LED_End_Work(void)
{
	LED3_TOGGLE;
}
/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
static void LED_Warning_Work(void)
{
		Breathing_lamp(&Red_Breathing);
}

/*
**********************************************************************************************
*函 数 名：bsp_redBreath / bsp_greenBreath / bsp_blueBreath
*功能说明：LED定时器占空比
*形    参：uint32_t duty ： 占空比
*返 回 值：
*
**********************************************************************************************
*/

static void bsp_redBreath(uint32_t duty)
{
	bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_5, TIM3, 2, 20000, duty);
}

static void bsp_greenBreath(uint32_t duty)
{
	bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_0, TIM3, 3, 20000, duty);
}

static void bsp_blueBreath(uint32_t duty)
{
	bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_1, TIM3, 4, 20000, duty);
}


/*
**********************************************************************************************
*函 数 名：bsp_InitLed
*功能说明：LED初始化
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
void bsp_InitLed(void)
{		
	Led_GPIO_Init();
	Breath_lampVarInit();
	
}













