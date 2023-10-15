/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_dma.h"

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
#define Reset_Abort_I0_Pin LL_GPIO_PIN_0
#define Reset_Abort_I0_GPIO_Port GPIOF
#define Reset_Abort_I0_EXTI_IRQn EXTI0_IRQn
#define Feed_Hold_I1_Pin LL_GPIO_PIN_1
#define Feed_Hold_I1_GPIO_Port GPIOF
#define Feed_Hold_I1_EXTI_IRQn EXTI1_IRQn
#define Stepper_En_Dis_Pin LL_GPIO_PIN_0
#define Stepper_En_Dis_GPIO_Port GPIOA
#define DirectionX_Pin LL_GPIO_PIN_1
#define DirectionX_GPIO_Port GPIOA
#define USART2_TX_Pin LL_GPIO_PIN_2
#define USART2_TX_GPIO_Port GPIOA
#define USART2_RX_Pin LL_GPIO_PIN_3
#define USART2_RX_GPIO_Port GPIOA
#define DirectionY_Pin LL_GPIO_PIN_4
#define DirectionY_GPIO_Port GPIOA
#define DirectionZ_Pin LL_GPIO_PIN_5
#define DirectionZ_GPIO_Port GPIOA
#define StepX_Pin LL_GPIO_PIN_6
#define StepX_GPIO_Port GPIOA
#define StepY_Pin LL_GPIO_PIN_7
#define StepY_GPIO_Port GPIOA
#define Spindle_PWM_Pin LL_GPIO_PIN_0
#define Spindle_PWM_GPIO_Port GPIOB
#define StepZ_Pin LL_GPIO_PIN_8
#define StepZ_GPIO_Port GPIOA
#define LimitX_I10_Pin LL_GPIO_PIN_10
#define LimitX_I10_GPIO_Port GPIOA
#define LimitX_I10_EXTI_IRQn EXTI15_10_IRQn
#define LimitY_I11_Pin LL_GPIO_PIN_11
#define LimitY_I11_GPIO_Port GPIOA
#define LimitY_I11_EXTI_IRQn EXTI15_10_IRQn
#define LimitZ_I12_Pin LL_GPIO_PIN_12
#define LimitZ_I12_GPIO_Port GPIOA
#define LimitZ_I12_EXTI_IRQn EXTI15_10_IRQn
#define T_SWDIO_Pin LL_GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin LL_GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define Cycle_Start_Resume_I3_Pin LL_GPIO_PIN_3
#define Cycle_Start_Resume_I3_GPIO_Port GPIOB
#define Cycle_Start_Resume_I3_EXTI_IRQn EXTI3_IRQn
#define Probe_I4_Pin LL_GPIO_PIN_4
#define Probe_I4_GPIO_Port GPIOB
#define Probe_I4_EXTI_IRQn EXTI4_IRQn
#define LD2_Pin LL_GPIO_PIN_8
#define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define GPIO(X) X##_GPIO_Port, X##_Pin
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
