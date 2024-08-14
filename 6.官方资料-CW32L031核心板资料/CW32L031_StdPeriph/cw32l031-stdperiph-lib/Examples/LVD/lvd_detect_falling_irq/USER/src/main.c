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
#include "cw32l031_lvd.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
volatile boolean_t gFlagIrq = FALSE;

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
 * @brief LVD I/O初始化
 *
 */
void LVD_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //打开GPIO时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk );
    //打开LVD时钟:PCLK.
    //...

    //set PA08 as output
    GPIO_InitStructure.Pins = GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    //set PA08 as LVD output
    PA08_AFx_LVDOUT();
    //set PB11 as LVD source
    PB11_ANALOG_ENABLE();
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    LVD_InitTypeDef LVD_InitStruct = {0};

    //LED初始化
    LED_Init();

    //配置测试IO口
    LVD_PortInit();

    LVD_InitStruct.LVD_Action = LVD_Action_Irq; //配置中断功能
    LVD_InitStruct.LVD_Source = LVD_Source_PB11; //配置LVD输入口
    LVD_InitStruct.LVD_Threshold = LVD_Threshold_1p8V; //配置LVD基准电压
    LVD_InitStruct.LVD_FilterEn = LVD_Filter_Enable;
    LVD_InitStruct.LVD_FilterClk = LVD_FilterClk_RC150K;
    LVD_InitStruct.LVD_FilterTime = LVD_FilterTime_4095Clk;
    LVD_Init(&LVD_InitStruct);

    LVD_TrigConfig(LVD_TRIG_FALL, ENABLE);
    LVD_EnableIrq(LVD_INT_PRIORITY);
    LVD_ClearIrq();
    FirmwareDelay(4800); //delay100us(1);
    LVD_Enable(); //LVD使能

    while (1)
    {
        if (gFlagIrq)
        {
            PB09_TOG();
            gFlagIrq = FALSE;
        }
    }
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

