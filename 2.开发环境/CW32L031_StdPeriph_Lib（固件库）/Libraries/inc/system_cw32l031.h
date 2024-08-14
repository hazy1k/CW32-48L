

#ifndef __SYSTEM_CW32L031_H
#define __SYSTEM_CW32L031_H
/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "base_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Global pre-processor symbols/macros ('define')                             */
/******************************************************************************/
#define HWWD_DISABLE        (1)

/**
 ******************************************************************************
 ** \brief Clock Setup macro definition
 **
 ** - 0: CLOCK_SETTING_NONE  - User provides own clock setting in application
 ** - 1: CLOCK_SETTING_CMSIS -
 ******************************************************************************/
#define CLOCK_SETTING_NONE  0u
#define CLOCK_SETTING_CMSIS 1u


/******************************************************************************/
/*                                                                            */
/*                      START OF USER SETTINGS HERE                           */
/*                      ===========================                           */
/*                                                                            */
/*                 All lines with '<<<' can be set by user.                   */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/


extern uint32_t SystemCoreClock;          // System Clock Frequency (Core Clock)
extern void SystemInit (void);            // Initialize the system
//extern void SystemCoreClockUpdate (void); // Update SystemCoreClock variable

/**
 *******************************************************************************
 ** This hook is part of wait loops.
 ******************************************************************************/

extern void FirmwareDelay(uint32_t DlyCnt );
extern void memclr(void* pu8Address, uint32_t u32Count);


#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_CW32L031_H__ */







