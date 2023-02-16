/* polyspace<MISRA-C3:1.1:Not a defect:Justified> Normal. */
/*******************************************************************************
|  File Name:  STD_Lib.c
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

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/


#include "STD_Lib.h"
//#include "STD_Interface.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define LIB_MAXVALUE_32     ((uint32)0xFFFFFFFFu)
#define LIB_MAXVALUE_16     ((uint16)0xFFFFu)
#define LIB_MAXVALUE_08     ((uint8)0xFFu)


/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/


/*******************************************************************************
Name            : LIB_SetMemory
Syntax          : void LIB_SetMemory(uint8 *pStr, uint8 val, uint16 len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : val                   :set value
Parameters(in)  : len                   :data length
Parameters(out) : None                  :-
Return value    : None
Description     : set value to pStr
Call By         : -
|******************************************************************************/

void LIB_SetMemory(uint8_t *pStr, uint8_t val, uint16_t len)
{
  

    if( (len > (uint16_t)0u) && (len < LIB_MAXVALUE_16) )
    {
        while( len != (uint16_t)0u )
        {
            *pStr = val;
         /* polyspace<MISRA-C3:17.8:Not a defect:Justified> Normal. */
	    pStr ++;/*PRQA S 0489*/
	    /* polyspace<MISRA-C3:17.8:Not a defect:Justified> Normal. */
            len --;
        }
    }

 
}


/*******************************************************************************
Name            : LIB_Compare
Syntax          : uint8 LIB_Compare(uint8 *pSrc, uint8 *pDes, uint16 len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                  :-
Return value    : None
Description     : Compare data from pDes to pSrc
Call By         : -
|******************************************************************************/

uint8 LIB_Compare(const uint8 *pSrc, const uint8 *pDes, uint16 len)
{
    uint8  e_u_result = (uint8)1;
    uint16 e_w_len;
    if( ( len > (uint16)0u ) && ( len < LIB_MAXVALUE_16 ) )
    {
        for( e_w_len = (uint16)0u; e_w_len < len; e_w_len++ )
        {
            if( pSrc[e_w_len] != pDes[e_w_len] )/*PRQA S 0492*/
            {
                e_u_result = (uint8)0;
                break;
            }
        }
    }

    return (e_u_result);
}


/*******************************************************************************
Name            : LIB_CompareR
Syntax          : uint8 LIB_CompareR(uint8 *pSrc, uint8 *pDes, uint16 len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                  :-
Return value    : None
Description     : Compare reverse data from pDes to pSrc
Call By         : -
|******************************************************************************/

uint8 LIB_CompareR(const uint8 *pSrc, const uint8 *pDes, uint16 len)
{
    uint8  e_u_result = 1;
    uint16 e_w_len;
    uint8  e_u_Rvalue = (uint8)0u;

    if( ( len > (uint16)0u ) && ( len < LIB_MAXVALUE_16 ) )
    {
        for( e_w_len = (uint16)0u; e_w_len < len; e_w_len++ )
        {
            e_u_Rvalue = (uint8)( ~pDes[e_w_len] );/*PRQA S 0492*/

            if( pSrc[e_w_len] != e_u_Rvalue )/*PRQA S 0492*/
            {
                e_u_result = 0;
                break;
            }
        }
    }

    return e_u_result;
}


/*******************************************************************************
Name            : LIB_Copy
Syntax          : void LIB_Copy(uint8 *pDes, uint8 *pSrc, uint16 len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy data from pSrc to pDes
Call By         : -
|******************************************************************************/

void LIB_Copy(uint8_t *pDes, const uint8_t *pSrc, uint16 len)
{
    uint16 e_w_len;
 

    if( ( len > (uint16)0u ) && ( len < LIB_MAXVALUE_16 ) )
    {
        for( e_w_len = (uint16)0u; e_w_len < len; e_w_len++ )
        {
            pDes[e_w_len] = pSrc[e_w_len];/*PRQA S 0492*/
        }
    }


}


/*******************************************************************************
Name            : LIB_CopyR
Syntax          : void LIB_CopyR(uint8 * pDes, uint8 * pSrc, uint16 len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy reverse data from pSrc to pDes
Call By         : -
|******************************************************************************/

