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
#include "cw32l031_flash.h"
#include "cw32l031_gpio.h"
#include "cw32l031cxtx_startkit.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t AddrBuf[256];

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);

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
 * @brief Main function of project
 * //对PAGE511进行擦除，并验证。
 * //对PAGE511进行写入，并验证。
 * @return int32_t
 */

int32_t main(void)
{
    uint8_t  Flag, tmp8;
    uint16_t i;

    //时钟初始化
    RCC_Configuration();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //PB8/PB9 推挽输出
    GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

    PB08_SETHIGH();
    PB09_SETHIGH();
    FirmwareDelay(2000000);    //暂停
    
    // 字节写入
    //erase
    FLASH_UnlockPages(512*127, 512*127);    //解锁127页
    Flag = FLASH_ErasePages(512*127, 512*127);    //擦除最后1页面
    FLASH_LockAllPages();
    if( Flag )
    {
        while(1);  //擦除失败
    }
    // blank check
    for( i=0; i<512; i++ )
    {
        tmp8 =  *((volatile uint8_t *)( 512*127 + i ));
        if( tmp8 != 0xFF )
        {
            while(1);  //对擦除后的结果进行验证失败
        }
    }
    //write
    for( i=0; i<=255; i++ )
    {
        AddrBuf[i] = i;
    }
    FLASH_UnlockPages( 512*127, 512*127 );    //解锁最后一个页面
    Flag = FLASH_WriteBytes(512*127, AddrBuf, 256 );   // 127页按字节写入
    if( Flag )
    {
        while(1);  //写入失败
    }
    //read compare
    for( i=0; i<=255; i++ )
    {
        tmp8 =  *((volatile uint8_t *)( 512*127 + i ));
        if( tmp8 != i )
        {
            while(1);  //对127页写入的数据进行验证失败
        }
    }
    for(i = 6; i; i--)
    {
        PB08_TOG();
        PB09_TOG();
        FirmwareDelay(50000);    //快闪表示通过
    }
    FirmwareDelay(2000000);    //暂停
    // 半字写入
    FLASH_UnlockPages( 512*127, 512*127 );    //解锁127页面
    Flag = FLASH_ErasePages(512*127, 512*127);    //擦除
    FLASH_LockAllPages();
    if( Flag )
    {
        while(1);  //擦除失败
    }
    for( i=0; i<512; i++ )
    {
        tmp8 =  *((volatile uint8_t *)( 512*127 + i ));
        if( tmp8 != 0xFF )
        {
            while(1);  //对擦除后的结果进行验证失败
        }
    }
    FLASH_UnlockPages( 512*127, 512*127 );    //解锁最后一个页面
    Flag = FLASH_WriteHalfWords(512*127, (uint16_t *)AddrBuf, 128);    //127页按半字写入
    if( Flag )
    {
        while(1);  //写入失败
    }
    //read
    for( i=0; i<=255; i++ )
    {
        tmp8 =  *((volatile uint8_t *)( 512*127 + i ));
        if( tmp8 != i )
        {
            while(1);  //对127页写入的数据进行验证失败
        }
    }
    for(i = 6; i; i--)
    {

        PB08_TOG();
        PB09_TOG();
        FirmwareDelay(50000);    //快闪表示通过
    }
    FirmwareDelay(2000000);    //暂停
    
    // 字写入
    FLASH_UnlockPages( 512*127, 512*127 );    //解锁127页面
    Flag = FLASH_ErasePages(512*127, 512*127);    //擦除
    FLASH_LockAllPages();
    if( Flag )
    {
        while(1);  //擦除失败
    }
    for( i=0; i<512; i++ )
    {
        tmp8 =  *((volatile uint8_t *)( 512*127 + i ));
        if( tmp8 != 0xFF )
        {
            while(1);  //对擦除后的结果进行验证失败
        }
    }
    FLASH_UnlockPages( 512*127, 512*127 );    //解锁最后一个页面
    Flag = FLASH_WriteWords(512*127, (uint32_t *)AddrBuf, 64);    //127页按字写入
    if( Flag )
    {
        while(1);  //写入失败
    }
    //read
    for( i=0; i<=255; i++ )
    {
        tmp8 =  *((volatile uint8_t *)( 512*127 + i ));
        if( tmp8 != i )
        {
            while(1);  //对127页写入的数据进行验证失败
        }
    }
    for(i = 6; i; i--)
    {

        PB08_TOG();
        PB09_TOG();
        FirmwareDelay(50000);    //快闪表示通过
    }
    FLASH_LockAllPages();
    
    //结束
    while(1)
    {
        PB08_TOG();
        PB09_TOG();
        FirmwareDelay(1000000);
    }

}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    CW_SYSCTRL->AHBEN_f.FLASH = 1;
    CW_SYSCTRL->AHBEN_f.GPIOC = 1;
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif
