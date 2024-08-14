/**
 * @file main.c
 * @author WHXY
 * @brief
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
BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct = {0};
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief
 *
 * @return int32_t
 */
int32_t main(void)
{
    /* System Clocks Configuration */
    RCC_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    /* 使用BTIM1的OV信号作为BTIM2的ITR输入，实现对PCLK的任意分频 */

    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER;
    BTIM_TimeBaseInitStruct.BTIM_Period = 49999;
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = 7;    // 8分频
    BTIM_TimeBaseInitStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;
    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);

    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_COUNTER;
    BTIM_TimeBaseInitStruct.BTIM_Period = 9;
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = 1;     // 1分频，使用TRS计数时，分频系数不加1
    BTIM_TimeBaseInitStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;
    BTIM_TimeBaseInit(CW_BTIM2, &BTIM_TimeBaseInitStruct);

    RCC_BTIM2ITRSelect(RCC_TIMITR_BTIM1);
    BTIM_SelectInputTrigger(CW_BTIM2, BTIM_TS_ITR);
    BTIM_OutputToggleCmd(CW_BTIM2, ENABLE); // BTIM2的toggle输出使能，用于测量
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    BTIM_ITConfig(CW_BTIM2, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM2, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);

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
    //   CW_SYSCTRL->APBEN2_f.BTIM = 1U;
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);
    __RCC_BTIM_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_5;

    GPIO_Init(CW_GPIOA, &GPIO_InitStruct); // PA5

    PA05_AFx_BTIM2TOGP(); // BTIM2的TOG输出分配到PA5

    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_9 | GPIO_PIN_8;

    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

    PB09_SETLOW();
    PB08_SETLOW();
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(BTIM1_IRQn);
    NVIC_EnableIRQ(BTIM2_IRQn);
    __enable_irq();
}

void BTIM1_IRQHandleCallback(void)
{

    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
    {
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
        PB09_TOG();
    }
}

void BTIM2_IRQHandleCallback(void)
{
    if (BTIM_GetITStatus(CW_BTIM2, BTIM_IT_OV))
    {
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_OV);
        PB08_TOG();
    }
}

void BTIM3_IRQHandleCallback(void)
{
}

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
