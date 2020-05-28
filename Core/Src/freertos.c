/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include <driver/include/fildes.h>

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void)
{
    HAL_IncTick();
}

/**
 * @brief  FreeRTOS initialization
 */
void MX_FREERTOS_Init(void)
{
  /* Create the thread(s) */
    extern void shell_task(void *);
    xTaskCreate(
        shell_task,
        "SHELL",
        256,
        NULL,
        tskIDLE_PRIORITY,
        NULL);
}
