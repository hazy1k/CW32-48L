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

static void GTIM1_Config(void);
static void GTIM2_Config(void);

void IrDA_Send(const uint8_t *pData, uint16_t len);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

uint32_t DelayTime;
static uint8_t DataBuffer[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
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
    /* System Clocks Configuration */
    RCC_Configuration();      // HSI时钟源，PCLK=HCLK=48MHz

    /* SWD 口将做普通IO口用，这段代码防止不能烧程序 */


    /* 关闭SWD口 */
    // GPIO_SWD2GPIO();

    /* GPIO Configuration */
    GPIO_Configuration();

    GTIM1_Config();
    GTIM2_Config();

    CW_SYSCTRL->IRMOD_f.MOD = 0;

    /* NVIC Configuration */
    NVIC_Configuration();

    while(1)
    {
        IrDA_Send(DataBuffer, 10);
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    ///< 当使用的时钟源HCLK大于24M,小于等于24MHz：设置FLASH 读等待周期为2 cycle
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
  *  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __RCC_GPIOB_CLK_ENABLE();
    PB09_AFx_IROUT();    

    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = GPIO_PIN_9;

    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();

    __enable_irq();
}

/**
 * @brief GTIM1设置为PWM，产生38KHz的载波
 *
 */
static void GTIM1_Config(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};

    __RCC_GTIM1_CLK_ENABLE();

    GTIM_InitStruct.Mode = GTIM_MODE_TIME;
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
    GTIM_InitStruct.Prescaler = 2;    // DCLK = 16MHz
    GTIM_InitStruct.ReloadValue = 16000 / 38 - 1;
    GTIM_InitStruct.ToggleOutState = DISABLE;

    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

    GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_LOW);
    GTIM_SetCompare1(CW_GTIM1, GTIM_InitStruct.ReloadValue / 3);

    GTIM_Cmd(CW_GTIM1, ENABLE);
}

static void GTIM2_Config(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};

    __RCC_GTIM2_CLK_ENABLE();

    GTIM_InitStruct.Mode = GTIM_MODE_TIME;
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
    GTIM_InitStruct.Prescaler = 47; // DCLK = 1MHz
    GTIM_InitStruct.ReloadValue = 2240-1;
    GTIM_InitStruct.ToggleOutState = DISABLE;

    GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct);

    GTIM_OCInit(CW_GTIM2, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_LOW);
    GTIM_SetCompare1(CW_GTIM2, 560);

    // GTIM_Cmd(CW_GTIM2, ENABLE);
}



void IrDA_Send(const uint8_t *pData, uint16_t len)
{
    uint8_t i;

    CW_GTIM1->CR0_f.EN = 1;
    while (len--)
    {
        for (i = 0; i < 8; i++)
        {
            if (*pData & (1 << i))
            {
                CW_GTIM2->CR0_f.EN = 0;
                CW_GTIM2->ICR_f.OV = 0;
                CW_GTIM2->CNT = 0;
                CW_GTIM2->CCR1 = 560;
                CW_GTIM2->ARR = 2239;
                CW_GTIM2->CR0_f.EN = 1;
                while(!CW_GTIM2->ISR_f.OV);
            }
            else
            {
                CW_GTIM2->CR0_f.EN = 0;
                CW_GTIM2->ICR_f.OV = 0;
                CW_GTIM2->CNT = 0;
                CW_GTIM2->CCR1 = 560;
                CW_GTIM2->ARR = 1119;
                CW_GTIM2->CR0_f.EN = 1;
                while (!CW_GTIM2->ISR_f.OV)
                    ;
            }
        }
    }
    CW_GTIM1->CNT = 0;

    CW_GTIM1->CR0_f.EN = 0;
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

