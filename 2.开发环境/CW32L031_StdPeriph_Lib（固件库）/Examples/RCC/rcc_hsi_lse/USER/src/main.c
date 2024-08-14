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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample HSI switch to LSI, LSI switch to HSI.
 **
 ******************************************************************************/
int32_t main(void)
{
    uint8_t res = 0U;

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    //PB8/PB9 推挽输出
    CW_SYSCTRL->AHBEN_f.GPIOB = 1;
    GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    
    
    PB08_SETLOW();
    PB09_SETLOW();

    RCC_PCLK_OUT();                           //通过PA03观察PCLK频率
    RCC_HCLK_OUT();                           //通过PA04观察HCLK频率


    while(1)
    {
        //============================================================================
        //以下从HSI切换到LSE
        RCC_LSE_Enable( RCC_LSE_MODE_OSC,RCC_LSE_AMP_LARGER, RCC_LSE_DRIVER_LARGER);                                   //开启外部高速时钟LSE
        res = RCC_SysClk_Switch( RCC_SYSCLKSRC_LSE );                        //切换系统时钟到LSE
        if( res == 0x00 )                                                    //切换系统时钟成功
        {
            RCC_HSI_Disable();                                               //切换时钟到LSE后关闭源时钟HSI           
            res = 10;
            while(res--)
            {
                PB08_TOG();
                PB09_TOG();
                FirmwareDelay(1000);
            } 
        }
        else
        {
            while(1)
            {
                PB08_SETHIGH();
                PB09_SETHIGH();
            }
        }

        //============================================================================
        //以下从LSE切换到HSI
        RCC_HSI_Enable( RCC_HSIOSC_DIV2 );                                   //开启内部高速时钟HSI = HSIOSC / 2
        res = RCC_SysClk_Switch( RCC_SYSCLKSRC_HSI );                        //切换系统时钟到HSI
        if( res == 0x00 )                                                    //切换系统时钟成功
        {
            RCC_LSE_Disable();                                               //切换时钟到HSI后关闭LSE时钟
            res = 10;
            while(res--)
            {
                PB08_TOG();
                PB09_TOG();
                FirmwareDelay(200000);
            } 
        }
        else
        {
            while(1)
            {
                PB08_SETHIGH();
                PB09_SETHIGH();
            }
        }
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

