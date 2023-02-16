/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.c
*	��    �� : V1.0
*	˵    �� : ���ô����ж�+FIFOģʽʵ�ֶ�����ڵ�ͬʱ����
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*		V1.1    2013-06-09 armfly  FiFo�ṹ����TxCount��Ա�����������жϻ�������; ���� ��FiFo�ĺ���
*		V1.2	2014-09-29 armfly  ����RS485 MODBUS�ӿڡ����յ����ֽں�ֱ��ִ�лص�������
*		V1.3	2015-07-23 armfly  ���� UART_T �ṹ�Ķ�дָ�뼸����Ա������������ __IO ����,�����Ż���
*					�ᵼ�´��ڷ��ͺ���������
*		V1.4	2015-08-04 armfly  ���UART4����bug  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART1);
*		V1.5	2015-10-08 armfly  �����޸Ĳ����ʵĽӿں���
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "bsp_uart_fifo.h"



/* ����ÿ�����ڽṹ����� */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* ���ջ����� */
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
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* �����ȳ�ʼ��ȫ�ֱ���,������Ӳ�� */

	InitHardUart();		/* ���ô��ڵ�Ӳ������(�����ʵ�) */

	RS485_InitTXE();	/* ����RS485оƬ�ķ���ʹ��Ӳ��������Ϊ������� */

	ConfigUartNVIC();	/* ���ô����ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��ΪUARTָ��
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: uartָ��
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
		/* �����κδ��� */
		return 0;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��Ϊ USART_TypeDef* USARTx
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: USART_TypeDef*,  USART1, USART2, USART3, UART4, UART5
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
		/* �����κδ��� */
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: comSendBuf
*	����˵��: �򴮿ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucaBuf: �����͵����ݻ�����
*			  _usLen : ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);          // ��COM�˿ں�ת��ΪUARTָ��,�ٽ���Ӧ��UART����PUARTָ�� 
	if (pUart == 0)
	{
		return;
	}

	if (pUart->SendBefor != 0)
	{
		pUart->SendBefor();		/* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	�� �� ��: comSendChar
*	����˵��: �򴮿ڷ���1���ֽڡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucByte: �����͵�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

/*
*********************************************************************************************************
*	�� �� ��: comGetChar
*	����˵��: �ӽ��ջ�������ȡ1�ֽڣ��������������������ݾ��������ء�
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*			  _pByte: ���յ������ݴ���������ַ
*	�� �� ֵ: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
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
*	�� �� ��: comClearTxFifo
*	����˵��: ���㴮�ڷ��ͻ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
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
*	�� �� ��: comClearRxFifo
*	����˵��: ���㴮�ڽ��ջ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
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
*	�� �� ��: comSetBaud
*	����˵��: ���ô��ڵĲ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
*			  _BaudRate: �����ʣ�0-4500000�� ���4.5Mbps
*	�� �� ֵ: ��
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
*	�� �� ��: USART_SetBaudRate
*	����˵��: �޸Ĳ����ʼĴ������������������á����ʹ�� USART_Init����, ����޸�Ӳ�����ز�����RX,TX����
*			  ���ݹ̼����� USART_Init���������������ò����ʵĲ��ֵ����������װΪһ������
*	��    ��: USARTx : USART1, USART2, USART3, UART4, UART5
*			  BaudRate : �����ʣ�ȡֵ 0 - 4500000
*	�� �� ֵ: ��
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

/* �����RS485ͨ�ţ��밴���¸�ʽ��д������ ���ǽ����� USART3��ΪRS485������ */

/*
*********************************************************************************************************
*	�� �� ��: RS485_InitTXE
*	����˵��: ����RS485����ʹ�ܿ��� TXE
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_InitTXE(void)
{

}

/*
*********************************************************************************************************
*	�� �� ��: RS485_SetBaud
*	����˵��: �޸�485���ڵĲ����ʡ�
*	��    ��: _baud : ������.0-4500000
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_SetBaud(uint32_t _baud)
{
	comSetBaud(COM3, _baud);
}

/*
*********************************************************************************************************
*	�� �� ��: RS485_SendBefor
*	����˵��: ��������ǰ��׼������������RS485ͨ�ţ�������RS485оƬΪ����״̬��
*			  ���޸� UartVarInit()�еĺ���ָ����ڱ������������� g_tUart2.SendBefor = RS485_SendBefor
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_SendBefor(void)
{
//	RS485_TX_EN();	/* �л�RS485�շ�оƬΪ����ģʽ */
}

