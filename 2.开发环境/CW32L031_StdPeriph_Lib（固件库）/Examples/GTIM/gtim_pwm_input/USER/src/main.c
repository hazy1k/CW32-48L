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
static uint32_t PWMPeriod = 0;
static uint32_t PWMWidth = 0;

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  使用GTIM1的CH1和CH2两个通道对PWM输入信号进行测量，CH1测量周期，
 *         CH2测量脉宽。
 *         GTIM1_CH1：PA06
 *         GTIM1_CH2: PA07
 *         将同一个PWM输入信号引入到PA06和PA07上。
 * @return int32_t
 */
int32_t main(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};
    GTIM_ICInitTypeDef GTIM_ICInitStruct = {0};
    /* System Clocks Configuration */
    RCC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();

    GTIM_InitStruct.Mode = GTIM_MODE_TIME;
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
    GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;
    GTIM_InitStruct.ReloadValue = 0xFFFF;
    GTIM_InitStruct.ToggleOutState = DISABLE;
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

    GTIM_ICInitStruct.CHx = GTIM_CHANNEL1;
    GTIM_ICInitStruct.ICFilter = GTIM_CHx_FILTER_NONE;
    GTIM_ICInitStruct.ICInvert = GTIM_CHx_INVERT_ON;
    GTIM_ICInitStruct.ICPolarity = GTIM_ICPolarity_Rising;
    GTIM_ICInit(CW_GTIM1, &GTIM_ICInitStruct);

    GTIM_ICInitStruct.CHx = GTIM_CHANNEL2;
    GTIM_ICInitStruct.ICPolarity = GTIM_ICPolarity_Falling;
    GTIM_ICInit(CW_GTIM1, &GTIM_ICInitStruct);

    GTIM_ITConfig(CW_GTIM1, GTIM_IT_CC1 | GTIM_IT_CC2 | GTIM_IT_OV, ENABLE);
    GTIM_Cmd(CW_GTIM1, ENABLE);

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
    __RCC_GTIM1_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();

    RCC_HSI_Enable(RCC_HSIOSC_DIV6);
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
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pins = GPIO_PIN_6 | GPIO_PIN_7;

    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);

    PA06_AFx_GTIM1CH1();
    PA07_AFx_GTIM1CH2();
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(GTIM1_IRQn);
    __enable_irq();
}

void GTIM1_IRQHandlerCallBack(void)
{
    static uint8_t stage = 0;
    static uint32_t cnt = 0;
    if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_OV))
    {
        GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_OV);
        if (stage == 1)
        {
            cnt++;
        }
    }

    if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_CC1))
    {
        if (stage == 0)
        {
            PWMPeriod = GTIM_GetCapture1(CW_GTIM1);
            stage = 1;
        }
        else if (stage == 1)
        {
            PWMPeriod = GTIM_GetCapture1(CW_GTIM1) + cnt * 65536 - PWMPeriod;
            stage = 0;
            cnt = 0;
        }
        GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_CC1);
    }

    if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_CC2))
    {
        if (stage == 1)
        {
            PWMWidth = GTIM_GetCapture2(CW_GTIM1) + cnt * 65536 - PWMPeriod;
            (void) PWMWidth;
        }
        GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_CC2);
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
* @retval None
*/
void
assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

