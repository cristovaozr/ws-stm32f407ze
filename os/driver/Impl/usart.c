#include "driver/include/fildes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"

static void MX_USART2_UART_Init(void);

struct usart_indev {
    USART_TypeDef *usart;
    QueueHandle_t tx_queue;
    QueueHandle_t rx_queue;
};

#define USART_INDEV(x)  ((struct usart_indev *)x)

static ssize_t usart_read(struct fildes *fildes, void *buf, size_t size)
{
    ssize_t ret;
    struct usart_indev *indev = USART_INDEV(fildes->internal_device);
    uint8_t *ubuf = (uint8_t *)buf;

    xSemaphoreTake(fildes->mutex, portMAX_DELAY);
    for(ret = 0; ret < (ssize_t)size; ret++) {
        xQueueReceive(indev->rx_queue, &ubuf[ret], portMAX_DELAY);
    }
    xSemaphoreGive(fildes->mutex);
    return ret;
}

static ssize_t usart_write(struct fildes *fildes, const void *buf, size_t size)
{
    ssize_t ret;
    struct usart_indev *indev = USART_INDEV(fildes->internal_device);
    const uint8_t *ubuf = (const uint8_t *)buf;

    xSemaphoreTake(fildes->mutex, portMAX_DELAY);
    for(ret = 0; ret < (ssize_t)size; ret++) {
        xQueueSend(indev->tx_queue, &ubuf[ret], portMAX_DELAY);
        LL_USART_EnableIT_TXE(indev->usart);
    }
    xSemaphoreGive(fildes->mutex);
    return ret;
}

static int usart_init(struct fildes *fildes)
{
    struct usart_indev *indev = USART_INDEV(fildes->internal_device);

    fildes->mutex = xSemaphoreCreateMutex();
    indev->tx_queue = xQueueCreate(128, sizeof(uint8_t));
    indev->rx_queue = xQueueCreate(128, sizeof(uint8_t));

    // Enable RXNE ISR
    MX_USART2_UART_Init();
    return 0;
}

static int usart_open(struct fildes *fildes, int flags)
{
    (void)flags;
    struct usart_indev *indev = USART_INDEV(fildes->internal_device);
    LL_USART_EnableIT_RXNE(indev->usart);
    return 0;
}

static int usart_close(struct fildes *fildes)
{
    struct usart_indev *indev = USART_INDEV(fildes->internal_device);
    LL_USART_DisableIT_TXE(indev->usart);
    LL_USART_DisableIT_RXNE(indev->usart);
    NVIC_DisableIRQ(USART2_IRQn);
    return 0;
}

const struct f_ops usart_fops = {
    .init = usart_init,
    .open = usart_open,
    .close = usart_close,
    .read = usart_read,
    .write = usart_write,
};

static struct usart_indev usart2_indev = {
    .usart = USART2
};

struct fildes usart = {
    .path = "tty0",
    .fops = &usart_fops,
    .internal_device = &usart2_indev
};



static void USART_IrqHandler(struct fildes *fildes)
{
    struct usart_indev *indev = USART_INDEV(fildes->internal_device);

    if (LL_USART_IsActiveFlag_TXE(indev->usart)) {
        BaseType_t contextSwitch;

        while (LL_USART_IsActiveFlag_TXE(indev->usart)) {
            uint8_t byte;
            if (xQueueReceiveFromISR(indev->tx_queue, &byte, &contextSwitch) == pdFAIL) {
                LL_USART_DisableIT_TXE(indev->usart);
                break;
            }
            LL_USART_TransmitData8(indev->usart, byte);
        }

        portYIELD_FROM_ISR(contextSwitch);
    }

    if (LL_USART_IsActiveFlag_RXNE(indev->usart)) {
        BaseType_t contextSwitch;

        while (LL_USART_IsActiveFlag_RXNE(indev->usart)) {
            uint8_t byte = LL_USART_ReceiveData8(indev->usart);
            xQueueSendFromISR(indev->rx_queue, &byte, &contextSwitch);
            // xQueueSendFromISR can return errQUEUE_FULL
        }

        portYIELD_FROM_ISR(contextSwitch);
    }
}

void USART2_IRQHandler(void)
{
    USART_IrqHandler(&usart);
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

    NVIC_SetPriority(USART2_IRQn, 10);
    NVIC_EnableIRQ(USART2_IRQn);
}