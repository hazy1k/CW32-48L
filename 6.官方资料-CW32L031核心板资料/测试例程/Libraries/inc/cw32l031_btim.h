/**
 * @file cw32l031_btim.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CW32L031_BTIM_H
#define __CW32L031_BTIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cw32l031.h"

typedef struct
{
    uint16_t BTIM_Prescaler;         /* 预分配系数，取值范围2的n次幂，n=0,1,2…15 */

    uint16_t BTIM_Mode;              /* 工作模式，00：定时器模式
                                         01：计数器模式
                                         10：触发计数模式
                                         11：门控计数模式  */

    uint16_t BTIM_Period;            /* 计数重载周期，取值范围0x0000 到 0xFFFF.  */

    uint16_t BTIM_OPMode;            /* 单次和连续模式控制 */

} BTIM_TimeBaseInitTypeDef;


#define IS_BTIM_ALL_PERIPH(PERIPH)                  (((PERIPH) == CW_BTIM1) || \
                                                    ((PERIPH) == CW_BTIM2) || \
                                                    ((PERIPH) == CW_BTIM3))


#define BTIM_OPMode_Single                          ((uint16_t)0x0020)
#define BTIM_OPMode_Repetitive                      ((uint16_t)0x0000)
#define IS_BTIM_OPM_MODE(MODE)                      (((MODE) == BTIM_OPMode_Single) || \
                                                    ((MODE) == BTIM_OPMode_Repetitive))

#define IS_BTIM_PRS_DIV(DIV)                        ((DIV) <= 0xFFFF)


/*  BTIM_Mode  */
#define BTIM_Mode_TIMER                             ((uint16_t)0x0000)
#define BTIM_Mode_COUNTER                           ((uint16_t)0x0002)
#define BTIM_Mode_TRIGGER                           ((uint16_t)0x00004)
#define BTIM_Mode_GATE                              ((uint16_t)0x0006)
#define IS_BTIM_MODE(MODE)                          (((MODE) == BTIM_Mode_TIMER) ||  \
                                                    ((MODE) == BTIM_Mode_COUNTER) || \
                                                    ((MODE) == BTIM_Mode_TRIGGER) || \
                                                    ((MODE) == BTIM_Mode_GATE))
/*  BTIM_interrupt_sources  */
#define BTIM_IT_OV                                  ((uint16_t)0x0001)
#define BTIM_IT_TI                                  ((uint16_t)0x0002)
#define BTIM_IT_TOP                                 ((uint16_t)0x0004)
#define IS_BTIM_IT(IT)                              ((((IT) & (uint16_t)0xFFF8) == 0x0000) && ((IT) != 0x0000))

#define IS_BTIM_GET_IT(IT)                          (((IT) == BTIM_IT_OV) || \
                                                    ((IT) == BTIM_IT_TI) || \
                                                    ((IT) == BTIM_IT_TOP))

/*  BTIM_DMA_sources  */
#define BTIM_DMA_OV                                 ((uint16_t)0x0001)
#define BTIM_DMA_TRS                                ((uint16_t)0x0002)
#define IS_BTIM_DMA_SOURCE(SOURCE)                  (((SOURCE) == BTIM_DMA_OV) ||\
                                                    ((SOURCE) == BTIM_DMA_TRS))

/*  BTIM_Internal_Trigger_Selection  */
#define BTIM_TS_ITR                                 ((uint16_t)0x0008)
#define BTIM_TS_ETR                                 ((uint16_t)0x0000)
#define IS_BTIM_TRIGGER_SELECTION(SELECTION)        (((SELECTION) == BTIM_TS_ITR) || \
                                                    ((SELECTION) == BTIM_TS_ETR))

/*  BTIM_ETR_Polarity  */
#define BTIM_ETR_Polarity_Positive                  ((uint16_t)0x0000)
#define BTIM_ETR_Polarity_Negative                  ((uint16_t)0x0010)
#define IS_BTIM_ETR_POLARITY(MODE)                  (((MODE) == BTIM_ETR_Polarity_Positive) || \
                                                    ((MODE) == BTIM_ETR_Polarity_Negative))

/*  BTIM_Prescaler_Reload_Mode  */
#define BTIM_PSCReloadMode_Update                   ((uint16_t)0x0000)
#define BTIM_PSCReloadMode_Immediate                ((uint16_t)0x0001)
#define IS_BTIM_PRESCALER_RELOAD(RELOAD)            (((RELOAD) == BTIM_PSCReloadMode_Update) || \
                                                    ((RELOAD) == BTIM_PSCReloadMode_Immediate))

