#include "FreeRTOS.h"
#include "task.h"
#include "driver/include/fildes.h"
#include "newlib/syscalls.h"

#include "stm32f4xx.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_utils.h"

static void start_hardware(void);
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

    start_hardware();

    /* Initialize all configured peripherals */
    // MX_GPIO_Init();
    // MX_FSMC_Init();
    // MX_I2C1_Init();
    // MX_I2S2_Init();
    // MX_SDIO_SD_Init();

    /* Init scheduler */
    MX_FREERTOS_Init(); 
    /* Start scheduler */
    vTaskStartScheduler();

    while (1);
}

static void start_hardware(void)
{
    extern struct fildes usart;
    store_fildes(&usart);

    open("tty0", 0); // Open first so its file descriptor is 0
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
    // if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
    //    Error_Handler();  
    // }
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

    LL_RCC_SetI2SClockSource(LL_RCC_I2S1_CLKSOURCE_PLLI2S);
}

/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{

}
