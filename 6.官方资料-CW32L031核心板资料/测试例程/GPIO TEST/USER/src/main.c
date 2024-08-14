
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2021-03-12  1.0  xiebin First version for Device Driver Library of Module.
 **
 ******************************************************************************/
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
 ** This sample toggle GPIOA
 **
 ******************************************************************************/
int32_t main(void)
{
	unsigned long i,key=0;
	unsigned long x=60000;
	
    GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);
		for(i=0;i<60000;i++);

    CW_SYSCTRL->AHBEN_f.GPIOB = 1;    //Open GPIOA Clk
    CW_SYSCTRL->AHBEN_f.GPIOC = 1;    //Open GPIOC Clk

    GPIO_InitStruct.Pins = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init( CW_GPIOC, &GPIO_InitStruct);

    //
    GPIO_InitStruct.Pins = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.IT   = GPIO_IT_NONE;
    GPIO_Init( CW_GPIOB, &GPIO_InitStruct);


    while(1)
    {
			  for(i=0;i<x;i++);
				for(i=0;i<x;i++);
				PC13_TOG();	
        //-----------------------------------------------------------------------
        //以下调用函数法读写PIN
        if( GPIO_ReadPin(CW_GPIOB, GPIO_PIN_2 )==GPIO_Pin_RESET ) key=0;
			  else if(key==0)					
        {
					 for(i=0;i<3000;i++);
           if( GPIO_ReadPin(CW_GPIOB, GPIO_PIN_2 )==GPIO_Pin_SET )
					 {
						 key=1;
						 if(x==60000)x=240000;
					   else  x=60000;
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif

