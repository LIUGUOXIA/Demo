/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.c
*	版    本 : V1.0
*	说    明 : 采用串口中断+FIFO模式实现多个串口的同时访问
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2013-02-01 armfly  正式发布
*		V1.1    2013-06-09 armfly  FiFo结构增加TxCount成员变量，方便判断缓冲区满; 增加 清FiFo的函数
*		V1.2	2014-09-29 armfly  增加RS485 MODBUS接口。接收到新字节后，直接执行回调函数。
*		V1.3	2015-07-23 armfly  增加 UART_T 结构的读写指针几个成员变量必须增加 __IO 修饰,否则优化后
*					会导致串口发送函数死机。
*		V1.4	2015-08-04 armfly  解决UART4配置bug  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART1);
*		V1.5	2015-10-08 armfly  增加修改波特率的接口函数
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "bsp_uart_fifo.h"



/* 定义每个串口结构体变量 */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif



static void UartVarInit(void);

static void InitHardUart(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);
static void ConfigUartNVIC(void);

void RS485_InitTXE(void);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* 必须先初始化全局变量,再配置硬件 */

	InitHardUart();		/* 配置串口的硬件参数(波特率等) */

	RS485_InitTXE();	/* 配置RS485芯片的发送使能硬件，配置为推挽输出 */

	ConfigUartNVIC();	/* 配置串口中断 */
}

/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为UART指针
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: uart指针
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM4)
	{
		#if UART4_FIFO_EN == 1
			return &g_tUart4;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM5)
	{
		#if UART5_FIFO_EN == 1
			return &g_tUart5;
		#else
			return 0;
		#endif
	}

	else
	{
		/* 不做任何处理 */
		return 0;
	}
}


