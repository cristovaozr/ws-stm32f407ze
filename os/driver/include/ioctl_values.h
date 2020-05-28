#ifndef OS_DRIVER_INCLUDE_IOCTL_VALUES_H_
#define OS_DRIVER_INCLUDE_IOCTL_VALUES_H_

#define IOCTL_GPIO_READ_PIN   0
#define IOCTL_GPIO_WRITE_PIN  1

struct gpio_ioctl { int pin; int value; };

#endif // OS_DRIVER_INCLUDE_IOCTL_VALUES_H_