/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
// ***** Librerías comunes *****
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// ***** Librerías del BSP *****
#include "stm32l476g_discovery.h"
#include "stm32l476g_discovery_glass_lcd.h"
#include "stm32l476g_discovery_compass.h"
#include "stm32l476g_discovery_gyroscope.h"

// ***** Librerías de la pantalla oled *****
#include "ssd1306.h"
#include "ssd1306_tests.h"

// ***** Librerías necesarias para los periféricos *****
#include "stm32l4xx_hal.h"
#include "usbd_core.h"
#include "usb_device.h"
#include "fatfs.h"
#include "sai.h"
#include "i2c.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

// ***** Variables del reproductor *****
#define AUDIO_BUFFER_SIZE          (1024 * 8)       // Tamaño en bytes del buffer con los samples
#define WAV_FILE_HEADER_SIZE        44              // Tamaño en bytes de la cabecera del archivo WAV

// ***** Variables del sistema de archivos Fat y del USB *****
extern FIL                     File;						// Handle del archivo
extern FATFS                   FatFs;						// Handle del sistema de archivos
uint8_t                 Buffer[AUDIO_BUFFER_SIZE];	// Buffer para los samples
uint32_t                ActualPosition;             // Offset de la lectura actual
uint32_t                PrevPosition;               // Offset anterior
USBD_HandleTypeDef	    hUsbDeviceFS;

// ***** Variables de estado del reproductor *****
typedef enum {
	AUDIOPLAYER_STATE_STOPPED = 0,
	AUDIOPLAYER_STATE_RUNNING,
	AUDIOPLAYER_STATE_PAUSED
} AudioPlayer_StateTypeDef;
AudioPlayer_StateTypeDef  AppState = AUDIOPLAYER_STATE_RUNNING;
bool audio_flag = 0;

// ***** Parámetros de la pantalla *****
extern const int width;
extern const int height;

// ***** Parámetros del menu *****
extern SSD1306_COLOR color;	// Color de fondo del texto
extern int menu;	// Indice del menu

// ***** Parámetros del juego *****
extern bool gameover;	// Bandera de Game Over
extern int x, y, score;	// Coordenadas de la pantalla y variable para los puntos
extern int fruitX[3], fruitY[3];	// Arrays con las coordenadas de las frutas
extern int snakeX[256], snakeY[256]; // Arrays con las coordenadas del cuerpo de la serpiente
extern int nTail;	// Número de pixeles que forman la cola de la serpiente
extern int state, prev_state;	// Estado en el que nos encontramos y el anterior a este

/* USER CODE END Variables */
osThreadId gameTaskHandle;
osThreadId usbTaskHandle;
osThreadId audioTaskHandle;
osThreadId inputTaskHandle;
osMessageQId joystickInputQueueHandle;
osMutexId qspiMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void Error_Handler(void);

// ***** Funciones de Audio *****
void AudioPlayer_Start(const TCHAR* file_name, bool bg_music, bool aflag);
void AudioPlayer_Pause(void);
void AudioPlayer_Stop(void);
void AudioPlayer_Resume(void);

// ***** Funciones del Juego *****
void Setup();		// Función para establecer las condiciones iniciales
void DrawOled();	// Función para dibujar la pantalla oled
void Algoritmo();	// Función que controla la ejecución del juego
void Fruta();		// Función para crear las frutas en la pantalla
void GameOver();	// Función para el estado de Game Over
void Leaderboard();	// Función para mostrar las mejores puntuaciones conseguidas

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
/* USER CODE END FunctionPrototypes */

