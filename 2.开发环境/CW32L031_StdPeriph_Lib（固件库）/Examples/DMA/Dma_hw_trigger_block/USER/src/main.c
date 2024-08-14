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
#include "cw32l031_rcc.h"
#include "cw32l031_dma.h"
#include "cw32l031_adc.h"
#include "cw32l031_gpio.h"

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


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
#define DMA_CYCLE_EN  1
uint32_t ADC_Result_Array[16] = {0};

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void ZeroMemory(uint8_t *buf,uint32_t size)
{
    uint32_t i = 0;
    for(i = 0; i < size; i ++)
    {
        *buf = 0x0;
    }
}

static void Error_Handle()
{
    while(1);
}

void DMA_CHANNEL1_IRQ_FUNCTION(void)
{
    if( DMA_GetITStatus(DMA_IT_TC1) )
    {
        DMA_ClearITPendingBit(DMA_IT_TC1);

#ifdef DMA_CYCLE_EN
        CW_DMACHANNEL1->CNT      = 0x10010;                              //重置CNT计数
        CW_DMACHANNEL1->DSTADDR  = (uint32_t)( &ADC_Result_Array[0] );   //重置目的地址
        DMA_Cmd(CW_DMACHANNEL1, ENABLE);  //使能DMA
#else
        ADC_Disable();    //传输完成，停止ADC
#endif
    }

    if( DMA_GetITStatus(DMA_IT_TE1) )
    {
        DMA_ClearITPendingBit(DMA_IT_TE1);
        Error_Handle();
    }
}
/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();

    NVIC_ClearPendingIRQ(DMACH1_IRQn);

    NVIC_EnableIRQ(DMACH1_IRQn);

    __enable_irq();
}


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 **
 **
 ******************************************************************************/

int32_t main(void)
{
    ADC_InitTypeDef   ADC_InitStruct = {0};
    DMA_InitTypeDef   DMA_InitStruct = {0};

    RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_DMA | RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOC, ENABLE);  //Open DMA and GPIOA/C Clk
    RCC_APBPeriphClk_Enable2(RCC_APB2_PERIPH_ADC, ENABLE);    //Open ADC Clk

    //PB08 PB09推挽输出
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    PB08_SETLOW();
    PB09_SETLOW();

    //配置ADC测试IO口
    PA00_ANALOG_ENABLE() ;      //PA00 (AIN0)

    //ADC初始化
    ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode; //单通道单次转换模式
    ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div128; //PCLK
    ADC_InitStruct.ADC_SampleTime = ADC_SampTime5Clk; //5个ADC时钟周期
    ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA; //VDDA参考电压
    ADC_InitStruct.ADC_InBufEn = ADC_BufEnable; //开启跟随器
    ADC_InitStruct.ADC_TsEn = ADC_TsDisable; //内置温度传感器禁用
    ADC_InitStruct.ADC_DMASOCEn = ADC_DMASOCEnable; //ADC转换完成触发DMA传输
    ADC_InitStruct.ADC_Align = ADC_AlignRight; //ADC转换结果右对齐
    ADC_InitStruct.ADC_AccEn = ADC_AccDisable; //转换结果累加不使能
    ADC_Init(&ADC_InitStruct);                                  //初始化ADC配置
    CW_ADC->CR1_f.DISCARD = FALSE;                              //配置数据更新策略，不包含在ADC结构体中
    CW_ADC->CR1_f.CHMUX = ADC_Vref1P2Input;   //ADC_ExInputCH0; //配置ADC输入通道，不包含在ADC结构体中

    ADC_ClearITPendingBit(ADC_IT_EOC);
    ADC_ITConfig(ADC_IT_EOC, ENABLE);
    ADC_EnableNvic(ADC_INT_PRIORITY);

    DMA_StructInit( &DMA_InitStruct );
    ZeroMemory((uint8_t *)&ADC_Result_Array[0],sizeof(ADC_Result_Array));
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_32BIT;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;
#ifdef DMA_CYCLE_EN
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Increase;
#else
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;
#endif

    DMA_InitStruct.DMA_TransferCnt = 16;
    DMA_InitStruct.DMA_SrcAddress = (uint32_t) &(CW_ADC->RESULT0);
    DMA_InitStruct.DMA_DstAddress = (uint32_t)&ADC_Result_Array[0];
    DMA_InitStruct.TrigMode = DMA_HardTrig;
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_SINGLETRANSCOM;
    DMA_Init(CW_DMACHANNEL1,&DMA_InitStruct);
    DMA_ClearITPendingBit(DMA_IT_ALL);
    DMA_ITConfig(CW_DMACHANNEL1, DMA_IT_TC|DMA_IT_TE, ENABLE);   //使能DMA_CHANNEL1中断
    NVIC_Configuration();   //使能DMA_CHANNEL1中断
    DMA_Cmd(CW_DMACHANNEL1, ENABLE);  //使能DMA

    //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

    while(1)
    {
        PB08_TOG();
        PB09_TOG();
        FirmwareDelay(1000000);
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
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

