/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-7-12
 *
 * @copyright Copyright (c) 2021
 *
 */
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "..\inc\main.h"
#include "cw32l031_systick.h"
#include "cw32l031_rcc.h"
#include "cw32l031_gpio.h"
#include "cw32l031_pwr.h"
#include "cw32l031_flash.h"
#include "cw32l031_pwr_modes.h"


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
volatile uint8_t gKeyStatus;  /* set to 1 after User Button interrupt  */
volatile uint32_t gFlagWakeUpIrq = 0;  /* set to 1 after RTC alarm interrupt    */

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    RCC_HSI_Enable( RCC_HSIOSC_DIV2); //配置系统时钟为HSI 24M
    //Enable Systick Clk
    InitTick(24000000ul); //初始化SysTick

    //Test only.
    RCC_PCLK_OUT();    // PA3输出PCLK

    //LED初始化
    LED_Init();

    BSP_PB_Init();

    while (1)
    {
        /* Configure User push-button as external interrupt generator */
        BSP_PB_Init();
        gKeyStatus = 0;

        /* Wait until User push-button is pressed to enter the Low Power mode.
         In the meantime, LED is blinks */
        do
        {
            /* Toggle LED */
            PB09_TOG();
            SysTickDelay(1000); //1s.
        }
        while (gKeyStatus == 0);

        /* Make sure LED is turned off to
         reduce low power mode consumption */
        PB09_SETLOW();

#if defined (SLEEP_MODE)
        /* Sleep Mode Entry
         - System Running at HSI (24 MHz)
         - Code running from Internal FLASH
         - All peripherals disabled.
         - Wakeup using EXTI Line (User push-button PA.05)
         */
        SleepModeTest();

#elif defined (DEEPSLEEP_MODE)
        /* DeepSleep Mode Entry
          - No IWDG
          - Wakeup using EXTI Line (User push-button PA.05)
        */
        DeepSleepModeTest();
#else
#error "Please select low power mode first!"
#endif /* SLEEP_MODE */

        //Test only.
        HclkOutTest();
    }

}

/**
 * @brief Configure User push-button as external interrupt generator
 *
 */
void BSP_PB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //打开GPIO时钟
    REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk);

    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    //Configure Button pin as input with External interrupt
    GPIO_InitStructure.IT = GPIO_IT_FALLING;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    //Enable and set Button EXTI Interrupt to the lowest priority
    NVIC_SetPriority(GPIOA_IRQn, 0x03);
    //清除中断标志并使能NVIC
    GPIOA_INTFLAG_CLR(GPIOx_ICR_PIN2_Msk);
    NVIC_EnableIRQ(GPIOA_IRQn);
}

/**
 * @brief LED I/O初始化
 *
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    //打开GPIO时钟
    REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOB_Msk);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pins = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    //LEDs are off.
    PB08_SETLOW();
    PB09_SETLOW();
}
/**
 * @brief This function is executed in case of error occurrence.
 *
 */
void Error_Handler(void)
{
    while (1)
    {
        /* Turn LED1,LED2 on */
        PB08_SETHIGH();
        PB09_SETHIGH();
    }
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @return None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

