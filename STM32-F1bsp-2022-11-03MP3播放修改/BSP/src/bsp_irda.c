/**
  ******************************************************************************
  * @file    bsp_irda.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   红外遥控器接口
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

uint8_t frame_cnt = 0;      /* 按键次数 */
uint8_t frame_flag = 0;    /* 一帧数据接收完成标志 */
uint8_t isr_cnt;  /* 用于计算进了多少次中断 */ 
uint32_t frame_data = 0;    /* 一帧数据缓存 */


static void bsp_irdaVarInit(void);
static void bsp_irdaHardInit(void);
static void NVIC_Configuration(void);

static IrdaHandle_t s_Irda;
static uint8_t ucRecvBuff[RecvBuffLen];
/*
**********************************************************************************************
*函 数 名：
*功能说明：
*形    参：
*返 回 值：
*
**********************************************************************************************
*/
void bsp_irdaInit(void)               //红外初始化接口
{
	bsp_irdaVarInit();
	bsp_irdaHardInit();
	NVIC_Configuration();
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
static void bsp_irdaVarInit(void)         //变量初始化
{
	
	for(int i= 0;i<RecvBuffLen;i++)
	{
		ucRecvBuff[i] = 0;
	}
	memset(ucRecvBuff,0,sizeof(ucRecvBuff));
	s_Irda.pRxBuff = ucRecvBuff;
	s_Irda.usBuffSize = RecvBuffLen;
	s_Irda.usIrdaRead = 0;
	s_Irda.usIrdaWrite = 0;
	
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
static void bsp_irdaHardInit(void)      //硬件初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(IRDA_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = IRDA_GPIO_PIN;
	GPIO_Init(IRDA_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(IRDA_GPIO_PORT_SOURCE, IRDA_GPIO_PIN_SOURCE);
	EXTI_InitStructure.EXTI_Line = IRDA_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
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
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = IRDA_EXTI_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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
/* 获取高电平的时间 */
uint8_t Get_Pulse_Time(void)
{
  uint8_t time = 0;
  while( IRDA_DATA_INPUT() )
  {
    time ++;
    bsp_DelayUS(20);     // 延时 20us
    if(time == 250)
      return time;   // 超时溢出   
  }
  return time;
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
uint8_t Irda_SetFifo(uint8_t IrdaData)     //若数据有效，则填入fifo
{
//	uint8_t irda_data = 0;
//	uint32_t tureframe = 0;
//	IrdaData = Irda_IsDataVailed(tureframe,&irda_data); 
//	if (!IrdaData )	
//	{
//		return 0;
//	}
//	else
//	{
//		return IrdaData;
//	}
	s_Irda.pRxBuff[s_Irda.usIrdaWrite] = IrdaData;
	if(++s_Irda.usIrdaWrite>=s_Irda.usBuffSize)
	{
		printf("IrdaData=%d\r\n",IrdaData);
		s_Irda.usIrdaWrite = 0;
		
	}
	
	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t Irda_GetFifoSize()
{

 uint16_t size = 0;
 if(s_Irda.usIrdaWrite >= s_Irda.usIrdaRead)
 {
 size = s_Irda.usIrdaWrite - s_Irda.usIrdaRead;
 }
 else
 {
size = s_Irda.usBuffSize+s_Irda.usIrdaWrite - s_Irda.usIrdaRead;
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

uint8_t Irda_GetFifo(void)        // 别的模块调用获取值
{
	 uint8_t Irda_data = 0;

 if(Irda_GetFifoSize()==0) {
 return 0;
 }

 Irda_data = s_Irda.pRxBuff[s_Irda.usIrdaRead];
 if(++s_Irda.usIrdaRead>=s_Irda.usBuffSize)
 {
 printf("Irda_data=%d\r\n",Irda_data);
 s_Irda.usIrdaRead = 0;
 
  }


 return Irda_data;
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
void Irda_Process(void)       //移植中断部分数据处理
{
	uint8_t pulse_time = 0;
	uint8_t leader_code_flag = 0;    /* 引导码标志位，当引导码出现时，表示一帧数据开始 */
	uint8_t irda_data = 0;          /* 数据暂存位 */
	uint8_t vailed_data = 0; 
		while(1)
		{
			if(IRDA_DATA_INPUT() == SET)     /* 只测量高电平的时间 */
			{
				pulse_time = Get_Pulse_Time();     
				
				if(pulse_time >= 250)   /* >=5ms 不是有用信号 当出现干扰或者连发码时，也会break跳出while(1)循环 */
				{
					break;                /* 跳出while(1)循环 */
				}
				
				if(pulse_time >= 200 && pulse_time < 250)             /* 获得前导位 4ms~4.5ms */
				{
					leader_code_flag = 1;
				}
				
				else if(pulse_time >= 10 && pulse_time <50)          /* 0.56ms: 0.2ms~1ms */
				{
					irda_data = 0;
				}
				
				else if(pulse_time >= 50 && pulse_time <100)          /* 1.68ms：1ms~2ms */
				{
					irda_data = 1;
				}
				
				else if(pulse_time >= 100 && pulse_time <= 200)       /* 2.1ms：2ms~4ms */
				{                /* 连发码，在第二次中断出现 */
					frame_flag = 1;      /* 一帧数据接收完成 */
					frame_cnt ++;       /* 按键次数加1 */
					isr_cnt ++;         /* 进中断一次加1 */

					printf("\r\n 按键次数frame_cnt=%d \r\n",frame_cnt);
					printf("\r\n 中断次数isr_cnt=%d \r\n",isr_cnt);

					if(Irda_IsDataVailed(frame_data,&vailed_data)==1)
					{
						Irda_SetFifo(vailed_data);       
					}		
					break;              /* 跳出while(1)循环 */
				}
				 		
				if(leader_code_flag == 1)
				{                       /* 在第一次中断中完成 */
					frame_data <<= 1;
					frame_data += irda_data;
					frame_cnt = 0;
					isr_cnt = 1;
								
						
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
uint8_t Irda_IsDataVailed(uint32_t  frame_data,uint8_t* VailedData)     //判断数据是否有效  有效返回1 无效返回0    VailedData
{
	uint8_t first_byte, sec_byte, tir_byte, fou_byte;  

  first_byte = frame_data >> 24;
  sec_byte = (frame_data>>16) & 0xff;
  tir_byte = frame_data >> 8;
  fou_byte = frame_data;
  

  
  if( (first_byte==(uint8_t)~sec_byte) && (first_byte==IRDA_ID) )
  {
    if( tir_byte == (u8)~fou_byte )
			*VailedData = tir_byte;
      return 1;
  }
  
  return 0;   /* 错误返回 */
	
}



void Test_Timer(void)
{
 printf("Test_Timer\r\n");
}





void Irda_10msMainfunction(void)
{
	uint8_t key_val = 0;
	uint16_t ALS = 0;
	key_val = Irda_GetFifo(); 
      switch( key_val )
      {
        case 162:
          Beep_SetStatus(BEEP_START_WORK,1);       //不响
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n POWER \r\n");
        break;
        
        case 226:
          Beep_SetStatus(BEEP_START_WORK,0);       //不响
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n MENU \r\n");
        break;
        
        case 34:
           printf("%.2f\r\n",GetVolate(CH2_ADC));
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n TEST \r\n");
        break;
        
        case 2:
        bsp_StartHardTimer(1, 10000, Test_Timer); 
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n + \r\n");
        break;
        
        case 194:
        
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n RETURN \r\n");
				
				 
        break;
        
        case 224:
         
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n |<< \r\n");
        break;
        
        case 168:
       
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n > \r\n");
        break;
        
        case 144:
       
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n >>| \r\n");
        break;
        
        case 104:
         
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 0 \r\n");
        break;
        
        case 152:
        
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n - \r\n");
        break;
        
        case 176:
       
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n C \r\n");
        break;
        
        case 48:
          ALS = Ap3216c_Light_intensity();
				  printf("ALS=%d\r\n",ALS);
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 1 \r\n");
        break;
        
        case 24:
        
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 2 \r\n");
        break;
        
        case 122:
        
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 3 \r\n");
        break;
        
        case 16:
         
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 4 \r\n");
        break;
        
        case 56:
      
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 5 \r\n");
        break;
        
        case 90:
        
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 6 \r\n");
        break;
        
          case 66:
      
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 7 \r\n");
        break;
        
        case 74:
        
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 8 \r\n");
        break;
        
        case 82:
         
          printf("\r\n key_val=%d \r\n",key_val);
          printf("\r\n 9 \r\n");
        break;
        
        default:       
        break;
      }      
	
}




/*********************************************END OF FILE**********************/
