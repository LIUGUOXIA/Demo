#ifndef __IRDA_H
#define	__IRDA_H


#include "stm32f10x.h"

//红外遥控使用的GPIO及时钟
#define IRDA_GPIO_PORT								GPIOD
#define IRDA_GPIO_PIN									GPIO_Pin_7
#define IRDA_GPIO_CLK									RCC_APB2Periph_GPIOD
#define IRDA_GPIO_PORT_SOURCE					GPIO_PortSourceGPIOD
#define IRDA_GPIO_PIN_SOURCE					GPIO_PinSource7

//中断相关
#define IRDA_EXTI_LINE								EXTI_Line7
#define IRDA_EXTI_IRQN								EXTI9_5_IRQn
#define IRDA_EXTI_IRQHANDLER_FUN			EXTI9_5_IRQHandler

 //读取引脚的电平
#define IRDA_DATA_INPUT()							 GPIO_ReadInputDataBit(IRDA_GPIO_PORT, IRDA_GPIO_PIN)

#define IRDA_ID 0
#define  RecvBuffLen    10

typedef struct
{
	//  uint8_t RecvBuff[10];
	uint8_t *pRxBuff;
	uint16_t usBuffSize;
	uint16_t usIrdaRead;
	uint16_t usIrdaWrite;
	
}IrdaHandle_t;


void bsp_irdaInit(void);
void Irda_Process(void);  
uint8_t Get_Pulse_Time(void);
uint8_t Irda_SetFifo(uint8_t IrdaData);
uint8_t Irda_GetFifo(void);
uint8_t Irda_IsDataVailed(uint32_t  frame_data,uint8_t* VailedData); 
void Irda_10msMainfunction(void);



#endif /* __IRDA_H */
