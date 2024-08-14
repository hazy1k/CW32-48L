/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    interrupts_cw32l031.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
 ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INTERRUPTS_CW32L031_H
#define __INTERRUPTS_CW32L031_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */


/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */


/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */


/* Exported functions prototypes ---------------------------------------------*/

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void WDT_IRQHandler(void);
extern void LVD_IRQHandler(void);
extern void RTC_IRQHandler(void);
extern void FLASHRAM_IRQHandler(void);
extern void RCC_IRQHandler(void);
extern void GPIOA_IRQHandler(void);
extern void GPIOB_IRQHandler(void);
extern void GPIOC_IRQHandler(void);
extern void GPIOF_IRQHandler(void);
extern void DMACH1_IRQHandler(void);
extern void DMACH23_IRQHandler(void);
extern void DMACH4_IRQHandler(void);
extern void ADC_IRQHandler(void);
extern void ATIM_IRQHandler(void);
extern void VC1_IRQHandler(void);
extern void VC2_IRQHandler(void);
extern void GTIM1_IRQHandler(void);
extern void GTIM2_IRQHandler(void);
extern void BTIM1_IRQHandler(void);
extern void BTIM2_IRQHandler(void);
extern void BTIM3_IRQHandler(void);
extern void I2C1_IRQHandler(void);
extern void SPI1_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void UART2_IRQHandler(void);
extern void UART3_IRQHandler(void);
extern void AWT_IRQHandler(void);
extern void FAULT_IRQHandler(void);

/* USER CODE BEGIN EFP */
extern void GPIOA_IRQHandlerCallback(void);
extern void WDT_IRQHandlerCallback(void);
extern void SysTick_HandlerCallback(void);
/* USER CODE END EFP */


#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPTS_CW32L031_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