/*
*********************************************************************************************************
*	�� �� ��: RS485_SendOver
*	����˵��: ����һ�����ݽ�������ƺ�������RS485ͨ�ţ�������RS485оƬΪ����״̬��
*			  ���޸� UartVarInit()�еĺ���ָ����ڱ������������� g_tUart2.SendOver = RS485_SendOver
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_SendOver(void)
{
//	RS485_RX_EN();	/* �л�RS485�շ�оƬΪ����ģʽ */
}


/*
*********************************************************************************************************
*	�� �� ��: RS485_SendBuf
*	����˵��: ͨ��RS485оƬ����һ�����ݡ�ע�⣬���������ȴ�������ϡ�
*	��    ��: _ucaBuf : ���ݻ�����
*			  _usLen : ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen)
{
	comSendBuf(COM3, _ucaBuf, _usLen);
}


/*
*********************************************************************************************************
*	�� �� ��: RS485_SendStr
*	����˵��: ��485���߷���һ���ַ�����0������
*	��    ��: _pBuf �ַ�����0����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_SendStr(char *_pBuf)
{
	RS485_SendBuf((uint8_t *)_pBuf, strlen(_pBuf));
}

/*
*********************************************************************************************************
*	�� �� ��: RS485_ReciveNew
*	����˵��: ���յ��µ�����
*	��    ��: _byte ���յ���������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RS485_ReciveNew(uint8_t _byte)
{
}

/*
*********************************************************************************************************
*	�� �� ��: UartVarInit
*	����˵��: ��ʼ��������صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						/* STM32 �����豸 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* ���ͻ�����ָ�� */
	g_tUart1.pRxBuf = g_RxBuf1;					/* ���ջ�����ָ�� */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart1.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usTxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usRxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart1.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart1.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart1.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart1.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						/* STM32 �����豸 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* ���ͻ�����ָ�� */
	g_tUart2.pRxBuf = g_RxBuf2;					/* ���ջ�����ָ�� */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart2.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart2.usTxRead = 0;						/* ����FIFO������ */
	g_tUart2.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart2.usRxRead = 0;						/* ����FIFO������ */
	g_tUart2.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart2.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart2.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart2.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart2.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						/* STM32 �����豸 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* ���ͻ�����ָ�� */
	g_tUart3.pRxBuf = g_RxBuf3;					/* ���ջ�����ָ�� */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart3.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart3.usTxRead = 0;						/* ����FIFO������ */
	g_tUart3.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart3.usRxRead = 0;						/* ����FIFO������ */
	g_tUart3.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart3.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart3.SendBefor = RS485_SendBefor;		/* ��������ǰ�Ļص����� */
	g_tUart3.SendOver = RS485_SendOver;			/* ������Ϻ�Ļص����� */
	g_tUart3.ReciveNew = RS485_ReciveNew;		/* ���յ������ݺ�Ļص����� */
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = UART4;						/* STM32 �����豸 */
	g_tUart4.pTxBuf = g_TxBuf4;					/* ���ͻ�����ָ�� */
	g_tUart4.pRxBuf = g_RxBuf4;					/* ���ջ�����ָ�� */
	g_tUart4.usTxBufSize = UART4_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart4.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart4.usTxRead = 0;						/* ����FIFO������ */
	g_tUart4.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart4.usRxRead = 0;						/* ����FIFO������ */
	g_tUart4.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart4.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart4.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart4.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart4.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = UART5;						/* STM32 �����豸 */
	g_tUart5.pTxBuf = g_TxBuf5;					/* ���ͻ�����ָ�� */
	g_tUart5.pRxBuf = g_RxBuf5;					/* ���ջ�����ָ�� */
	g_tUart5.usTxBufSize = UART5_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart5.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart5.usTxRead = 0;						/* ����FIFO������ */
	g_tUart5.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart5.usRxRead = 0;						/* ����FIFO������ */
	g_tUart5.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart5.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart5.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart5.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart5.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

}

