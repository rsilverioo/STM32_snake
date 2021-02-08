################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/l3gd20/l3gd20.c 

OBJS += \
./Drivers/BSP/Components/l3gd20/l3gd20.o 

C_DEPS += \
./Drivers/BSP/Components/l3gd20/l3gd20.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/l3gd20/l3gd20.o: ../Drivers/BSP/Components/l3gd20/l3gd20.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/ssd1306" -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Src -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/BSP/STM32L476G-Discovery" -I../USB_DEVICE/App -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FatFs/src -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/l3gd20/l3gd20.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

