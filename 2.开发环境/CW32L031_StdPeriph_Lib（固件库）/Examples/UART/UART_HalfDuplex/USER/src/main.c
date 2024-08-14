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
//UARTy
#define  USARTy                         CW_UART1
#define  USARTy_CLK                     RCC_APB2_PERIPH_UART1
#define  USARTy_APBClkENx               RCC_APBPeriphClk_Enable2

#define  USARTy_GPIO_CLK                RCC_AHB_PERIPH_GPIOA
#define  USARTy_GPIO                    CW_GPIOA
#define  USARTy_TxPin                   GPIO_PIN_8
#define  USARTy_AFTX                    PA08_AFx_UART1TXD()
#define  USARTy_TXPUR                   PA08_PUR_ENABLE();

//UARTz
#define  USARTz                         CW_UART2
#define  USARTz_CLK                     RCC_APB1_PERIPH_UART2
#define  USARTz_APBClkENx               RCC_APBPeriphClk_Enable1

#define  USARTz_GPIO_CLK                RCC_AHB_PERIPH_GPIOA
#define  USARTz_GPIO                    CW_GPIOA
#define  USARTz_TxPin                   GPIO_PIN_6
#define  USARTz_AFTX                    PA06_AFx_UART2TXD()
#define  USARTz_TXPUR                   PA06_PUR_ENABLE()

#define  USARTyz_BaudRate               9600
#define  USARTyz_UclkFreq               8000000

#define  TxBufferSize1                  (ARRAY_SZ(TxBuffer1) - 1)
#define  TxBufferSize2                  (ARRAY_SZ(TxBuffer2) - 1)

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer1[] = "\r\nCW32L031 UART HalfDuplex: USARTy -> USARTz\r\n";
uint8_t TxBuffer2[] = "\r\nCW32L031 UART HalfDuplex: USARTz -> USARTy\r\n";
uint8_t RxBuffer1[TxBufferSize2];
uint8_t RxBuffer2[TxBufferSize1];
uint32_t NbrOfDataToRead1 = TxBufferSize2;
uint32_t NbrOfDataToRead2 = TxBufferSize1;
uint8_t TxCounter1 = 0, RxCounter1 = 0;
uint8_t TxCounter2 = 0, RxCounter2 = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

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

    //单线半双工 USARTy
    USART_HalfDuplexCmd(USARTy, ENABLE);

    //单线半双工 USARTz
    USART_HalfDuplexCmd(USARTz, ENABLE);

    //USARTy -> USARTz
    while(NbrOfDataToRead2--)
    {
        //USARTy发送一个字节数据
        USART_SendData_8bit(USARTy, TxBuffer1[TxCounter1++]);
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET);

        //USARTz 等待RC
        while(USART_GetFlagStatus(USARTz, USART_FLAG_RC) == RESET);
        USART_ClearFlag(USARTz, USART_FLAG_RC);
        RxBuffer2[RxCounter2++] = USART_ReceiveData_8bit(USARTz);
    }

    //USARTz -> USARTy
    while(NbrOfDataToRead1--)
    {
        //USARTz发送一个字节数据
        USART_SendData_8bit(USARTz, TxBuffer2[TxCounter2++]);
        while(USART_GetFlagStatus(USARTz, USART_FLAG_TXE)== RESET);

        //USARTy 等待RC
        while(USART_GetFlagStatus(USARTy,USART_FLAG_RC) == RESET);
        USART_ClearFlag(USARTy, USART_FLAG_RC);
        RxBuffer1[RxCounter1++] = USART_ReceiveData_8bit(USARTy);
    }

    //检查收发数据一致性
    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);

    if(TransferStatus1 == PASSED && TransferStatus2 == PASSED) //PASSED
    {
        //LED1亮
        PB09_SETHIGH();
    }
    else //FAILED
    {
        //LED2亮
        PB08_SETHIGH();
    }

    while(1)
    {

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
    RCC_AHBPeriphClk_Enable(USARTy_GPIO_CLK | USARTz_GPIO_CLK | RCC_AHB_PERIPH_GPIOC, ENABLE);
    USARTy_APBClkENx(USARTy_CLK, ENABLE);
    USARTz_APBClkENx(USARTz_CLK, ENABLE);
}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //UART TX 复用
    USARTy_AFTX;
    USARTz_AFTX;

    //UART TX PUR
    USARTy_TXPUR;
    USARTz_TXPUR;

    GPIO_InitStructure.Pins = USARTy_TxPin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD; //开漏输出
    GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = USARTz_TxPin;
    GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);

    //PB9 LED1 / PB8 LED2
    GPIO_InitStructure.Pins = GPIO_PIN_9 | GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    //LED灭
    PB09_SETLOW();
    PB08_SETLOW();
}

/**
 * @brief 配置UART
 *
 */
void UART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure = {0};

    USART_InitStructure.USART_BaudRate = USARTyz_BaudRate;
    USART_InitStructure.USART_Over = USART_Over_16;
    USART_InitStructure.USART_Source = USART_Source_PCLK;
    USART_InitStructure.USART_UclkFreq = USARTyz_UclkFreq;
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTy, &USART_InitStructure);
    USART_Init(USARTz, &USART_InitStructure);
}

/**
 * @brief Compares two buffers
 *
 * @param pBuffer1 :buffers to be compared
 * @param pBuffer2 :buffers to be compared
 * @param BufferLength :buffer's length
 * @return TestStatus
 *     @arg PASSED: pBuffer1 identical to pBuffer2
 *     @arg FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
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

