/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "app_sd.h"
#include "bsp_can.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osMessageQId rxRegsHandle;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId tskBlinkHandle;
osThreadId app_SDHandle;
osThreadId lcdHandle;
osThreadId loadcellHandle;
osThreadId app_receiveRegHandle;
osThreadId regTransmitHandle;
osMutexId loadcellMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
extern void app_blink(void const * argument);
extern void tsk_SD(void const * argument);
extern void tsk_lcd(void const * argument);
extern void tsk_loadcell(void const * argument);
extern void tsk_receiveReg(void const * argument);
extern void tsk_transmitReg(void const * argument);

extern void MX_FATFS_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of loadcellMutex */
  osMutexDef(loadcellMutex);
  loadcellMutexHandle = osMutexCreate(osMutex(loadcellMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  osMessageQDef(rxRegs, 16, can_reg_t*);
  rxRegsHandle = osMessageCreate(osMessageQ(rxRegs), NULL);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityLow, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of tskBlink */
  osThreadDef(tskBlink, app_blink, osPriorityLow, 0, 128);
  tskBlinkHandle = osThreadCreate(osThread(tskBlink), NULL);

  /* definition and creation of app_SD */
  osThreadDef(app_SD, tsk_SD, osPriorityNormal, 0, 1000);
  app_SDHandle = osThreadCreate(osThread(app_SD), NULL);

  /* definition and creation of lcd */
  osThreadDef(lcd, tsk_lcd, osPriorityNormal, 0, 500);
  lcdHandle = osThreadCreate(osThread(lcd), NULL);

  /* definition and creation of loadcell */
  osThreadDef(loadcell, tsk_loadcell, osPriorityNormal, 0, 128);
  loadcellHandle = osThreadCreate(osThread(loadcell), NULL);

  /* definition and creation of app_receiveReg */
  osThreadDef(app_receiveReg, tsk_receiveReg, osPriorityNormal, 0, 128);
  app_receiveRegHandle = osThreadCreate(osThread(app_receiveReg), NULL);

  /* definition and creation of regTransmit */
  osThreadDef(regTransmit, tsk_transmitReg, osPriorityNormal, 0, 128);
  regTransmitHandle = osThreadCreate(osThread(regTransmit), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
