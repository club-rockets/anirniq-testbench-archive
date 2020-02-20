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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "enums.h"
#include "stm32f407xx.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define CAN_BOARD BENCHTEST

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define loadcell_data2_Pin GPIO_PIN_2
#define loadcell_data2_GPIO_Port GPIOE
#define loadcell_data_Pin GPIO_PIN_4
#define loadcell_data_GPIO_Port GPIOE
#define loadcell_clk_Pin GPIO_PIN_6
#define loadcell_clk_GPIO_Port GPIOE
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define PRESSURE1_Pin GPIO_PIN_5
#define PRESSURE1_GPIO_Port GPIOA
#define PRESSURE2_Pin GPIO_PIN_6
#define PRESSURE2_GPIO_Port GPIOA
#define PRESSURE3_Pin GPIO_PIN_7
#define PRESSURE3_GPIO_Port GPIOA
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_12
#define SD_CS_GPIO_Port GPIOB
#define SD_CLK_Pin GPIO_PIN_13
#define SD_CLK_GPIO_Port GPIOB
#define SD_MISO_Pin GPIO_PIN_14
#define SD_MISO_GPIO_Port GPIOB
#define SD_MOSI_Pin GPIO_PIN_15
#define SD_MOSI_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_8
#define LCD_D4_GPIO_Port GPIOD
#define LCD_D5_Pin GPIO_PIN_9
#define LCD_D5_GPIO_Port GPIOD
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D6_GPIO_Port GPIOD
#define LCD_D7_Pin GPIO_PIN_11
#define LCD_D7_GPIO_Port GPIOD
#define LD4_Pin GPIO_PIN_15
#define LD4_GPIO_Port GPIOD
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define LCD_E_Pin GPIO_PIN_3
#define LCD_E_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_6
#define LCD_RS_GPIO_Port GPIOD
#define LCD_RW_Pin GPIO_PIN_7
#define LCD_RW_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define loadcell_data3_Pin GPIO_PIN_0
#define loadcell_data3_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
