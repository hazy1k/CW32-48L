
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "log.h"
#include "system_cw32l031.h"
#include "cw32l031_gpio.h"
#include "cw32l031_rcc.h"
#include "cw32l031_uart.h"


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
static void SerialInit(uint32_t BaudRate);
static void SerialSend(uint8_t Data);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint8_t const pow2_table[] = {0, 1, 2, 3, 4, 5, 6, 7};

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

void LogInit(void)
{
    SerialInit(LOG_SERIAL_BPS);
}

static void SerialInit(uint32_t BaudRate)
{
    uint32_t PCLK_Freq;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    PCLK_Freq = SystemCoreClock >> pow2_table[CW_SYSCTRL->CR0_f.HCLKPRS];
    PCLK_Freq >>= pow2_table[CW_SYSCTRL->CR0_f.PCLKPRS];

    // 调试串口使用UART1
    //  PA8->TX
    //  PA9<-RX
    // 时钟使能
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_UART1_CLK_ENABLE();

    // 先设置UART TX RX 复用，后设置GPIO的属性，避免口线上出现毛刺
    PA08_AFx_UART1TXD();
    PA09_AFx_UART1RXD();

    GPIO_InitStructure.Pins = GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_Over = USART_Over_16;
    USART_InitStructure.USART_Source = USART_Source_PCLK;
    USART_InitStructure.USART_UclkFreq = PCLK_Freq;
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


    USART_Init(CW_UART1, &USART_InitStructure);

}

static void SerialSend(uint8_t Data)
{

    USART_SendData_8bit(CW_UART1, Data);
    while (USART_GetFlagStatus(CW_UART1, USART_FLAG_TXE) == RESET);

}

int fputc(int ch, FILE *f)
{
    SerialSend(ch);
    return ch;
}

size_t __write(int handle, const unsigned char * buffer, size_t size)
{   
    size_t nChars = 0;

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return 0;
    }


    for (/* Empty */; size != 0; --size)
    {
        SerialSend(*buffer++);
        ++nChars;
    }

    return nChars;
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