/*
*********************************************************************************************************
*	�� �� ��: InitHardUart
*	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ��ʺ���STM32-F4������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;

/***************************************************************************************************/
	
#if UART1_FIFO_EN == 1		/* ����1 TX = PA9   RX = PA10 �� TX = PB6   RX = PB7*/

	/* ��1���� ����GPIO */
	#if 1	/* TX = PA9   RX = PA10 */
		RCC_APB2PeriphClockCmd(UART1_GPIO_CLK, ENABLE);     		/* �� GPIO ʱ�� */

		RCC_APB2PeriphClockCmd(UART1_CLK, ENABLE);        /* �� UART ʱ�� */

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		/* �������Ϊ���� ����ģʽ */
		GPIO_InitStructure.GPIO_Pin = UART1_GPIO_TX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART1_GPIO_TX_PORT, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = UART1_GPIO_RX_PIN;
		GPIO_Init(UART1_GPIO_RX_PORT, &GPIO_InitStructure);
	#else	/* TX = PB6   RX = PB7  */
		/* �� GPIO ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* �� PB6 ӳ��Ϊ USART1_TX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);

		/* �� PB7 ӳ��Ϊ USART1_RX */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* ���� USART Tx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP | GPIO_Mode_AF_PP;		/* �������Ϊ���� ����ģʽ */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART2_FIFO_EN == 1		/* ����2 TX = PD5   RX = PD6 ��  TX = PA2�� RX = PA3  */
	/* ��1���� ����GPIO */
	#if 0	/* ����2 TX = PA2   RX = PA3 */
		/* �� GPIO ʱ�� */
		RCC_APB2PeriphClockCmd(UART2_GPIO_CLK, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB1PeriphClockCmd(UART2_CLK, ENABLE);

		/* �� PA2 ӳ��Ϊ USART2_TX */
		GPIO_PinAFConfig(UART2_GPIO_TX_PORT, UART2_GPIO_TX_SOURCE, GPIO_AF_USART2);

		/* �� PA3 ӳ��Ϊ USART2_RX */
		GPIO_PinAFConfig(UART2_GPIO_RX_PORT, UART2_GPIO_RX_SOURCE, GPIO_AF_USART2);

		/* ���� USART Tx Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP | GPIO_Mode_AF_PP;		/* �������Ϊ���� ����ģʽ */
		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART2_GPIO_TX_PORT, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_RX_PIN;
		GPIO_Init(UART2_GPIO_RX_PORT, &GPIO_InitStructure);

	#else	/* ����2   TX = PA2�� RX = PA3 */
		/* �� GPIO ʱ�� */
		RCC_APB2PeriphClockCmd(UART2_GPIO_CLK, ENABLE);

		/* �� UART ʱ�� */
		RCC_APB1PeriphClockCmd(UART2_CLK, ENABLE);

		/* �� PA2 ӳ��Ϊ USART2_TX. ��STM32-V5���У�PA2 �ܽ�������̫�� */
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);


		/* ���� USART Tx Ϊ���ù��� */
   GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_AF_PP;		/* �������Ϊ���� ����ģʽ */

		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART2_GPIO_TX_PORT, &GPIO_InitStructure);

		/* ���� USART Rx Ϊ���ù��� */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = UART2_GPIO_RX_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART2_GPIO_RX_PORT, &GPIO_InitStructure);
	#endif

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART2_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		/* ��ѡ�����ģʽ */
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART2, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART3_FIFO_EN == 1			/* ����3 TX = PB10   RX = PB11 */

	/* ���� PB2Ϊ��������������л� RS485оƬ���շ�״̬ */
//	{
//		RCC_APB1PeriphClockCmd(RCC_RS485_TXEN, ENABLE);

//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP | GPIO_Mode_AF_PP;		/* �������Ϊ���� ����ģʽ */
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

