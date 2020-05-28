#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_bus.h"
#include "driver/include/fildes.h"
#include "driver/include/ioctl_values.h"

#define JOY_A_PC13_Pin          LL_GPIO_PIN_13
#define JOY_A_PC13_GPIO_Port    GPIOC
#define JOY_B_PE6_Pin           LL_GPIO_PIN_5
#define JOY_B_PE6_GPIO_Port     GPIOE
#define JOY_C_PE4_Pin           LL_GPIO_PIN_4
#define JOY_C_PE4_GPIO_Port     GPIOE
#define JOY_D_PE2_Pin           LL_GPIO_PIN_2
#define JOY_D_PE2_GPIO_Port     GPIOE
#define JOY_CTR_PA5_Pin         LL_GPIO_PIN_5
#define JOY_CTR_PA5_GPIO_Port   GPIOA
#define USER1_PA0_Pin           LL_GPIO_PIN_0
#define USER1_PA0_GPIO_Port     GPIOA
#define USER2_PF10_Pin          LL_GPIO_PIN_10
#define USER2_PF10_GPIO_Port    GPIOF

struct gpio_indev {
    GPIO_TypeDef *gpios[7];
    uint32_t pins[7];
};

static void MX_GPIO_Init(void);

static int gpio_init(struct fildes *fildes)
{
    MX_GPIO_Init();
    return 0;
}

static int gpio_open(struct fildes *fildes, int flags)
{
    return 0;
}

static int gpio_close(struct fildes *fildes)
{
    return 0;
}

static int gpio_ioctl(struct fildes *fildes, unsigned long request, long data)
{
    int ret;
    struct gpio_indev *indev = (struct gpio_indev *)fildes->internal_device;
    struct gpio_ioctl *gpio = (struct gpio_ioctl *)data;

    switch (request) {
    case IOCTL_GPIO_READ_PIN: 
        gpio->value = LL_GPIO_IsInputPinSet(indev->gpios[gpio->pin], indev->pins[gpio->pin]);
        ret = 0;
        break;
    
    case IOCTL_GPIO_WRITE_PIN: 
        if (gpio->value) {
            LL_GPIO_SetOutputPin(indev->gpios[gpio->pin], indev->pins[gpio->pin]);
        } else {
            LL_GPIO_ResetOutputPin(indev->gpios[gpio->pin], indev->pins[gpio->pin]);
        }
        ret = 0;
        break;

    default:
        ret = -1;
        break;
    }

    return ret;
}

static struct gpio_indev gpio_indev = {
    .gpios = {
        JOY_A_PC13_GPIO_Port, 
        JOY_B_PE6_GPIO_Port,
        JOY_C_PE4_GPIO_Port,
        JOY_D_PE2_GPIO_Port,
        JOY_CTR_PA5_GPIO_Port,
        USER1_PA0_GPIO_Port,
        USER2_PF10_GPIO_Port
    },
    .pins = {
        JOY_A_PC13_Pin,
        JOY_B_PE6_Pin,
        JOY_C_PE4_Pin,
        JOY_D_PE2_Pin,
        JOY_CTR_PA5_Pin,
        USER1_PA0_Pin,
        USER2_PF10_Pin
    }
};

static struct f_ops gpio_fops = {
    .init = gpio_init,
    .open = gpio_open,
    .close = gpio_close,
    .ioctl = gpio_ioctl
};

struct fildes gpio_driver = {
    .path = "gpio0",
    .fops = &gpio_fops,
    .internal_device = &gpio_indev
};

static void MX_GPIO_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

    /**/
    GPIO_InitStruct.Pin = JOY_D_PE2_Pin|JOY_C_PE4_Pin|JOY_B_PE6_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = JOY_A_PC13_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(JOY_A_PC13_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = USER2_PF10_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(USER2_PF10_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = USER1_PA0_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(USER1_PA0_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = JOY_CTR_PA5_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(JOY_CTR_PA5_GPIO_Port, &GPIO_InitStruct);
}