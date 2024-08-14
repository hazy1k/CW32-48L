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
#include "cw32l031_vc.h"

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
 * @brief VC I/O初始化
 *
 */
void VC_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    //打开GPIO时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk );
    //打开VC时钟
    REGBITS_SET( CW_SYSCTRL->APBEN2, SYSCTRL_APBEN2_VC_Msk );

    //set PA11 as output
    GPIO_InitStructure.Pins = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    //set PA11 as VC output
    PA11_AFx_VC1OUT();
    //set PA00 as VC1_CH0 P-INPUT
    PA00_ANALOG_ENABLE();
    //set PA01 as VC1_CH1 N-INPUT
    PA01_ANALOG_ENABLE();
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    VC_InitTypeDef VC_InitStruct = {0};
    VC_BlankTypeDef VC_BlankStruct = {0};
    VC_OutTypeDef VC_OutStruct = {0};

    RCC_HSI_Enable( RCC_HSIOSC_DIV2 ); //配置系统时钟为HSI 24M
    InitTick( 24000000ul ); //初始化SysTick

    //LED初始化
    LED_Init();

    //配置测试IO口
    VC_PortInit();
    //VC通道初始化
    VC_InitStruct.VC_InputP = VC_InputP_Ch0;
    VC_InitStruct.VC_InputN = VC_InputN_Bgr1P2; //VC_InputN_Ch1; //
    VC_InitStruct.VC_Hys = VC_Hys_10mV;
    VC_InitStruct.VC_Resp = VC_Resp_High;
    VC_InitStruct.VC_FilterEn = VC_Filter_Enable;
    VC_InitStruct.VC_FilterClk = VC_FltClk_RC150K;
    VC_InitStruct.VC_FilterTime = VC_FltTime_4095Clk;
    VC_InitStruct.VC_Window = VC_Window_Disable;
    VC_InitStruct.VC_Polarity = VC_Polarity_Low;
    VC1_ChannelInit(&VC_InitStruct);
    //VC Blank窗口初始化
    VC1VC2_BlankInit(&VC_BlankStruct);
    VC1_BlankCfg(&VC_BlankStruct);
    //VC输出连接初始化
    VC1VC2_OutInit(&VC_OutStruct);
    VC1_OutputCfg(&VC_OutStruct);
    //VC中断设置
    VC1_ITConfig(VC_IT_HIGH, ENABLE);
    VC1_EnableIrq(VC_INT_PRIORITY);
    VC1_ClearIrq();
    VC1_EnableChannel();
    while (1)
    {
        if(gFlagIrq)
        {
            PB09_TOG();
            gFlagIrq = FALSE;
            SysTickDelay(1000); //1s.
            VC_EnableNvic(VC1_IRQn, VC_INT_PRIORITY);
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