/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为 USART_TypeDef* USARTx
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: USART_TypeDef*,  USART1, USART2, USART3, UART4, UART5
*********************************************************************************************************
*/
USART_TypeDef *ComToUSARTx(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return USART1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return USART2;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return USART3;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM4)
	{
		#if UART4_FIFO_EN == 1
			return UART4;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM5)
	{
		#if UART5_FIFO_EN == 1
			return UART5;
		#else
			return 0;
		#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: comSendBuf
*	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _ucaBuf: 待发送的数据缓冲区
*			  _usLen : 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);          // 将COM端口号转换为UART指针,再将对应的UART赋给PUART指针 
	if (pUart == 0)
	{
		return;
	}

	if (pUart->SendBefor != 0)
	{
		pUart->SendBefor();		/* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	函 数 名: comSendChar
*	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _ucByte: 待发送的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

/*
*********************************************************************************************************
*	函 数 名: comGetChar
*	功能说明: 从接收缓冲区读取1字节，非阻塞。无论有无数据均立即返回。
*	形    参: _ucPort: 端口号(COM1 - COM5)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return 0;
	}

	return UartGetChar(pUart, _pByte);
}

/*
*********************************************************************************************************
*	函 数 名: comClearTxFifo
*	功能说明: 清零串口发送缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usTxWrite = 0;
	pUart->usTxRead = 0;
	pUart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: comClearRxFifo
*	功能说明: 清零串口接收缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: comSetBaud
*	功能说明: 设置串口的波特率
*	形    参: _ucPort: 端口号(COM1 - COM5)
*			  _BaudRate: 波特率，0-4500000， 最大4.5Mbps
*	返 回 值: 无
*********************************************************************************************************
*/
void comSetBaud(COM_PORT_E _ucPort, uint32_t _BaudRate)
{
	USART_TypeDef* USARTx;
	
	USARTx = ComToUSARTx(_ucPort);
	if (USARTx == 0)
	{
		return;
	}
	
	USART_SetBaudRate(USARTx, _BaudRate);
}

/*
*********************************************************************************************************
*	函 数 名: USART_SetBaudRate
*	功能说明: 修改波特率寄存器，不更改其他设置。如果使用 USART_Init函数, 则会修改硬件流控参数和RX,TX配置
*			  根据固件库中 USART_Init函数，将其中配置波特率的部分单独提出来封装为一个函数
*	形    参: USARTx : USART1, USART2, USART3, UART4, UART5
*			  BaudRate : 波特率，取值 0 - 4500000
*	返 回 值: 无
*********************************************************************************************************
*/
void USART_SetBaudRate(USART_TypeDef* USARTx, uint32_t BaudRate)
{
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;
	RCC_ClocksTypeDef RCC_ClocksStatus;

	/* Check the parameters */
	assert_param(IS_USART_ALL_PERIPH(USARTx));
	assert_param(IS_USART_BAUDRATE(BaudRate));  

	/*---------------------------- USART BRR Configuration -----------------------*/
	/* Configure the USART Baud Rate */
	RCC_GetClocksFreq(&RCC_ClocksStatus);

	if (USARTx == USART1)
	{
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	}
	else
	{
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
	}

	/* Determine the integer part */
	if ((USARTx->CR1 & USART_CR1_OVER8) != 0)
	{
		/* Integer part computing in case Oversampling mode is 8 Samples */
		integerdivider = ((25 * apbclock) / (2 * (BaudRate)));    
	}
	else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
	{
		/* Integer part computing in case Oversampling mode is 16 Samples */
		integerdivider = ((25 * apbclock) / (4 * (BaudRate)));    
	}
	tmpreg = (integerdivider / 100) << 4;

	/* Determine the fractional part */
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	/* Implement the fractional part in the register */
	if ((USARTx->CR1 & USART_CR1_OVER8) != 0)
	{
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
	}
	else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
	{
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	}

	/* Write to USART BRR register */
	USARTx->BRR = (uint16_t)tmpreg;
}

/* 如果是RS485通信，请按如下格式编写函数， 我们仅举了 USART3作为RS485的例子 */

/*
*********************************************************************************************************
*	函 数 名: RS485_InitTXE
*	功能说明: 配置RS485发送使能口线 TXE
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_InitTXE(void)
{

}

/*
*********************************************************************************************************
*	函 数 名: RS485_SetBaud
*	功能说明: 修改485串口的波特率。
*	形    参: _baud : 波特率.0-4500000
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SetBaud(uint32_t _baud)
{
	comSetBaud(COM3, _baud);
}

/*
*********************************************************************************************************
*	函 数 名: RS485_SendBefor
*	功能说明: 发送数据前的准备工作。对于RS485通信，请设置RS485芯片为发送状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_tUart2.SendBefor = RS485_SendBefor
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendBefor(void)
{
//	RS485_TX_EN();	/* 切换RS485收发芯片为发送模式 */
}

/*
*********************************************************************************************************
*	函 数 名: RS485_SendOver
*	功能说明: 发送一串数据结束后的善后处理。对于RS485通信，请设置RS485芯片为接收状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_tUart2.SendOver = RS485_SendOver
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendOver(void)
{
//	RS485_RX_EN();	/* 切换RS485收发芯片为接收模式 */
}


