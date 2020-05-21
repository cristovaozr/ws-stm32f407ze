#include <driver/include/fildes.h>

#include <stm32f4xx.h>
#include <stm32f4xx_ll_usart.h>
#include <stm32f4xx_ll_gpio.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_hal.h>

static void MX_USART2_UART_Init(void);

static ssize_t usart_Read(FileDescriptor *fildes, void *buf, size_t size)
{
    ssize_t ret;
    xSemaphoreTake(fildes->mutex, portMAX_DELAY);
    for(ret = 0; ret < (ssize_t)size; ret++) {
        xQueueReceive(fildes->rxQueue, &((uint8_t *)buf)[ret], portMAX_DELAY);
    }
    xSemaphoreGive(fildes->mutex);
    return ret;
}

static ssize_t usart_Write(FileDescriptor *fildes, const void *buf, size_t size)
{
    ssize_t ret;
    xSemaphoreTake(fildes->mutex, portMAX_DELAY);
    for(ret = 0; ret < (ssize_t)size; ret++) {
        xQueueSend(fildes->txQueue, &((const uint8_t *)buf)[ret], portMAX_DELAY);
        LL_USART_EnableIT_TXE((USART_TypeDef *)fildes->internalDevice);
    }
    xSemaphoreGive(fildes->mutex);
    return ret;
}

static int usart_Init(FileDescriptor *fildes)
{
    fildes->txQueue = xQueueCreate(32, sizeof(uint8_t));
    fildes->rxQueue = xQueueCreate(32, sizeof(uint8_t));
    fildes->mutex = xSemaphoreCreateMutex();

    // Enable RXNE ISR
    MX_USART2_UART_Init();
    LL_USART_EnableIT_RXNE((USART_TypeDef *)fildes->internalDevice);
    return 0;
}

static int usart_Fini(FileDescriptor *fildes)
{
    return 0;
}

static int usart_Open(FileDescriptor *fildes)
{
    LL_USART_EnableIT_RXNE((USART_TypeDef *)fildes->internalDevice);
    NVIC_SetPriority(USART2_IRQn, 15);
    NVIC_EnableIRQ(USART2_IRQn);
    return 0;
}

static int usart_Close(FileDescriptor *fildes)
{
    LL_USART_DisableIT_TXE((USART_TypeDef *)fildes->internalDevice);
    LL_USART_DisableIT_RXNE((USART_TypeDef *)fildes->internalDevice);
    NVIC_DisableIRQ(USART2_IRQn);
    return 0;
}

const FileOperations usartFileOperations = {
    .init = usart_Init,
    .fini = usart_Fini,
    .open = usart_Open,
    .close = usart_Close,
    .read = usart_Read,
    .write = usart_Write,
};

FileDescriptor usartFileDescriptor = {
    .fileOperations = &usartFileOperations,
    .internalDevice = USART2,
};

static void USART_IrqHandler(FileDescriptor *usart)
{
    if (LL_USART_IsActiveFlag_TXE((USART_TypeDef *)usart->internalDevice)) {
        BaseType_t contextSwitch;

        while (LL_USART_IsActiveFlag_TXE((USART_TypeDef *)usart->internalDevice)) {
            uint8_t byte;
            if (xQueueReceiveFromISR(usart->txQueue, &byte, &contextSwitch) == pdFAIL) {
                LL_USART_DisableIT_TXE(usart->internalDevice);
                break;
            }
            LL_USART_TransmitData8((USART_TypeDef *)usart->internalDevice, byte);
        }

        portYIELD_FROM_ISR(contextSwitch);
    }

    if (LL_USART_IsActiveFlag_RXNE((USART_TypeDef *)usart->internalDevice)) {
        BaseType_t contextSwitch;

        while (LL_USART_IsActiveFlag_RXNE((USART_TypeDef *)usart->internalDevice)) {
            uint8_t byte = LL_USART_ReceiveData8((USART_TypeDef *)usart->internalDevice);
            xQueueSendFromISR(usart->rxQueue, &byte, &contextSwitch);
            // xQueueSendFromISR can return errQUEUE_FULL
        }

        portYIELD_FROM_ISR(contextSwitch);
    }
}

void USART2_IRQHandler(void)
{
    USART_IrqHandler(&usartFileDescriptor);
}

static void MX_USART2_UART_Init(void)
{
    LL_USART_InitTypeDef USART_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    /**USART2 GPIO Configuration  
    PA2   ------> USART2_TX
    PA3   ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART2, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(USART2);
    LL_USART_Enable(USART2);
}