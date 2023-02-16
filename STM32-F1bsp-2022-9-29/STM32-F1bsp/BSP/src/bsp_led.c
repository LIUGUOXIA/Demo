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
		LED3_ON;
	
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

}

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
*	�� �� ��: bsp_InitLed
*	����˵��: 
*	��    ��:  _no : 
*	�� �� ֵ: 
*********************************************************************************************************
*/


void bsp_InitLed(void)
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







