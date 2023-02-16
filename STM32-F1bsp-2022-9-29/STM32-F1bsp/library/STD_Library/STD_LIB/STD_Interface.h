/*******************************************************************************
|  File Name:  STD_Interface.h
|  Description:  Implementation of the Interface
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

#if !defined (_STD_INTERFACE_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_INTERFACE_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
/* polyspace<MISRA-C3:1.1:Not a defect:Justified> Normal */
#include "stm32f10x.h"




/* polyspace<MISRA-C3:1.1:Not a defect:Justified> Normal */
/*#include "interrupt_manager.h"*/
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* PRQA S 3458 L1 */ /* MD_MSR_19.4 *//* polyspace<MISRA-C3:D4.9:Not a defect:Justified> Normal */
#define	LIB_ENTER_EXCLUSIVE_AREA_0()    MCAL_MCUCORE_ENTER_CRITICAL_AREA()     /*SuspendAllInterrupts()*/
/* PRQA L:L1 */

/* PRQA S 3458 L1 */ /* MD_MSR_19.4 *//* polyspace<MISRA-C3:D4.9:Not a defect:Justified> Normal */
#define	LIB_EXIT_EXCLUSIVE_AREA_0()     MCAL_MCUCORE_EXIT_CRITICAL_AREA()     /*ResumeAllInterrupts()*/
     
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
