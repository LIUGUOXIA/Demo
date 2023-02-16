/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "stm32f10x.h"

#ifndef __BSP_LED_H
#define __BSP_LED_H


#define LED1_GPIO_PORT					GPIOB
#define LED2_GPIO_PORT					GPIOB
#define LED3_GPIO_PORT					GPIOB

#define	LED1_GPIO_PIN           GPIO_Pin_5
#define	LED2_GPIO_PIN           GPIO_Pin_0
#define	LED3_GPIO_PIN           GPIO_Pin_1

///* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬

/* �������IO�ĺ� */
#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)           //�ߵ�ƽϨ��
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)          //�͵�ƽ����

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)              //�ߵ�ƽϨ��
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)            //�͵�ƽ����

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)          //�ߵ�ƽϨ��
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)        //�͵�ƽ����



/* ������ɫ������߼��÷�ʹ��PWM�ɻ��ȫ����ɫ,��Ч������ */

//��
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//��
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//��
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//��(��+��)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//��(��+��)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//��(��+��)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//��(��+��+��)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//��(ȫ���ر�)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF



typedef enum
{
	LED_DETECT_WORK = 0,
	LED_START_WORK,
	LED_DOING_WORK,
	LED_END_WORK,
	LED_WARNING_WORK,
	LED_MODE_MAX_NUM
}LED_Mode_Enum;


typedef enum
{
    LED_BYTE_IDX0 = 0u,
    LED_BYTE_IDX1,
    LED_BYTE_IDX2,
    LED_BYTE_IDX3,
    LED_BYTE_IDX4,
    LED_BYTE_MAX_NUM
} LedByteIdx_Enum;


typedef enum
{
    LED_BIT_0 = 0u,
    LED_BIT_1,
    LED_BIT_2,
    LED_BIT_3,
    LED_BIT_4,
    LED_BIT_5,
    LED_BIT_6,
    LED_BIT_7
} LedBitOffset_Enum;



typedef void(*pLedHdlFun)();

typedef struct
{
//		LED_Mode_Enum LedMode_ID;
		LedByteIdx_Enum ucByteIdx;
		LedBitOffset_Enum ucBitOffset;
		pLedHdlFun LedHandleFun;
} LedHdlCfg_Struct;



/* ���ⲿ���õĺ������� */
void bsp_InitLed(void);
void Led_5msMainFunction(void);
void LED_SetStatus(LED_Mode_Enum Led_Mode,uint8_t status);    //����ģ����ýӿ�

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