/*  BTIM_External_Trigger_Filter  */
#define BTIM_EXT_FILTER_NONE                        0x0000
#define BTIM_EXT_FILTER_PCLK_N2                     0x0010
#define BTIM_EXT_FILTER_PCLK_N4                     0x0020
#define BTIM_EXT_FILTER_PCLK_N6                     0x0030
#define BTIM_EXT_FILTER_PCLKDIV4_N4                 0x0040
#define BTIM_EXT_FILTER_PCLKDIV4_N6                 0x0050
#define BTIM_EXT_FILTER_PCLKDIV16_N4                0x0060
#define BTIM_EXT_FILTER_PCLKDIV16_N6                0x0070
#define IS_BTIM_EXT_FILTER(EXTFILTER)               (((EXTFILTER) == BTIM_EXT_FILTER_NONE) || \
                                                    ((EXTFILTER) == BTIM_EXT_FILTER_PCLK_N2) || \
                                                    ((EXTFILTER) == BTIM_EXT_FILTER_PCLK_N4) || \
                                                    ((EXTFILTER) == BTIM_EXT_FILTER_PCLK_N6) || \
                                                    ((EXTFILTER) == BTIM_EXT_FILTER_PCLKDIV4_N4) || \
                                                    ((EXTFILTER) == BTIM_EXT_FILTER_PCLKDIV4_N6) || \
                                                    ((EXTFILTER) == BTIM_EXT_FILTER_PCLKDIV16_N4) || \
                                                    ((EXTFILTER) == BTIM_EXT_FILTER_PCLKDIV16_N6))

/*   BTIM_Exported_Functions  */
void BTIM_DeInit(BTIM_TypeDef* BTIMx);
void BTIM_TimeBaseInit(BTIM_TypeDef* BTIMx, BTIM_TimeBaseInitTypeDef* BTIM_TimeBaseInitStruct);
void BTIM_TimeBaseStructInit(BTIM_TimeBaseInitTypeDef* BTIM_TimeBaseInitStruct);
void BTIM_Cmd(BTIM_TypeDef* BTIMx, FunctionalState NewState);
void BTIM_ITConfig(BTIM_TypeDef* BTIMx, uint16_t BTIM_IT, FunctionalState NewState);
void BTIM_DMACmd(BTIM_TypeDef* BTIMx, uint16_t BTIM_DMASource, FunctionalState NewState);
void BTIM_PrescalerConfig(BTIM_TypeDef* BTIMx, uint16_t Prescaler, uint16_t BTIM_PSCReloadMode);
void BTIM_ModeConfig(BTIM_TypeDef* BTIMx, uint16_t BTIM_Mode);
void BTIM_SelectInputTrigger(BTIM_TypeDef* BTIMx, uint16_t BTIM_InputTriggerSource);
void BTIM_SelectOnePulseMode(BTIM_TypeDef* BTIMx, uint16_t BTIM_OPMode);
void BTIM_OutputToggleCmd(BTIM_TypeDef* BTIMx, FunctionalState NewState);
void BTIM_ETRConfig(BTIM_TypeDef* BTIMx, uint16_t BTIM_ExtTRGPolarity, uint16_t ExtTRGFilter);
void BTIM_SetCounter(BTIM_TypeDef* BTIMx, uint16_t Counter);
void BTIM_SetAutoreload(BTIM_TypeDef* BTIMx, uint16_t Autoreload);
uint16_t BTIM_GetCounter(BTIM_TypeDef* BTIMx);
uint16_t BTIM_GetAutoreload(BTIM_TypeDef* BTIMx);
uint16_t BTIM_GetPrescaler(BTIM_TypeDef* BTIMx);
FlagStatus BTIM_GetFlagStatus(BTIM_TypeDef* BTIMx, uint16_t BTIM_FLAG);
void BTIM_ClearFlag(BTIM_TypeDef* BTIMx, uint16_t BTIM_FLAG);
ITStatus BTIM_GetITStatus(BTIM_TypeDef* BTIMx, uint16_t BTIM_IT);
void BTIM_ClearITPendingBit(BTIM_TypeDef* BTIMx, uint16_t BTIM_IT);

#ifdef __cplusplus
}
#endif

#endif 