/*
*********************************************************************************************************
*	函 数 名: RS485_SendBuf
*	功能说明: 通过RS485芯片发送一串数据。注意，本函数不等待发送完毕。
*	形    参: _ucaBuf : 数据缓冲区
*			  _usLen : 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen)
{
	comSendBuf(COM3, _ucaBuf, _usLen);
}


/*
*********************************************************************************************************
*	函 数 名: RS485_SendStr
*	功能说明: 向485总线发送一个字符串，0结束。
*	形    参: _pBuf 字符串，0结束
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendStr(char *_pBuf)
{
	RS485_SendBuf((uint8_t *)_pBuf, strlen(_pBuf));
}

/*
*********************************************************************************************************
*	函 数 名: RS485_ReciveNew
*	功能说明: 接收到新的数据
*	形    参: _byte 接收到的新数据
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_ReciveNew(uint8_t _byte)
{
}

/*
*********************************************************************************************************
*	函 数 名: UartVarInit
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						/* STM32 串口设备 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* 发送缓冲区指针 */
	g_tUart1.pRxBuf = g_RxBuf1;					/* 接收缓冲区指针 */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart1.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart1.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart1.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart1.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart1.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart1.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart1.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart1.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart1.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						/* STM32 串口设备 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* 发送缓冲区指针 */
	g_tUart2.pRxBuf = g_RxBuf2;					/* 接收缓冲区指针 */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart2.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart2.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart2.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart2.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart2.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart2.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart2.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart2.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart2.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						/* STM32 串口设备 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* 发送缓冲区指针 */
	g_tUart3.pRxBuf = g_RxBuf3;					/* 接收缓冲区指针 */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart3.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart3.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart3.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart3.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart3.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart3.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart3.SendBefor = RS485_SendBefor;		/* 发送数据前的回调函数 */
	g_tUart3.SendOver = RS485_SendOver;			/* 发送完毕后的回调函数 */
	g_tUart3.ReciveNew = RS485_ReciveNew;		/* 接收到新数据后的回调函数 */
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = UART4;						/* STM32 串口设备 */
	g_tUart4.pTxBuf = g_TxBuf4;					/* 发送缓冲区指针 */
	g_tUart4.pRxBuf = g_RxBuf4;					/* 接收缓冲区指针 */
	g_tUart4.usTxBufSize = UART4_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart4.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart4.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart4.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart4.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart4.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart4.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart4.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart4.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart4.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = UART5;						/* STM32 串口设备 */
	g_tUart5.pTxBuf = g_TxBuf5;					/* 发送缓冲区指针 */
	g_tUart5.pRxBuf = g_RxBuf5;					/* 接收缓冲区指针 */
	g_tUart5.usTxBufSize = UART5_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart5.usTxWrite = 0;						/* 发送FIFO写索引 */
	g_tUart5.usTxRead = 0;						/* 发送FIFO读索引 */
	g_tUart5.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart5.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart5.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart5.usTxCount = 0;						/* 待发送的数据个数 */
	g_tUart5.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart5.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart5.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
#endif

}

/*
*********************************************************************************************************
*	函 数 名: InitHardUart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;

/***************************************************************************************************/
	
