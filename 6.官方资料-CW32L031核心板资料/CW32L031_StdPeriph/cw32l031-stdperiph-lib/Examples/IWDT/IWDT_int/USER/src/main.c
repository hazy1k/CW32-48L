/**
 * @file main.c
 * @author WHXY
 * @version 0.1
 * @date 2022-7-12
 *
 * @copyright Copyright (c) 2021
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
#include "../inc/main.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IWDT_DURATION 250 // 250ms
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

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
 * @brief
 * @return int32_t
 */
int32_t main(void)
{
    IWDT_InitTypeDef IWDT_InitStruct = {0};

    /* System Clocks Configuration */
    RCC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    SysTick_Config(SystemCoreClock / 1000 * 250); // 250ms
    LED_Init();    

    IWDT_InitStruct.IWDT_ITState = ENABLE;
    IWDT_InitStruct.IWDT_OverFlowAction = IWDT_OVERFLOW_ACTION_INT; //溢出后产生中断不复位
    IWDT_InitStruct.IWDT_Pause = IWDT_SLEEP_PAUSE;
    IWDT_InitStruct.IWDT_Prescaler = IWDT_Prescaler_DIV4;
    IWDT_InitStruct.IWDT_ReloadValue = (IWDT_FREQ >> 2) / 1000 * 280 - 1; // 由于IWDT的时钟为RC10K, 设置为280实际溢出时间为256ms左右
    IWDT_InitStruct.IWDT_WindowValue = 0xFFF;

    IWDT_Init(&IWDT_InitStruct);

    IWDT_Cmd();

    /* NVIC Configuration */
    NVIC_Configuration();

    while (1)
    {
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    ///< 当使用的时钟源HCLK大于24M,小于等于48MHz：设置FLASH 读等待周期为2 cycle
    ///< 当使用的时钟源HCLK大于48M,小于等于72MHz：设置FLASH 读等待周期为3 cycle
    __RCC_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_2);
    
    /* 0. HSI使能并校准 */
    RCC_HSI_Enable(RCC_HSIOSC_DIV1);

    /* 1. 设置HCLK和PCLK的分频系数　*/
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
    RCC_SystemCoreClockUpdate(48000000);

    /* 4. 配置外设时钟 */
    __RCC_IWDT_CLK_ENABLE(); //使能IWDT模块
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 配置PA2为按键输入 */

    GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pins = GPIO_PIN_2;
    
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(WDT_IRQn);
    NVIC_SetPriority(GPIOA_IRQn, 0);
    NVIC_SetPriority(SysTick_IRQn, 1);
    NVIC_EnableIRQ(GPIOA_IRQn);
    __enable_irq();
}

void GPIOA_IRQHandlerCallback(void)
{
}

void SysTick_HandlerCallback(void)
{
    LED1.LED_Toggle(&LED1);
    IWDT_Refresh();
}

void WDT_IRQHandlerCallback(void)
{
    if (CW_IWDT->SR & IWDT_SR_OV_Msk)
    {
        IWDT_ClearOVFlag();
        LED2.LED_Toggle(&LED2);
    }
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
