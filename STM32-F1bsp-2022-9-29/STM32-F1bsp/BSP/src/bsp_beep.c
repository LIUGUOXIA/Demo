/*
*********************************************************************************************************
*
*	ģ������ : ����������ģ��
*	�ļ����� : bsp_beep.c
*	��    �� : V1.1
*	˵    �� : ����������.
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2014-10-20 armfly  ��ʽ����
*		V1.1    2015-10-06 armfly  ���Ӿ���������������ʱ���η�����������
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#define TASK_CYCLE_TIME     10
#define S2_TIME_COUNT       (2000/TASK_CYCLE_TIME)


static void Beep_Detect_Work(void);
static void Beep_Start_Work(void);
static void Beep_Doing_Work(void);
static void Beep_Warning_Work(void);
static void Beep_End_Work(void);
static uint8_t s_ucBeepData[BEEP_BYTE_MAX_NUM] = {0};


const Beep_HdlCfg_Struct c_stBeepHdlCfgArray[BEEP_MODE_MAX_NUM] = 
{
	{BEEP_BYTE_0, BEEP_BIT_0, Beep_Detect_Work},
	{BEEP_BYTE_0, BEEP_BIT_1, Beep_Start_Work},
	{BEEP_BYTE_0, BEEP_BIT_2, Beep_Doing_Work},
	{BEEP_BYTE_0, BEEP_BIT_3, Beep_Warning_Work},
	{BEEP_BYTE_0, BEEP_BIT_4, Beep_End_Work},
	
};


BEEP_T g_tBeep;		/* ���������ȫ�ֽṹ����� */


/*
*********************************************************************************************************
*	�� �� ��: BEEP_InitHard
*	����˵��: ��ʼ��������Ӳ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitBeepHard(void)
{
#ifdef	BEEP_HAVE_POWER		/* ��Դ������ */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOF��ʱ�� */
	RCC_APB2PeriphClockCmd(GPIO_RCC_BEEP, ENABLE);

	BEEP_DISABLE();

//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BEEP;
	GPIO_Init(GPIO_PORT_BEEP, &GPIO_InitStructure);
