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
#include "cw32l031_adc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t valueAdc;
uint32_t valueAdcAcc;
volatile uint8_t gFlagIrq;
uint16_t gCntEoc = 0;
uint8_t cntSample;

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
 * @brief ADC I/O初始化
 *
 */
void ADC_PortInit(void)
{
    //打开GPIO时钟
    REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk);
    //打开ADC时钟
    REGBITS_SET(CW_SYSCTRL->APBEN2, SYSCTRL_APBEN2_ADC_Msk);

    //set PA01 as AIN1 INPUT
    PA01_ANALOG_ENABLE();
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    ADC_WdtTypeDef ADC_WdtStructure = {0};
    ADC_SingleChTypeDef ADC_SingleChStructure = {0};


    //LED初始化
    LED_Init();

    //使用sysTick作为1ms
    InitTick(8000000);

    //配置ADC测试IO口
    ADC_PortInit();

    //ADC默认值初始化
    ADC_StructInit(&ADC_InitStructure);
    //ADC工作时钟配置
#ifdef SGL_MORE_MODE_EN
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div4;
#else
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;
#endif

    //ADC模拟看门狗通道初始化
    ADC_WdtInit(&ADC_WdtStructure);

    //配置单通道转换模式
    ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull;
    ADC_SingleChStructure.ADC_Chmux = ADC_ExInputCH1; //选择ADC转换通道，AIN1:PA01
    ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure;
    ADC_SingleChStructure.ADC_WdtStruct = ADC_WdtStructure;

#ifdef SGL_ONE_MODE_EN
    ADC_SingleChOneModeCfg(&ADC_SingleChStructure);
    ADC_ITConfig(ADC_IT_EOC, ENABLE);

#elif defined (SGL_MORE_MODE_EN)
    cntSample = 0xFF; //转换次数为0xFF + 1次.
    ADC_SingleChMoreModeCfg(&ADC_SingleChStructure, cntSample);
    ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOA, ENABLE);

#elif defined (SGL_CONTINUOUS_MODE_EN)
    ADC_SingleChContinuousModeCfg(&ADC_SingleChStructure);
    ADC_ITConfig(ADC_IT_EOC, ENABLE);
#else
#error "Please select ADC's running mode first in main.h!"
#endif

    //ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOA, ENABLE);
    ADC_EnableIrq(ADC_INT_PRIORITY);
    ADC_ClearITPendingAll();

    //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

    while (1)
    {
#ifdef SGL_MORE_MODE_EN
        while (!(gFlagIrq & ADC_ISR_EOA_Msk));
        gFlagIrq = 0u;
        ADC_GetAccResult(&valueAdcAcc);

        if (gCntEoc != (cntSample + 1))
        {
            PB08_SETHIGH();
            PB09_SETHIGH();
            while (1); //error issue.
        }
        gCntEoc = 0u;

#else
        while (!(gFlagIrq & ADC_ISR_EOC_Msk));
        gFlagIrq = 0u;
        valueAdc = ADC_GetConversionValue();
#endif

        PB09_TOG();
        SysTickDelay(300);

#ifndef SGL_CONTINUOUS_MODE_EN
        ADC_SoftwareStartConvCmd(ENABLE); //启动下一次ADC转换
#endif

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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

