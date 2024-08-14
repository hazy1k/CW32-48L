/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "base_types.h"
#include "cw32l031.h"
#include "system_cw32l031.h"
#include "interrupts_cw32l031.h"


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
//#define SGL_ONE_MODE_EN     /*!< 单输入通道单次采样模式 */
#define SGL_MORE_MODE_EN          /*!< 单输入通道多次采样模式 */
//#define SGL_CONTINUOUS_MODE_EN         /*!< 单输入通道连续采样模式 */
/* USER CODE END EM */


/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
void LED_Init(void);
/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
