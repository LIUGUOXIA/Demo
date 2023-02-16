/*******************************************************************************
|  File Name:  StdLib.h
|  Description:  Implementation of the Lib Management
|-------------------------------------------------------------------------------
| (c) This software is the proprietary of NingBo Joyson New Energy(JOYSON).
|     All rights are reserved by JOYSON.
|-------------------------------------------------------------------------------
| Initials      Name                   Company
| --------      --------------------   -----------------------------------------
| XYJ           YaJun Xia              JOYSON
|-------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-------------------------------------------------------------------------------
| Date          Version      Author    Description
| ------------  --------     -------   ------------------------------------
| 2016-11-30    01.00.00     XYJ       Creation
|
|******************************************************************************/
/* polyspace:begin<MISRA-C3:D4.9:Not a defect:Justified> Normal */
#if !defined (_STD_LIB_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_LIB_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
//#include "STD_Lib_Cfg.h"
#include "stm32f10x.h"
#include "std_type.h"


/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define LIB_SET_UINT32_BIT(Data, Offset)    ((Data) |= (uint32)((uint32)1u << (Offset)))       //32位Data的第Offset位置1
#define LIB_SET_UINT16_BIT(Data, Offset)    ((Data) |= (uint16)((uint16)1u << (Offset)))       //16位Data的第Offset位置1
#define LIB_SET_UINT8_BIT(Data, Offset)     ((Data) |= (uint8)((uint8) 1u << (Offset)))        //8位Data的第Offset位置1

#define LIB_RESET_UINT32_BIT(Data, Offset)  ((Data) &= (uint32)(~(uint32)((uint32)1u << (Offset))))      //32位Data的第Offset位清0
#define LIB_RESET_UINT16_BIT(Data, Offset)  ((Data) &= (uint16)(~(uint16)((uint16)1u << (Offset))))      //16位Data的第Offset位清0
#define LIB_RESET_UINT8_BIT(Data, Offset)   ((Data) &= (uint8 )(~(uint8 )((uint8 )1u << (Offset))))      //8位Data的第Offset位清0

#define LIB_GET_UINT32_BIT(Data, Offset)    (((uint8)(((uint32) (Data)) >> (Offset))) & (uint8)0x01u)
#define LIB_GET_UINT16_BIT(Data, Offset)    (((uint8)(((uint16) (Data)) >> (Offset))) & (uint8)0x01u)
#define LIB_GET_UINT8_BIT(Data, Offset)     (((uint8)(((uint8 ) (Data)) >> (Offset))) & (uint8)0x01u)

#define LIB_GET_BYTE_0(Data)                ((uint8) (Data))
#define LIB_GET_BYTE_1(Data)                ((uint8)((Data)>>(uint8)8u))
#define LIB_GET_BYTE_2(Data)                ((uint8)((Data)>>(uint8)16u))
#define LIB_GET_BYTE_3(Data)                ((uint8)((Data)>>(uint8)24u))

#define LIB_GET_BYTE_LIT_UINT16(Data)		((uint16)Data[0u] + ((uint16)Data[1u] << 8u))
#define LIB_GET_BYTE_BIG_UINT16(Data)		((uint16)Data[1u] + ((uint16)Data[0u] << 8u))



#define M_PI        3.14159265358979323846
#define NUMBER      6370693.4856530580439461631130889
#define MEM_MOD_LITLE   0//
#define MEM_MOD_BIG     1//




/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.4:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:5.7:Not a defect:Justified> NO fluence */