//		GPIO_InitStructure.GPIO_Pin = PIN_RS485_TXEN;
//		GPIO_Init(PORT_RS485_TXEN, &GPIO_InitStructure);
//	}

	/* �� GPIO ʱ�� */
	RCC_APB2PeriphClockCmd(UART3_GPIO_CLK, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB1PeriphClockCmd(UART3_CLK, ENABLE);



	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;		/* �������Ϊ���� ����ģʽ */

	GPIO_InitStructure.GPIO_Pin = UART3_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART3_GPIO_TX_PORT, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = UART3_GPIO_RX_PIN;
  GPIO_Init(UART3_GPIO_RX_PORT, &GPIO_InitStructure);

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART3_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART3, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART4_FIFO_EN == 1			/* ����4 TX = PC10   RX = PC11 */
	/* ��1���� ����GPIO */

	RCC_APB2PeriphClockCmd(UART4_GPIO_CLK, ENABLE);    	/* �� GPIO ʱ�� */

	RCC_APB1PeriphClockCmd(UART4_CLK, ENABLE);       	/* �� UART ʱ�� */

	/* �� PC10 ӳ��Ϊ UART4_TX */
//	GPIO_PinAFConfig(UART4_GPIO_TX_PORT, UART4_GPIO_TX_SOURCE, GPIO_AF_USART1);

	/* �� PC11 ӳ��Ϊ UART4_RX */
//	GPIO_PinAFConfig(UART4_GPIO_RX_PORT, UART4_GPIO_RX_SOURCE, GPIO_AF_USART1);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	/* ����ģʽ */
	GPIO_InitStructure.GPIO_Pin = UART4_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART4_GPIO_TX_PORT, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = UART4_GPIO_RX_PIN;
	GPIO_Init(UART4_GPIO_RX_PORT, &GPIO_InitStructure);

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART4_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	      /* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(UART4, ENABLE);		   /* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(UART4, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

/***************************************************************************************************/

#if UART5_FIFO_EN == 1			/* ����5 TX = PC12   RX = PD2 */
	/* ��1���� ����GPIO */

	RCC_APB2PeriphClockCmd(UART5_GPIO_TX_CLK |UART5_GPIO_RX_CLK, ENABLE);     	/* �� GPIO ʱ�� */

	RCC_APB1PeriphClockCmd(UART5_CLK, ENABLE);        	/* �� UART ʱ�� */

	/* �� PC12 ӳ��Ϊ UART5_TX */
//	GPIO_PinAFConfig(UART5_GPIO_TX_PORT, UART5_GPIO_TX_SOURCE, GPIO_AF_UART5);

	/* �� PD2 ӳ��Ϊ UART5_RX */
//	GPIO_PinAFConfig(UART5_GPIO_RX_PORT, UART5_GPIO_RX_SOURCE, GPIO_AF_UART5);

	/* ���� UART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	/* ����ģʽ */
	GPIO_InitStructure.GPIO_Pin = UART5_GPIO_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART5_GPIO_TX_PORT, &GPIO_InitStructure);

	/* ���� UART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = UART5_GPIO_RX_PIN;
	GPIO_Init(UART5_GPIO_RX_PORT, &GPIO_InitStructure);

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = UART5_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(UART5, ENABLE);		/* ʹ�ܴ��� */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(UART5, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
#endif

}

/*
*********************************************************************************************************
*	�� �� ��: ConfigUartNVIC
*	����˵��: ���ô���Ӳ���ж�.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	/*	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  --- �� bsp.c �� bsp_Init() �������ж����ȼ��� */