void LIB_CopyR(uint8_t *pDes, const uint8 *pSrc, uint16 len)
{/* polyspace<MISRA-C3:D4.5:Not a defect:Justified> Normal */
    uint16 e_w_Len;
 

    if( ( len > (uint16)0u ) && ( len < LIB_MAXVALUE_16 ) )
    {
        for( e_w_Len = (uint16)0u; e_w_Len < len; e_w_Len++ )
        {
            pDes[e_w_Len] = (uint8)( ~pSrc[e_w_Len] );/*PRQA S 0492*/
        }
    }


}


/*******************************************************************************
Name            : LIB_CalculateAverage
Syntax          : uint16 LIB_CalculateAverage(const uint16 *pData, uint8 length)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pData                 :data pointer
Parameters(in)  : length                :data length
Parameters(out) : None                  :-
Return value    : None
Description     : Use bubble sort method
Call By         : -
|******************************************************************************/

uint16 LIB_CalculateAverage(const uint16 *pData, uint8 length)
{
    uint8  i;
    uint32 e_dw_sum = (uint32)0u;
    uint16 e_w_average = (uint16)0u;

    if( ( length > (uint8)0u ) && ( length < LIB_MAXVALUE_08 ) )
    {
        for( i = (uint8)0u; i < length; i++ )
        {
            e_dw_sum += pData[i];/*PRQA S 0492*/
        }

        e_w_average = (uint16)( e_dw_sum / length );
    }
    else
    {
    }

    return (e_w_average);
}


/*******************************************************************************
Name            : LIB_ReadData
Syntax          : void LIB_ReadData(uint8 * pDes, uint32 pSrcAdd, uint32 len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrcAdd               :source data address
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy data from pSrcAdd to pDes
Call By         : -
|******************************************************************************/

void LIB_ReadData(uint8_t *pDes, uint32_t pSrcAdd, uint32_t len)
{

 
}

/*******************************************************************************
Name            : LIB_WriteData
Syntax          : void LIB_WriteData( uint8 *pSrc, uint32 pDesAdd, uint32 len )
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrcAdd               :source data address
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy data from pSrc to pSrcAdd
Call By         : -
|******************************************************************************/

void LIB_WriteData( uint8_t *pSrc, uint32_t pSrcAdd, uint32_t len )
{
  


}

/*******************************************************************************
Name            : LIB_StatusFilter
Syntax          : uint16 LIB_StatusFilter(LibFilterStruct *pFilterStatus, uint16 filterMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterMaxCnt              :filter count
Parameters(out) : None                      :-
Return value    : None
Description     : data Filter
Call By         : -
|******************************************************************************/

uint8 LIB_StatusFilter( LibFilterStruct *pFilterStatus, uint16 filterMaxCnt )
{
    uint8  e_u_reValue = 0;
    uint16 e_w_filterMaxCnt = (uint16)0u;

    if(( filterMaxCnt > (uint16)0u ) && ( filterMaxCnt < LIB_MAXVALUE_16 ))
    {
        e_w_filterMaxCnt = filterMaxCnt;
    }
    else
    {
        e_w_filterMaxCnt = LIB_MAXVALUE_16;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucLastStatus )
    {
        pFilterStatus->ucLastStatus = pFilterStatus->ucStatus;
        pFilterStatus->usFilterCount = (uint16)0u;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucValidStatus )
    {
        if( pFilterStatus->usFilterCount < e_w_filterMaxCnt )
        {
            pFilterStatus->usFilterCount++;
        }
        else
        {
            pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
            e_u_reValue = 1;
        }
    }
    else
    {
    }

    return (e_u_reValue);
}

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : data Filter
Call By         : -
|******************************************************************************/

