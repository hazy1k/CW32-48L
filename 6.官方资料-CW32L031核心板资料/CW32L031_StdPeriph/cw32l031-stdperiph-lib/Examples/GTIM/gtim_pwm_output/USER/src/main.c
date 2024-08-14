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


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint32_t Period = 500;    // 周期，单位us
uint32_t PosWidth = 0;    // 正脉宽，单位us
uint8_t Dir = 1;    // 计数方向 1增加，0 减少
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void PWM_OutputConfig(void);

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
 * @brief  GTIM1计数，CH3通道比较，PB08做PWM输出
 *         中断服务程序中，每4000个周期，增加1/4
 * @return int32_t
 */
int32_t main(void)
{
    /* System Clocks Configuration */
    RCC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    PWM_OutputConfig();

    /* NVIC Configuration */
    NVIC_Configuration();

    while(1)
    {
        /* 中断服务程序见GTIM1_IRQHandler() */

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
    
    __RCC_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_2);
    
    /* 0. HSI使能并校准 */
    RCC_HSI_Enable(RCC_HSIOSC_DIV1);

    /* 1. 设置HCLK和PCLK的分频系数　*/
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
    RCC_SystemCoreClockUpdate(48000000);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    /* PB8(LED2) 作为GTIM1的CH3 PWM 输出 */
    __RCC_GPIOB_CLK_ENABLE();

    PB08_AFx_GTIM1CH3();
    PB08_DIGTAL_ENABLE();
    PB08_DIR_OUTPUT();
    PB08_PUSHPULL_ENABLE();

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

void PWM_OutputConfig(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};

    __RCC_GTIM1_CLK_ENABLE();

    GTIM_InitStruct.Mode = GTIM_MODE_TIME;
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
    GTIM_InitStruct.Prescaler = 11;    // 计数时钟TCLKD = TCLK/(PSC+1)
    GTIM_InitStruct.ReloadValue = 2000 - 1;
    GTIM_InitStruct.ToggleOutState = DISABLE;

    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL3, GTIM_OC_OUTPUT_PWM_HIGH);
    GTIM_SetCompare3(CW_GTIM1, PosWidth);
    GTIM_ITConfig(CW_GTIM1, GTIM_IT_OV, ENABLE);

    GTIM_Cmd(CW_GTIM1, ENABLE);
}

void GTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */
    // 中断每500us进入一次，每50ms改变一次PosWidth
    static uint16_t TimeCnt = 0;
    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_OV);
    if (TimeCnt++ >= 100)    // 50ms
    {
        TimeCnt = 0;
        if (Dir)
        {
            PosWidth += 20;    // 5us
        }
        else
        {
            PosWidth -= 20;
        }
        if (PosWidth >= CW_GTIM1->ARR)
        {
            Dir = 0;
        }
        if (0 == PosWidth)
        {
            Dir = 1;
        }
        GTIM_SetCompare3(CW_GTIM1, PosWidth);
    }
    /* USER CODE END */
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
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

