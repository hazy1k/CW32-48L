/*
* cw32l031_pwr_modes.c
*
*  Created on: 2022-01-18
*      Author: Socrates
*/

/**
******************************************************************************
* @file    PWR/PWR_CurrentConsumption/cw32l031_pwr_modes.c
* @author WHXY
* @brief   This file provides firmware functions to manage the following
*          functionalities of the CW32L031 Low Power Modes:
*           - Sleep Mode
*           - DeepSleep mode with or without RTC
******************************************************************************/



/* Includes ------------------------------------------------------------------*/
#include "cw32l031_pwr_modes.h"
#include "..\inc\main.h"
#include "cw32l031_pwr.h"
#include "cw32l031_gpio.h"
#include "cw32l031_systick.h"
#include "cw32l031_rcc.h"
#include "cw32l031_flash.h"
#include "cw32l031_vc.h"
#include "cw32l031_adc.h"

/** @addtogroup CW32L031_HAL_Examples
* @{
*/

/** @addtogroup PWR_CurrentConsumption
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
//RTC_HandleTypeDef RTCHandle = {0};

/* Private function prototypes -----------------------------------------------*/
static void SYSCLKConfig_DeepSleep(void);

/* Private functions ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern volatile uint8_t gKeyStatus;
/* USER CODE END EV */

/**
* @brief  This function configures the system to enter Sleep mode for
*         current consumption measurement purpose.
*         Sleep Mode
*         ==========
*            - System Running at HSI (24 MHz)
*            - Code running from Internal FLASH
*            - All peripherals disabled.
*            - Wakeup using EXTI Line (User push-button)
* @param  None
* @return None
*/
void SleepModeTest(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    PWR_InitTypeDef PWR_InitStructure = {0};

    /* Configure all GPIO as analog to reduce current consumption on non used IOs */
    /* Enable GPIOs clock */
    /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
    //打开GPIO时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
                 SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Pins = GPIO_PIN_All;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    GPIO_Init(CW_GPIOC, &GPIO_InitStructure);
    GPIO_Init(CW_GPIOF, &GPIO_InitStructure);

    /* Disable GPIOs clock */
    REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
                 SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

    /* Configure User push-button as external interrupt generator */
    BSP_PB_Init();

    /*Suspend Tick increment to prevent wakeup by Systick interrupt.
    Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
    SuspendTick();

    /* Request to enter SLEEP mode */
    //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
    PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Disable;
    PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
    PWR_Config(&PWR_InitStructure);
    PWR_GotoLpmMode();

    /* Resume Tick interrupt if disabled prior to sleep mode entry*/
    ResumeTick();

    /* Initialize led */
    LED_Init();
}

/**
* @brief  This function configures the system to enter Deep-Sleep mode
*         for current consumption measurement purpose.
*         Deep-Sleep Mode
*         ==========
*           - No IWDG
*           - Wakeup using EXTI Line (User push-button)
* @param  None
* @return None
*/
void DeepSleepModeTest(void)
{
    uint8_t res = 0u;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    PWR_InitTypeDef PWR_InitStructure = {0};

    /* Configure all GPIO as analog to reduce current consumption on non used IOs */
    /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
    /* Enable GPIOs clock */
    //打开GPIO时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
                 SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Pins = GPIO_PIN_All;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    GPIO_Init(CW_GPIOC, &GPIO_InitStructure);
    GPIO_Init(CW_GPIOF, &GPIO_InitStructure);

    //关闭GPIO时钟
    REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
                 SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

    /* Configure User Button */
    BSP_PB_Init();

    /* Configures system clock after wake-up from DeepSleep: enable HSI and PLL with HSI as source*/
    // 唤醒后自动使用内部高速时钟（HSI）
    RCC_WAKEUPCLK_Config(RCC_SYSCTRL_WAKEUPCLKEN);

    /* Enter Stop Mode */
    PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
    PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Enable;
    PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
    PWR_Config(&PWR_InitStructure);

    //打开FLASH时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);
    REGBITS_SET(CW_FLASH->CR1, FLASH_CR1_STANDBY_Msk); //打开FLASH低功耗使能控制
    //关闭FLASH时钟
    REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);

#ifdef SLEEP_LSE_EN
    /*Suspend Tick increment to prevent wakeup by Systick interrupt.
    Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
    //SuspendTick();	//Comment it if using HSI.

    //Open LSE.
    RCC_LSE_Enable( RCC_LSE_MODE_OSC,RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_LARGER);                                   //开启外部高速时钟LSE
    //Switch clock to LSE from HSI
    res = RCC_SysClk_Switch( RCC_SYSCLKSRC_LSE );	//切换系统时钟到LSE
    if( res == 0x00 )	//切换系统时钟成功
    {
        RCC_HSI_Disable();	//切换时钟到LSE后关闭源时钟HSI
        FirmwareDelay( 1400 );
    }
    else
    {
        Error_Handler();
    }
#endif

    PWR_GotoLpmMode();

#ifdef SLEEP_LSE_EN
    /* Resume Tick interrupt if disabled prior to sleep mode entry*/
    //ResumeTick();

    RCC_LSE_Disable();
#endif

    /* Configures system clock after wake-up from DeepSleep: enable HSI and PLL with HSI as source*/
    SYSCLKConfig_DeepSleep();

    /* Initialize LED on the board */
    LED_Init();

    /* Inserted Delay */
    SysTickDelay(200);

    /* Turn LED1 On */
    PB09_SETHIGH();
}


/**
* @brief  Configures system clock after wake-up from DeepSleep: enable HSE, PLL
*         and select PLL as system clock source.
* @param  None
* @return None
*/
static void SYSCLKConfig_DeepSleep(void)
{
    FirmwareDelay( 1000ul );
    //打开FLASH时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);
    FLASH_SetLatency( FLASH_Latency_2 ); //系统时钟大于24M需要配置FlashWait
    //关闭FLASH时钟
    REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);

    RCC_HSI_Enable( RCC_HSIOSC_DIV1); //配置系统时钟为HSI 48M

    /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
    RCC_SYSCLKSRC_Config(RCC_SYSCLKSRC_HSI);

}

/**
* @brief 系统时钟测试
*
*/
void HclkOutTest(void)
{
    //打开GPIO时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk );
    //配置PA3,PA4为输出
    REGBITS_CLR( CW_GPIOA->ANALOG, bv3 | bv4 );
    REGBITS_CLR( CW_GPIOA->DIR, bv3 | bv4 );
    PA03_AFx_PCLKOUT(); //配置PA03输出PCLK
    PA04_AFx_HCLKOUT(); //配置PA04输出HCLK
}


/*****************************END OF FILE****/