uint8 LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
{
    uint8  e_u_reValue = 0;
    uint16 e_w_filterInMaxCnt = (uint16)0u;
    uint16 e_w_filterOutMaxCnt = (uint16)0u;
    if(( filterInMaxCnt > (uint16)0u ) && ( filterInMaxCnt < ((uint16)0xFFFFu) ))
    {
    	e_w_filterInMaxCnt = filterInMaxCnt;
    }
    else
    {
    	e_w_filterInMaxCnt = ((uint16)0xFFFFu);
    }
    if(( filterOutMaxCnt > (uint16)0u ) && ( filterOutMaxCnt < ((uint16)0xFFFFu) ))
    {
    	e_w_filterOutMaxCnt = filterOutMaxCnt;
    }
    else
    {
    	e_w_filterOutMaxCnt = ((uint16)0xFFFFu);
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucLastStatus )
    {
        pFilterStatus->ucLastStatus = pFilterStatus->ucStatus;
        pFilterStatus->usFilterCount = (uint16)0u;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucValidStatus )
    {
    	if(pFilterStatus->ucValidStatus == 1)
    	{
    	       if( pFilterStatus->usFilterCount < e_w_filterOutMaxCnt )
    	        {
    	            pFilterStatus->usFilterCount++;
    	        }
    	        else
    	        {
    	            pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
    	            e_u_reValue = 1;
    	        }
    	}
    	else if(pFilterStatus->ucValidStatus == 0)
    	{
        	if( pFilterStatus->usFilterCount < e_w_filterInMaxCnt )
            {
                pFilterStatus->usFilterCount++;
            }
            else
            {
                pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
                e_u_reValue = 1;
            }
    	}
    }
    else
    {
    }
    return (e_u_reValue);
}


/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : data Filter
Call By         : -
|******************************************************************************/