#if UART1_FIFO_EN == 1		/* 串口1 TX = PA9   RX = PA10 或 TX = PB6   RX = PB7*/

	/* 第1步： 配置GPIO */
	#if 1	/* TX = PA9   RX = PA10 */
		RCC_APB2PeriphClockCmd(UART1_GPIO_CLK, ENABLE);     		/* 打开 GPIO 时钟 */

		RCC_APB2PeriphClockCmd(UART1_CLK, ENABLE);        /* 打开 UART 时钟 */

		/* 配置 USART Tx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		/* 输出类型为推挽 复用模式 */
		GPIO_InitStructure.GPIO_Pin = UART1_GPIO_TX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART1_GPIO_TX_PORT, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = UART1_GPIO_RX_PIN;
		GPIO_Init(UART1_GPIO_RX_PORT, &GPIO_InitStructure);
	#else	/* TX = PB6   RX = PB7  */
		/* 打开 GPIO 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		/* 打开 UART 时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* 将 PB6 映射为 USART1_TX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);

		/* 将 PB7 映射为 USART1_RX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* 配置 USART Tx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP | GPIO_Mode_AF_PP;		/* 输出类型为推挽 复用模式 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART1, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART2_FIFO_EN == 1		/* 串口2 TX = PD5   RX = PD6 或  TX = PA2， RX = PA3  */
	/* 第1步： 配置GPIO */
	#if 0	/* 串口2 TX = PA2   RX = PA3 */
		/* 打开 GPIO 时钟 */
		RCC_APB2PeriphClockCmd(UART2_GPIO_CLK, ENABLE);

		/* 打开 UART 时钟 */
		RCC_APB1PeriphClockCmd(UART2_CLK, ENABLE);

		/* 将 PA2 映射为 USART2_TX */
		GPIO_PinAFConfig(UART2_GPIO_TX_PORT, UART2_GPIO_TX_SOURCE, GPIO_AF_USART2);

		/* 将 PA3 映射为 USART2_RX */
		GPIO_PinAFConfig(UART2_GPIO_RX_PORT, UART2_GPIO_RX_SOURCE, GPIO_AF_USART2);

		/* 配置 USART Tx 为复用功能 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP | GPIO_Mode_AF_PP;		/* 输出类型为推挽 复用模式 */
		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART2_GPIO_TX_PORT, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_RX_PIN;
		GPIO_Init(UART2_GPIO_RX_PORT, &GPIO_InitStructure);

	#else	/* 串口2   TX = PA2， RX = PA3 */
		/* 打开 GPIO 时钟 */
		RCC_APB2PeriphClockCmd(UART2_GPIO_CLK, ENABLE);

		/* 打开 UART 时钟 */
		RCC_APB1PeriphClockCmd(UART2_CLK, ENABLE);

		/* 将 PA2 映射为 USART2_TX. 在STM32-V5板中，PA2 管脚用于以太网 */
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);


		/* 配置 USART Tx 为复用功能 */
   GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_AF_PP;		/* 输出类型为推挽 复用模式 */

		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART2_GPIO_TX_PORT, &GPIO_InitStructure);

		/* 配置 USART Rx 为复用功能 */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_RX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART2_GPIO_RX_PORT, &GPIO_InitStructure);
	#endif

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART2_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		/* 仅选择接收模式 */
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART2, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART3_FIFO_EN == 1			/* 串口3 TX = PB10   RX = PB11 */

	/* 配置 PB2为推挽输出，用于切换 RS485芯片的收发状态 */
//	{
//		RCC_APB1PeriphClockCmd(RCC_RS485_TXEN, ENABLE);

//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP | GPIO_Mode_AF_PP;		/* 输出类型为推挽 复用模式 */
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */

//		GPIO_InitStructure.GPIO_Pin = PIN_RS485_TXEN;
//		GPIO_Init(PORT_RS485_TXEN, &GPIO_InitStructure);
//	}

	/* 打开 GPIO 时钟 */
	RCC_APB2PeriphClockCmd(UART3_GPIO_CLK, ENABLE);

	/* 打开 UART 时钟 */
	RCC_APB1PeriphClockCmd(UART3_CLK, ENABLE);



	/* 配置 USART Tx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;		/* 输出类型为推挽 复用模式 */

	GPIO_InitStructure.GPIO_Pin = UART3_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART3_GPIO_TX_PORT, &GPIO_InitStructure);

	/* 配置 USART Rx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = UART3_GPIO_RX_PIN;
  GPIO_Init(UART3_GPIO_RX_PORT, &GPIO_InitStructure);

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART3_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART3, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART4_FIFO_EN == 1			/* 串口4 TX = PC10   RX = PC11 */
	/* 第1步： 配置GPIO */

	RCC_APB2PeriphClockCmd(UART4_GPIO_CLK, ENABLE);    	/* 打开 GPIO 时钟 */

	RCC_APB1PeriphClockCmd(UART4_CLK, ENABLE);       	/* 打开 UART 时钟 */

	/* 将 PC10 映射为 UART4_TX */
//	GPIO_PinAFConfig(UART4_GPIO_TX_PORT, UART4_GPIO_TX_SOURCE, GPIO_AF_USART1);

	/* 将 PC11 映射为 UART4_RX */
