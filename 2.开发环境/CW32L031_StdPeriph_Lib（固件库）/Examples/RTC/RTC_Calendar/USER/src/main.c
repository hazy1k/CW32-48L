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
#include "../inc/main.h"


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
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void delay(uint16_t ms);
void ShowTime(void);
void RTC_IRQHandlerCallBack(void);

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
 ** This sample toggle GPIOA.00
 **
 ******************************************************************************/

/**
 * @brief  RTC时钟测试，初始化日历，使用间隔中断0.5秒通过Log输出日期时间
 * @return int32_t
 */
int32_t main(void)
{
    RTC_InitTypeDef RTC_InitStruct = {0};
    RTC_AlarmTypeDef RTC_AlarmStruct = {0};

    /* System Clocks Configuration */
    RCC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    /* NVIC Configuration */
    NVIC_Configuration();

    LogInit();

    printf("RTC Init...\r\n");
    printf("    (RTC CR0:%04x,CR1:%04x,CR2:%04x,RESET FLAG:0x%08x)\r\n",CW_RTC->CR0,CW_RTC->CR1,CW_RTC->CR2,CW_SYSCTRL->RESETFLAG);

    //RCC_HSE_Enable(RCC_HSE_MODE_OSC, RCC_HSE_FREQ_GT24M,RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE);  //选择HSE为RTC时钟
    RCC_LSE_Enable(RCC_LSE_MODE_OSC, RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_NORMAL);  // 选择LSE为RTC时钟
    //RCC_LSI_Enable();                                                           // 选择LSI为RTC时钟

    RTC_InitStruct.DateStruct.Day = 0x21;             //设置日期，DAY、MONTH、YEAR必须为BCD方式，星期为0~6，代表星期日，星期一至星期六
    RTC_InitStruct.DateStruct.Month = RTC_Month_June;
    RTC_InitStruct.DateStruct.Week = RTC_Weekday_Monday;
    RTC_InitStruct.DateStruct.Year = 0x21;

    printf("-------Set Date as 20%x/%x/%x\r\n", RTC_InitStruct.DateStruct.Year,RTC_InitStruct.DateStruct.Month,RTC_InitStruct.DateStruct.Day);

    RTC_InitStruct.TimeStruct.Hour = 0x11;         //设置时间，HOUR、MINIUTE、SECOND必须为BCD方式，用户须保证HOUR、AMPM、H24之间的关联正确性
    RTC_InitStruct.TimeStruct.Minute = 0x58;
    RTC_InitStruct.TimeStruct.Second = 0x59;
    RTC_InitStruct.TimeStruct.AMPM = 0;
    RTC_InitStruct.TimeStruct.H24 = 0;
    printf("-------Set Time as %02x:%02x:%02x\r\n", RTC_InitStruct.TimeStruct.Hour,RTC_InitStruct.TimeStruct.Minute,RTC_InitStruct.TimeStruct.Second);
    RTC_InitStruct.RTC_ClockSource = RTC_RTCCLK_FROM_LSE;
    RTC_Init(&RTC_InitStruct);    //  用户需选定需要使用的时钟源

    printf("=====Set interval period as 0.5s...\r\n");
    RTC_SetInterval(RTC_INTERVAL_EVERY_0_5S);

    /* 设置闹钟为工作日上午的6：45 */
    printf("=====Set AlarmA at 6:45 on workday...\r\n");

    RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_WeekMON | RTC_AlarmMask_WeekTUE |
                                    RTC_AlarmMask_WeekWED | RTC_AlarmMask_WeekTHU |
                                    RTC_AlarmMask_WeekFRI;
    RTC_AlarmStruct.RTC_AlarmTime.Hour = 6;
    RTC_AlarmStruct.RTC_AlarmTime.Minute = 0x45;
    RTC_AlarmStruct.RTC_AlarmTime.Second = 0;
    RTC_SetAlarm(RTC_Alarm_A, &RTC_AlarmStruct);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    printf("=====Enable ALRAMA and INTERVAL IT...\r\n");

    RTC_ITConfig(RTC_IT_ALARMA | RTC_IT_INTERVAL, ENABLE);

    while(1)
    {

    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);
    RCC_LSI_Enable();
    RCC_LSE_Enable(RCC_LSE_MODE_OSC, RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_NORMAL);
    RCC_APBPeriphClk_Enable1(RCC_APB1_PERIPH_RTC, ENABLE);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{

}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(RTC_IRQn);
    __enable_irq();
}

void delay(uint16_t ms)
{
    uint32_t i;
    for( ; ms >0; ms--)
    {
        i = 796;
        while(i--);
    }
}

void ShowTime(void)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    static uint8_t *WeekdayStr[7]= {"SUN","MON","TUE","WED","THU","FRI","SAT"};
    static uint8_t *H12AMPMStr[2][2]= {{"AM","PM"},{"",""}};

    RTC_GetDate(&RTC_DateStruct);
    RTC_GetTime(&RTC_TimeStruct);

    printf(".Date is 20%02x/%02x/%02x(%s).Time is %02x%s:%02x:%02x\r\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Day, WeekdayStr[RTC_DateStruct.Week], RTC_TimeStruct.Hour, H12AMPMStr[RTC_TimeStruct.H24][RTC_TimeStruct.AMPM],RTC_TimeStruct.Minute, RTC_TimeStruct.Second);
}

void RTC_IRQHandlerCallBack(void)
{
    if (RTC_GetITState(RTC_IT_ALARMA))
    {
        RTC_ClearITPendingBit(RTC_IT_ALARMA);
        printf("*********Alarm!!!!\r\n");
    }
    if (RTC_GetITState(RTC_IT_INTERVAL))
    {
        RTC_ClearITPendingBit(RTC_IT_INTERVAL);
        ShowTime();

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
