/**
  ******************************************************************************
  * File Name          : I2C.h
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#include "cs43l22.h"
   
/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN Private defines */

#define AUDIO_I2C_ADDRESS                       ((uint16_t) 0x94)

/* USER CODE END Private defines */

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */

/* High Layer codec functions */
uint32_t AUDIO_Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
void     AUDIO_DeInit(void);
uint32_t AUDIO_ReadID(uint16_t DeviceAddr);
uint32_t AUDIO_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size);
uint32_t AUDIO_Pause(uint16_t DeviceAddr);
uint32_t AUDIO_Resume(uint16_t DeviceAddr);
uint32_t AUDIO_Stop(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t AUDIO_SetVolume(uint16_t DeviceAddr, uint8_t Volume);
uint32_t AUDIO_SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq);
uint32_t AUDIO_SetMute(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t AUDIO_SetOutputMode(uint16_t DeviceAddr, uint8_t Output);
uint32_t AUDIO_Reset(uint16_t DeviceAddr);

/* AUDIO IO functions */
void      AUDIO_IO_Init(void);
void      AUDIO_IO_DeInit(void);
void      AUDIO_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t   AUDIO_IO_Read(uint8_t Addr, uint8_t Reg);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