#endif
	
	g_tBeep.ucMute = 0;	/* �رվ��� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Start
*	����˵��: ������������
*	��    ��: _usBeepTime : ����ʱ�䣬��λ10ms; 0 ��ʾ������
*			  _usStopTime : ֹͣʱ�䣬��λ10ms; 0 ��ʾ��������
*			  _usCycle : ���д����� 0 ��ʾ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_Start(uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle)
{
	if (_usBeepTime == 0 || g_tBeep.ucMute == 1)
	{
		return;
	}

	g_tBeep.usBeepTime = _usBeepTime;
	g_tBeep.usStopTime = _usStopTime;
	g_tBeep.usCycle = _usCycle;
	g_tBeep.usCount = 0;
	g_tBeep.usCycleCount = 0;
	g_tBeep.ucState = 0;
	g_tBeep.ucEnalbe = 1;	/* ������ȫ�ֲ�������ʹ�ܷ�����־ */

	BEEP_ENABLE();			/* ��ʼ���� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Stop
*	����˵��: ֹͣ��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_Stop(void)
{
	g_tBeep.ucEnalbe = 0;

	if ((g_tBeep.usStopTime == 0) || (g_tBeep.usCycle == 0))
	{
		BEEP_DISABLE();	/* ����������Ʊ�־����ֹͣ����������ֹͣ�����ж����ֿ��� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Pause
*	����˵��: ����TIM��ͻ��ԭ����ʱ���η�������ͨ�� BEEP_Resume �ָ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_Pause(void)
{
	BEEP_Stop();
	
	g_tBeep.ucMute = 1;		/* ���� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Resume
*	����˵��: �ָ���������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_Resume(void)
{
	BEEP_Stop();
	
	g_tBeep.ucMute = 0;		/* ������� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_KeyTone
*	����˵��: ���Ͱ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_KeyTone(void)
{
	BEEP_Start(5, 1, 1);	/* ����50ms��ͣ10ms�� 1�� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Pro
*	����˵��: ÿ��10ms����1�θú��������ڿ��Ʒ������������ú����� bsp_timer.c �б����á�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_Pro(void)
{
	if ((g_tBeep.ucEnalbe == 0) || (g_tBeep.usStopTime == 0) || (g_tBeep.ucMute == 1))
	{
		return;
	}

	if (g_tBeep.ucState == 0)
	{
		if (g_tBeep.usStopTime > 0)	/* ��Ϸ��� */
		{
			if (++g_tBeep.usCount >= g_tBeep.usBeepTime)
			{
				BEEP_DISABLE();		/* ֹͣ���� */
				g_tBeep.usCount = 0;
				g_tBeep.ucState = 1;
			}
		}
		else
		{
			;	/* �����κδ����������� */
		}
	}
	else if (g_tBeep.ucState == 1)
	{
		if (++g_tBeep.usCount >= g_tBeep.usStopTime)
		{
			/* ��������ʱ��ֱ������stopֹͣΪֹ */
			if (g_tBeep.usCycle > 0)
			{
				if (++g_tBeep.usCycleCount >= g_tBeep.usCycle)
				{
					/* ѭ����������ֹͣ���� */
					g_tBeep.ucEnalbe = 0;
				}

				if (g_tBeep.ucEnalbe == 0)
				{
					g_tBeep.usStopTime = 0;
					return;
				}
			}

			g_tBeep.usCount = 0;
			g_tBeep.ucState = 0;

			BEEP_ENABLE();			/* ��ʼ���� */
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
static void Beep_Detect_Work(void)
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
static void Beep_Start_Work(void)
{
//		GPIO_SetBits(GPIO_PORT_BEEP,GPIO_PIN_BEEP);
//		BEEP_Pro();
//		BEEP_ENABLE();
//
	 BEEP_Start(5, 1, 1);	/* ����50ms��ͣ10ms�� 1�� */
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
static void Beep_Doing_Work(void)
{
		uint16_t ucCount = 0;
	  GPIO_SetBits(GPIO_PORT_BEEP,GPIO_PIN_BEEP);
	if(ucCount++ > S2_TIME_COUNT)
	{
	    ucCount = 0;
		  GPIO_ResetBits(GPIO_PORT_BEEP,GPIO_PIN_BEEP);
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
static void Beep_Warning_Work(void)
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
static void Beep_End_Work(void)
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
void Beep_SetStatus(Beep_Mode_Enum Beep_Mode, uint8_t status)
{
		Beep_Mode_Enum Beep_ModeVal;
		uint8_t ByteIDx, BitOffset;
		for(Beep_ModeVal = 0u; Beep_ModeVal < BEEP_MODE_MAX_NUM; Beep_ModeVal++)
		{
				if(Beep_Mode == Beep_ModeVal)
				{
						ByteIDx = c_stBeepHdlCfgArray[Beep_ModeVal].ucByteIDx;
						BitOffset = c_stBeepHdlCfgArray[Beep_ModeVal].ucBitOffset;
						if(status == 1)
						{
								LIB_SET_UINT8_BIT(s_ucBeepData[ByteIDx], BitOffset);  
						}
						else
						{
								LIB_RESET_UINT8_BIT(s_ucBeepData[ByteIDx], BitOffset); 
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
void Beep_10msMainFunction(void)
{
		Beep_Mode_Enum Beep_ModeVal;
		uint8_t ByteIDx, BitOffset;
	  for(Beep_ModeVal = 0u; Beep_ModeVal < BEEP_MODE_MAX_NUM; Beep_ModeVal++)
		{
				ByteIDx = c_stBeepHdlCfgArray[Beep_ModeVal].ucByteIDx;
			  BitOffset = c_stBeepHdlCfgArray[Beep_ModeVal].ucBitOffset;
				if(1 == LIB_GET_UINT8_BIT(s_ucBeepData[ByteIDx], BitOffset))
				{
						c_stBeepHdlCfgArray[Beep_ModeVal].BeepHandleFun();
					   break;
				}
				else
				{
					
				}
		}
		if(Beep_ModeVal == BEEP_MODE_MAX_NUM)
		{
				GPIO_ResetBits(GPIO_PORT_BEEP,GPIO_PIN_BEEP);
		
		}
}



/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
