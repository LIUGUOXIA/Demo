/*******************************************************************************
|  File Name:  Std_Lib_Cfg.h
|  Description:  Implementation of the Lib Config Management
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

#if!defined (_STD_LIB_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_LIB_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define STD_LIB_DISABLE                 (0U)
#define STD_LIB_ENABLE                  (1U)

#define STD_LIBCFG_SETMEMORY            STD_LIB_ENABLE
#define STD_LIBCFG_COMPARE              STD_LIB_ENABLE
#define STD_LIBCFG_COMPARER             STD_LIB_ENABLE
#define STD_LIBCFG_COPY                 STD_LIB_ENABLE
#define STD_LIBCFG_COPYR                STD_LIB_ENABLE    
#define STD_LIBCFG_CALCULATEAVERAGE     STD_LIB_ENABLE
#define STD_LIBCFG_READDATA             STD_LIB_ENABLE
#define STD_LIBCFG_WRITEDATA            STD_LIB_ENABLE
#define STD_LIBCFG_STATUSFILTER         STD_LIB_ENABLE
#define STD_LIBCFG_STATUSFILTERTYPE2	STD_LIB_ENABLE
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif
/*EOF*/
