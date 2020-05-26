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
#include "main.h"

#include <stdio.h>
#include <driver/include/fildes.h>

static TaskHandle_t defaultTaskHandle;
static void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);
__weak void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
}

/**
 * @brief  FreeRTOS initialization
 */
void MX_FREERTOS_Init(void)
{
  /* Create the thread(s) */
    extern struct fildes usart;
    store_fildes(&usart);
    usart.fops->open(&usart);

    extern void shell_task(void *);
    xTaskCreate(
        shell_task,
        "SHELL",
        256,
        NULL,
        tskIDLE_PRIORITY,
        NULL);
}
