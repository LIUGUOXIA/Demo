/*******************************************************************************
|  File Name:  STD_Crc.c
|  Description:  Implementation of the crc Management
|-------------------------------------------------------------------------------
| (c) This software is the proprietary of NingBo Joyson New Energy(JOYSON).
|     All rights are reserved by JOYSON.
|-------------------------------------------------------------------------------
| Initials      Name                   Company
| --------      --------------------   -----------------------------------------
| XYJ           YaJun Xia              CAEA
|-------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-------------------------------------------------------------------------------
| Date          Version      Author    Description
| ------------  --------     -------   ------------------------------------
| 2021-08-23    01.00.00     XYJ       Creation
|
|******************************************************************************/

/*******************************************************************************
|******************************************************************************/
#include "STD_Crc.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Global NVM KAM Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/****************************************************************************************
 				            Global Variable Definition Section
*****************************************************************************************/


#if (( STD_CRCCFG_CLASS_CRC8 == STD_CRC_ENABLE)&& (STD_CRCCFG_8_MODE == STD_CRC_8_TABLE))
static const uint8 StdCrc_Table8[STD_BufferSize] =
{
    0x00u, 0x1du, 0x3au, 0x27u,
    0x74u, 0x69u, 0x4eu, 0x53u,
    0xe8u, 0xf5u, 0xd2u, 0xcfu,
    0x9cu, 0x81u, 0xa6u, 0xbbu,
    0xcdu, 0xd0u, 0xf7u, 0xeau,
    0xb9u, 0xa4u, 0x83u, 0x9eu,
    0x25u, 0x38u, 0x1fu, 0x02u,
    0x51u, 0x4cu, 0x6bu, 0x76u,
    0x87u, 0x9au, 0xbdu, 0xa0u,
    0xf3u, 0xeeu, 0xc9u, 0xd4u,
    0x6fu, 0x72u, 0x55u, 0x48u,
    0x1bu, 0x06u, 0x21u, 0x3cu,
    0x4au, 0x57u, 0x70u, 0x6du,
    0x3eu, 0x23u, 0x04u, 0x19u,
    0xa2u, 0xbfu, 0x98u, 0x85u,
    0xd6u, 0xcbu, 0xecu, 0xf1u,
    0x13u, 0x0eu, 0x29u, 0x34u,
    0x67u, 0x7au, 0x5du, 0x40u,
    0xfbu, 0xe6u, 0xc1u, 0xdcu,
    0x8fu, 0x92u, 0xb5u, 0xa8u,
    0xdeu, 0xc3u, 0xe4u, 0xf9u,
    0xaau, 0xb7u, 0x90u, 0x8du,
    0x36u, 0x2bu, 0x0cu, 0x11u,
    0x42u, 0x5fu, 0x78u, 0x65u,
    0x94u, 0x89u, 0xaeu, 0xb3u,
    0xe0u, 0xfdu, 0xdau, 0xc7u,
    0x7cu, 0x61u, 0x46u, 0x5bu,
    0x08u, 0x15u, 0x32u, 0x2fu,
    0x59u, 0x44u, 0x63u, 0x7eu,
    0x2du, 0x30u, 0x17u, 0x0au,
    0xb1u, 0xacu, 0x8bu, 0x96u,
    0xc5u, 0xd8u, 0xffu, 0xe2u,
    0x26u, 0x3bu, 0x1cu, 0x01u,
    0x52u, 0x4fu, 0x68u, 0x75u,
    0xceu, 0xd3u, 0xf4u, 0xe9u,
    0xbau, 0xa7u, 0x80u, 0x9du,
    0xebu, 0xf6u, 0xd1u, 0xccu,
    0x9fu, 0x82u, 0xa5u, 0xb8u,
    0x03u, 0x1eu, 0x39u, 0x24u,
    0x77u, 0x6au, 0x4du, 0x50u,
    0xa1u, 0xbcu, 0x9bu, 0x86u,
    0xd5u, 0xc8u, 0xefu, 0xf2u,
    0x49u, 0x54u, 0x73u, 0x6eu,
    0x3du, 0x20u, 0x07u, 0x1au,
    0x6cu, 0x71u, 0x56u, 0x4bu,
    0x18u, 0x05u, 0x22u, 0x3fu,
    0x84u, 0x99u, 0xbeu, 0xa3u,
    0xf0u, 0xedu, 0xcau, 0xd7u,
    0x35u, 0x28u, 0x0fu, 0x12u,
    0x41u, 0x5cu, 0x7bu, 0x66u,
    0xddu, 0xc0u, 0xe7u, 0xfau,
    0xa9u, 0xb4u, 0x93u, 0x8eu,
    0xf8u, 0xe5u, 0xc2u, 0xdfu,
    0x8cu, 0x91u, 0xb6u, 0xabu,
    0x10u, 0x0du, 0x2au, 0x37u,
    0x64u, 0x79u, 0x5eu, 0x43u,
    0xb2u, 0xafu, 0x88u, 0x95u,
    0xc6u, 0xdbu, 0xfcu, 0xe1u,
    0x5au, 0x47u, 0x60u, 0x7du,
    0x2eu, 0x33u, 0x14u, 0x09u,
    0x7fu, 0x62u, 0x45u, 0x58u,
    0x0bu, 0x16u, 0x31u, 0x2cu,
    0x97u, 0x8au, 0xadu, 0xb0u,
    0xe3u, 0xfeu, 0xd9u, 0xc4u
};
#endif

#if (( STD_CRCCFG_CLASS_CRC16 == STD_CRC_ENABLE)&& (STD_CRCCFG_16_MODE == STD_CRC_16_TABLE))
static const uint16 StdCrc_Table16[STD_BufferSize] =
{
	0x0000u, 0x1021u, 0x2042u, 0x3063u, 0x4084u, 0x50a5u, 0x60c6u, 0x70e7u,
	0x8108u, 0x9129u, 0xa14au, 0xb16bu, 0xc18cu, 0xd1adu, 0xe1ceu, 0xf1efu,
	0x1231u, 0x0210u, 0x3273u, 0x2252u, 0x52b5u, 0x4294u, 0x72f7u, 0x62d6u,
	0x9339u, 0x8318u, 0xb37bu, 0xa35au, 0xd3bdu, 0xc39cu, 0xf3ffu, 0xe3deu,
	0x2462u, 0x3443u, 0x0420u, 0x1401u, 0x64e6u, 0x74c7u, 0x44a4u, 0x5485u,
	0xa56au, 0xb54bu, 0x8528u, 0x9509u, 0xe5eeu, 0xf5cfu, 0xc5acu, 0xd58du,
	0x3653u, 0x2672u, 0x1611u, 0x0630u, 0x76d7u, 0x66f6u, 0x5695u, 0x46b4u,
	0xb75bu, 0xa77au, 0x9719u, 0x8738u, 0xf7dfu, 0xe7feu, 0xd79du, 0xc7bcu,
	0x48c4u, 0x58e5u, 0x6886u, 0x78a7u, 0x0840u, 0x1861u, 0x2802u, 0x3823u,
	0xc9ccu, 0xd9edu, 0xe98eu, 0xf9afu, 0x8948u, 0x9969u, 0xa90au, 0xb92bu,
	0x5af5u, 0x4ad4u, 0x7ab7u, 0x6a96u, 0x1a71u, 0x0a50u, 0x3a33u, 0x2a12u,
	0xdbfdu, 0xcbdcu, 0xfbbfu, 0xeb9eu, 0x9b79u, 0x8b58u, 0xbb3bu, 0xab1au,
	0x6ca6u, 0x7c87u, 0x4ce4u, 0x5cc5u, 0x2c22u, 0x3c03u, 0x0c60u, 0x1c41u,
	0xedaeu, 0xfd8fu, 0xcdecu, 0xddcdu, 0xad2au, 0xbd0bu, 0x8d68u, 0x9d49u,
	0x7e97u, 0x6eb6u, 0x5ed5u, 0x4ef4u, 0x3e13u, 0x2e32u, 0x1e51u, 0x0e70u,
	0xff9fu, 0xefbeu, 0xdfddu, 0xcffcu, 0xbf1bu, 0xaf3au, 0x9f59u, 0x8f78u,
	0x9188u, 0x81a9u, 0xb1cau, 0xa1ebu, 0xd10cu, 0xc12du, 0xf14eu, 0xe16fu,
	0x1080u, 0x00a1u, 0x30c2u, 0x20e3u, 0x5004u, 0x4025u, 0x7046u, 0x6067u,
	0x83b9u, 0x9398u, 0xa3fbu, 0xb3dau, 0xc33du, 0xd31cu, 0xe37fu, 0xf35eu,
	0x02b1u, 0x1290u, 0x22f3u, 0x32d2u, 0x4235u, 0x5214u, 0x6277u, 0x7256u,
	0xb5eau, 0xa5cbu, 0x95a8u, 0x8589u, 0xf56eu, 0xe54fu, 0xd52cu, 0xc50du,
	0x34e2u, 0x24c3u, 0x14a0u, 0x0481u, 0x7466u, 0x6447u, 0x5424u, 0x4405u,
	0xa7dbu, 0xb7fau, 0x8799u, 0x97b8u, 0xe75fu, 0xf77eu, 0xc71du, 0xd73cu,
	0x26d3u, 0x36f2u, 0x0691u, 0x16b0u, 0x6657u, 0x7676u, 0x4615u, 0x5634u,
	0xd94cu, 0xc96du, 0xf90eu, 0xe92fu, 0x99c8u, 0x89e9u, 0xb98au, 0xa9abu,
	0x5844u, 0x4865u, 0x7806u, 0x6827u, 0x18c0u, 0x08e1u, 0x3882u, 0x28a3u,
	0xcb7du, 0xdb5cu, 0xeb3fu, 0xfb1eu, 0x8bf9u, 0x9bd8u, 0xabbbu, 0xbb9au,
	0x4a75u, 0x5a54u, 0x6a37u, 0x7a16u, 0x0af1u, 0x1ad0u, 0x2ab3u, 0x3a92u,
	0xfd2eu, 0xed0fu, 0xdd6cu, 0xcd4du, 0xbdaau, 0xad8bu, 0x9de8u, 0x8dc9u,
	0x7c26u, 0x6c07u, 0x5c64u, 0x4c45u, 0x3ca2u, 0x2c83u, 0x1ce0u, 0x0cc1u,
	0xef1fu, 0xff3eu, 0xcf5du, 0xdf7cu, 0xaf9bu, 0xbfbau, 0x8fd9u, 0x9ff8u,
	0x6e17u, 0x7e36u, 0x4e55u, 0x5e74u, 0x2e93u, 0x3eb2u, 0x0ed1u, 0x1ef0u
};
#endif
#if (( STD_CRCCFG_CLASS_CRC32 == STD_CRC_ENABLE)&& (STD_CRCCFG_32_MODE == STD_CRC_32_TABLE))
static const uint32 StdCrc_Table32[STD_BufferSize] =
{
	0x00000000uL, 0x77073096uL, 0xEE0E612CuL, 0x990951BAuL,
	0x076DC419uL, 0x706AF48FuL, 0xE963A535uL, 0x9E6495A3uL,
	0x0EDB8832uL, 0x79DCB8A4uL, 0xE0D5E91EuL, 0x97D2D988uL,
	0x09B64C2BuL, 0x7EB17CBDuL, 0xE7B82D07uL, 0x90BF1D91uL,
	0x1DB71064uL, 0x6AB020F2uL, 0xF3B97148uL, 0x84BE41DEuL,
	0x1ADAD47DuL, 0x6DDDE4EBuL, 0xF4D4B551uL, 0x83D385C7uL,
	0x136C9856uL, 0x646BA8C0uL, 0xFD62F97AuL, 0x8A65C9ECuL,
	0x14015C4FuL, 0x63066CD9uL, 0xFA0F3D63uL, 0x8D080DF5uL,
	0x3B6E20C8uL, 0x4C69105EuL, 0xD56041E4uL, 0xA2677172uL,
	0x3C03E4D1uL, 0x4B04D447uL, 0xD20D85FDuL, 0xA50AB56BuL,
	0x35B5A8FAuL, 0x42B2986CuL, 0xDBBBC9D6uL, 0xACBCF940uL,
	0x32D86CE3uL, 0x45DF5C75uL, 0xDCD60DCFuL, 0xABD13D59uL,
	0x26D930ACuL, 0x51DE003AuL, 0xC8D75180uL, 0xBFD06116uL,
	0x21B4F4B5uL, 0x56B3C423uL, 0xCFBA9599uL, 0xB8BDA50FuL,
	0x2802B89EuL, 0x5F058808uL, 0xC60CD9B2uL, 0xB10BE924uL,
	0x2F6F7C87uL, 0x58684C11uL, 0xC1611DABuL, 0xB6662D3DuL,
	0x76DC4190uL, 0x01DB7106uL, 0x98D220BCuL, 0xEFD5102AuL,
	0x71B18589uL, 0x06B6B51FuL, 0x9FBFE4A5uL, 0xE8B8D433uL,
	0x7807C9A2uL, 0x0F00F934uL, 0x9609A88EuL, 0xE10E9818uL,
	0x7F6A0DBBuL, 0x086D3D2DuL, 0x91646C97uL, 0xE6635C01uL,
	0x6B6B51F4uL, 0x1C6C6162uL, 0x856530D8uL, 0xF262004EuL,
	0x6C0695EDuL, 0x1B01A57BuL, 0x8208F4C1uL, 0xF50FC457uL,
	0x65B0D9C6uL, 0x12B7E950uL, 0x8BBEB8EAuL, 0xFCB9887CuL,
	0x62DD1DDFuL, 0x15DA2D49uL, 0x8CD37CF3uL, 0xFBD44C65uL,
	0x4DB26158uL, 0x3AB551CEuL, 0xA3BC0074uL, 0xD4BB30E2uL,
	0x4ADFA541uL, 0x3DD895D7uL, 0xA4D1C46DuL, 0xD3D6F4FBuL,
	0x4369E96AuL, 0x346ED9FCuL, 0xAD678846uL, 0xDA60B8D0uL,
	0x44042D73uL, 0x33031DE5uL, 0xAA0A4C5FuL, 0xDD0D7CC9uL,
	0x5005713CuL, 0x270241AAuL, 0xBE0B1010uL, 0xC90C2086uL,
	0x5768B525uL, 0x206F85B3uL, 0xB966D409uL, 0xCE61E49FuL,
	0x5EDEF90EuL, 0x29D9C998uL, 0xB0D09822uL, 0xC7D7A8B4uL,
	0x59B33D17uL, 0x2EB40D81uL, 0xB7BD5C3BuL, 0xC0BA6CADuL,
	0xEDB88320uL, 0x9ABFB3B6uL, 0x03B6E20CuL, 0x74B1D29AuL,
	0xEAD54739uL, 0x9DD277AFuL, 0x04DB2615uL, 0x73DC1683uL,
	0xE3630B12uL, 0x94643B84uL, 0x0D6D6A3EuL, 0x7A6A5AA8uL,
	0xE40ECF0BuL, 0x9309FF9DuL, 0x0A00AE27uL, 0x7D079EB1uL,
	0xF00F9344uL, 0x8708A3D2uL, 0x1E01F268uL, 0x6906C2FEuL,
	0xF762575DuL, 0x806567CBuL, 0x196C3671uL, 0x6E6B06E7uL,
	0xFED41B76uL, 0x89D32BE0uL, 0x10DA7A5AuL, 0x67DD4ACCuL,
	0xF9B9DF6FuL, 0x8EBEEFF9uL, 0x17B7BE43uL, 0x60B08ED5uL,
	0xD6D6A3E8uL, 0xA1D1937EuL, 0x38D8C2C4uL, 0x4FDFF252uL,
	0xD1BB67F1uL, 0xA6BC5767uL, 0x3FB506DDuL, 0x48B2364BuL,
	0xD80D2BDAuL, 0xAF0A1B4CuL, 0x36034AF6uL, 0x41047A60uL,
	0xDF60EFC3uL, 0xA867DF55uL, 0x316E8EEFuL, 0x4669BE79uL,
	0xCB61B38CuL, 0xBC66831AuL, 0x256FD2A0uL, 0x5268E236uL,
	0xCC0C7795uL, 0xBB0B4703uL, 0x220216B9uL, 0x5505262FuL,
	0xC5BA3BBEuL, 0xB2BD0B28uL, 0x2BB45A92uL, 0x5CB36A04uL,
	0xC2D7FFA7uL, 0xB5D0CF31uL, 0x2CD99E8BuL, 0x5BDEAE1DuL,
	0x9B64C2B0uL, 0xEC63F226uL, 0x756AA39CuL, 0x026D930AuL,
	0x9C0906A9uL, 0xEB0E363FuL, 0x72076785uL, 0x05005713uL,
	0x95BF4A82uL, 0xE2B87A14uL, 0x7BB12BAEuL, 0x0CB61B38uL,
	0x92D28E9BuL, 0xE5D5BE0DuL, 0x7CDCEFB7uL, 0x0BDBDF21uL,
	0x86D3D2D4uL, 0xF1D4E242uL, 0x68DDB3F8uL, 0x1FDA836EuL,
	0x81BE16CDuL, 0xF6B9265BuL, 0x6FB077E1uL, 0x18B74777uL,
	0x88085AE6uL, 0xFF0F6A70uL, 0x66063BCAuL, 0x11010B5CuL,
	0x8F659EFFuL, 0xF862AE69uL, 0x616BFFD3uL, 0x166CCF45uL,
	0xA00AE278uL, 0xD70DD2EEuL, 0x4E048354uL, 0x3903B3C2uL,
	0xA7672661uL, 0xD06016F7uL, 0x4969474DuL, 0x3E6E77DBuL,
	0xAED16A4AuL, 0xD9D65ADCuL, 0x40DF0B66uL, 0x37D83BF0uL,
	0xA9BCAE53uL, 0xDEBB9EC5uL, 0x47B2CF7FuL, 0x30B5FFE9uL,
	0xBDBDF21CuL, 0xCABAC28AuL, 0x53B39330uL, 0x24B4A3A6uL,
	0xBAD03605uL, 0xCDD70693uL, 0x54DE5729uL, 0x23D967BFuL,
	0xB3667A2EuL, 0xC4614AB8uL, 0x5D681B02uL, 0x2A6F2B94uL,
	0xB40BBE37uL, 0xC30C8EA1uL, 0x5A05DF1BuL, 0x2D02EF8DuL
};
#endif


/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/


/*code*/
#if ( STD_CRCCFG_CLASS_CRC8 == STD_CRC_ENABLE)

/****************************************************************************************
 *函数名称  : uint8 CRC_CalculateCRC8 (const uint8 * Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, uint8 Crc_IsFirstCall)

 *参数      : 需要计算的数据*Crc_DataPtr： 0-255
			长度 Crc_Length ： 0-4294967296 
			CRC起始值 Crc_StartValue8： 0-255 
			第一次访问 Crc_IsFirstCall： 0-1

 *返回值    : CRC 值

 *描述      : CRC8 校验

 *编辑时间  :
 初版
 *备注      :
 *****************************************************************************************/
uint8 CRC_CalculateCRC8 (const uint8 * Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, uint8 Crc_IsFirstCall)
{
  uint8  Crc_Value;

#if (STD_CRCCFG_8_MODE == STD_CRC_8_TABLE)
  uint32 Crc_LoopCounter;
#else
  uint8  loopIndex;
  uint32 CrcLen;
#endif

  if( Crc_IsFirstCall == (uint8)TRUE )
  {
    Crc_Value = STD_CRC_INITIAL_VALUE8;
  }
  else
  {
    Crc_Value = (STD_CRC_FINAL_XOR_CRC8 ^ Crc_StartValue8);
  }

#if (STD_CRCCFG_8_MODE == STD_CRC_8_TABLE)
  /* Calculate CRC value by using a table */
  for (Crc_LoopCounter = 0u; Crc_LoopCounter < Crc_Length; Crc_LoopCounter++)
  {
    Crc_Value = StdCrc_Table8[Crc_Value ^ Crc_DataPtr[Crc_LoopCounter]];
  }
#else
  /* Calculate CRC value via CRC8 routine */
  for (CrcLen = 0u; CrcLen < Crc_Length; CrcLen++)
  {
    /*XOR next byte of Crc_DataPtr with current CRC value. This is equivalent to calculating CRC value of concatenated bytes */
    Crc_Value ^= Crc_DataPtr[CrcLen];/*PRQA S 0492*/

    /* Perform modulo-2 division, a bit at a time */
    for (loopIndex = 0u; loopIndex < STD_LoopNum; loopIndex++)
    {
      /* If MSB is 1, CRC value is XORed with polynomial */
      if ((Crc_Value & (uint8) 0x80u) > (uint8)0u)
      {
        Crc_Value = ( (uint8)(Crc_Value << 1u) ) ^ STD_CRC_POLYNOMIAL_8;
      }
      else
      {
        Crc_Value = (uint8) (Crc_Value << 1u);
      }
    }
  }
#endif

  /*Final CRC value is XORed */
  return Crc_Value ^ STD_CRC_FINAL_XOR_CRC8;
}
#endif

#if ( STD_CRCCFG_CLASS_CRC16 == STD_CRC_ENABLE)



/****************************************************************************************
 *函数名称  : uint16 CRC_CalculateCRC16(const uint8 * Crc_DataPtr, uint32 Crc_Length, uint16 Crc_StartValue16, uint8 Crc_IsFirstCall)

 *参数      : 需要计算的数据*Crc_DataPtr： 0-255
				长度 Crc_Length ： 0-4294967296 
				CRC起始值 Crc_StartValue16： 0-65535 
				第一次访问 Crc_IsFirstCall： 0-1

 *返回值    : CRC 值

 *描述      : CRC16 校验

 *编辑时间  :
 初版
 *备注      :
 *****************************************************************************************/
uint16 CRC_CalculateCRC16(const uint8 * Crc_DataPtr, uint32 Crc_Length, uint16 Crc_StartValue16, uint8 Crc_IsFirstCall)
{
#if (STD_CRCCFG_16_MODE == STD_CRC_16_TABLE)
  uint8 tableIndex;
#else
  uint8  loopIndex;
#endif
  /* polyspace<MISRA-C3:D4.5:Not a defect:Justified> Normal */
  uint16 crcValue;
  uint32 CrcLen;

  if (Crc_IsFirstCall == (uint8)TRUE)
  {
    crcValue = STD_CRC_INITIAL_VALUE16;
  }
  else
  {
    crcValue = Crc_StartValue16;
  }
#if (STD_CRCCFG_16_MODE == STD_CRC_16_TABLE)
	/*Calculate CRC value by using a table */
  for(CrcLen = 0u; CrcLen < Crc_Length; CrcLen++)
  {
    tableIndex = ((uint8) (crcValue >> 8u)) ^ Crc_DataPtr[CrcLen];
    crcValue = (crcValue << 8u) ^ StdCrc_Table16[tableIndex];
  }
#else
 /*Perform CRC calculation for each byte in Crc_DataPtr */
  for(CrcLen = 0u; CrcLen < Crc_Length; CrcLen++)
  {
    /*XOR next byte of Crc_DataPtr with current CRC value. This is equivalent to calculating CRC value of concatenated bytes */
    crcValue ^= (uint16)((uint16)Crc_DataPtr[CrcLen] << 8u);/*PRQA S 0492*/

    /*Perform modulo-2 division, a bit at a time */
    for(loopIndex = 0u; loopIndex < STD_LoopNum; loopIndex++)
    {
      /*If MSB is 1, CRC value is XORed with polynomial */
      if((crcValue & (uint16)0x8000u) > (uint16)0u)
      {
        crcValue = ( (uint16)(crcValue << 1u) ) ^ STD_CRC_POLYNOMIAL_16;
      }
      else
      {
        crcValue <<= 1u;
      }
    }
  }
#endif
  return crcValue;
}
#endif
#if ( STD_CRCCFG_CLASS_CRC32 == STD_CRC_ENABLE)



/****************************************************************************************
 *函数名称  : uint8 CRC_CalculateCRC8 (const uint8 * Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, uint8 Crc_IsFirstCall)

 *参数      : 需要计算的数据*Crc_DataPtr： 0-255
			长度 Crc_Length ： 0-4294967296 
			CRC起始值 Crc_StartValue32： 0-4294967296
			第一次访问 Crc_IsFirstCall： 0-1

 *返回值    : CRC 值

 *描述      : CRC32 校验

 *编辑时间  :
 初版
 *备注      :
 *****************************************************************************************/

uint32 CRC_CalculateCRC32(const uint8 * Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall)
{
#if (STD_CRCCFG_32_MODE != STD_CRC_32_TABLE)
  uint8 loopIndex;
#endif
  uint32 Crc_Value;
  uint32 CrcLen;

  /*Set initial value depending on whether it's the first call or a subsequent call */
  if( Crc_IsFirstCall == TRUE )
  {
    Crc_Value = STD_CRC_INITIAL_VALUE32;
  }
  else
  {
    Crc_Value = (STD_CRC_FINAL_XOR_CRC32 ^ Crc_StartValue32);
  }
#if (STD_CRCCFG_32_MODE == STD_CRC_32_TABLE)
	/*Calculate CRC value by using a table */
	for (CrcLen = 0; CrcLen < Crc_Length; CrcLen++)
	{
		Crc_Value ^= 0xFFuL & (uint32) Crc_DataPtr[CrcLen];
		Crc_Value = ( Crc_Value >> 8u ) ^ ( StdCrc_Table32 [(uint8)(Crc_Value )]);
	}
#else
	/*Perform CRC calculation for each byte in Crc_DataPtr */
	for(CrcLen = 0; CrcLen < Crc_Length; CrcLen++)
	{
		/*CRC32 routine considers reflection of input and output data. Instead of reflecting input and output data, a reflected polynomial is used */
		/* XOR next byte of Crc_DataPtr with current CRC value. This is equivalent to calculating CRC value of concatenated bytes */
		Crc_Value ^= 0xFFuL & (uint32) Crc_DataPtr[CrcLen];

		/*Perform modulo-2 division, a bit at a time */
		for (loopIndex = 0u; loopIndex < STD_LoopNum; loopIndex++)
		{
			/*If LSB is 1, CRC value is XORed with polynomial */
			/*Direction of modulo-2 division is reverse compared to Crc8 and Crc16, due to the reflection of the polynomial */
			if ( (Crc_Value & 1u) > 0 )
			{
			  Crc_Value = ( Crc_Value >> 1u ) ^ STD_CRC_POLYNOMIAL_32;
			}
			else
			{
			  Crc_Value = Crc_Value >> 1u;
			}
		}
	}
#endif
  return Crc_Value ^ STD_CRC_FINAL_XOR_CRC32;
}
#endif

/*******************************************************************************
Name			: Ea_Init
Syntax			: void Ea_Init(void)
Sync/Async		: Synchronous
Reentrancy		:
Parameters(in)	: None						:-
Parameters(out) : None						:-
Return value	: None						:-
Description		:   initialize
Call By			: -
|******************************************************************************/
unsigned int GetCRC16(unsigned char *ptr,  u16 len)
{ 
    u16 index;
    u8 crch = 0xFF;  //贌CRC貣逇
    u8 crcl = 0xFF;  //謲CRC貣逇
    u8  TabH[] = 
	   {  //CRC贌位貣逇值噎
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,  
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,  
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,  
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,  
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40  
    };  
		 
    u8 TabL[] =
    		{  //CRC謲位貣逇值噎
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,  
        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,  
        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,  
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,  
        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,  
        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,  
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,  
        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,  
        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,  
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,  
        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,  
        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,  
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,  
        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,  
        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,  
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,  
        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,  
        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,  
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,  
        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,  
        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,  
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,  
        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,  
        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,  
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,  
        0x43, 0x83, 0x41, 0x81, 0x80, 0x40  
    } ;	
    while (len--)  //輪蹋指吱婴讏CRC
    {
        index = crch ^ *ptr++;
        crch = crcl ^ TabH[index];
        crcl = TabL[index];
    }
    return ((crch<<8) | crcl);  
} 

/*EOF*/
