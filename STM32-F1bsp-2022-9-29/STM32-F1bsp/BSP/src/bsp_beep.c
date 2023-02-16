/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器驱动模块
*	文件名称 : bsp_beep.c
*	版    本 : V1.1
*	说    明 : 驱动蜂鸣器.
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2014-10-20 armfly  正式发布
*		V1.1    2015-10-06 armfly  增加静音函数。用于临时屏蔽蜂鸣器发声。
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
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


BEEP_T g_tBeep;		/* 定义蜂鸣器全局结构体变量 */


/*
*********************************************************************************************************
*	函 数 名: BEEP_InitHard
*	功能说明: 初始化蜂鸣器硬件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitBeepHard(void)
{
#ifdef	BEEP_HAVE_POWER		/* 有源蜂鸣器 */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIOF的时钟 */
	RCC_APB2PeriphClockCmd(GPIO_RCC_BEEP, ENABLE);

	BEEP_DISABLE();

//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BEEP;
	GPIO_Init(GPIO_PORT_BEEP, &GPIO_InitStructure);
#endif
	
	g_tBeep.ucMute = 0;	/* 关闭静音 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Start
*	功能说明: 启动蜂鸣音。
*	形    参: _usBeepTime : 蜂鸣时间，单位10ms; 0 表示不鸣叫
*			  _usStopTime : 停止时间，单位10ms; 0 表示持续鸣叫
*			  _usCycle : 鸣叫次数， 0 表示持续鸣叫
*	返 回 值: 无
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
	g_tBeep.ucEnalbe = 1;	/* 设置完全局参数后再使能发声标志 */

	BEEP_ENABLE();			/* 开始发声 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Stop
*	功能说明: 停止蜂鸣音。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_Stop(void)
{
	g_tBeep.ucEnalbe = 0;

	if ((g_tBeep.usStopTime == 0) || (g_tBeep.usCycle == 0))
	{
		BEEP_DISABLE();	/* 必须在清控制标志后再停止发声，避免停止后在中断中又开启 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Pause
*	功能说明: 由于TIM冲突等原因，临时屏蔽蜂鸣音。通过 BEEP_Resume 恢复
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_Pause(void)
{
	BEEP_Stop();
	
	g_tBeep.ucMute = 1;		/* 静音 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Resume
*	功能说明: 恢复蜂鸣器正常功能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_Resume(void)
{
	BEEP_Stop();
	
	g_tBeep.ucMute = 0;		/* 解除静音 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_KeyTone
*	功能说明: 发送按键音
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_KeyTone(void)
{
	BEEP_Start(5, 1, 1);	/* 鸣叫50ms，停10ms， 1次 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Pro
*	功能说明: 每隔10ms调用1次该函数，用于控制蜂鸣器发声。该函数在 bsp_timer.c 中被调用。
*	形    参: 无
*	返 回 值: 无
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
		if (g_tBeep.usStopTime > 0)	/* 间断发声 */
		{
			if (++g_tBeep.usCount >= g_tBeep.usBeepTime)
			{
				BEEP_DISABLE();		/* 停止发声 */
				g_tBeep.usCount = 0;
				g_tBeep.ucState = 1;
			}
		}
		else
		{
			;	/* 不做任何处理，连续发声 */
		}
	}
	else if (g_tBeep.ucState == 1)
	{
		if (++g_tBeep.usCount >= g_tBeep.usStopTime)
		{
			/* 连续发声时，直到调用stop停止为止 */
			if (g_tBeep.usCycle > 0)
			{
				if (++g_tBeep.usCycleCount >= g_tBeep.usCycle)
				{
					/* 循环次数到，停止发声 */
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

			BEEP_ENABLE();			/* 开始发声 */
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
static void Beep_Detect_Work(void)
{

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
static void Beep_Start_Work(void)
{
//		GPIO_SetBits(GPIO_PORT_BEEP,GPIO_PIN_BEEP);
//		BEEP_Pro();
//		BEEP_ENABLE();
//
	 BEEP_Start(5, 1, 1);	/* 鸣叫50ms，停10ms， 1次 */
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
static void Beep_Warning_Work(void)
{

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
static void Beep_End_Work(void)
{

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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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



/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