void gameTaskBody(void const * argument);
void usbTaskBody(void const * argument);
void audioTaskBody(void const * argument);
void inputTaskBody(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of qspiMutex */
  osMutexDef(qspiMutex);
  qspiMutexHandle = osMutexCreate(osMutex(qspiMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of joystickInputQueue */
  osMessageQDef(joystickInputQueue, 1, uint16_t);
  joystickInputQueueHandle = osMessageCreate(osMessageQ(joystickInputQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of gameTask */
  osThreadDef(gameTask, gameTaskBody, osPriorityNormal, 0, 4096);
  gameTaskHandle = osThreadCreate(osThread(gameTask), NULL);

  /* definition and creation of usbTask */
  osThreadDef(usbTask, usbTaskBody, osPriorityHigh, 0, 256);
  usbTaskHandle = osThreadCreate(osThread(usbTask), NULL);

  /* definition and creation of audioTask */
  osThreadDef(audioTask, audioTaskBody, osPriorityNormal, 0, 1024);
  audioTaskHandle = osThreadCreate(osThread(audioTask), NULL);

  /* definition and creation of inputTask */
  osThreadDef(inputTask, inputTaskBody, osPriorityNormal, 0, 128);
  inputTaskHandle = osThreadCreate(osThread(inputTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_gameTaskBody */
/**
  * @brief  Function implementing the gameTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_gameTaskBody */
void gameTaskBody(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN appTaskBody */

	/* Esperamos por el qspiMutex */
	osMutexWait(qspiMutexHandle, osWaitForever);

	/* Montamos el sistema de archivos del módulo FatFs */
	if(f_mount(&FatFs, (TCHAR const*)USERPath, 1) != FR_OK)
	{
		/* Error de inicialización de FatFs */
		if (f_mkfs((TCHAR const*)USERPath, 0, 128, NULL, 0) != FR_OK)
		{
			Error_Handler();
		}
		else {
			/* Segundo intento de montar el sistema de archivos */
			if(f_mount(&FatFs, (TCHAR const*)USERPath, 1) != FR_OK)
			{
				Error_Handler();
			}
		}
	}

	/* Soltamos el qspiMutex */
	osMutexRelease(qspiMutexHandle);

	/* Inicializamos el driver de audio */
	if(CS43L22_ID != AUDIO_ReadID(AUDIO_I2C_ADDRESS))
	{
		Error_Handler();
	}
	if(0 != AUDIO_Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_HEADPHONE, 60, AUDIO_FREQUENCY_44K))
	{
		Error_Handler();
	}

	/* Iniciamos el reproductor */
	AudioPlayer_Start("snake.wav", 0, 0);

	/* Infinite loop */
	for(;;)
	{
		if(gameover == true)
		{
			/* Audio y Pantalla de Game Over */
			AudioPlayer_Start("gameover.wav", 0, 1);
			ssd1306_SetCursor(32, 4);
			ssd1306_WriteString("GAME", Font_16x26, White);
			ssd1306_SetCursor(32, 34);
			ssd1306_WriteString("OVER", Font_16x26, White);
			ssd1306_UpdateScreen();
			osDelay(1500);

			/* Esperamos por el qspiMutex */
			osMutexWait(qspiMutexHandle, osWaitForever);

			GameOver();

			/* Soltamos el qspiMutex */
			osMutexRelease(qspiMutexHandle);
		}
		else
		{
			Algoritmo();	// En caso contrario, calculamos lanueva configuración
			DrawOled();		// y la mostramos por pantalla
		}

		osDelay(100);		// Espera de 100 ms para que el juego no vaya demasiado rápido

	}
  /* USER CODE END appTaskBody */
}

/* USER CODE BEGIN Header_usbTaskBody */
/**
 * @brief Function implementing the usbTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_usbTaskBody */
void usbTaskBody(void const * argument)
{
  /* USER CODE BEGIN usbTaskBody */
	uint32_t USB_VBUS_counter = 0;	// Variable del bus USB

	/* Infinite loop */
	for(;;)
	{

		USB_VBUS_counter = 0;
		/* Comprobamos si USB_VBUS está disponible */
		while (USB_VBUS_counter < 5)
		{
			osDelay(10);
			if (HAL_GPIO_ReadPin(USB_VBUS_GPIO_Port, USB_VBUS_Pin) != GPIO_PIN_RESET)
			{
				USB_VBUS_counter++;
			}
			else {
				break;
			}
		}
		if(USB_VBUS_counter >= 5 && menu == 0)
		{

			ssd1306_Init();
			ssd1306_Fill(Black);
			ssd1306_SetCursor(32, 4);
			ssd1306_WriteString("MODO", Font_16x26, White);
			ssd1306_SetCursor(40, 34);
			ssd1306_WriteString("USB", Font_16x26, White);
			ssd1306_UpdateScreen();

			/* Paramos el reproductor de audio, si está funcionando */
			if (AppState != AUDIOPLAYER_STATE_STOPPED)
			{
				AudioPlayer_Stop();
			}

			/* Esperamos por el qspiMutex */
			osMutexWait(qspiMutexHandle, osWaitForever);

			/* Inicializamos el USB */
			MX_USB_DEVICE_Init();

			/* RED LED ON */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

			/* USB_VBUS availability check */
			while (USB_VBUS_counter)
			{
				/* Wait 100ms, then check the USB_VBUS availability */
				osDelay(100);

				if (HAL_GPIO_ReadPin(USB_VBUS_GPIO_Port, USB_VBUS_Pin) == GPIO_PIN_RESET)
				{
					USB_VBUS_counter--;
				}
				else {
					USB_VBUS_counter = 5;
				}
			}

			/* Deinicializamos el USB */
			USBD_DeInit(&hUsbDeviceFS);

			/* Soltamos el qspiMutex */
			osMutexRelease(qspiMutexHandle);

			/* RED LED OFF */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

			/* Reiniciamos el sistema para recargar la configuración */
			NVIC_SystemReset();
		}
		/* Cada 1 segundo comprobamos si el USB_VBUS está disponible */
		osDelay(1000);
	}
  /* USER CODE END usbTaskBody */
}

/* USER CODE BEGIN Header_audioTaskBody */
/**
 * @brief Function implementing the audioTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_audioTaskBody */
void audioTaskBody(void const * argument)
{
  /* USER CODE BEGIN audioTaskBody */
	UINT bufferOffset = 0;	// Offset del buffer
	UINT bytesRead = 0;		// Número de bytes leidos del archivo
	osEvent event;

	/* Pausamos la tarea */
	osThreadSuspend(NULL);

	/* Infinite loop */
	for(;;)
	{
		event = osSignalWait(A | B, osWaitForever);
		if( event.status == osEventSignal )
		{
			switch(event.value.v)
			{
			case B:
				bufferOffset = AUDIO_BUFFER_SIZE/2;	// Offset para el canal B
				break;
			case A:
				bufferOffset = 0;	// Offset para el canal A
				break;
			default:
				/* No somos capaces de streamear más datos => limpiamos el buffer => velocidad de SYSCLK */
				break;
			}

			/* Esperamos por el qspiMutex */
			osMutexWait(qspiMutexHandle, osWaitForever);

			/* Llenamos el buffer con datos */
			if (f_read(&File, (uint8_t *)(Buffer + bufferOffset), AUDIO_BUFFER_SIZE/2, &bytesRead) != FR_OK)
			{
				Error_Handler();
			}

			/* GREEN LED ON */
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

			ActualPosition += bytesRead;

			/* Comprobamos si se ha llegado al final del archivo */
			if((ActualPosition + (AUDIO_BUFFER_SIZE/2)) > f_size(&File))
			{
				if(audio_flag == 1){	// En caso de no estar reproduciendo la música de fondo entramos aquí

					if (gameover == 1)	// Si estamos en la situación de Game Over necesitamos parar la reproducción
					{
						/* Paramos el playback */
						if (AUDIO_Stop(AUDIO_I2C_ADDRESS, CODEC_PDWN_SW) != 0)
						{
							Error_Handler();
						}
						/* Paramos la transferencia por DMA */
						if (HAL_SAI_DMAStop(&hsai_BlockA1) != HAL_OK)
						{
							Error_Handler();
						}

						osMutexRelease(qspiMutexHandle);	// Soltamos el mutex
						vTaskSuspend( NULL );	// Y suspendemos la tarea
					}

					/* Cerramos el archivo de audio */
					if(f_close(&File) != FR_OK)
					{
						Error_Handler();
					}

					osMutexRelease(qspiMutexHandle);	// Soltamos el mutex

					/* Paramos la transferencia por DMA */
					if (HAL_SAI_DMAStop(&hsai_BlockA1) != HAL_OK)
					{
						Error_Handler();
					}

					AudioPlayer_Start("snake.wav", 1, 0);	// Reiniciamos la reproducción de la música de fondo
				}
				else	// Si está cargada la música de fondo, rebobinamos el
				{		// archivo de audio para reproducirlo desde el principio

					if (f_rewind(&File) != FR_OK)
					{
						Error_Handler();
					}

					if (f_lseek(&File, WAV_FILE_HEADER_SIZE) != FR_OK)
					{
						Error_Handler();
					}

					/* Nos saltamos la cabecera del archivo WAV */
					ActualPosition = WAV_FILE_HEADER_SIZE;
				}
			}

			/* Soltamos el qspiMutex */
			osMutexRelease(qspiMutexHandle);
		}
		else {
			// No hacemos nada :)
		}
	}
  /* USER CODE END audioTaskBody */
}

/* USER CODE BEGIN Header_inputTaskBody */
/**
 * @brief Function implementing the inputTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_inputTaskBody */
void inputTaskBody(void const * argument)
{
  /* USER CODE BEGIN inputTaskBody */
	osEvent event;

	/* Infinite loop */
	for(;;)
	{
		if (menu == 1)	// Juego usando el DPAD
		{
			event = osMessageGet(joystickInputQueueHandle, osWaitForever);

			if (event.status == osEventMessage)
			{
				/* Leemos la entrada del DPAD, y cambiamos de estado de acuerdo a eso */
				switch(event.value.v)
				{
					case JOY_LEFT_Pin:
						if(prev_state!=2)
							state = 1;
						break;
					case JOY_RIGHT_Pin:
						if(prev_state!=1)
							state = 2;
						break;
					case JOY_UP_Pin:
						if(prev_state!=4)
							state = 3;
						break;
					case JOY_DOWN_Pin:
						if(prev_state!=3)
							state = 4;
						break;
					case JOY_CENTER_Pin:
						state = 5;
						break;
					default:
						break;
				}
			}
		}
		else if (menu == 2)	// Juego usando el Giroscopio/Acelerómetro
		{
			float axis_gyro[3] = {0};
			float Xval, Yval = 0;
			BSP_GYRO_Reset();	// Recalibramos el giroscopio

			while(1)
			{
				/* Leemos los datos angulares del Giroscopio */
				BSP_GYRO_GetXYZ(axis_gyro);

				/* Damos formato a los nuevos valores y los evaluamos */
				Xval = abs((axis_gyro[0]));
				Yval = abs((axis_gyro[1]));

				if(Xval>Yval){
					if(axis_gyro[0] > 20000.0f){
						//Arriba
						//Evitar que pueda girar 180º
						state = 3;
					}else if(axis_gyro[0] < -20000.0f){
						//Abajo
						state = 4;
					}
				}else if (Yval>Xval){
					if(axis_gyro[1] < -20000.0f){
						//Izquierda
						state = 1;
					}else if(axis_gyro[1] > 20000.0f){
						//Derecha
						state = 2;
					}
				}

			}
		}
	}
  /* USER CODE END inputTaskBody */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief Application Error Handler
 * @param  None
 * @retval None
 */
//void Error_Handler(void)
//{
//  while(1)
//  {
/* Error management behavior to be added here */
//  }
//}

/**
 * @brief Tx Transfer completed callbacks.
 * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
 *                the configuration information for SAI module.
 * @retval None
 */
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
	/* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SAI_TxCpltCallback could be implemented in the user file
	 */
	osSignalSet(audioTaskHandle, B);
}

/**
 * @brief Tx Transfer Half completed callbacks
 * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
 *                the configuration information for SAI module.
 * @retval None
 */
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
	/* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SAI_TxHalfCpltCallback could be implenetd in the user file
	 */
	osSignalSet(audioTaskHandle, A);
}


/**
 * @brief AudioPlayer Start method
 * @param  None
 * @retval None
 */
void AudioPlayer_Start(const TCHAR* file_name, bool bg_music, bool aflag)
{
	audio_flag = aflag;

	/* Cargamos el codec de audio y habilitamos su reproducción */
	if(0 != AUDIO_Play(AUDIO_I2C_ADDRESS, NULL, 0))
	{
		Error_Handler();
	}

	/* Esperamos por el qspiMutex */
	osMutexWait(qspiMutexHandle, osWaitForever);

	/* Abrimos el archivo de audio en modo lectura */
	if(f_open(&File, file_name, FA_READ) != FR_OK)
	{
		Error_Handler();
	}

	if (bg_music == 0 )
	{
		/* Nos saltamos la cabacera del archivo WAV */
		PrevPosition = ActualPosition;
		ActualPosition = WAV_FILE_HEADER_SIZE;
	}
	else
	{
		/* Si venimos de una reproducción anterior, seguimos reproduciendo desde donde lo dejamos */
		ActualPosition = PrevPosition;
	}

	/* Movemos el puntero del archivo hasta la dirección correcta */
	if (f_lseek(&File, ActualPosition) != FR_OK)
	{
		Error_Handler();
	}

	/* Soltamos el qspiMutex */
	osMutexRelease(qspiMutexHandle);

	/* Iniciamos la transferencia por el DMA desde el buffer */
	if(HAL_OK != HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t *)Buffer, AUDIO_BUFFER_SIZE/2))
	{
		Error_Handler();
	}

	/* Cambiamos el estado de la aplicación */
	AppState = AUDIOPLAYER_STATE_RUNNING;

	/* Volvemos a activar la tarea */
	osThreadResume(audioTaskHandle);
}

// *************************************
// ***** Funciones del reproductor *****
// *************************************

/**
 * @brief AudioPlayer Resume method
 * @param  None
 * @retval None
 */
void AudioPlayer_Resume(void)
{
	/* Restablecemos la transferencia por DMA */
	if (HAL_SAI_DMAResume(&hsai_BlockA1) != HAL_OK)
	{
		Error_Handler();
	}
	/* Restablecemos la reproducción */
	if (AUDIO_Resume(AUDIO_I2C_ADDRESS) != 0)
	{
		Error_Handler();
	}

	AppState = AUDIOPLAYER_STATE_RUNNING;

	/* Volvemos a activar la tarea */
	osThreadResume(audioTaskHandle);
}

/**
 * @brief AudioPlayer Pause method
 * @param  None
 * @retval None
 */
void AudioPlayer_Pause(void)
{
	/* Paramos el playback */
	if (AUDIO_Pause(AUDIO_I2C_ADDRESS) != 0)
	{
		Error_Handler();
	}
	/* Paramos la tranferencia por DMA */
	if (HAL_SAI_DMAPause(&hsai_BlockA1) != HAL_OK)
	{
		Error_Handler();
	}

	/* Esperamos por el qspiMutex */
	osMutexWait(qspiMutexHandle, osWaitForever);

	/* Pausamos la tarea */
	osThreadSuspend(audioTaskHandle);

	/* Soltamos el qspiMutex */
	osMutexRelease(qspiMutexHandle);

	/* Apagamos el LED Verde para indicar que ya no hay reproducción de audio */
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

	/* Cambiamos el estado del reproductor */
	AppState = AUDIOPLAYER_STATE_PAUSED;
}

/**
 * @brief AudioPlayer Stop method
 * @param  None
 * @retval None
 */
void AudioPlayer_Stop(void)
{
	/* Paramos el playback */
	if (AUDIO_Stop(AUDIO_I2C_ADDRESS, CODEC_PDWN_SW) != 0)
	{
		Error_Handler();
	}

	/* Paramos la tranferencia por DMA */
	if (HAL_SAI_DMAStop(&hsai_BlockA1) != HAL_OK)
	{
		Error_Handler();
	}

	/* Esperamos por el qspiMutex */
	osMutexWait(qspiMutexHandle, osWaitForever);

	/* Pausamos la tarea */
	osThreadSuspend(audioTaskHandle);

	/* Cerramos el archivo de audio */
	if(f_close(&File) != FR_OK)
	{
		Error_Handler();
	}

	/* Soltamos el qspiMutex */
	osMutexRelease(qspiMutexHandle);

	/* Apagamos el LED Verde para indicar que ya no hay reproducción de audio */
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

	AppState = AUDIOPLAYER_STATE_STOPPED;
}

/**
  * @brief  GPIO EXTI Callback function
  *         Handle USB VBUS detection upon External interrupt
  * @param  GPIO_Pin
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin) // Usamos las interrupciones para leer la entrada del DPAD
  {
	case JOY_CENTER_Pin:
		osMessagePut(joystickInputQueueHandle, JOY_CENTER_Pin, 0);
		break;
	case JOY_LEFT_Pin:
		osMessagePut(joystickInputQueueHandle, JOY_LEFT_Pin, 0);
		break;
	case JOY_RIGHT_Pin:
		osMessagePut(joystickInputQueueHandle, JOY_RIGHT_Pin, 0);
		break;
	case JOY_UP_Pin:
		osMessagePut(joystickInputQueueHandle, JOY_UP_Pin, 0);
		break;
	case JOY_DOWN_Pin:
		osMessagePut(joystickInputQueueHandle, JOY_DOWN_Pin, 0);
		break;
	default:
		Error_Handler();
		break;
  }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
