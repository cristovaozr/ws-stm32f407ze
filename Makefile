##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.7.1] date: [Sat May 16 20:53:25 BRT 2020]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = ws-stm32f407ze

######################################
# building variables
######################################
# debug build?
DEBUG = 1

MCU_SRAM_SIZE = 131072

#######################################
# paths
#######################################
# Build path
BUILD_DIR = /tmp/build

######################################
# source
######################################
# C sources
C_SOURCES =  \
    Core/Src/main.c \
    Core/Src/gpio.c \
    Core/Src/freertos.c \
    Core/Src/fsmc.c \
    Core/Src/i2c.c \
    Core/Src/i2s.c \
    Core/Src/sdio.c \
    Core/Src/usart.c \
    Core/Src/stm32f4xx_it.c \
    Core/Src/stm32f4xx_hal_msp.c \
    Core/Src/stm32f4xx_hal_timebase_tim.c \
    Core/Src/system_stm32f4xx.c

C_SOURCES += \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c \
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c

C_SOURCES += \
    FreeRTOS/croutine.c \
    FreeRTOS/event_groups.c \
    FreeRTOS/list.c \
    FreeRTOS/queue.c \
    FreeRTOS/stream_buffer.c \
    FreeRTOS/tasks.c \
    FreeRTOS/timers.c \
    FreeRTOS/portable/MemMang/heap_czr.c \
    FreeRTOS/portable/GCC/ARM_CM4F/port.c

C_SOURCES += \
    os/newlib/close.c \
    os/newlib/lseek.c \
    os/newlib/read.c \
    os/newlib/write.c

C_SOURCES += \
    os/driver/fildes.c \
    os/driver/Impl/usartImpl.c

# ASM sources
ASM_SOURCES =  \
	Core/startup_stm32f407xx.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
	-DUSE_FULL_LL_DRIVER \
	-DUSE_HAL_DRIVER \
	-DSTM32F407xx \
	-DMCU_SRAM_SIZE=$(MCU_SRAM_SIZE)

# AS includes
AS_INCLUDES =  \
	-ICore/Inc

# C includes
C_INCLUDES =  \
    -ICore/Inc \
    -IDrivers/STM32F4xx_HAL_Driver/Inc \
    -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
    -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
    -IDrivers/CMSIS/Include \
    -IFreeRTOS/include \
    -IFreeRTOS/portable/GCC/ARM_CM4F \
    -Ios

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) -Wall \
    -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) -Wall \
    -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = Core/STM32F407ZETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) \
    -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

# $(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
# 	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	

$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