//	GPIO_PinAFConfig(UART4_GPIO_RX_PORT, UART4_GPIO_RX_SOURCE, GPIO_AF_USART1);

	/* 配置 USART Tx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	/* 复用模式 */
	GPIO_InitStructure.GPIO_Pin = UART4_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART4_GPIO_TX_PORT, &GPIO_InitStructure);

	/* 配置 USART Rx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = UART4_GPIO_RX_PIN;
	GPIO_Init(UART4_GPIO_RX_PORT, &GPIO_InitStructure);

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART4_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	      /* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(UART4, ENABLE);		   /* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(UART4, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART5_FIFO_EN == 1			/* 串口5 TX = PC12   RX = PD2 */
	/* 第1步： 配置GPIO */

	RCC_APB2PeriphClockCmd(UART5_GPIO_TX_CLK |UART5_GPIO_RX_CLK, ENABLE);     	/* 打开 GPIO 时钟 */

	RCC_APB1PeriphClockCmd(UART5_CLK, ENABLE);        	/* 打开 UART 时钟 */

	/* 将 PC12 映射为 UART5_TX */
//	GPIO_PinAFConfig(UART5_GPIO_TX_PORT, UART5_GPIO_TX_SOURCE, GPIO_AF_UART5);

	/* 将 PD2 映射为 UART5_RX */
//	GPIO_PinAFConfig(UART5_GPIO_RX_PORT, UART5_GPIO_RX_SOURCE, GPIO_AF_UART5);

	/* 配置 UART Tx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	/* 复用模式 */
	GPIO_InitStructure.GPIO_Pin = UART5_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART5_GPIO_TX_PORT, &GPIO_InitStructure);

	/* 配置 UART Rx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = UART5_GPIO_RX_PIN;
	GPIO_Init(UART5_GPIO_RX_PORT, &GPIO_InitStructure);

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = UART5_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(UART5, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(UART5, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

}

/*
*********************************************************************************************************
*	函 数 名: ConfigUartNVIC
*	功能说明: 配置串口硬件中断.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	/*	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  --- 在 bsp.c 中 bsp_Init() 中配置中断优先级组 */

#if UART1_FIFO_EN == 1
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART2_FIFO_EN == 1
	/* 使能串口2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART3_FIFO_EN == 1
	/* 使能串口3中断t */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART4_FIFO_EN == 1
	/* 使能串口4中断t */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART5_FIFO_EN == 1
	/* 使能串口5中断t */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

}

/*
*********************************************************************************************************
*	函 数 名: UartSend
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
//参数：_pUart指向串口设备结构体的指针；_ucaBuf指向缓冲区的指针；_usLen发送长度大小
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;           //i用来遍历发送的数据长度

	for (i = 0; i < _usLen; i++)         
	{
		/* 如果发送缓冲区已经满了，则等待缓冲区空 */
	#if 0
		/*
			在调试GPRS例程时，下面的代码出现死机，while 死循环
			原因： 发送第1个字节时 _pUart->usTxWrite = 1；_pUart->usTxRead = 0;
			将导致while(1) 无法退出
		*/
		while (1)
		{
			uint16_t usRead;

			DISABLE_INT();
			usRead = _pUart->usTxRead;
			ENABLE_INT();

			if (++usRead >= _pUart->usTxBufSize)
			{
				usRead = 0;
			}

			if (usRead != _pUart->usTxWrite)
			{
				break;
			}
		}
	#else
		/* 当 _pUart->usTxBufSize == 1 时, 下面的函数会死掉(待完善) */
		while (1)
		{
			__IO uint16_t usCount;

			DISABLE_INT();
			usCount = _pUart->usTxCount;     //将待发送的数据个数赋给usCount计数
			ENABLE_INT();          //使能全局中断函数

			 //保证每次写入缓冲区时都能保证等待发送的个数都小于缓冲区本身的大小，当等待发送的数据个数超过缓冲区本身的大小时，程序将陷入死循环。
			if (usCount < _pUart->usTxBufSize)      
			{
				break;
			}
		}
	#endif

		/* 将新数据填入发送缓冲区 */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];   //通过此条语句将要发送的数据送入缓冲区,同时在将要发送的数据存储到缓冲区之后，就可以通过触发中断发送数据

		DISABLE_INT();     // 禁止全局中断
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)   //如果当前的写指针已经移动到FIFO的顶端时，对指针进行复位。
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;     //等待发送的数据个数   
		ENABLE_INT();
	}

	USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);    //使能串口发送中断
}

