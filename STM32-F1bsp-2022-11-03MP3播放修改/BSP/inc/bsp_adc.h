#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

/*
***************************�Ե�ADC��GPIO����ӳ��****************************

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



// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_GPIO_PORT                 GPIOC

// ע��
// 1-PC0 �ڰԵ�����ӵ��Ƿ�������Ĭ�ϱ�����
// 2-PC0 ��ָ��������ӵ���SPI FLASH�� Ƭѡ��Ĭ�ϱ�����
// ���� PC0 �� ADC ת��ͨ����ʱ�򣬽�����ܻ������

// ת��ͨ������


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


// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************��������********************************/
void ADCx_Init(void);
void ADC_20msMainfunction(void);

void bsp_InitAdc();
float GetVolate(ADC_CH_t ADC_CH);
#endif /* __ADC_H */

