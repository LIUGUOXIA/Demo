/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.h
*	˵    �� : ͷ�ļ�
*
*	Copyright:
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#include "stm32f10x.h"

/*
	�����Ҫ���Ĵ��ڶ�Ӧ�Ĺܽţ��������޸� bsp_uart_fifo.c�ļ��е� static void InitHardUart(void)����
*/

/* ����ʹ�ܵĴ���, 0 ��ʾ��ʹ�ܣ������Ӵ����С���� 1��ʾʹ�� */
#ifdef STM32_X3	/* ������ STM32-X4 ������ */
	/* ������STM32-X3�����崮�ڷ��䣺
		PA9/USART1_TX	  --- ��ӡ���Կ�
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
		�Ե�STM32-V2 ���ڷ��䣺
		������1�� 
			PA9/USART1_TX	  --- ��ӡ���Կ�
			PA10/USART1_RX

		������2�� PA2 �ܽ�������̫���� RX�ܽ����ڽ���GPS�ź�
			PA2/USART2_TX/ETH_MDIO (������̫�����������ڷ�����)
			PA3/USART2_RX	;��GPSģ�����

		������3�� RS485 ͨ�� - TTL ���� �� ����
			PB10/USART3_TX
			PB11/USART3_RX

		������4�� --- ���������á�
			PC10/USART4_TX
			PC11/USART4_RX
		������5�� --- ���������á�
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



/* ����1��GPIO --- RS323 */
#define UART1_GPIO_TX_PORT      GPIOA
#define UART1_GPIO_TX_PIN       GPIO_Pin_9
#define UART1_GPIO_TX_SOURCE    GPIO_PinSource9

#define UART1_GPIO_RX_PORT      GPIOA
#define UART1_GPIO_RX_PIN       GPIO_Pin_10
#define UART1_GPIO_RX_SOURCE    GPIO_PinSource10

#define UART1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define UART1_CLK               RCC_APB2Periph_USART1

/* ����2��GPIO  --- ֻ����RX�� û��TX��Ӳ��û�нӣ�����0ŷķ���ϣ� */
#define UART2_GPIO_TX_PORT      GPIOA
#define UART2_GPIO_TX_PIN       GPIO_Pin_2
#define UART2_GPIO_TX_SOURCE    GPIO_PinSource2

#define UART2_GPIO_RX_PORT      GPIOA
#define UART2_GPIO_RX_PIN       GPIO_Pin_3
#define UART2_GPIO_RX_SOURCE    GPIO_PinSource3

#define UART2_GPIO_CLK          RCC_APB2Periph_GPIOA
#define UART2_CLK								RCC_APB1Periph_USART2

/* ����3��GPIO - RS485 */
#define UART3_GPIO_TX_PORT      GPIOB
#define UART3_GPIO_TX_PIN       GPIO_Pin_10
#define UART3_GPIO_TX_SOURCE    GPIO_PinSource10

#define UART3_GPIO_RX_PORT      GPIOB
#define UART3_GPIO_RX_PIN       GPIO_Pin_11
#define UART3_GPIO_RX_SOURCE    GPIO_PinSource11

#define UART3_GPIO_CLK          RCC_APB2Periph_GPIOB
#define UART3_CLK								RCC_APB1Periph_USART3

/* ����4��GPIO - ��SDIO�ӿ� */
#define UART4_GPIO_TX_PORT      GPIOC
#define UART4_GPIO_TX_PIN       GPIO_Pin_10
#define UART4_GPIO_TX_SOURCE    GPIO_PinSource10

#define UART4_GPIO_RX_PORT      GPIOC
#define UART4_GPIO_RX_PIN       GPIO_Pin_11
#define UART4_GPIO_RX_SOURCE    GPIO_PinSource11

#define UART4_GPIO_CLK          RCC_APB2Periph_GPIOC
#define UART4_CLK							  RCC_APB1Periph_UART4

/* ����5��GPIO - ��SDIO�ӿ� */
#define UART5_GPIO_TX_PORT      GPIOC
#define UART5_GPIO_TX_PIN       GPIO_Pin_12
#define UART5_GPIO_TX_CLK       RCC_APB2Periph_GPIOC
#define UART5_GPIO_TX_SOURCE    GPIO_PinSource12

#define UART5_GPIO_RX_PORT      GPIOD
#define UART5_GPIO_RX_PIN       GPIO_Pin_2
#define UART5_GPIO_RX_CLK       RCC_APB2Periph_GPIOD
#define UART5_GPIO_RX_SOURCE    GPIO_PinSource2

#define UART5_CLK								RCC_APB1Periph_UART5



	/* RS485оƬ����ʹ��GPIO, PB2 */
	#define RCC_RS485_TXEN 	RCC_AHB1Periph_GPIOB
	#define PORT_RS485_TXEN  GPIOB
	#define PIN_RS485_TXEN	 GPIO_Pin_2

	#define RS485_RX_EN()	PORT_RS485_TXEN->BSRRH = PIN_RS485_TXEN
	#define RS485_TX_EN()	PORT_RS485_TXEN->BSRRL = PIN_RS485_TXEN
#endif

/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4	/* UART5, PC12, PD2 */
}COM_PORT_E;

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
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


/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	__IO uint16_t usTxWrite;	/* ���ͻ�����дָ�� */
	__IO uint16_t usTxRead;		/* ���ͻ�������ָ�� */
	__IO uint16_t usTxCount;	/* �ȴ����͵����ݸ��� */

	__IO uint16_t usRxWrite;	/* ���ջ�����дָ�� */
	__IO uint16_t usRxRead;		/* ���ջ�������ָ�� */
	__IO uint16_t usRxCount;	/* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
