/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
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

///* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态

/* 定义控制IO的宏 */
#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)           //高电平熄灭
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)          //低电平点亮

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)              //高电平熄灭
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)            //低电平点亮

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)          //高电平熄灭
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)        //低电平点亮



/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

//红
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//绿
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//蓝
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//黄(红+绿)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//紫(红+蓝)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//青(绿+蓝)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//白(红+绿+蓝)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//黑(全部关闭)
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



/* 供外部调用的函数声明 */
void bsp_InitLed(void);
void Led_5msMainFunction(void);
void LED_SetStatus(LED_Mode_Enum Led_Mode,uint8_t status);    //其他模块调用接口

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
