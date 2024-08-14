/**
 * @file cw32l031_gpio.c
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

/* Includes ------------------------------------------------------------------*/
#include "cw32l031_gpio.h"

//=============================================================================
//GPIO 初始化，多个引脚
/**
 * @brief GPIO 初始化，可同时设置多个引脚或单个引脚
 *
 * @param GPIOx 参数为下列参数之一：
 *        @arg CW_GPIOA CW_GPIOB CW_GPIOC  CW_GPIOF
 * @param GPIO_Init 参见GPIO_InitTypeDef的定义
 */
void GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    switch ((uint32_t)GPIOx)
    {
        case (uint32_t)CW_GPIOA:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOA_Msk;
            break;
        case (uint32_t)CW_GPIOB:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOB_Msk;
            break;
        case (uint32_t)CW_GPIOC:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOC_Msk;
            break;
        case (uint32_t)CW_GPIOF:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOF_Msk;
            break;
        default:
            break;
    }
    if( GPIO_Init->Mode == GPIO_MODE_ANALOG )                        //模拟功能
    {
        REGBITS_SET( GPIOx->ANALOG, GPIO_Init->Pins );
    }
    else
    {
        REGBITS_CLR( GPIOx->ANALOG, GPIO_Init->Pins );                //数字功能
        if( (GPIO_Init->Mode & GPIO_MODE_INPUT) == GPIO_MODE_INPUT )  //输入模式
        {
            REGBITS_SET( GPIOx->DIR, GPIO_Init->Pins );
            if( GPIO_Init->Mode == GPIO_MODE_INPUT_PULLUP )
            {
                REGBITS_SET( GPIOx->PUR, GPIO_Init->Pins );
                REGBITS_CLR( GPIOx->PDR, GPIO_Init->Pins );
            }
            if( GPIO_Init->Mode == GPIO_MODE_INPUT_PULLDOWN )
            {
                REGBITS_SET( GPIOx->PDR, GPIO_Init->Pins );
                REGBITS_CLR( GPIOx->PUR, GPIO_Init->Pins );
            }

            REGBITS_CLR( GPIOx->RISEIE, GPIO_Init->Pins );
            REGBITS_CLR( GPIOx->FALLIE, GPIO_Init->Pins );
            
            if( ( GPIO_Init->IT & GPIO_IT_RISING ) == GPIO_IT_RISING )
            {
                REGBITS_SET( GPIOx->RISEIE, GPIO_Init->Pins );
            }
            if( ( GPIO_Init->IT & GPIO_IT_FALLING ) == GPIO_IT_FALLING )
            {
                REGBITS_SET( GPIOx->FALLIE, GPIO_Init->Pins );
            }
            
        }
        else                                                           //输出模式
        {
            REGBITS_CLR( GPIOx->DIR, GPIO_Init->Pins );
            if( GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP )
            {
                REGBITS_CLR( GPIOx->OPENDRAIN, GPIO_Init->Pins );
            }
            else
            {
                REGBITS_SET( GPIOx->OPENDRAIN, GPIO_Init->Pins );
            }
        }
    }
}

//=============================================================================
//GPIO 反初始化， 多个引脚
/**
 * @brief GPIO 去初始化，可同时设置多个引脚或单个引脚
 *
 * @param GPIOx 参数为下列参数之一：
 *        @arg CW_GPIOA CW_GPIOB CW_GPIOC  CW_GPIOF
 *
 * @param GPIO_Pins 参数取值如下
 *        @arg GPIO_PIN_0/1/2.../15/ALL 可通过与运算同时操作多个引脚
 */
void GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pins)
{
    REGBITS_SET( GPIOx->ANALOG, GPIO_Pins );
    REGBITS_SET( GPIOx->DIR, GPIO_Pins );
    REGBITS_CLR( GPIOx->PUR, GPIO_Pins );
    REGBITS_CLR( GPIOx->PDR, GPIO_Pins );
    REGBITS_CLR( GPIOx->RISEIE, GPIO_Pins );
    REGBITS_CLR( GPIOx->FALLIE, GPIO_Pins );   
    REGBITS_CLR( GPIOx->OPENDRAIN, GPIO_Pins );
}
//=============================================================================
//获取指定引脚电平，一个引脚
//注意：建议采用宏函数来获取引脚电平，例PA00_GETVALUE()
/**
 * @brief 获取一个引脚的电平
 *
 * @param GPIOx 参数为下列参数之一：
 *        @arg CW_GPIOA CW_GPIOB CW_GPIOC  CW_GPIOF
 * @param GPIO_Pin 参数取值如下
 *        @arg GPIO_PIN_0/1/2.../15
 * @return GPIO_PinState 取值如下:
 *        @arg GPIO_Pin_SET
 *        @arg GPIO_Pin_RESET
 */
GPIO_PinState GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if( GPIOx->IDR & GPIO_Pin )
    {
        return( GPIO_Pin_SET );
    }

    return( GPIO_Pin_RESET );
}

//=============================================================================
//设置指定引脚电平，多个引脚
//注意：如只操作一只引脚，建议采用宏函数，例PA00_SETHIGH(), PA00_SETLOW()
/**
 * @brief 设置指定引脚电平，可同时设置多个引脚
 *
 * @param GPIOx GPIOx 参数为下列参数之一：
 *        @arg CW_GPIOA CW_GPIOB CW_GPIOC  CW_GPIOF
 * @param GPIO_Pins 参数取值如下
 *        @arg GPIO_PIN_0/1/2.../15/ALL;
 * @param PinState 取值如下:
 *        @arg GPIO_Pin_SET
 *        @arg GPIO_Pin_RESET
 */
void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins, GPIO_PinState PinState)
{
    if( PinState == GPIO_Pin_SET )
    {
        GPIOx->BSRR = GPIO_Pins;
    }
    else
    {
        GPIOx->BRR = GPIO_Pins;
    }
}

/**
 * @brief 设置指定引脚端口的输出值
 * 
 * @param GPIOx GPIOx 参数为下列参数之一：
 *        @arg CW_GPIOA CW_GPIOB CW_GPIOC  CW_GPIOF
 * @param Value 
 */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t Value)
{
    GPIOx->ODR = Value;

}


//=============================================================================
//翻转指定引脚电平，多个引脚
//注意：如只操作一只引脚，建议采用宏函数，例PA00_TOG()
/**
 * @brief 翻转指定引脚电平，可同时设置多个引脚
 *
 * @param GPIOx 参数为下列参数之一：
 *        @arg CW_GPIOA CW_GPIOB CW_GPIOC  CW_GPIOF
 * @param GPIO_Pins
 */
void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins)
{
    GPIOx->TOG = GPIO_Pins;
}


//=============================================================================
//将PA13/PA14设置成GPIO
/**
 * @brief 将PA13/PA14设置成GPIO
 *
 */
void GPIO_SWD2GPIO( void )
{
    CW_SYSCTRL->CR2 =  CW_SYSCTRL->CR2 | ( 0x5A5A0000 | bv1 );
}

//=============================================================================
//将PA13/PA14设置成SWD
/**
 * @brief 将PA13/PA14设置成SWD，PA13/SWDIO PA14/SWCLK
 *
 */
void GPIO_GPIO2SWD( void )
{
    CW_SYSCTRL->CR2 =  0x5A5A0000 | ( CW_SYSCTRL->CR2 & (~(bv1)) );
}

//=============================================================================
//配置端口滤波
// FltClk 只能是宏定义中定义的参数
/**
 * @brief 配置端口滤波
 *
 * @param GPIOx 参数为下列参数之一：
 *        @arg CW_GPIOA CW_GPIOB CW_GPIOC CW_GPIOF
 * @param GPIO_Pins
 * @param FltClk 只能是宏定义中定义的参数 GPIO_FLTCLK_xxx
 */
void GPIO_ConfigFilter( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins, uint32_t FltClk )
{
    GPIOx->FILTER = FltClk | GPIO_Pins;
}

//=============================================================================
//配置端口辅助功能
//请使用CW32L031_gpio.h中的宏定义来配置每个端口的辅助功能