uint16 LIB_FifoFillter(uint16 *pData, uint16 len)
{
	uint16 min = 0, max = 0, verg = 0, invaild_count = 0,vaild_count = 0;
	uint32 sum = 0;
	uint16 tmp;
	uint16 i = 0;
	
	if(pData == NULL || len == 0)
	{
		return 0;
	}
	else
	{
		min = pData[0];
		max = pData[0];
		for( i = 0; i < len; i++)
	    {
			tmp = pData[i];
	        if(pData[i] != 0)
	        {
	        	if(min == 0)
	        	{
	        		min = pData[i];
	        	}
	            if(min > pData[i])
	            {
	                min = pData[i];
	            }
	            if(max < pData[i])
	            {
	                max = pData[i];
	            }
	        }
			else
	        {
	            invaild_count++;
	        }
			sum += pData[i];
	    }
		vaild_count = len - invaild_count;
		if(vaild_count > 2)
		{
			sum -= min;
			sum -= max;
			verg = sum / (vaild_count - 2);
		}
		else if(vaild_count == 2)
		{
			verg = sum / 2;
		}
		else if((vaild_count = 0) || (vaild_count = 1))
		{
			verg = 0;
		}
		else
		{
			verg = 0;
		}
	}
	return verg;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 字符串转换为无符号字符
Call By         : -
|******************************************************************************/
uint8_t LIB_DecToUint8(uint8_t * str,uint8_t len)
{
    uint8_t i,sum=0;

    for(i=0;i<len;i++)
    {
        if(!isdigit(str[i]))    
	    return 0;
        sum=sum*10+str[i]-'0';
  }
    return sum;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 16进制装换为无符号字符
Call By         : -
|******************************************************************************/

uint16_t  LIB_hex2uint(uint8_t * str)
{
    uint16_t RetValue=0;
    while(1)
    {
        if(*str>='0'&&*str<='9')
        {
            RetValue*=16;
            RetValue+=(*str++)-'0';//
        }
        else if(*str>='A'&&*str<='F')
        {
            RetValue*=16;
            RetValue+=(*str++)-'A'+10; 
        }
        else
        {
            return RetValue;
        }
    }
}

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 字符串转换为整行数据
Call By         : -
|******************************************************************************/
uint32_t  LIB_dec2uint(uint8_t * str)
{
    uint32_t RetValue=0;
    while(*str>='0'&&*str<='9')
    {
        RetValue*=10;
        RetValue+=(*str++)-'0';
    }
    return RetValue;
}

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 字符串转换为整行数据
Call By         : -
|******************************************************************************/
void  LIB_str2uint(uint8_t * str,uint16_t *substrlen)
{
   uint16_t RetValue=0;
  int i=0;

    while(str[i]>='0' && str[i]<='9')
      {
     
        RetValue*=10;
        RetValue+=(str[i]-'0');
  
        i++; 
    
      };
  *substrlen=RetValue; 
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
Call By         : -
|******************************************************************************/
uint16_t LIB_DecToUint16(uint8_t * str,uint8_t len)
{
    uint8_t i;
    uint16_t sum=0;
	
    for(i=0;i<len;i++)
    sum=sum*10+str[i]-'0';
    return sum;
}


/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
Call By         : -
|******************************************************************************/
void LIB_Uint8ToDec2(uint8_t value,uint8_t* destbuff)
{
    destbuff[0]=value/10+'0';
    destbuff[1]=value%10+'0';
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
Call By         : -
|******************************************************************************/
uint8_t LIB_Uint8ToDec(uint8_t value,uint8_t* destbuff)
{
    if(value>99)
    {
        destbuff[2]=value%10+'0';
        value/=10;
        destbuff[1]=value%10+'0';
        value/=10;
        destbuff[0]=value+'0';
        return 3;
    }
     else if(value>9)
    {
        destbuff[1]=value%10+'0';
        value/=10;
        destbuff[0]=value+'0';
        return 2;
    }
     destbuff[0]=value+'0';
     return 1;
}

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
Call By         : -
|******************************************************************************/
uint8_t LIB_Uint16ToDec(uint16_t value,uint8_t* dest)
{
     uint8_t i=5,len=0;
  
     while(i--)
     {
        dest[i]=value%10+'0';
        len++;
        value/=10;
        if(!value)  break;
     }

     for(i=0;i<len;i++)  dest[i]=dest[i+5-len];
    
     return len;
}


/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 查询相等的字符串,如果有相等的,
**                返回的应该是相同字符串的第一个字符的指针
                  uint8 *str:被搜索的字符串,uint16 strlen:被搜索字符串的长度
**                char *substr:需要寻找的字符串,uint8 substrlen:需要寻找的字符串的长度

                  
Call By         : -
|******************************************************************************/
uint8_t * LIB_SearchString(uint8_t *str,uint16_t strlen,char *substr,uint8_t substrlen)
{
    if(!strlen||!substrlen)  return 0;
    if(strlen<substrlen)  return 0;
    
    strlen-=substrlen;//
    do
    {
        if(strncmp((char*)str,substr, substrlen)==0)  return str;
        str++;
		
    }while(strlen--);
    
    return 0;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     :  Uint32IPDatatoDecWithDot
                   功能描述: 将32位整形变量变化为IP格式(如:211.139.111.40)
Call By         : -
|******************************************************************************/

uint8_t LIB_Uint32IPDatatoDecWithDot(uint32_t IPData,uint8_t* destbuff)
{
    uint8_t* Tmp=(uint8_t*)&IPData;
    uint8_t len,len1;

    len=LIB_Uint8ToDec(Tmp[0],&destbuff[0]);
    destbuff[len]='.';
    len1=LIB_Uint8ToDec(Tmp[1],&destbuff[len+1]);
    len=len+len1+1;
    destbuff[len]='.';
    len1=LIB_Uint8ToDec(Tmp[2],&destbuff[len+1]);
    len=len+len1+1;
    destbuff[len]='.';
    len1=LIB_Uint8ToDec(Tmp[3],&destbuff[len+1]);
    len=len+len1+1;

    return len;	
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     :  
                   
Call By         : -
|******************************************************************************/
uint16_t LIB_HexToASCII(uint8_t* data, uint8_t* buffer, uint16_t len)
{ 
    const static uint8_t ascTable[17] = {"0123456789ABCDEF"}; 
    uint8_t *tmp_p = buffer; 
    uint16_t i, pos; 
	
    pos = 0; 
    for(i = 0; i < len; i++) 
	{
		tmp_p[pos++] = ascTable[data[i] >> 4];
		tmp_p[pos++] = ascTable[data[i] & 0x0f]; 
	}
    tmp_p[pos] = '\0'; 
    return pos; 
} 
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     :  
                   
Call By         : -
|******************************************************************************/
uint8_t LIB_ASCIITOHex(uint8_t* OData,uint8_t* NData,uint8_t len)
{ 
	int i,j,tmp_len; 
	uint8_t tmpData; 
	uint8_t *O_buf = OData;
	uint8_t *N_buf = NData;
	
	for(i = 0; i < len; i++) 
	{ 
		if ((O_buf[i] >= '0') && (O_buf[i] <= '9')) 
		{ 
			tmpData = O_buf[i] - '0'; 
		} 
		else if ((O_buf[i] >= 'A') && (O_buf[i] <= 'F'))    /*A....F*/ 
		{ 
			tmpData = O_buf[i] - 0x37; 
		} 
		else if((O_buf[i] >= 'a') && (O_buf[i] <= 'f'))      /*a....f */
		{ 
			tmpData = O_buf[i] - 0x57; 
		}
		else 
		{ 
			return 0; 
		} 
		O_buf[i] = tmpData; 
	} 
	
	for(tmp_len = 0,j = 0; j < i; j+=2) 
	{ 
		N_buf[tmp_len++] = (O_buf[j]<<4) | O_buf[j+1]; 
	} 
	
	return tmp_len; 
	
}

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     :  
                   
Call By         : -
|******************************************************************************/
uint32_t LIB_StrToUint32(uint8_t *ptr, uint8_t mem_mod)
{
	uint32_t temp = 0;
	
	if(MEM_MOD_LITLE == mem_mod)
	{
		temp =  *ptr++;
	    temp += (uint32_t)((*ptr++)<<8);
	    temp += (uint32_t)((*ptr++)<<16);
	    temp += (uint32_t)((*ptr++)<<24);
	}
	else if(MEM_MOD_BIG== mem_mod)
	{
		temp += (uint32_t)((*ptr++)<<24);
	 	temp += (uint32_t)((*ptr++)<<16);
	 	temp += (uint32_t)((*ptr++)<<8);
		temp +=  *ptr;
	}
	else
		temp = 0;
	
	return temp;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     :  
                   
Call By         : -
|******************************************************************************/
uint16_t LIB_StrToUint16(uint8_t *ptr, uint8_t mem_mod)
{
	uint16_t temp = 0;
	
	if(MEM_MOD_LITLE == mem_mod)
	{
		temp =  *ptr++;
	    temp += (uint16_t)(*ptr<<8);
	}
	else if(MEM_MOD_BIG== mem_mod)
	{
		temp = (uint16_t)((*ptr++)<<8);
	 	temp += *ptr;
	}
	else
		temp = 0;
	
	return temp;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 在一个字符串中寻找两个指定字符中间的字符串
                  返回的应该是相同字符串的第一个字符的指针和长度
                  uint8 *str:被搜索的字符串,uint16 strlen:被搜索字符串的长度
                  pData = GetStringMiddle(pPos,rxPos, '"', 1, '"', 2, &usLen);
Call By         : -
|******************************************************************************/
uint8_t* LIB_GetStringMiddle(uint8_t *str,uint16_t strlen, char str1,uint16_t Nostr1, char str2,uint16_t Nostr2,uint16_t *substrlen)
{
    uint16_t No1p;		//字符1 位置计数器
    uint16_t No2p;		//字符2 位置计数器
    uint16_t i;
    uint8_t *Firststr= NULL;	//首字符位置
    uint8_t *Secstr= NULL;    //要找到第二个字符的指针
    uint8_t Str1Ok;
    uint8_t Str2Ok;	
	
//被搜索字符长度为0，则返回0值
    if(strlen==0)  return 0;
//位置累计清零
    No1p=0;
    No2p=0;
//是否找到标记置零	
    Str1Ok=FALSE;
    Str2Ok=FALSE;	
//查找字符位置	
    for (i=0;i<strlen;i++)  //遍历朱字符串
    {
        if (*(str+i)==str1)  //找到第一个字符
        {
            No1p++;           //字符str1的出现的第NO1p
            if (No1p==Nostr1) 
            {
                Firststr=str+i;//保存出现首次出现的位置
                Str1Ok=TRUE;
            }
        }

        if (*(str+i)==str2) 
        {
            No2p++;
            if (No2p==Nostr2) 
            {
                Secstr=str+i;	
                Str2Ok=TRUE;
            }
        }
        if (Str1Ok && Str2Ok) 
		{
			break;  //首尾字符都找到
		}
    }
	
    if (Str1Ok&&Str2Ok&&Secstr!=Firststr) 
    {
        if (Secstr>Firststr)   //第二个字符比第一个位置长
        {
            *substrlen=Secstr-Firststr-1;		//保存字符		
            return Firststr+1;
        }
        *substrlen=Firststr-Secstr-1;				
        return Secstr+1;			
    }
	return 0;
}

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 将ASCII格式转换为Hex
                   
Call By         : -
|******************************************************************************/
uint8_t LIB_SingleASCIITOHex(uint8_t NData)
{ 
    if ((NData >= '0') && (NData <= '9'))   
		return  (NData - '0');
    else if ((NData >= 'A') && (NData <= 'F'))   
		return  (NData - 0x37);
    else if ((NData >= 'a') && (NData <= 'f'))   
		return  (NData - 0x57);	
    else 
		return 0xff;
} 
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 将ASCII格式转换为Hex
                   
Call By         : -
|******************************************************************************/
uint8_t * LIB_GetStringBack(uint8_t *str,uint16_t strlen,char str1,uint16_t Nostr1)
{

	uint16_t No1p;		//字符1 位置计数器
	uint16_t i;		    //字符1 位置计数器	
		
	uint8_t *Firststr=NULL;	//首字符位置
	uint8_t Str1Ok=FALSE;

    //被搜索字符长度为0，则返回0值
    if(strlen==0)  return 0;
    //位置累计清零
	No1p=0;

    //查找字符位置	
    for (i=0;i< strlen; i++)
	{
		if (*(str+i)==str1) 
		{
			No1p++;
			if (No1p==Nostr1) 
			{
				Firststr=str+i;
				Str1Ok=TRUE;
				break;
			}
		}
	}
	
	if (Str1Ok)	
		return Firststr+1;
			
	else 	
		return 0;

}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
uint8_t LIB_Hex2ToUint8(uint8_t *ahex)
{
    uint8_t sum,Ch;
   
    if ((Ch=*ahex++)<='9')
        sum=Ch-'0';
    else
        sum=Ch-'A'+10;
    if ((Ch=*ahex)<='9')
        sum=sum*16+Ch-'0';
    else
        sum=sum*16+Ch-'A'+10;
   
    return sum;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
uint8_t LIB_Dec3ToUint8(uint8_t * ahex,uint8_t len)
{
    uint8_t i,sum=0;
   
    for(i=0;i<len;i++)
    {
        sum=sum*10+*ahex-'0';
        ahex++;
    }
    return sum;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
uint8_t LIB_Dec2ToUint8(uint8_t *ahex)
{
    uint8_t i,sum=0;
   
    for(i=0;i<2;i++)
    {
        sum=sum*10+*ahex-'0';
        ahex++;
    }
    return sum;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
void LIB_HexToDatabuff(uint8_t *buff,uint8_t *hex,uint16_t hexlen)
{
    hexlen>>=1;
    while(hexlen--)
    {
       *buff++=LIB_Hex2ToUint8(hex);
       hex+=2;
    }
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
void LIB_ConvertBCDB(uint8_t *Abuff,uint8_t len)
{
    uint8_t *p,temp;
     
    p=Abuff++;
    len>>=1;
   
    while (len--)
    {
        temp=*p;
        *p=*Abuff;
        *Abuff=temp;
        p+=2;
        Abuff+=2;
    }
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
void LIB_Uint8ToHex2(uint8_t value,uint8_t* destbuff)
{
    uint8_t lowByte;
   
    lowByte=value&0x0F;
    value/=16;
    if (value>9)
        destbuff[0]=value+'A'-10; 
    else
        destbuff[0]=value+'0';
   
    if (lowByte>9)
        destbuff[1]=lowByte+'A'-10; 
    else
        destbuff[1]=lowByte+'0';
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
void LIB_DataToHexbuff(uint8_t* dest,uint8_t* src,uint8_t srclen)
{
    while(srclen--)
    {
        LIB_Uint8ToHex2(*src,dest);
        src++;
        dest+=2;
    }
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
double LIB_deg2rad(double deg)
{
    return (deg*M_PI)/180.0;
}

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 快速求地球两点距离算法，传入单位角度，传出单位米
                  lat1为起点纬度，lo1为起点经度
                  lat2为终点纬度，lo2为终点经度
                   
Call By         : -
|******************************************************************************/
double LIB_distance(double lat1,double lon1,double lat2,double lon2)
{
    double test=sin(LIB_deg2rad(lat1)) * sin(LIB_deg2rad(lat2)) + cos(LIB_deg2rad(lat1)) * cos(LIB_deg2rad(lat2)) * cos(LIB_deg2rad(lon1 - lon2));
    return NUMBER*acos(test);
} 

/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   异或和校验函数
Call By         : -
|******************************************************************************/
uint8_t LIB_Lrc(uint8_t *p, uint16_t len)
{
	uint8_t sum = 0;
	uint32_t i;

	for(i = 0; i < len; i++)
	{
		sum ^= p[i];
	}
	return sum;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                 将存储在p所指的缓存的len个字节，去掉最大的offsize个字节
                 去掉最小的offsize个字节,剩下的取平均
Call By         : -
|******************************************************************************/
uint16_t LIB_DigitFilter1(uint16_t*p, uint16_t len, uint16_t offsize)
{
    uint16_t i,ii;
    uint16_t temp,result;
    uint32_t sum=0;

	if(len <= offsize*2)
		return 0;
    for(ii=len-1; ii>0; ii--)//排序:从小到大
	{
		for(i=0; i<ii; i++)
		{
			if(p[i] > p[i+1] )
			{
				temp = p[i];
				p[i] = p[i+1];
				p[i + 1] = temp;
			}
		}	
	}
    for(i=offsize; i<len-offsize; i++)
        sum += p[i];
    result = (sum/(len-offsize*2)) & 0xffff;
    return result;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                   
Call By         : -
|******************************************************************************/
uint8_t LIB_floatToByte(float fValue, uint8_t *arr)
	{
	unsigned char  *pf;
    unsigned char *px;
    unsigned char i;   //计数器
    pf =(unsigned char *)&fValue;            /*unsigned char型指针取得浮点数的首地址*/
    px = arr;                               	/*字符数组arr准备存储浮点数的四个字节,px指针指向字节数组arr*/

    for(i=0;i<4;i++)
    {
        *(px+i)=*(pf+i);     /*使用unsigned char型指针从低地址一个字节一个字节取出*/
    }
	
	return 1;
}
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16 filterInMaxCnt ,uint16 filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : 
                  
Call By         : -
|******************************************************************************/
float LIB_byteToFloat(uint8_t *arr)
	{
	return *((float*)arr);
}


/*******************************************************************************
Name            : 
Syntax          : 
Sync/Async      : 
Reentrancy      :
Parameters(in)  : 
Parameters(in)  : 
Parameters(in)  : 
Parameters(out) : 
Return value    : 
Description     : 
Call By         : 
|******************************************************************************/
unsigned int GetCRC16(unsigned char *ptr,  u16 len)
{ 
    u16 index;
    u8 crch = 0xFF;  //高CRC字节
    u8 crcl = 0xFF;  //低CRC字节
    u8  TabH[] = {  //CRC高位字节值表
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
    } ;  
    u8 TabL[] = {  //CRC低位字节值表
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
 
    while (len--)  //计算指定长度CRC
    {
        index = crch ^ *ptr++;
        crch = crcl ^ TabH[index];
        crcl = TabL[index];
    }
    
    return ((crch<<8) | crcl);  
} 




/*EOF*/
