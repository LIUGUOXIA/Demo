/**
  ******************************************************************************
  * @file    bsp_irda.c
  * @author  fire
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   ADC��ͨ��DMA��ȡ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "bsp.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0,0,0};



#if CH1_ADC_ENABLE
uint16_t  s_AdcCh1_Buff[CH1_ADC_BUFF_SIZE];
ADC_Fifo_t  s_AdcCh1_Handle ;
#endif  

#if CH2_ADC_ENABLE
uint16_t  s_AdcCh2_Buff[CH2_ADC_BUFF_SIZE];
ADC_Fifo_t   s_AdcCh2_Handle ;
#endif  

#if CH3_ADC_ENABLE
uint16_t  s_AdcCh3_Buff[CH3_ADC_BUFF_SIZE];
ADC_Fifo_t   s_AdcCh3_Handle ;
#endif  

#if CH4_ADC_ENABLE
uint16_t  s_AdcCh4_Buff[CH4_ADC_BUFF_SIZE];
ADC_Fifo_t   s_AdcCh4_Handle ;
#endif  

#if CH5_ADC_ENABLE
uint16_t  s_AdcCh5_Buff[CH5_ADC_BUFF_SIZE];
ADC_Fifo_t   s_AdcCh5_Handle ;
#endif  

#if CH6_ADC_ENABLE
uint16_t  s_AdcCh6_Buff[CH6_ADC_BUFF_SIZE];
ADC_Fifo_t   s_AdcCh6_Handle ;
#endif  

/*
**********************************************************************************************
*�� �� ����
*����˵����
*��    �Σ�
*�� �� ֵ��
*
**********************************************************************************************
*/
void Bsp_InitAdcFifo(ADC_Fifo_t* pFifo,uint16_t* pBuff,uint16_t buffsize)
{
	pFifo->ucPRecvBuff = pBuff;
    pFifo->adcBuffSize = buffsize;
	memset((uint8_t*)pBuff,0,buffsize*2);
	pFifo->adcWriteIndex = 0;
	pFifo->adcReadIndex = 0;
	pFifo->Voltage = 0;
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
void bsp_InitAdcVar()
{
#if CH1_ADC_ENABLE
Bsp_InitAdcFifo(&s_AdcCh1_Handle,s_AdcCh1_Buff,CH1_ADC_BUFF_SIZE);
#endif 

#if CH2_ADC_ENABLE
Bsp_InitAdcFifo(&s_AdcCh2_Handle,s_AdcCh2_Buff,CH2_ADC_BUFF_SIZE);
#endif 

#if CH3_ADC_ENABLE
Bsp_InitAdcFifo(&s_AdcCh3_Handle,s_AdcCh3_Buff,CH3_ADC_BUFF_SIZE);
#endif 

#if CH4_ADC_ENABLE
Bsp_InitAdcFifo(&s_AdcCh4_Handle,s_AdcCh4_Buff,CH4_ADC_BUFF_SIZE);
#endif 

#if CH5_ADC_ENABLE
Bsp_InitAdcFifo(&s_AdcCh5_Handle,s_AdcCh5_Buff,CH5_ADC_BUFF_SIZE);
#endif 

#if CH6_ADC_ENABLE
Bsp_InitAdcFifo(&s_AdcCh6_Handle,s_AdcCh6_Buff,CH6_ADC_BUFF_SIZE);
#endif 

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
ADC_Fifo_t*  ADC_GetHandle(ADC_CH_t ch)
{
   if(ch == CH1_ADC)
   {
    #if CH1_ADC_ENABLE==1
       return &s_AdcCh1_Handle;
	#else 
      return 0;
	#endif 
   }

    if(ch == CH2_ADC)
   {
    #if CH2_ADC_ENABLE==1
       return &s_AdcCh2_Handle;
	#else 
      return 0;
	#endif 
   }

    if(ch == CH3_ADC)
   {
    #if CH3_ADC_ENABLE==1
       return &s_AdcCh3_Handle;
	#else 
      return 0;
	#endif 
   }

    if(ch == CH4_ADC)
   {
    #if CH4_ADC_ENABLE==1
       return &s_AdcCh4_Handle;
	#else 
      return 0;
	#endif 
   }

    if(ch == CH5_ADC)
   {
    #if CH5_ADC_ENABLE==1
       return &s_AdcCh5_Handle;
	#else 
      return 0;
	#endif 
   }

 if(ch == CH6_ADC)
   {
    #if CH6_ADC_ENABLE==1
       return &s_AdcCh6_Handle;
	#else 
      return 0;
	#endif 
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
float ADC_GetVoltage(ADC_CH_t ch)
{
	float ret = 0.0;
   ADC_Fifo_t* AdcFifoHanle;
   AdcFifoHanle = ADC_GetHandle(ch);
   if(AdcFifoHanle !=0)
   {
	  return AdcFifoHanle->Voltage;
   }

   else 
   {
	return ret;
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
uint8_t ADC_SetFifo(ADC_CH_t ch,uint16_t data)
{
	uint8_t ret = 0;
	ADC_Fifo_t* AdcFifoHanle;
   AdcFifoHanle = ADC_GetHandle(ch);
   if(AdcFifoHanle !=0)
   {
     AdcFifoHanle->ucPRecvBuff[AdcFifoHanle->adcWriteIndex] = data;
	 if(++AdcFifoHanle->adcWriteIndex >= AdcFifoHanle->adcBuffSize)
	 {
		AdcFifoHanle->adcWriteIndex = 0;
	 }
     ret = 1;
   }
   else
   {

   }
   return ret;
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
uint16_t  ADC_GetFifoSize(ADC_CH_t ch)
{
    ADC_Fifo_t* AdcFifoHanle;
    uint16_t  size = 0; 
	AdcFifoHanle = ADC_GetHandle(ch);
	if (AdcFifoHanle == 0)
	{
		return 0;
	}
	if(AdcFifoHanle->adcWriteIndex >= AdcFifoHanle->adcReadIndex)
	{
		size = AdcFifoHanle->adcWriteIndex - AdcFifoHanle->adcReadIndex;
	}
	else 
	{
	 size = AdcFifoHanle->adcWriteIndex - AdcFifoHanle->adcReadIndex+ AdcFifoHanle->adcBuffSize;

	}
   return size;
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
uint8_t ADC_GetFifo(ADC_CH_t ch,uint16_t* ReadBuff,uint16_t ReadSize)
{
   ADC_Fifo_t* AdcFifoHanle;
	uint8_t  Rollback = 0;
	uint16_t UsedBufSize = 0;
	uint16_t FirstPartLen = 0;
	uint16_t SecondPartLen = 0;
	uint16_t i = 0;
    AdcFifoHanle = ADC_GetHandle(ch);
	if (AdcFifoHanle == 0)
	{
		return 0;
	}
	if(AdcFifoHanle->adcWriteIndex >= AdcFifoHanle->adcReadIndex)
	{
	   UsedBufSize =  AdcFifoHanle->adcWriteIndex - AdcFifoHanle->adcReadIndex;
       Rollback = 0;	
	}
	else
	{
	  UsedBufSize = AdcFifoHanle->adcWriteIndex - AdcFifoHanle->adcReadIndex+ AdcFifoHanle->adcBuffSize;
      Rollback = 1;
	}
    if(UsedBufSize < ReadSize)
	{
		return 0;
	}
	if(Rollback)
	{
		FirstPartLen = AdcFifoHanle->adcBuffSize - AdcFifoHanle->adcReadIndex;
		if(FirstPartLen >= ReadSize) 
		{				
			for(i = 0; i < ReadSize; i++)
			{
				ReadBuff[i] = AdcFifoHanle->ucPRecvBuff[AdcFifoHanle->adcReadIndex + i];
			}
		}
		else 
		{
			for(i = 0; i < FirstPartLen; i++)
			{
				ReadBuff[i] = AdcFifoHanle->ucPRecvBuff[AdcFifoHanle->adcReadIndex + i];
			}

			SecondPartLen = ReadSize - FirstPartLen;
			for(i = 0; i < SecondPartLen; i++)
			{
				ReadBuff[FirstPartLen + i] = AdcFifoHanle->ucPRecvBuff[i];
			}
		}
	}
	else
	{
		for(i = 0; i < ReadSize; i++)
		{
			ReadBuff[i] = AdcFifoHanle->ucPRecvBuff[AdcFifoHanle->adcReadIndex + i];
		}	
	}
  AdcFifoHanle->adcReadIndex = (AdcFifoHanle->adcReadIndex + ReadSize)%AdcFifoHanle->adcBuffSize;
  return 1;

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
void ADC_20msMainfunction(void)
{
	uint16_t buff[10];
	uint16_t value = 0;
	ADC_Fifo_t* AdcFifoHanle;
	for(int i =0;i < CHMAX_ADC;i++)
	{
	 ADC_SetFifo(i,ADC_ConvertedValue[i]);
	 if( ADC_GetFifoSize(i) == 5)
	 {
      if(ADC_GetFifo(i,buff,5) ==1)
	  {
        //�˲�
			 value =  LIB_FifoFillter(buff,5);
		  //�õ���ѹֵ
			 AdcFifoHanle = ADC_GetHandle(i);
			AdcFifoHanle->Voltage = value/4096*3.3;
			printf("Voltage=%f\r\n",AdcFifoHanle->Voltage);

	  } 

	 }

	}


}


/**
**********************************************************************************************
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
**********************************************************************************************
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );     // �� ADC IO�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = ADC_PIN1| ADC_PIN2| ADC_PIN3| ADC_PIN4| ADC_PIN5| ADC_PIN6;	// ���� ADC IO ����ģʽ					 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);				 // ��ʼ�� ADC IO
}

/**
**********************************************************************************************
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
**********************************************************************************************
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);        // ��DMAʱ��
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );          	 // ��ADCʱ��
	
	DMA_DeInit(ADC_DMA_CHANNEL);        // ��λDMA������
	
	// ���� DMA ��ʼ���ṹ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );    // �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;       // �洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;             // ����Դ��������
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;                // ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   // ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // �洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;      	// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	              // ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;       // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;           // ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);      // ��ʼ��DMA
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);               // ʹ�� DMA ͨ��
	
	// ADC ģʽ����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;       // ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;          // ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;       // ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     // �����ⲿ����ת���������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;           // ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	              // ת��ͨ������
	
	ADC_Init(ADC_x, &ADC_InitStructure);       // ��ʼ��ADC

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);          	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	
	// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	
	
	ADC_DMACmd(ADC_x, ENABLE);           // ʹ��ADC DMA ����
	ADC_Cmd(ADC_x, ENABLE);              // ����ADC ������ʼת��
	ADC_ResetCalibration(ADC_x);         // ��ʼ��ADC У׼�Ĵ���  

	while(ADC_GetResetCalibrationStatus(ADC_x));     // �ȴ�У׼�Ĵ�����ʼ�����
	
	ADC_StartCalibration(ADC_x);              // ADC��ʼУ׼
	
	while(ADC_GetCalibrationStatus(ADC_x));   // �ȴ�У׼���
	 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);   	// ����û�в����ⲿ����������ʹ���������ADCת��
}

/**
**********************************************************************************************
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
**********************************************************************************************
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
/*********************************************END OF FILE**********************/
