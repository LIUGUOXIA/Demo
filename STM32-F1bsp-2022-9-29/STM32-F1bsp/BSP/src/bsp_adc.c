/**
  ******************************************************************************
  * @file    bsp_irda.c
  * @author  fire
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   ADC多通道DMA读取
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
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
        //滤波
			 value =  LIB_FifoFillter(buff,5);
		  //得到电压值
			 AdcFifoHanle = ADC_GetHandle(i);
			AdcFifoHanle->Voltage = value/4096*3.3;
			printf("Voltage=%f\r\n",AdcFifoHanle->Voltage);

	  } 

	 }

	}


}


/**
**********************************************************************************************
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
**********************************************************************************************
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );     // 打开 ADC IO端口时钟
	
	GPIO_InitStructure.GPIO_Pin = ADC_PIN1| ADC_PIN2| ADC_PIN3| ADC_PIN4| ADC_PIN5| ADC_PIN6;	// 配置 ADC IO 引脚模式					 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);				 // 初始化 ADC IO
}

/**
**********************************************************************************************
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
**********************************************************************************************
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);        // 打开DMA时钟
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );          	 // 打开ADC时钟
	
	DMA_DeInit(ADC_DMA_CHANNEL);        // 复位DMA控制器
	
	// 配置 DMA 初始化结构体
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );    // 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;       // 存储器地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;             // 数据源来自外设
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;                // 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   // 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // 存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;      	// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	              // 循环传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;       // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;           // 禁止存储器到存储器模式，因为是从外设到存储器
	
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);      // 初始化DMA
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);               // 使能 DMA 通道
	
	// ADC 模式配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;       // 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;          // 扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;       // 连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     // 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;           // 转换结果右对齐
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	              // 转换通道个数
	
	ADC_Init(ADC_x, &ADC_InitStructure);       // 初始化ADC

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);          	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	
	
	ADC_DMACmd(ADC_x, ENABLE);           // 使能ADC DMA 请求
	ADC_Cmd(ADC_x, ENABLE);              // 开启ADC ，并开始转换
	ADC_ResetCalibration(ADC_x);         // 初始化ADC 校准寄存器  

	while(ADC_GetResetCalibrationStatus(ADC_x));     // 等待校准寄存器初始化完成
	
	ADC_StartCalibration(ADC_x);              // ADC开始校准
	
	while(ADC_GetCalibrationStatus(ADC_x));   // 等待校准完成
	 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);   	// 由于没有采用外部触发，所以使用软件触发ADC转换
}

/**
**********************************************************************************************
  * @brief  ADC初始化
  * @param  无
  * @retval 无
**********************************************************************************************
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
/*********************************************END OF FILE**********************/
