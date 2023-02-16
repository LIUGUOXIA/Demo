#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

/*
***************************霸道ADC的GPIO引脚映射****************************

PA0		ADC1_IN0		ADC2_IN0		ADC3_IN0
PA1		ADC1_IN1		ADC2_IN1		ADC3_IN1
PA2		ADC1_IN2		ADC2_IN2		ADC3_IN2
PA3		ADC1_IN3		ADC2_IN3		ADC3_IN3
PA4		ADC1_IN4		ADC2_IN4
PA5		ADC1_IN5		ADC2_IN5
PA6		ADC1_IN6		ADC2_IN6
PA7		ADC1_IN7		ADC2_IN7
PB0		ADC1_IN8		ADC2_IN8
PB1		ADC1_IN9		ADC2_IN9
PC0		ADC1_IN10		ADC2_IN10		ADC3_IN10
PC1		ADC1_IN11		ADC2_IN11		ADC3_IN11
PC2		ADC1_IN12		ADC2_IN12		ADC3_IN12
PC3		ADC1_IN13		ADC2_IN13		ADC3_IN13
PC4		ADC1_IN14		ADC2_IN14
PC5		ADC1_IN15		ADC2_IN15

PF6										ADC3_IN4
PF7										ADC3_IN5
PF8										ADC3_IN6
PF9										ADC3_IN7
PF10									ADC3_IN8
***************************************************************************
*/



// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_GPIO_PORT                 GPIOC

// 注意
// 1-PC0 在霸道里面接的是蜂鸣器，默认被拉低
// 2-PC0 在指南者里面接的是SPI FLASH的 片选，默认被拉高
// 所以 PC0 做 ADC 转换通道的时候，结果可能会有误差

// 转换通道个数


#define CH1_ADC_ENABLE    1
#define CH2_ADC_ENABLE    1
#define CH3_ADC_ENABLE    1
#define CH4_ADC_ENABLE    1
#define CH5_ADC_ENABLE    1
#define CH6_ADC_ENABLE    1


#define CH1_ADC_BUFF_SIZE    11
#define CH2_ADC_BUFF_SIZE    11
#define CH3_ADC_BUFF_SIZE    11
#define CH4_ADC_BUFF_SIZE    11
#define CH5_ADC_BUFF_SIZE    11
#define CH6_ADC_BUFF_SIZE    11



typedef enum{
CH1_ADC = 0,
CH2_ADC,
CH3_ADC,
CH4_ADC,
CH5_ADC,
CH6_ADC,
CHMAX_ADC
}ADC_CH_t;


typedef  struct 
{
float Voltage;
uint16_t* ucPRecvBuff;
uint16_t  adcBuffSize;
uint16_t  adcWriteIndex;
uint16_t  adcReadIndex;

}ADC_Fifo_t;


#define    NOFCHANEL										 6

#define    ADC_PIN1                      GPIO_Pin_0
#define    ADC_CHANNEL1                  ADC_Channel_10

#define    ADC_PIN2                      GPIO_Pin_1
#define    ADC_CHANNEL2                  ADC_Channel_11

#define    ADC_PIN3                      GPIO_Pin_2
#define    ADC_CHANNEL3                  ADC_Channel_12

#define    ADC_PIN4                      GPIO_Pin_3
#define    ADC_CHANNEL4                  ADC_Channel_13

#define    ADC_PIN5                      GPIO_Pin_4
#define    ADC_CHANNEL5                  ADC_Channel_14

#define    ADC_PIN6                      GPIO_Pin_5
#define    ADC_CHANNEL6                  ADC_Channel_15


// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************函数声明********************************/
void ADCx_Init(void);
void ADC_20msMainfunction(void);

void bsp_InitAdc();
float GetVolate(ADC_CH_t ADC_CH);
#endif /* __ADC_H */

