/*
 * cw32l031_pwr_modes.h
 *
 *  Created on: 2021Äê6ÔÂ04ÈÕ
 *      Author: Socrates
 */

#ifndef _CW32L031_PWR_MODES_H
#define _CW32L031_PWR_MODES_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#if !defined (SLEEP_MODE) && !defined (DEEPSLEEP_MODE)

    /* Uncomment the corresponding line to select the CW32L031 Low Power mode
    you want to mesure  */
    //#define SLEEP_MODE
    #define DEEPSLEEP_MODE
#endif

//#define SLEEP_LSE_EN

#if !defined (SLEEP_MODE) && !defined (DEEPSLEEP_MODE)
    #error "Please select first the target CW32L031 Low Power mode to be measured (in cw32l031_pwr_modes.h file)"
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SleepModeTest(void);
void DeepSleepModeTest(void);

void HclkOutTest(void);
#endif /* _CW32L031_PWR_MODES_H */
