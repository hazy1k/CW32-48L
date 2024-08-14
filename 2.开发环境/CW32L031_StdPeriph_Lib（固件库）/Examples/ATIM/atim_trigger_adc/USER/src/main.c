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
static void ATIM_Config(void);
static void ADC_Config(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint16_t ADC_ConvertedValue = 0;
float ADC_ConvertedVoltage = 0.0;
uint8_t ADC_ConvertedFlag = 0;
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  本例中，ATIM在PWM模式下配置，ATIM的OC3B时间用于触发ADC
 * ADC配置为单通道单次转换
 * @return int32_t
 */
int32_t main(void)
{
    /* System Clocks Configuration */
    RCC_Configuration(); // HSI时钟源，PCLK=HCLK=8MHz

    /* GPIO Configuration */
    GPIO_Configuration();

    ATIM_Config();
    ADC_Config();

    /* NVIC Configuration */
    NVIC_Configuration();

    while (1)
    {
        if (ADC_ConvertedFlag)
        {
            ADC_ConvertedFlag = 0;
            ADC_ConvertedVoltage = ADC_ConvertedValue * 3.3 / 0xfff;
        }
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* 0. HSI使能并校准 */
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);

    /* 1. 设置HCLK和PCLK的分频系数　*/
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
    
    RCC_SystemCoreClockUpdate(8000000);

    /* 2.使能外设时钟 */
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_ATIM_CLK_ENABLE();
    __RCC_ADC_CLK_ENABLE();
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
    GPIO_InitStruct.Pins = GPIO_PIN_9;


    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

    PB00_ANALOG_ENABLE();
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pins = GPIO_PIN_0;

    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
}

/**
 * @brief ATIM边沿对齐，上计数，64分频，DCLK=1MHz，period=10ms
 *        OC3B匹配事件触发ADC
 */
static void ATIM_Config(void)
{
    ATIM_InitTypeDef ATIM_InitStruct = {0};
    ATIM_OCInitTypeDef ATIM_OCInitStruct = {0};

    __RCC_ATIM_CLK_ENABLE();

    ATIM_InitStruct.BufferState = ENABLE;                            //使能缓存寄存器
    ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;                   //选择PCLK时钟计数
    ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN; //边沿对齐
    ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;             //向上计数；
    ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;         //连续运行模式
    ATIM_InitStruct.OverFlowMask = DISABLE;                          // 重复计数器上溢出不屏蔽
    ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV8;                 // 8分频
    ATIM_InitStruct.ReloadValue = 9999;                              // 重载周期9999+1
    ATIM_InitStruct.RepetitionCounter = 0;                           // 重复周期0
    ATIM_InitStruct.UnderFlowMask = DISABLE;                         // 重复计数下溢出不屏蔽

    ATIM_Init(&ATIM_InitStruct);

    ATIM_OCInitStruct.BufferState = DISABLE;
    ATIM_OCInitStruct.OCDMAState = DISABLE;
    ATIM_OCInitStruct.OCInterruptSelect = ATIM_OC_IT_UP_COUNTER;
    ATIM_OCInitStruct.OCInterruptState = DISABLE;
    ATIM_OCInitStruct.OCMode = ATIM_OCMODE_PWM1;
    ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_NONINVERT;
    ATIM_OC3BInit(&ATIM_OCInitStruct);

    ATIM_ADCTriggerConfig(ATIM_ADC_TRIGGER_CH3B, ENABLE);
    ATIM_SetCompare3B(4999);
    ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);

    ATIM_Cmd(ENABLE);
}

/**
 * @brief ADC 使用通道8单次转换，转换结束后触发中断
 *
 */
static void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStruct = {0};

    __RCC_ADC_CLK_ENABLE();

    ADC_InitStruct.ADC_AccEn = DISABLE;
    ADC_InitStruct.ADC_Align = ADC_AlignRight;
    ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div4; // ADCCLK=2MHz
    ADC_InitStruct.ADC_InBufEn = DISABLE;
    ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode;
    ADC_InitStruct.ADC_SampleTime = ADC_SampTime5Clk;
    ADC_InitStruct.ADC_TsEn = DISABLE;
    ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA;

    ADC_Init(&ADC_InitStruct);

    CW_ADC->CR1_f.CHMUX = 8; // PB0
    ADC_ExtTrigCfg(ADC_TRIG_ATIM, ENABLE);
    ADC_ITConfig(ADC_IT_EOC, ENABLE);
    ADC_Enable();
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
    NVIC_EnableIRQ(ADC_IRQn);
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

void ADC_IRQHandlerCallBack(void)
{
    if (ADC_GetITStatus(ADC_IT_EOC))
    {
        ADC_ClearITPendingBit(ADC_IT_EOC);
        ADC_ConvertedValue = ADC_GetConversionValue();
        ADC_ConvertedFlag = 1;
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
