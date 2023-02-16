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
		RCC_APB2PeriphClockCmd(BEEP_GPIO_RCC, ENABLE);

		BEEP_DISABLE();
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
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
	BEEP_ON;
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
	  GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
	if(ucCount++ > S2_TIME_COUNT)
	{
	    ucCount = 0;
		  GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
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
	BEEP_ON;
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
//		if(Beep_ModeVal == BEEP_MODE_MAX_NUM)
//		{
//				GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
//		}
}



/***************************** ���������� www.armfly.com (END OF FILE) *********************************/



int melody[] = {50, 50, 50, 50, 200, 200, 200, 400, 400, 500, 500, 500};

void Sound(u16 frq)
{
	u32 time;
	if(frq != 1000)
	{
		time = 500000/((u32)frq);
		BEEP_ON; 
		bsp_DelayUS(time);
		BEEP_OFF;
		bsp_DelayUS(time);
//		PBeep = 1;
//		delay_us(time);
//		PBeep = 0;
//		delay_us(time);

	}else
//		delay_us(1000);
	bsp_DelayUS(1000);
}
void Sound2(u16 time)
{
		BEEP_ON; 
		bsp_DelayUS(time);
	  BEEP_OFF;
	  bsp_DelayUS(time);
//    PBeep = 1;
//    delay_ms(time);
//    PBeep = 0;
//    delay_ms(time);
}
void play_successful(void)
{
    int id=0;
    for(id = 0 ;id < 12 ;id++)
    {
        Sound2(melody[id]);
    }
}
void play_failed(void)
{
    int id=0;
    for(id = 11 ;id >=0 ;id--)
    {
        Sound2(melody[id]);
    }
}
void play_music(void)
{
	//              ��7  1   2   3   4   5   6   7  ��1 ��2 ��3 ��4 ��5 ������
	uc16 tone[] = {247,262,294,330,349,392,440,494,523,587,659,698,784,1000};//��Ƶ���ݱ�
	//�쳾���
	u8 music[]={5,5,6,8,7,6,5,6,13,13,//����
                5,5,6,8,7,6,5,3,13,13,
                2,2,3,5,3,5,6,3,2,1,
                6,6,5,6,5,3,6,5,13,13,

                5,5,6,8,7,6,5,6,13,13,
                5,5,6,8,7,6,5,3,13,13,
                2,2,3,5,3,5,6,3,2,1,
                6,6,5,6,5,3,6,1,	

                13,8,9,10,10,9,8,10,9,8,6,
                13,6,8,9,9,8,6,9,8,6,5,
                13,2,3,5,5,3,5,5,6,8,7,6,
                6,10,9,9,8,6,5,6,8};	
	
	u8 time[] = {2,4,2,2,2,2,2,8,4,4, //ʱ��
                2,4,2,2,2,2,2,8,4,4, 
                2,4,2,4,2,2,4,2,2,8,
                2,4,2,2,2,2,2,8,4,4, 

                2,4,2,2,2,2,2,8,4,4, 
                2,4,2,2,2,2,2,8,4,4, 
                2,4,2,4,2,2,4,2,2,8,
                2,4,2,2,2,2,2,8,

                4,2,2,2,4,2,2,2,2,2,8,
                4,2,2,2,4,2,2,2,2,2,8,
                4,2,2,2,4,2,2,5,2,6,2,4,
                2,2,2,4,2,4,2,2,12};	
	u32 yanshi;
	u16 i,e;
	yanshi = 10;
	for(i=0;i<sizeof(music)/sizeof(music[0]);i++)
	{
			for(e=0;e<((u16)time[i])*tone[music[i]]/yanshi;e++)
		  {
					Sound((u32)tone[music[i]]);
			}	
	}
}