#if UART1_FIFO_EN == 1
	/* ʹ�ܴ���1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART2_FIFO_EN == 1
	/* ʹ�ܴ���2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART3_FIFO_EN == 1
	/* ʹ�ܴ���3�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART4_FIFO_EN == 1
	/* ʹ�ܴ���4�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART5_FIFO_EN == 1
	/* ʹ�ܴ���5�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

}

/*
*********************************************************************************************************
*	�� �� ��: UartSend
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//������_pUartָ�򴮿��豸�ṹ���ָ�룻_ucaBufָ�򻺳�����ָ�룻_usLen���ͳ��ȴ�С
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;           //i�����������͵����ݳ���

	for (i = 0; i < _usLen; i++)         
	{
		/* ������ͻ������Ѿ����ˣ���ȴ��������� */
	#if 0
		/*
			�ڵ���GPRS����ʱ������Ĵ������������while ��ѭ��
			ԭ�� ���͵�1���ֽ�ʱ _pUart->usTxWrite = 1��_pUart->usTxRead = 0;
			������while(1) �޷��˳�
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
		/* �� _pUart->usTxBufSize == 1 ʱ, ����ĺ���������(������) */
		while (1)
		{
			__IO uint16_t usCount;

			DISABLE_INT();
			usCount = _pUart->usTxCount;     //�������͵����ݸ�������usCount����
			ENABLE_INT();          //ʹ��ȫ���жϺ���

			 //��֤ÿ��д�뻺����ʱ���ܱ�֤�ȴ����͵ĸ�����С�ڻ���������Ĵ�С�����ȴ����͵����ݸ�����������������Ĵ�Сʱ������������ѭ����
			if (usCount < _pUart->usTxBufSize)      
			{
				break;
			}
		}
	#endif

		/* �����������뷢�ͻ����� */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];   //ͨ��������佫Ҫ���͵��������뻺����,ͬʱ�ڽ�Ҫ���͵����ݴ洢��������֮�󣬾Ϳ���ͨ�������жϷ�������

		DISABLE_INT();     // ��ֹȫ���ж�
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)   //�����ǰ��дָ���Ѿ��ƶ���FIFO�Ķ���ʱ����ָ����и�λ��
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;     //�ȴ����͵����ݸ���   
		ENABLE_INT();
	}

	USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);    //ʹ�ܴ��ڷ����ж�
}

/*
*********************************************************************************************************
*	�� �� ��: UartGetChar
*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
*	��    ��: _pUart : �����豸
*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	ENABLE_INT();

	/* �������д������ͬ���򷵻�0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* �Ѿ��]�������򷵻� */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* �Ӵ��ڽ���FIFOȡ1������ */   //*_pByte���Ǵӻ�����������ֵ

		/* ��дFIFO������ */
		DISABLE_INT();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)    //pUart->usRxRead�ȼ�һ���� _pUart->usRxBufSize���Ƚ�
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
*	�� �� ��: Uart_GetFifoSize
*	����˵��: 
*	��    ��: _pUart : �����豸
*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
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
*	�� �� ��: UartIRQ
*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
*	��    ��: _pUart : �����豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{
	/* ��������ж�  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
		uint8_t ch;

		//�ڽ����ж������ȡ��������λ�Ĵ�����ֵ����ֵ�洢��FIFO�У�Ȼ��ͨ��UartGetChar()������ȡ��������ֵ
		ch = USART_ReceiveData(_pUart->uart);        //ͨ���⺯���ӽ�����λ�Ĵ����ж�ȡ�յ�������
		_pUart->pRxBuf[_pUart->usRxWrite] = ch;      //�����ݴ�����ջ�������ÿ�����ж�һ�δ�ȡһ��
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)    //�������дָ���ƶ���FIFO�Ķ��ˣ���λ����дָ��
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)     //�����δ��ȡ�������ݸ���С�ڻ�������С�����������δ��ȡ���ݸ��� 
		{
			_pUart->usRxCount++;
		}

		/* �ص�����,֪ͨӦ�ó����յ�������,һ���Ƿ���1����Ϣ��������һ����� */
		//if (_pUart->usRxWrite == _pUart->usRxRead)
		//if (_pUart->usRxCount == 1)
		{
			if (_pUart->ReciveNew)
			{
				_pUart->ReciveNew(ch);
			}
		}
	}

	/* �����ͻ��������ж� */
	if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

			/* ʹ�����ݷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
		}
		else
		{
			/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);  //�������ͨ�����ÿ⺯��ʵ��
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)      //������Ͷ�ָ���ƶ���������FIFO�Ķ��ˣ���λ���Ͷ�ָ��
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;    //ÿ����һ���ֽ�֮����������ݸ�����һ
		}

	}
	/* ����bitλȫ��������ϵ��ж� */
	else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);

			/* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
			if (_pUart->SendOver)
			{
				_pUart->SendOver();
			}
		}
		else
		{
			/* ��������£��������˷�֧ */

			/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */   //��������£�������������֧
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);      
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)        //����˷�֧���ٴη��ͻ�������ֵ
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	����˵��: USART�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf�������������� */
	comSendChar(COM1, ch);

	return ch;