/*
*********************************************************************************************************
*	函 数 名: UartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	ENABLE_INT();

	/* 如果读和写索引相同，则返回0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* 已经沒有数据则返回 */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* 从串口接收FIFO取1个数据 */   //*_pByte便是从缓冲区读出的值

		/* 改写FIFO读索引 */
		DISABLE_INT();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)    //pUart->usRxRead先加一再与 _pUart->usRxBufSize做比较
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		ENABLE_INT();
		return 1;
	}
}


/*
*********************************************************************************************************
*	函 数 名: Uart_GetFifoSize
*	功能说明: 
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
uint16_t Uart_GetFifoSize(COM_PORT_E port)
{
	UART_T *pUart;
	uint16_t size = 0; 
	pUart = ComToUart(port);
	if (pUart == 0)
	{
		return 0;
	}
	if(pUart->usRxWrite >= pUart->usRxRead)
	{

		size = pUart->usRxWrite - pUart->usRxRead; 

	}
	else
	{
		size = pUart->usRxBufSize + pUart->usRxWrite - pUart->usRxRead;
	}
	 return size;
}




uint8_t Uart_GetFifo(COM_PORT_E port,uint8_t* ReadBuff,uint16_t ReadSize)
{
	UART_T *pUart;
 uint8_t Rollback = 0;
 uint16_t UsedBufSize = 0;
 uint16_t FirstPartLen = 0;
 uint16_t SecondPartLen = 0;
 uint16_t i = 0;
 pUart = ComToUart(port);
if (pUart == 0 || ReadSize ==0 )
 {
 return 0;
}
 if(pUart->usRxWrite >= pUart->usRxRead)
 {
UsedBufSize = pUart->usRxWrite - pUart->usRxRead;
 Rollback = 0; 
}
else
 {
 UsedBufSize = pUart->usRxBufSize + pUart->usRxWrite - pUart->usRxRead;
 Rollback = 1;
 }
if(UsedBufSize < ReadSize)
{
 return 0;
 }

 if(Rollback)
{
FirstPartLen = pUart->usRxBufSize -pUart->usRxRead;
if(FirstPartLen >= ReadSize) 
 { 
 for(i = 0; i < ReadSize; i++)
 {
ReadBuff[i] = pUart->pRxBuf[pUart->usRxRead + i];
 }
 }
else 
 {
for(i = 0; i < FirstPartLen; i++)
 {
 ReadBuff[i] = pUart->pRxBuf[pUart->usRxRead + i];
 }

 SecondPartLen = ReadSize - FirstPartLen;
for(i = 0; i < SecondPartLen; i++)
 {
 ReadBuff[FirstPartLen + i] = pUart->pRxBuf[i];
 }
 }
 }
else
{
 for(i = 0; i < ReadSize; i++)
 {
ReadBuff[i] = pUart->pRxBuf[pUart->usRxRead + i];
} 
 }

 pUart->usRxRead = (pUart->usRxRead+ ReadSize)%pUart->usRxBufSize;

 return 1;

}




/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{
	/* 处理接收中断  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		/* 从串口接收数据寄存器读取数据存放到接收FIFO */
		uint8_t ch;

		//在接收中断里面读取到接收移位寄存器的值并将值存储到FIFO中，然后通过UartGetChar()函数读取缓冲区的值
		ch = USART_ReceiveData(_pUart->uart);        //通过库函数从接收移位寄存器中读取收到的数据
		_pUart->pRxBuf[_pUart->usRxWrite] = ch;      //将数据存入接收缓冲区，每进入中断一次存取一次
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)    //如果接收写指针移动到FIFO的顶端，则复位接收写指针
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)     //如果还未读取的新数据个数小于缓冲区大小，则继续接收未读取数据个数 
		{
			_pUart->usRxCount++;
		}

		/* 回调函数,通知应用程序收到新数据,一般是发送1个消息或者设置一个标记 */
		//if (_pUart->usRxWrite == _pUart->usRxRead)
		//if (_pUart->usRxCount == 1)
		{
			if (_pUart->ReciveNew)
			{
				_pUart->ReciveNew(ch);
			}
		}
	}

	/* 处理发送缓冲区空中断 */
	if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
			USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

			/* 使能数据发送完毕中断 */
			USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
		}
		else
		{
			/* 从发送FIFO取1个字节写入串口发送数据寄存器 */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);  //此条语句通过调用库函数实现
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)      //如果发送读指针移动到缓冲区FIFO的顶端，复位发送读指针
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;    //每发送一个字节之后待发送数据个数减一
		}

	}
	/* 数据bit位全部发送完毕的中断 */
	else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* 如果发送FIFO的数据全部发送完毕，禁止数据发送完毕中断 */
			USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);

			/* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
			if (_pUart->SendOver)
			{
				_pUart->SendOver();
			}
		}
		else
		{
			/* 正常情况下，不会进入此分支 */

			/* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */   //正常情况下，不会进入这个分支
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);      
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)        //进入此分支后，再次发送缓冲区的值
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}
#endif

#if UART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
	UartIRQ(&g_tUart3);
}
#endif

#if UART4_FIFO_EN == 1
void UART4_IRQHandler(void)
{
	UartIRQ(&g_tUart4);
}
#endif

#if UART5_FIFO_EN == 1
void UART5_IRQHandler(void)
{
	UartIRQ(&g_tUart5);
}
#endif


/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
	comSendChar(COM1, ch);

	return ch;
#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
	/* 写一个字节到USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 1	/* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
	uint8_t ucData;

	while(comGetChar(COM1, &ucData) == 0);

	return ucData;
#else
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
#endif
}


void my_printf(void)
{
	printf("调用硬件定时器成功\r\n");
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
void UART_5msMainFunction(void)      //任务调用接口
{
	uint8_t cmd;
	uint8_t ret = 0;
	uint8_t data = 0;
	uint32_t time = 0;
	static uint16_t duty = 0;
  
	if(comGetChar(COM1, &cmd))
	{
			switch(cmd)
			{
					case '1':
						bsp_StartTimer(1, 10);
						break;
					case'2':
						ret  = bsp_CheckTimer(1);
						if(ret == 1)
						{
								printf("软件定时器1 is ok\r\n");
						}
						else
						{
						
						}
						break;
					case '3':
						time = bsp_GetRunTime();
						printf("系统运行时间%d\r\n",time);
							break;
					case '4':
						bsp_StartHardTimer(1, 1000, my_printf);
						LED_SetStatus(LED_DOING_WORK,1);
						break;
					case '5':
						Beep_SetStatus(BEEP_DOING_WORK, 1); 
						LED_SetStatus(LED_END_WORK,1);
						printf("case5 ok\r\n");
						break;
					
					case '6':
						LED_SetStatus(LED_START_WORK,1);      
						printf("LED_START_WORK is ok\r\n");
						break;
					case '7':
						duty+=10;
						if(duty>255)
						{
							duty = 0;
							
						}
						printf("duty:%d\r\n",duty);
						bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_5, TIM3, 2, 20000, (duty * 10000) /255);	//
		//				bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_0, TIM3, 3, 20000, (duty * 10000) /255);	
		//				bsp_SetTIMOutPWM(GPIOB, GPIO_Pin_1, TIM3, 4, 20000, (duty * 10000) /255);
						break;
						
					case '8':
						
//					GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
					  printf("----------\r\n");
						BEEP_ENABLE();
//					Beep_SetStatus(BEEP_WARNING_WORK, 1); 
//					BEEP_ON;                          //Beep_10msMainFunction任务打开的话会被影响而不发声
						break;
					case '9':
//				  GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN); 
//Beep_10msMainFunction  此任务打开的话Set   Reset会被影响（其实开了，但很快就被任务里的判断条件关闭）而不发声（Beep_10msMainFunction一直循环进入，任务里最后的判断条件把蜂鸣器关了）
						BEEP_DISABLE();                   //Beep_10msMainFunction任务打开的话会被影响而不发声
//					Beep_SetStatus(BEEP_WARNING_WORK, 0);     
						break;
					
					case'a':
						Music_Select(MUSIC_HAPPY, 1);
						printf("MusicPlay End\r\n");
						break;
					case'b':
					  Music_Select(MUSIC_SAD, 1);
					  Music_Select(MUSIC_ABSOLUTE, 1);
					//	play_successful();
					//  PrintfLogo();
						break;
				
			}
	}
}




/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

#define FIRMWARE_LIB_VERSION_NUM       "V3.5.0"
#define CHIP_ID				            		 "STM32F103ZET6"
#define INTERNAL_FLASH_START_ADDR			 ((uint32_t)0x08008000)
#define INTERNAL_FLASH_END_ADDR    		 ((uint32_t)0x0800C000)
#define INTERNAL_FLASH_SIZE						 (((uint32_t)0x0800C000)-((uint32_t)0x08008000))
#define INTERNAL_FLASH_PAGE_SIZE			 (INTERNAL_FLASH_SIZE/((uint16_t)0x800))    //一页为2048   即每页大小2KByte
#define DOMINANT_FREQUENCY       				(SystemCoreClock/1000000)
#define INTERNAL_SRAM_START_ADDR				0x20000000
#define INTERNAL_SRAM_SIZE							0x10000






/*定义STM32 MCU的类型*/
typedef enum {
  STM32F0,
  STM32F1,
  STM32F2,
  STM32F3,
  STM32F4,
  STM32F7,
  STM32L0,
  STM32L1,
  STM32L4,
  STM32H7,
}MCUTypedef;
 
