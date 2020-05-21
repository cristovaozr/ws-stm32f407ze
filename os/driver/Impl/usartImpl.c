#include <driver/include/fildes.h>

#include <stm32f4xx.h>
#include <stm32f4xx_ll_usart.h>

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
    return 0;
}

static int usart_Close(FileDescriptor *fildes)
{
    LL_USART_DisableIT_TXE((USART_TypeDef *)fildes->internalDevice);
    LL_USART_DisableIT_RXNE((USART_TypeDef *)fildes->internalDevice);
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

void USART2_IrqHandler(void)
{
    USART_IrqHandler(&usartFileDescriptor);
}