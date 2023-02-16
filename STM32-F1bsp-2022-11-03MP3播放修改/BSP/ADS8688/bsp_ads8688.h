#ifndef __BSP_ADS8688_H_
#define __BSP_ADS8688_H_

#include "stm32f10x.h"
/*****************************************************************************************/
#define SPI_CS_PORT				GPIOB
#define SPI_CS_PIN				GPIO_Pin_4

#define SPI_MISO_PORT			GPIOB
#define SPI_MISO_PIN			GPIO_Pin_3

#define SPI_MOSI_PORT			GPIOC
#define SPI_MOSI_PIN			GPIO_Pin_12

#define SPI_SCK_PORT			GPIOA
#define SPI_SCK_PIN				GPIO_Pin_1

/*****************************************************************************************/

/*
MOSI  9    PC12
MISO  13   PB3
CS    10   PB4
SCK   2    PA1
*/

#define ADS8688_CS_L       GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define ADS8688_CS_H       GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define ADS8688_SCK_L     GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define ADS8688_SCK_H     GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define ADS8688_MOSI_L     GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define ADS8688_MOSI_H     GPIO_SetBits(GPIOC,GPIO_Pin_12)

#define ADS8688_MISO       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)   


//通道测量部分
#define _10_10V  0X00
#define _0_10V   0X01
#define _1_5V    0X02
#define _0_20mA  0X03
#define _4_20mA  0X04


//Command Register 

#define MAN_Ch_0	0XC000
#define MAN_Ch_1	0XC400
#define MAN_Ch_2	0XC800
#define MAN_Ch_3	0XCC00
#define MAN_Ch_4	0XD000
#define MAN_Ch_5	0XD400
#define MAN_Ch_6	0XD800
#define MAN_Ch_7	0XDC00
#define MAN_AUX		0XE000

//Program Register

#define Channel Power Down 		  0X02
#define Feature Select			    0X03

#define Channel_0_Input_Range		0X05
#define Channel_1_Input_Range		0X06
#define Channel_2_Input_Range		0X07
#define Channel_3_Input_Range		0X08
#define Channel_4_Input_Range		0X09
#define Channel_5_Input_Range		0X0A
#define Channel_6_Input_Range		0X0B
#define Channel_7_Input_Range		0X0C

#define Ch_0_Hysteresis				    0X15
#define Ch_0_High_Threshold_MSB		0X16
#define Ch_0_High_Threshold_LSB		0X17
#define Ch_0_Low_Threshold_MSB		0X18
#define Ch_0_Low_Threshold_LSB		0X19

#define Ch_7_Hysteresis				    0X38
#define Ch_7_High_Threshold_MSB		0X39
#define Ch_7_High_Threshold_LSB		0X3A
#define Ch_7_Low_Threshold_MSB		0X3B
#define Ch_7_Low_Threshold_LSB		0X3C

#define Command_Read_Back		0X3F	

#define WRITE 1
#define READ 0

#define CH7_EN  0X80
#define CH6_EN  0X40
#define CH5_EN  0X20
#define CH4_EN  0X10
#define CH3_EN  0X08
#define CH2_EN  0X04
#define CH1_EN  0X02
#define CH0_EN  0X01

#define CH7_PD  0X80
#define CH6_PD  0X40
#define CH5_PD  0X20
#define CH4_PD  0X10
#define CH3_PD  0X08
#define CH2_PD  0X04
#define CH1_PD  0X02
#define CH0_PD  0X01

#define VREF_25_25		  0X00      //±2.5*ref     ref=4.096V   ±10.24V
#define VREF_125_125	  0X01      //±1.25*ref    ±5.12V
#define VREF_0625_0625	0X02      //±0.625*ref   ±2.56V
#define VREF_0_25		    0X05      //0-2.5*ref     0-10.24V
#define VREF_0_125		  0X06      //0-1.25*ref    0-5.12V








