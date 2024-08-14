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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CW32L031CXTX_STARTKIT_H
#define __CW32L031CXTX_STARTKIT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "cw32l031.h"
#include "cw32l031_gpio.h"


/* type definitions ('typedef') ----------------------------------------------*/
typedef struct
{
    GPIO_TypeDef *LED_Port;
    uint32_t LED_Pin;
    void (*LED_On)(void *LED);
    void (*LED_Off)(void *LED);
    void (*LED_Toggle)(void *LED);
} LED_InitTypeDef;

typedef struct
{
    GPIO_TypeDef *Port;
    uint32_t Pin;      
    FlagStatus (*Read)(void *self);    
}KEY_ClassTypeDef;

/* pre-processor symbols/macros ('#define') ----------------------------------*/
#define KEY1_PORT    CW_GPIOA
#define KEY2_PORT    CW_GPIOA
#define KEY1_PIN     GPIO_PIN_2
#define KEY2_PIN     GPIO_PIN_1

#define LED1_PORT    CW_GPIOB
#define LED2_PORT    CW_GPIOB
#define LED1_PIN     GPIO_PIN_9
#define LED2_PIN     GPIO_PIN_8

/* Global variable definitions (declared in header file with 'extern') ------*/

extern KEY_ClassTypeDef KEY1, KEY2;
extern LED_InitTypeDef LED1 , LED2;

/* Global function prototypes -----------------------------------------------*/
void LED_Init(void);
void KEY_Init(void);


#ifdef __cplusplus
}
#endif

#endif