uint32_t idAddr[]={0x1FFFF7AC,  /*STM32F0唯一ID起始地址*/
                0x1FFFF7E8,  /*STM32F1唯一ID起始地址*/
                0x1FFF7A10,  /*STM32F2唯一ID起始地址*/
                0x1FFFF7AC,  /*STM32F3唯一ID起始地址*/
                0x1FFF7A10,  /*STM32F4唯一ID起始地址*/
                0x1FF0F420,  /*STM32F7唯一ID起始地址*/
                0x1FF80050,  /*STM32L0唯一ID起始地址*/
                0x1FF80050,  /*STM32L1唯一ID起始地址*/
                0x1FFF7590,  /*STM32L4唯一ID起始地址*/
                0x1FF0F420}; /*STM32H7唯一ID起始地址*/
 
/*获取MCU的唯一ID*/
void GetSTM32MCUID(uint32_t *id,MCUTypedef type)
{
  if(id!=NULL)
  {
    id[0]=*(uint32_t*)(idAddr[type]);
    id[1]=*(uint32_t*)(idAddr[type]+4);
    id[2]=*(uint32_t*)(idAddr[type]+8);
  }
}

void PrintfLogo(void)
{
	printf("**********************************************\r\n");
	
	printf("固件库版本号：%s\r\n", FIRMWARE_LIB_VERSION_NUM);
	printf("芯片ID：%0x%0x%0x\r\n", *(__IO u32*)(0X1FFFF7E8),*(__IO u32*)(0X1FFFF7EC),*(__IO u32*)(0X1FFFF7F0));
	printf("内部FLASH起始地址：%x\r\n", FLASH_BASE);
	printf("内部FLASH大小：%x\r\n", *(__IO u32*)(0X1FFFF7E0));
	printf("内部FLASH页大小：%x\r\n", INTERNAL_FLASH_PAGE_SIZE);
	printf("主频：%dMHZ\r\n", DOMINANT_FREQUENCY);
	printf("内部SRAM起始地址：%x\r\n", SRAM_BASE);
	printf("内部SRAM大小：%x\r\n", INTERNAL_SRAM_SIZE);

}








