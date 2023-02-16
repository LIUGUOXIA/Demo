/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.h
*	说    明 : 头文件
*
*	Copyright:
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#include "stm32f10x.h"

/*
	如果需要更改串口对应的管脚，请自行修改 bsp_uart_fifo.c文件中的 static void InitHardUart(void)函数
*/

/* 定义使能的串口, 0 表示不使能（不增加代码大小）， 1表示使能 */
#ifdef STM32_X3	/* 安富莱 STM32-X4 开发板 */
	/* 安富莱STM32-X3开发板串口分配：
		PA9/USART1_TX	  --- 打印调试口
		PA10/USART1_RX
	*/
	#define	UART1_FIFO_EN	1
	#define	UART2_FIFO_EN	0
	#define	UART3_FIFO_EN	0
	#define	UART4_FIFO_EN	0
	#define	UART5_FIFO_EN	0
	#define	UART6_FIFO_EN	0
#else
	/*
		霸道STM32-V2 串口分配：
		【串口1】 
			PA9/USART1_TX	  --- 打印调试口
			PA10/USART1_RX

		【串口2】 PA2 管脚用于以太网； RX管脚用于接收GPS信号
			PA2/USART2_TX/ETH_MDIO (用于以太网，不做串口发送用)
			PA3/USART2_RX	;接GPS模块输出

		【串口3】 RS485 通信 - TTL 跳线 和 排针
			PB10/USART3_TX
			PB11/USART3_RX

		【串口4】 --- 不做串口用。
			PC10/USART4_TX
			PC11/USART4_RX
		【串口5】 --- 不做串口用。
			PC12/USART5_TX
			PD2/USART5_RX

	*/
	#define	UART1_FIFO_EN	1
	#define	UART2_FIFO_EN	0
	#define	UART3_FIFO_EN	0
	#define	UART4_FIFO_EN	0
	#define	UART5_FIFO_EN	0



//#define RCC_AHB1Periph_GPIOA
//#define RCC_AHB1Periph_GPIOB
//#define RCC_AHB1Periph_GPIOC
//#define RCC_AHB1Periph_GPIOD
//#define RCC_AHB1Periph_GPIOG



/* 串口1的GPIO --- RS323 */
#define UART1_GPIO_TX_PORT      GPIOA
#define UART1_GPIO_TX_PIN       GPIO_Pin_9
#define UART1_GPIO_TX_SOURCE    GPIO_PinSource9

#define UART1_GPIO_RX_PORT      GPIOA
#define UART1_GPIO_RX_PIN       GPIO_Pin_10
#define UART1_GPIO_RX_SOURCE    GPIO_PinSource10

#define UART1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define UART1_CLK               RCC_APB2Periph_USART1

/* 串口2的GPIO  --- 只用了RX， 没用TX，硬件没有接（可用0欧姆接上） */
#define UART2_GPIO_TX_PORT      GPIOA
#define UART2_GPIO_TX_PIN       GPIO_Pin_2
#define UART2_GPIO_TX_SOURCE    GPIO_PinSource2

#define UART2_GPIO_RX_PORT      GPIOA
#define UART2_GPIO_RX_PIN       GPIO_Pin_3
#define UART2_GPIO_RX_SOURCE    GPIO_PinSource3

#define UART2_GPIO_CLK          RCC_APB2Periph_GPIOA
#define UART2_CLK								RCC_APB1Periph_USART2

/* 串口3的GPIO - RS485 */
#define UART3_GPIO_TX_PORT      GPIOB
#define UART3_GPIO_TX_PIN       GPIO_Pin_10
#define UART3_GPIO_TX_SOURCE    GPIO_PinSource10

#define UART3_GPIO_RX_PORT      GPIOB
#define UART3_GPIO_RX_PIN       GPIO_Pin_11
#define UART3_GPIO_RX_SOURCE    GPIO_PinSource11

#define UART3_GPIO_CLK          RCC_APB2Periph_GPIOB
#define UART3_CLK								RCC_APB1Periph_USART3

/* 串口4的GPIO - 在SDIO接口 */
#define UART4_GPIO_TX_PORT      GPIOC
#define UART4_GPIO_TX_PIN       GPIO_Pin_10
#define UART4_GPIO_TX_SOURCE    GPIO_PinSource10

#define UART4_GPIO_RX_PORT      GPIOC
#define UART4_GPIO_RX_PIN       GPIO_Pin_11
#define UART4_GPIO_RX_SOURCE    GPIO_PinSource11

#define UART4_GPIO_CLK          RCC_APB2Periph_GPIOC
#define UART4_CLK							  RCC_APB1Periph_UART4

/* 串口5的GPIO - 在SDIO接口 */
#define UART5_GPIO_TX_PORT      GPIOC
#define UART5_GPIO_TX_PIN       GPIO_Pin_12
#define UART5_GPIO_TX_CLK       RCC_APB2Periph_GPIOC
#define UART5_GPIO_TX_SOURCE    GPIO_PinSource12

#define UART5_GPIO_RX_PORT      GPIOD
#define UART5_GPIO_RX_PIN       GPIO_Pin_2
#define UART5_GPIO_RX_CLK       RCC_APB2Periph_GPIOD
#define UART5_GPIO_RX_SOURCE    GPIO_PinSource2

#define UART5_CLK								RCC_APB1Periph_UART5



	/* RS485芯片发送使能GPIO, PB2 */
	#define RCC_RS485_TXEN 	RCC_AHB1Periph_GPIOB
	#define PORT_RS485_TXEN  GPIOB
	#define PIN_RS485_TXEN	 GPIO_Pin_2

	#define RS485_RX_EN()	PORT_RS485_TXEN->BSRRH = PIN_RS485_TXEN
	#define RS485_TX_EN()	PORT_RS485_TXEN->BSRRL = PIN_RS485_TXEN
#endif

/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4	/* UART5, PC12, PD2 */
}COM_PORT_E;

/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			115200
	#define UART1_TX_BUF_SIZE	1*1024
	#define UART1_RX_BUF_SIZE	1*1024
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			115200    //9600
	#define UART2_TX_BUF_SIZE	1*1024
	#define UART2_RX_BUF_SIZE	2*1024
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			9600
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif


/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	__IO uint16_t usTxWrite;	/* 发送缓冲区写指针 */
	__IO uint16_t usTxRead;		/* 发送缓冲区读指针 */
	__IO uint16_t usTxCount;	/* 等待发送的数据个数 */

	__IO uint16_t usRxWrite;	/* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;		/* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;	/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_T;

void bsp_InitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);
void comSetBaud(COM_PORT_E _ucPort, uint32_t _BaudRate);

void USART_SetBaudRate(USART_TypeDef* USARTx, uint32_t BaudRate);

void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);
void RS485_SendStr(char *_pBuf);
void RS485_SetBaud(uint32_t _baud);

void UART_5msMainFunction(void);
uint16_t Uart_GetFifoSize(COM_PORT_E port);
uint8_t Uart_GetFifo(COM_PORT_E port,uint8_t* ReadBuff,uint16_t ReadSize);
void PrintfLogo(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
