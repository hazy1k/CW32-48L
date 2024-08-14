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
static void BTIM_Config(void);
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
BTIM_TimeBaseInitTypeDef BTIM_InitStructure = {0};

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
    RCC_Configuration(); //

    /* GPIO Configuration */
    GPIO_Configuration();

    LED_Init();

    BTIM_Config();
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
    /* 0. HSE使能 */
    __RCC_GPIOF_CLK_ENABLE();
    PF00_ANALOG_ENABLE();
    PF01_ANALOG_ENABLE();

    RCC_HSE_Enable(RCC_HSE_MODE_OSC, 16000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE);
    RCC_HSE_OUT();

    /* 1. 设置HCLK和PCLK的分频系数　*/
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1); 
    
    RCC_SysClk_Switch(RCC_SYSCLKSRC_HSE);    // 切换系统时钟源    
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __RCC_GPIOA_CLK_ENABLE();

    PA07_AFx_BTIM1TOGP();
    PA09_AFx_BTIM1TOGP();
    PA10_AFx_BTIM1TOGN();
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;


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
    NVIC_EnableIRQ(BTIM1_IRQn);
    __enable_irq();
}

static void BTIM_Config(void)
{
    __RCC_BTIM_CLK_ENABLE();

    BTIM_InitStructure.BTIM_Mode = BTIM_Mode_TIMER;
    BTIM_InitStructure.BTIM_OPMode = BTIM_OPMode_Repetitive;
    BTIM_InitStructure.BTIM_Prescaler = 256; // 16MHz / 256分频=62500Hz
    BTIM_InitStructure.BTIM_Period = 62500/20 -1;      // 20Hz溢出一次，触发TOG得到10Hz输出

    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_InitStructure);
    BTIM_OutputToggleCmd(CW_BTIM1, ENABLE);
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);
}

void BTIM1_IRQHandleCallback(void)
{
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
    {
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
        LED1.LED_Toggle(&LED1);
    }
}
void BTIM2_IRQHandleCallback(void)
{
}
void BTIM3_IRQHandleCallback(void)
{
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
