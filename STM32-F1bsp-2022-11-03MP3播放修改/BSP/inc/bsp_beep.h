/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器模块
*	文件名称 : bsp_beep.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H


#include "stm32f10x.h"


#define BEEP_HAVE_POWER		/* 定义此行表示有源蜂鸣器，直接通过GPIO驱动, 无需PWM */

#ifdef	BEEP_HAVE_POWER		/* 有源蜂鸣器 */

	/* 霸道V2-PC0 */

	#define BEEP_GPIO_PORT	GPIOC
	#define BEEP_GPIO_PIN	  GPIO_Pin_0
	#define BEEP_GPIO_RCC   RCC_APB2Periph_GPIOC

	#define BEEP_ENABLE()	  BEEP_GPIO_PORT->BSRR = BEEP_GPIO_PIN			/* 使能蜂鸣器鸣叫 */
	#define BEEP_DISABLE()	BEEP_GPIO_PORT->BRR = BEEP_GPIO_PIN			/* 禁止蜂鸣器鸣叫 */
#else		/* 无源蜂鸣器 */
	/* PA8 ---> TIM1_CH1 */

	/* 1500表示频率1.5KHz，5000表示50.00%的占空比 */
	#define BEEP_ENABLE()	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_8, TIM1, 1, 1500, 5000);

	/* 禁止蜂鸣器鸣叫 */
	#define BEEP_DISABLE()	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_8, TIM1, 1, 1500, 0);
#endif


#define BEEP_ON         GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_OFF				GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)

/*
**********************************************************************************************
*
*
*
*
*
**********************************************************************************************
*/
typedef struct _BEEP_T
{
	uint8_t ucEnalbe;
	uint8_t ucState;
	uint16_t usBeepTime;
	uint16_t usStopTime;
	uint16_t usCycle;
	uint16_t usCount;
	uint16_t usCycleCount;
	uint8_t ucMute;		/* 1表示静音 */	
}BEEP_T;


/************************************************************************************************/
typedef enum
{
		BEEP_BYTE_0 = 0u,
		BEEP_BYTE_1,
	  BEEP_BYTE_2,
	  BEEP_BYTE_3,
	  BEEP_BYTE_4,
	  BEEP_BYTE_5,
		BEEP_BYTE_MAX_NUM
}Beep_ByteID_Enum;


typedef enum
{
		BEEP_BIT_0 = 0u,
	  BEEP_BIT_1,
		BEEP_BIT_2,
		BEEP_BIT_3,
		BEEP_BIT_4,
		BEEP_BIT_5,
		BEEP_BIT_6,
		BEEP_BIT_7
}Beep_BitOffset_Enum;

typedef enum
{
		BEEP_DETECT_WORK = 0,
		BEEP_START_WORK,
		BEEP_DOING_WORK,
		BEEP_WARNING_WORK,
		BEEP_END_WORK,
		BEEP_MODE_MAX_NUM
}Beep_Mode_Enum;

typedef void (*pBeepHdlFun)();

typedef struct
{
		Beep_ByteID_Enum ucByteIDx;
		Beep_BitOffset_Enum ucBitOffset;
		pBeepHdlFun BeepHandleFun;
}Beep_HdlCfg_Struct;


/************************************************************************************************/


/* 供外部调用的函数声明 */
void bsp_InitBeepHard(void);
void BEEP_Start(uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle);
void BEEP_Stop(void);
void BEEP_KeyTone(void);
void BEEP_Pro(void);

void BEEP_Pause(void);
void BEEP_Resume(void);

void Beep_SetStatus(Beep_Mode_Enum Beep_Mode, uint8_t status);
void Beep_10msMainFunction(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/


#include "stdlib.h"	  

//定义GPIOB的位地址变量宏，位输入宏，输出宏
#define PBeep PBout(5)

void Sound(u16 frq);
void Sound2(u16 time);
void play_music(void);
void play_successful(void);
void play_failed(void);






