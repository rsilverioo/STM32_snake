/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v2.0_Cube
  * @brief          : Memory management layer.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "stm32l476g_discovery.h"
#include "stm32l476g_discovery_qspi.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern QSPI_HandleTypeDef QSPIHandle;
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */
#define STORAGE_BLK_SIZ_USER             0x1000
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */
  
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
}; 
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */
uint8_t QSPI_ReadBuffer[N25Q128A_SUBSECTOR_SIZE] = {0};
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes over USB FS IP
  * @param  lun:
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_FS(uint8_t lun)
{
  /* USER CODE BEGIN 2 */
	if (BSP_QSPI_GetStatus() != QSPI_OK)
	{
		HAL_QSPI_Init(&QSPIHandle);

		if (BSP_QSPI_Init() != QSPI_OK)
		{
			return (USBD_FAIL);
		}
	}
	return (USBD_OK);
  /* USER CODE END 2 */
}

/**
  * @brief  .
  * @param  lun: .
  * @param  block_num: .
  * @param  block_size: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
	//  *block_num  = STORAGE_BLK_NBR;
	//  *block_size = STORAGE_BLK_SIZ;
	QSPI_Info info;

	BSP_QSPI_GetInfo(&info);
	*block_num = (info.FlashSize)/STORAGE_BLK_SIZ_USER;
	*block_size = STORAGE_BLK_SIZ_USER;
	return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
  /* USER CODE BEGIN 4 */
	uint8_t status = QSPI_ERROR;
	int8_t ret = USBD_FAIL;

	status = BSP_QSPI_GetStatus();

	switch(status)
	{
	case QSPI_OK:
		ret = USBD_OK;
		break;
	case QSPI_BUSY:
		ret = USBD_BUSY;
		break;
	case QSPI_ERROR:
		ret = USBD_FAIL;
		break;
	default:
		ret = USBD_FAIL;
		break;
	}

	return ret;
	//  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */
	uint32_t erasedSector = 1;
	BSP_QSPI_Read(QSPI_ReadBuffer, blk_addr * STORAGE_BLK_SIZ_USER, blk_len * STORAGE_BLK_SIZ_USER);

	for (uint32_t i = 0; i < STORAGE_BLK_SIZ_USER; i++)
	{
		if (QSPI_ReadBuffer[i] != 0xFF)
		{
			erasedSector = 0;
			break;
		}
	}
	if (erasedSector == 1)
	{
		for (uint32_t i = 0; i < STORAGE_BLK_SIZ_USER; i++)
		{
			buf[i] = 0x00;
		}
	}
	else {
		for (uint32_t i = 0; i < STORAGE_BLK_SIZ_USER; i++)
		{
			buf[i] = QSPI_ReadBuffer[i];
		}
	}
	return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */
	uint32_t needErase = 0;
	uint32_t eraseOperation = 1;
	BSP_QSPI_Read(QSPI_ReadBuffer, blk_addr * STORAGE_BLK_SIZ_USER, STORAGE_BLK_SIZ_USER);
	for (uint32_t i = 0; i < STORAGE_BLK_SIZ_USER; i++)
	{
		if (buf[i] != 0x00)
		{
			eraseOperation = 0;
			break;
		}
	}
	for (uint32_t i = 0; i < STORAGE_BLK_SIZ_USER; i++)
	{
		if (QSPI_ReadBuffer[i] != 0xFF)
		{
			needErase = 1;
			break;
		}
	}
	if (eraseOperation == 1)
	{
		if (needErase == 1)
		{
			BSP_LED_On(LED_RED);
			BSP_QSPI_Erase_Block(blk_addr * STORAGE_BLK_SIZ_USER);
			BSP_LED_Off(LED_RED);
		}
	}
	else {
		if (needErase == 1)
		{
			BSP_LED_On(LED_RED);
			BSP_QSPI_Erase_Block(blk_addr * STORAGE_BLK_SIZ_USER);
			BSP_LED_Off(LED_RED);

			BSP_LED_On(LED_GREEN);
			BSP_QSPI_Write(buf, blk_addr * STORAGE_BLK_SIZ_USER, STORAGE_BLK_SIZ_USER);
			BSP_LED_Off(LED_GREEN);
		}
		else {
			BSP_LED_On(LED_GREEN);
			BSP_QSPI_Write(buf, blk_addr * STORAGE_BLK_SIZ_USER, STORAGE_BLK_SIZ_USER);
			BSP_LED_Off(LED_GREEN);
		}
	}
	return (USBD_OK);
  /* USER CODE END 7 */
}

/**
  * @brief  .
  * @param  None
  * @retval .
  */
int8_t STORAGE_GetMaxLun_FS(void)
{
  /* USER CODE BEGIN 8 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
