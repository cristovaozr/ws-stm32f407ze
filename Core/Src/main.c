#include <FreeRTOS.h>
#include <task.h>

#include "main.h"
#include "i2c.h"
#include "i2s.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

static void SystemClock_Config(void);
extern void MX_FREERTOS_Init(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    SystemClock_Config();

    /* Initialize all configured peripherals */
    // MX_GPIO_Init();
    // MX_FSMC_Init();
    // MX_I2C1_Init();
    // MX_I2S2_Init();
    // MX_SDIO_SD_Init();
    MX_USART2_UART_Init();

    /* Init scheduler */
    MX_FREERTOS_Init(); 
    /* Start scheduler */
    vTaskStartScheduler();

    while (1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
    if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
       Error_Handler();  
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSE_Enable();
    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLLI2S_ConfigDomain_I2S(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLI2SM_DIV_4, 192, LL_RCC_PLLI2SR_DIV_4);
    LL_RCC_PLL_Enable();
    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1);
    LL_RCC_PLLI2S_Enable();

    /* Wait till PLL is ready */
    while(LL_RCC_PLLI2S_IsReady() != 1);

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
    LL_SetSystemCoreClock(168000000);

    /* Update the time base */
    if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK) {
        Error_Handler();  
    }
    LL_RCC_SetI2SClockSource(LL_RCC_I2S1_CLKSOURCE_PLLI2S);
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{

}
