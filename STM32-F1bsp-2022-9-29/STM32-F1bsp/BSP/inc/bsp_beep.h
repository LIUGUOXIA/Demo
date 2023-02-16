/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : bsp_beep.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H


#include "stm32f10x.h"




#define BEEP_HAVE_POWER		/* ������б�ʾ��Դ��������ֱ��ͨ��GPIO����, ����PWM */

#ifdef	BEEP_HAVE_POWER		/* ��Դ������ */

	/* �Ե�V2-PC0 */
	#define GPIO_RCC_BEEP   RCC_APB2Periph_GPIOC
	#define GPIO_PORT_BEEP	GPIOC
	#define GPIO_PIN_BEEP	  GPIO_Pin_0

	#define BEEP_ENABLE()	  GPIO_PORT_BEEP->BSRR = GPIO_PIN_BEEP			/* ʹ�ܷ��������� */
	#define BEEP_DISABLE()	GPIO_PORT_BEEP->BRR = GPIO_PIN_BEEP			/* ��ֹ���������� */
#else		/* ��Դ������ */
	/* PA8 ---> TIM1_CH1 */

	/* 1500��ʾƵ��1.5KHz��5000��ʾ50.00%��ռ�ձ� */
	#define BEEP_ENABLE()	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_8, TIM1, 1, 1500, 5000);

	/* ��ֹ���������� */
	#define BEEP_DISABLE()	bsp_SetTIMOutPWM(GPIOA, GPIO_Pin_8, TIM1, 1, 1500, 0);
#endif


/*
**********************************************************************************************
*
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
	uint8_t ucMute;		/* 1��ʾ���� */	
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


/* ���ⲿ���õĺ������� */
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