typedef struct Type_FilterStruct
{
    uint8_t ucStatus;
    uint8_t ucLastStatus;
    uint8_t ucValidStatus;
    uint16_t usFilterCount;
/* polyspace<MISRA-C3:5.6:Not a defect:Justified> NO fluence */
} LibFilterStruct;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern unsigned int GetCRC16(unsigned char *ptr, u16 len);
extern void LIB_SetMemory(uint8_t *pStr, uint8_t val, uint16_t len);
extern uint8_t LIB_Compare(const uint8_t *pSrc, const uint8_t *pDes, uint16_t len);
extern uint8_t LIB_CompareR(const uint8_t *pSrc, const uint8_t *pDes, uint16_t len);
extern void LIB_Copy(uint8_t *pDes, const uint8_t *pSrc, uint16_t len);
extern void LIB_CopyR(uint8_t *pDes, const uint8_t *pSrc, uint16_t len);
extern uint16_t LIB_CalculateAverage(const uint16_t *pData, uint8_t length);
extern void LIB_ReadData(uint8_t *pDes, uint32_t pSrcAdd, uint32_t len);
extern void LIB_WriteData( uint8_t *pSrc, uint32_t pSrcAdd, uint32_t len );
extern uint8_t LIB_StatusFilter(LibFilterStruct *pFilterStatus, uint16_t filterMaxCnt);
uint8_t LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16_t filterInMaxCnt ,uint16_t filterOutMaxCnt);
extern uint16 LIB_FifoFillter(uint16 *pData, uint16 len);


extern uint8_t LIB_DecToUint8(uint8_t * str,uint8_t len);
extern uint16_t  LIB_hex2uint(uint8_t * str);
extern uint32_t  LIB_dec2uint(uint8_t * str);
extern void  LIB_str2uint(uint8_t * str,uint16_t *substrlen);
extern uint16_t LIB_DecToUint16(uint8_t * str,uint8_t len);
extern void LIB_Uint8ToDec2(uint8_t value,uint8_t* destbuff);
extern uint8_t LIB_Uint8ToDec(uint8_t value,uint8_t* destbuff);	
extern uint8_t LIB_Uint16ToDec(uint16_t value,uint8_t* dest);
extern uint8_t * LIB_SearchString(uint8_t *str,uint16_t strlen,char *substr,uint8_t substrlen);
extern uint8_t LIB_Uint32IPDatatoDecWithDot(uint32_t IPData,uint8_t* destbuff);
extern uint16_t LIB_HexToASCII(uint8_t* data, uint8_t* buffer, uint16_t len);
extern uint8_t LIB_ASCIITOHex(uint8_t* OData,uint8_t* NData,uint8_t len);
extern uint32_t LIB_StrToUint32(uint8_t *ptr, uint8_t mem_mod);
extern uint16_t LIB_StrToUint16(uint8_t *ptr, uint8_t mem_mod);
extern uint8_t* LIB_GetStringMiddle(uint8_t *str,uint16_t strlen, char str1,uint16_t Nostr1, char str2,uint16_t Nostr2,uint16_t *substrlen);
extern uint8_t LIB_SingleASCIITOHex(uint8_t NData);
extern uint8_t * LIB_GetStringBack(uint8_t *str,uint16_t strlen,char str1,uint16_t Nostr1);
extern uint8_t LIB_Hex2ToUint8(uint8_t *ahex);
extern uint8_t LIB_Dec3ToUint8(uint8_t * ahex,uint8_t len);
extern uint8_t LIB_Dec2ToUint8(uint8_t *ahex);
extern void LIB_HexToDatabuff(uint8_t *buff,uint8_t *hex,uint16_t hexlen);
extern void LIB_ConvertBCDB(uint8_t *Abuff,uint8_t len);	
extern void LIB_Uint8ToHex2(uint8_t value,uint8_t* destbuff);
extern void LIB_DataToHexbuff(uint8_t* dest,uint8_t* src,uint8_t srclen);
extern double LIB_deg2rad(double deg);
extern double LIB_distance(double lat1,double lon1,double lat2,double lon2);
extern uint8_t LIB_Lrc(uint8_t *p, uint16_t len);
extern uint16_t LIB_DigitFilter1(uint16_t*p, uint16_t len, uint16_t offsize);
extern uint8_t LIB_floatToByte(float fValue, uint8_t *arr);
extern float LIB_byteToFloat(uint8_t *arr);

#endif

/*EOF*/
