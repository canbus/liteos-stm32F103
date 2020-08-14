/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Uart1TxPin_Pin GPIO_PIN_9
#define Uart1TxPin_GPIO_Port GPIOA
#define Uart1RxPin_Pin GPIO_PIN_10
#define Uart1RxPin_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_7
#define LED1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LED1_ON()  HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF() HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_SET)

#define LED2_ON()  HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_RESET)
#define LED2_OFF() HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_SET)

#define LED3_ON()  HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_RESET)
#define LED3_OFF() HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_SET)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