/*************************************************************************************************************/
//命令寄存器 
#define NO_OP			0x0000
/************************************************************
在选定模式下继续操作  
在设备操作期间将SDI线连续保持为低电平（相当于向所有16位写入0）将继续在最后选择的模式（STDBY，PWR_DN或AUTO_RST，MAN_Ch_n）下进行设备操作。
在这种模式下，设备将遵循程序寄存器（地址01h至3Ch）中已配置的相同设置。
*************************************************************/
#define STDBY			0x8200 
/************************************************************
待机模式   
器件支持低功耗待机模式（STDBY），在该模式下，仅部分电路断电。
内部基准电压源和缓冲器未断电，在退出STDBY模式时，可以在20 μs内为器件加电。退出STDBY模式时，程序寄存器不会复位为默认值。
*************************************************************/
#define PWR_DN  	0x8300
/************************************************************
掉电模式（PWR_DN）
器件支持硬件和软件掉电模式（PWR_DN），在该模式下，所有内部电路均被掉电，包括内部基准电压源和缓冲器。
如果设备在内部参考模式下运行（REFSEL = 0），则退出PWR_DN模式后，设备加电并转换所选模拟输入通道至少需要15 ms的时间。
设备的硬件电源模式在RST / PD（输入）部分中说明。
硬件和软件掉电模式之间的主要区别在于，当设备从硬件掉电唤醒时，程序寄存器会重置为默认值，但是当设备从软件唤醒时，程序寄存器的先前设置会保留掉电。
*************************************************************/
#define RST				0x8500
/************************************************************
重置程序寄存器（RST）
器件支持硬件和软件复位（RST）模式，在该模式下，所有程序寄存器均复位为其默认值。也可以使用硬件引脚将设备置于RST模式
**************************************************************/
#define AUTO_RST	0xA000
/*************************************************************
自动通道启用
自动扫描所有模拟信道上的输入信号。自动扫描的通道序列可由程序寄存器中的自动扫描顺序控制寄存器（01h至02h）配置；请参阅程序寄存器映射部分。
在此模式下，设备以升序连续循环通过所选通道，从最低通道开始，转换程序寄存器中选择的所有通道。
序列完成后，设备返回到程序寄存器中的最低计数通道，并重复该序列。通过设置程序寄存器的范围选择寄存器，可以配置自动扫描序列中每个通道的输入电压范围。
***************************************************************/
#define MAN_CH_0	0xC000/**/
#define MAN_CH_1	0xC400/**/
#define MAN_CH_2	0xC800/**/
#define MAN_CH_3	0xCC00/**/
#define MAN_CH_4	0xD000/**/
#define MAN_CH_5	0xD400/**/
#define MAN_CH_6	0xD800/**/
#define MAN_CH_7	0xDC00
/************************************************************************
手动通道n选择（MAN_Ch_n）
通过在手动通道n扫描模式（MAN_Ch_n）中进行操作，可以对设备进行编程以转换特定的模拟输入通道。
通过在命令寄存器中写入有效的手动通道n选择命令（MAN_Ch_n）来完成该编程
*************************************************************************/
#define MAN_AUX		0XE000                  /*AUX通道*/



//程序寄存器
#define AUTO_SEQ_EN	0x01	
/*********************************************************
自动扫描序列使能寄存器
该寄存器选择单个通道以在AUTO_RST模式下进行排序。 
该寄存器的默认值为FFh，这意味着在默认条件下所有通道都包含在自动扫描序列中。
*********************************************************/
#define CH_PD				0x02	
/*********************************************************
通道掉电寄存器
该寄存器关闭掉AUTO_RST模式下不包含的单个通道的电源。 该寄存器的默认值为00h，这意味着在默认条件下所有通道均已上电。
*********************************************************/																
#define F_S					0X03	
/********************************************************
器件功能选择控制寄存器
该寄存器中的位可用于配置菊花链操作的设备ID，并配置SDO上的输出位格式。
********************************************************/
			
			
//各通道范围选择寄存器	默认值为00h		 				
#define CHIR_0		0x05/**/
#define CHIR_1		0x06/**/
#define CHIR_2		0x07/**/
#define CHIR_3		0x08/**/
#define CHIR_4		0x09/**/
#define CHIR_5		0x0A/**/
#define CHIR_6		0x0B/**/
#define CHIR_7		0x0C/**/

#define CMD_READ	0x3F	
/********************************************************
命令回读寄存器
该寄存器允许用户读取设备的操作模式。 执行此命令后，设备将输出在前一个数据帧中执行的命令字。
有关命令寄存器的所有信息在前8位中包含，并且后8位为0。
*********************************************************/											


#define REF 4083
//输入范围
#define ADS8688_IR_N2_5V    0x00  //±2.5*ref  ref=4.096V  ±10.24V
#define ADS8688_IR_N1_25V   0x01  //±1.25*ref    ±5.12V
#define ADS8688_IR_N0_625V  0x02  //±0.625*ref   ±2.56V
#define ADS8688_IR_2_5V   	0x05  //0-2.5*ref      0-10.24V
#define ADS8688_IR_1_25V    0x06  //0-1.25*ref     0-5.12V

#define CONST_N2_5V_LSB_mV  	2.5*2*REF/65535
#define CONST_N1_25V_LSB_mV  	1.25*2*REF/65535
#define CONST_N0_625V_LSB_mV  0.625*2*REF/65535
#define CONST_2_5V_LSB_mV  		2.5*REF/65535
#define CONST_1_25V_LSB_mV 	 	1.25*REF/65535

//通道测量部分
#define _10_10V  0X00
#define _0_10V   0X01
#define _1_5V    0X02
#define _0_20mA  0X03
#define _4_20mA  0X04

#define VREF_25_25			0X00
#define VREF_125_125		0X01
#define VREF_0625_0625	0X02
#define VREF_0_25				0X05
#define VREF_0_125			0X06





void ADS8688_GPIO_Init(void);
uint8_t ADS8688_SPI_Read8Bit(void);
void ADS8688_SPI_Write8Bit(uint8_t data);
void ADS8688_WriteCmd(uint16_t cmd);
void SOFT_RESET(void);
void AUTO_RST_Mode(void);
void MAN_CH_Mode(uint16_t ch);
uint8_t ADS8688_ReadReg(uint8_t addr);
void ADS8688_WriteReg(uint8_t addr,uint8_t data);
void Set_Auto_Scan_Sequence(uint8_t seq);
void Set_CH_Range(uint8_t ch,uint8_t range);
void Get_AUTO_RST_Mode_Data(uint16_t *data, uint8_t chnum);
uint16_t Get_MAN_CH_Mode_Data(void);





#endif


