################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery.c \
../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_compass.c \
../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_glass_lcd.c \
../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_gyroscope.c \
../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_qspi.c 

OBJS += \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery.o \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_compass.o \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_glass_lcd.o \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_gyroscope.o \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_qspi.o 

C_DEPS += \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery.d \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_compass.d \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_glass_lcd.d \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_gyroscope.d \
./Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_qspi.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery.o: ../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/ssd1306" -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Src -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/BSP/STM32L476G-Discovery" -I../USB_DEVICE/App -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FatFs/src -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_compass.o: ../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_compass.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/ssd1306" -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Src -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/BSP/STM32L476G-Discovery" -I../USB_DEVICE/App -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FatFs/src -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_compass.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_glass_lcd.o: ../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_glass_lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/ssd1306" -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Src -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/BSP/STM32L476G-Discovery" -I../USB_DEVICE/App -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FatFs/src -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_glass_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_gyroscope.o: ../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_gyroscope.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/ssd1306" -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Src -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/BSP/STM32L476G-Discovery" -I../USB_DEVICE/App -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FatFs/src -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_gyroscope.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_qspi.o: ../Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_qspi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../USB_DEVICE/Target -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/ssd1306" -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Src -I"C:/Users/rsilv/STM32CubeIDE/workspace_1.2.0/snake_Game/Drivers/BSP/STM32L476G-Discovery" -I../USB_DEVICE/App -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FatFs/src -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery_qspi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

