/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-7-12
 *
 * @copyright Copyright (c) 2022
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
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//HCLK for SysTick
#define  HCLKFREQ                       8000000
//UARTx
#define  DEBUG_USARTx                   CW_UART1
#define  DEBUG_USART_CLK                RCC_APB2_PERIPH_UART1
#define  DEBUG_USART_APBClkENx          RCC_APBPeriphClk_Enable2
#define  DEBUG_USART_BaudRate           9600
#define  DEBUG_USART_UclkFreq           8000000

//UARTx GPIO
#define  DEBUG_USART_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_PIN_8
#define  DEBUG_USART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_PIN_9

//GPIO AF
#define  DEBUG_USART_AFTX               PA08_AFx_UART1TXD()
#define  DEBUG_USART_AFRX               PA09_AFx_UART1RXD()

//DMA
#define  DEBUG_USART_TX_DMA             CW_DMACHANNEL2
#define  DEBUG_USART_RX_DMA             CW_DMACHANNEL1
#define  DEBUG_USART_TxTrigSource       DMA_HardTrig_UART1_TXBufferE
#define  DEBUG_USART_RxTrigSource       DMA_HardTrig_UART1_RXBufferNE
#define  DEBUG_DMA_BUFFSIZE             5000
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
void UART_Configuration(void);
void DMA_Configuration(void);
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxRxBuffer[DEBUG_DMA_BUFFSIZE];
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
 ******************************************************************************/
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();

    //配置GPIO
    GPIO_Configuration();

    //配置UART
    UART_Configuration();

    //配置DMA
    DMA_Configuration();

    //初始化SysTick
    InitTick(HCLKFREQ);

    USART_SendString(DEBUG_USARTx, "\r\nCW32L031 UART DMA Transmission\r\n" );

    //填充TxRxBuffer
    for(int i=0; i<DEBUG_DMA_BUFFSIZE; i++)
    {
        TxRxBuffer[i] = i;
    }

    //使能UART DMA TXD
    USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);

    while(1)
    {
        //CPU空闲，LED1闪烁
        PB09_TOG();
        SysTickDelay(500);
    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSI = 8MHz = HCLK = PCLK
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);

    //外设时钟使能
    RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK | RCC_AHB_PERIPH_DMA | RCC_AHB_PERIPH_GPIOC, ENABLE);
    DEBUG_USART_APBClkENx(DEBUG_USART_CLK, ENABLE);
}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //UART TX RX 复用
    DEBUG_USART_AFTX;
    DEBUG_USART_AFRX;

    GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    //PB9 LED1
    GPIO_InitStructure.Pins = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
}

/**
 * @brief 配置UART
 *
 */
void UART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure = {0};

    USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;
    USART_InitStructure.USART_Over = USART_Over_16;
    USART_InitStructure.USART_Source = USART_Source_PCLK;
    USART_InitStructure.USART_UclkFreq = DEBUG_USART_UclkFreq;
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEBUG_USARTx, &USART_InitStructure);
}

/**
 * @brief 配置DMA
 *
 */
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    //初始化DMA TX
    DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
    DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
    DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Increase;
    DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Fix;
    DMA_InitStructure.TrigMode = DMA_HardTrig;
    DMA_InitStructure.HardTrigSource = DEBUG_USART_TxTrigSource;
    DMA_InitStructure.DMA_TransferCnt = DEBUG_DMA_BUFFSIZE;
    DMA_InitStructure.DMA_SrcAddress = (uint32_t)TxRxBuffer;
    DMA_InitStructure.DMA_DstAddress = (uint32_t)&DEBUG_USARTx->TDR;
    DMA_Init(DEBUG_USART_TX_DMA, &DMA_InitStructure);
    DMA_Cmd(DEBUG_USART_TX_DMA, ENABLE);

    // //初始化DMA RX
    // DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
    // DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
    // DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Fix;
    // DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Increase;
    // DMA_InitStructure.TrigMode = DMA_HardTrig;
    // DMA_InitStructure.HardTrigSource = DEBUG_USART_RxTrigSource;
    // DMA_InitStructure.DMA_TransferCnt = DEBUG_DMA_BUFFSIZE;
    // DMA_InitStructure.DMA_SrcAddress = (uint32_t)&DEBUG_USARTx->RDR;
    // DMA_InitStructure.DMA_DstAddress = (uint32_t)TxRxBuffer;
    // DMA_Init(DEBUG_USART_RX_DMA, &DMA_InitStructure);
    // DMA_Cmd(DEBUG_USART_RX_DMA, ENABLE);
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

