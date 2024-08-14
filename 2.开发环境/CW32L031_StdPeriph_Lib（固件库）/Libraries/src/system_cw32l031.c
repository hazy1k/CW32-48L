/******************************************************************************
* Include files                                                              *
******************************************************************************/
#include "cw32l031.h"


/******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/

uint32_t SystemCoreClock = 8000000;

void SystemCoreClockUpdate(void) // Update SystemCoreClock variable
{
    SystemCoreClock = 8000000;
}

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemInit(void)
{
    //Load TrimCode in this.
    //...

    SystemCoreClockUpdate();
    CW_SYSCTRL->HSI_f.TRIM = *((volatile uint16_t *)0x00100A00U);
    CW_SYSCTRL->LSI_f.TRIM = *((volatile uint16_t *)0x00100A02U);
    //Init Hide thing.
    //...
}


void FirmwareDelay(uint32_t DlyCnt )
{
    volatile uint32_t thisCnt = DlyCnt;
    while( thisCnt-- )
    {
        ;
    }
}

/**
 * \brief   clear memory
 *
 * \param   [in]  start addr
 * \param   [in]  memory size(byte)
 * \retval  void
 */
void MemClr(void *pu8Address, uint32_t u32Count)
{
    uint8_t *pu8Addr = (uint8_t *)pu8Address;

    if(NULL == pu8Addr)
    {
        return;
    }

    while (u32Count--)
    {
        *pu8Addr++ = 0;
    }
}