#else	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
	/* дһ���ֽڵ�USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��getchar�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 1	/* �Ӵ��ڽ���FIFO��ȡ1������, ֻ��ȡ�����ݲŷ��� */
	uint8_t ucData;

	while(comGetChar(COM1, &ucData) == 0);

	return ucData;
#else
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
#endif
}


void my_printf(void)
{
	printf("����Ӳ����ʱ���ɹ�\r\n");
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
void UART_5msMainFunction(void)      //������ýӿ�
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
								printf("�����ʱ��1 is ok\r\n");
						}
						else
						{
						
						}
						break;
					case '3':
						time = bsp_GetRunTime();
						printf("ϵͳ����ʱ��%d\r\n",time);
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
//					BEEP_ON;                          //Beep_10msMainFunction����򿪵Ļ��ᱻӰ���������
						break;
					case '9':
//				  GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN); 
//Beep_10msMainFunction  ������򿪵Ļ�Set   Reset�ᱻӰ�죨��ʵ���ˣ����ܿ�ͱ���������ж������رգ�����������Beep_10msMainFunctionһֱѭ�����룬�����������ж������ѷ��������ˣ�
						BEEP_DISABLE();                   //Beep_10msMainFunction����򿪵Ļ��ᱻӰ���������
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




/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

#define FIRMWARE_LIB_VERSION_NUM       "V3.5.0"
#define CHIP_ID				            		 "STM32F103ZET6"
#define INTERNAL_FLASH_START_ADDR			 ((uint32_t)0x08008000)
#define INTERNAL_FLASH_END_ADDR    		 ((uint32_t)0x0800C000)
#define INTERNAL_FLASH_SIZE						 (((uint32_t)0x0800C000)-((uint32_t)0x08008000))
#define INTERNAL_FLASH_PAGE_SIZE			 (INTERNAL_FLASH_SIZE/((uint16_t)0x800))    //һҳΪ2048   ��ÿҳ��С2KByte
#define DOMINANT_FREQUENCY       				(SystemCoreClock/1000000)
#define INTERNAL_SRAM_START_ADDR				0x20000000
#define INTERNAL_SRAM_SIZE							0x10000






/*����STM32 MCU������*/
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
 
uint32_t idAddr[]={0x1FFFF7AC,  /*STM32F0ΨһID��ʼ��ַ*/
                0x1FFFF7E8,  /*STM32F1ΨһID��ʼ��ַ*/
                0x1FFF7A10,  /*STM32F2ΨһID��ʼ��ַ*/
                0x1FFFF7AC,  /*STM32F3ΨһID��ʼ��ַ*/
                0x1FFF7A10,  /*STM32F4ΨһID��ʼ��ַ*/
                0x1FF0F420,  /*STM32F7ΨһID��ʼ��ַ*/
                0x1FF80050,  /*STM32L0ΨһID��ʼ��ַ*/
                0x1FF80050,  /*STM32L1ΨһID��ʼ��ַ*/
                0x1FFF7590,  /*STM32L4ΨһID��ʼ��ַ*/
                0x1FF0F420}; /*STM32H7ΨһID��ʼ��ַ*/
 
/*��ȡMCU��ΨһID*/
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
	
	printf("�̼���汾�ţ�%s\r\n", FIRMWARE_LIB_VERSION_NUM);
	printf("оƬID��%0x%0x%0x\r\n", *(__IO u32*)(0X1FFFF7E8),*(__IO u32*)(0X1FFFF7EC),*(__IO u32*)(0X1FFFF7F0));
	printf("�ڲ�FLASH��ʼ��ַ��%x\r\n", FLASH_BASE);
	printf("�ڲ�FLASH��С��%x\r\n", *(__IO u32*)(0X1FFFF7E0));
	printf("�ڲ�FLASHҳ��С��%x\r\n", INTERNAL_FLASH_PAGE_SIZE);
	printf("��Ƶ��%dMHZ\r\n", DOMINANT_FREQUENCY);
	printf("�ڲ�SRAM��ʼ��ַ��%x\r\n", SRAM_BASE);
	printf("�ڲ�SRAM��С��%x\r\n", INTERNAL_SRAM_SIZE);

}








