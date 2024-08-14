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
#include "main.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//UARTx
#define  DEBUG_USARTx                   CW_UART1
#define  DEBUG_USART_CLK                RCC_APB2_PERIPH_UART1
#define  DEBUG_USART_APBClkENx          RCC_APBPeriphClk_Enable2
#define  DEBUG_USART_BaudRate           9600
#define  DEBUG_USART_UclkFreq           8000000

//UARTx_GPIO
#define  DEBUG_USART_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_PIN_8
#define  DEBUG_USART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_PIN_9

//UART_GPIO_AF
#define  DEBUG_USART_AFTX               PA08_AFx_UART1TXD()
#define  DEBUG_USART_AFRX               PA09_AFx_UART1RXD()

//UARTx中断
#define  DEBUG_USART_IRQ                UART1_IRQn

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
void USART_SendString(UART_TypeDef* USARTx, char *String);

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
 ******************************************************************************/
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();

    //配置UART
    UART_Configuration();

    USART_SendString(DEBUG_USARTx, "\r\nCW32L031 UART AutoBaudRate\r\n");
    USART_SendString(DEBUG_USARTx, "\r\nPC发送以0xF8作为数据头的数据流\r\n");

    while(1)
    {
        //中断收发
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
}

/**
 * @brief 配置UART 自动波特率检测模式1
 *
 */
void UART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //时钟使能
    RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK, ENABLE);
    DEBUG_USART_APBClkENx(DEBUG_USART_CLK, ENABLE);

    //UART TX RX 复用
    DEBUG_USART_AFTX;
    DEBUG_USART_AFRX;

    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

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
    
    //优先级
    NVIC_SetPriority(DEBUG_USART_IRQ, 0);
    //UARTx中断使能
    NVIC_EnableIRQ(DEBUG_USART_IRQ);
    //使能UARTx TIMOV/BAUD中断
    USART_ITConfig(DEBUG_USARTx, USART_IT_TIMOV | USART_IT_BAUD, ENABLE);

    USART_SetAutoReload(DEBUG_USARTx, 0xFFFFFF); //TIMARR重载值
    USART_TimerModeConfig(DEBUG_USARTx, USART_TimerMode_AutoBaudRate1); //自动波特率检测模式1
}

/**
 * @brief UART1_IRQHandlerCallBack
 *        UART中断收发 0xF8
 */
void UART1_IRQHandlerCallBack(void)
{
    /* USER CODE BEGIN */
    static uint32_t cnt = 0;
    uint32_t temp = 0;
    uint8_t TxRxBuffer;
    if(USART_GetITStatus(DEBUG_USARTx, USART_IT_TIMOV)) //TIMARR溢出中断，记录溢出次数cnt
    {
        USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_TIMOV);
	    cnt++; 
    }
    if(USART_GetITStatus(DEBUG_USARTx, USART_IT_BAUD)) //自动波特率检测完成中断，修改波特率
    {
        temp = (16777216*cnt + USART_GetCounter(DEBUG_USARTx)) / 4;
        DEBUG_USARTx->BRRI = (uint16_t)(temp >> 4);
        DEBUG_USARTx->BRRF = (uint16_t)(temp & 0x0F);
        USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_BAUD);
        USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_RC);
        USART_ITConfig(DEBUG_USARTx, USART_IT_RC, ENABLE);//RC中断使能
        cnt = 0;
    }
    if(USART_GetITStatus(DEBUG_USARTx, USART_IT_RC) != RESET) //中断收发数据
    {
        USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_RC);
        TxRxBuffer = USART_ReceiveData_8bit(DEBUG_USARTx);
        USART_SendData_8bit(DEBUG_USARTx, TxRxBuffer);
    }
    /* USER CODE END */
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

