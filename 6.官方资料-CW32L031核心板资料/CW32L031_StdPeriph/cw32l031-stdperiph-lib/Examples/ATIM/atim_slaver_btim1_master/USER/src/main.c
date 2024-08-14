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
void ATIM_IRQHandlerCallBack(void);

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
 * @brief  配置ATIM为从设备， BTIM1为主设备。
 *         BTIM1使用内部时钟PCLK计数，4分频，频率为1MHz，单次运行，50ms溢出，触发ATIM运行
 *
 * @return int32_t
 */
int32_t main(void)
{
    ATIM_InitTypeDef ATIM_InitStruct = {0};
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseStruct = {0};

    /* System Clocks Configuration */
    RCC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();

    LogInit();
    printf("Initial is beginning...\n");

    ATIM_InitStruct.BufferState = DISABLE;
    ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;
    ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;
    ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;
    ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_SINGLE;
    ATIM_InitStruct.OverFlowMask = DISABLE;
    ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV1; // 计算时钟4MHz
    ATIM_InitStruct.ReloadValue = 39999;             // 溢出周期10ms
    ATIM_InitStruct.RepetitionCounter = 0;
    ATIM_InitStruct.UnderFlowMask = DISABLE;

    ATIM_Init(&ATIM_InitStruct);
    ATIM_SlaverModeConfig(ATIM_SLAVER_TYPE_TRIGGER);
    ATIM_TriggerSelect(ATIM_TRIGGER_SOURCE_ITR);

    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);
    RCC_ATIMITRSelect(RCC_TIMITR_BTIM1);

    BTIM_TimeBaseStruct.BTIM_Mode = BTIM_Mode_TIMER;
    BTIM_TimeBaseStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;
    BTIM_TimeBaseStruct.BTIM_Period = 49999;
    BTIM_TimeBaseStruct.BTIM_Prescaler = 3;    // 4分频
    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseStruct);

    BTIM_OutputToggleCmd(CW_BTIM1, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);

    printf("ATIM is running.\n");

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
    RCC_HSI_Enable(RCC_HSIOSC_DIV12);
    __RCC_ATIM_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_BTIM_CLK_ENABLE();
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
    GPIO_InitStruct.Pins = GPIO_PIN_0 | GPIO_PIN_9;


    PB00_AFx_BTIM1TOGN();

    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

    PB09_SETLOW();
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);
    __enable_irq();
}

void ATIM_IRQHandlerCallBack(void)
{
    if (ATIM_GetITStatus(ATIM_IT_OVF))
    {
        ATIM_ClearITPendingBit(ATIM_IT_OVF);
        PB09_TOG();
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
