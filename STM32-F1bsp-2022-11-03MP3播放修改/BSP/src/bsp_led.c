/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2015-10-11 armfly  ��ʽ����
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/*
	STM32-V6 �������LEDָʾ������74HC574�����ġ�������CPU��IOֱ��������
	74HC574����������Ϊ : bsp_ext_io.c
*/

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void LED_GetDataBuff(uint8_t *buff)
{
	memcpy(buff,s_ucLedData,LED_BYTE_MAX_NUM);
}	



/*
*********************************************************************************************************
*	�� �� ��: Led_GPIO_Init
*	����˵��: LED��GPIO��ʼ��
*	��    ��:  _no : 
*	�� �� ֵ: 
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
*�� �� ����Breath_lampVarInit
*����˵���������Ʊ�����ʼ��
*��    �Σ�
*�� �� ֵ��
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
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/

//4S ռ�ձ���Ҫ��0���ӵ�10000  ��10ms������dis�任400�Σ���ôÿһ��dis�任��ֵ�Ƕ���25;
static uint8_t Breathing_lamp(Breath_T *pLED)             //���ļ������������ú����ƵĽӿ�
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
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
void LED_SetStatus(LED_Mode_Enum Led_Mode,uint8_t status)      //����ģ����ýӿ�
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
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
void Led_5msMainFunction(void)           //������ýӿ�  һֱ��ѯ5ms��ѯһ�Σ�����ѯ�յ����ⲿ���õĽӿ����ź�ʱִ���ڲ�����
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
*�� �� ����Breathing_lamp_10msMainFunction
*����˵����������10�����������
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
void Breathing_lamp_10msMainFunction(void)      //������ýӿ�  һֱ��ѯ10ms��ѯһ�Σ�
{
		Breathing_lamp(&Red_Breathing);
		Breathing_lamp(&Green_Breathing);
		Breathing_lamp(&Blue_Breathing);
		
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
static void LED_Start_Work(void)
{
		Breathing_lamp(&Green_Breathing);
	
}
/*
**********************************************************************************************
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
static void LED_Doing_Work(void)
{
	Breathing_lamp(&Blue_Breathing);
}
/*
**********************************************************************************************
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
static void  LED_End_Work(void)
{
	LED3_TOGGLE;
}
/*
**********************************************************************************************
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
static void LED_Warning_Work(void)
{
		Breathing_lamp(&Red_Breathing);
}

/*
**********************************************************************************************
*�� �� ����bsp_redBreath / bsp_greenBreath / bsp_blueBreath
*����˵����LED��ʱ��ռ�ձ�
*��    �Σ�uint32_t duty �� ռ�ձ�
*�� �� ֵ��
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
*�� �� ����bsp_InitLed
*����˵����LED��ʼ��
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
void bsp_InitLed(void)
{		
	Led_GPIO_Init();
	Breath_lampVarInit();
	
}













