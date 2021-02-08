/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "lcd.h"
#include "quadspi.h"
#include "sai.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

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
//#include "ssd1306_tests.h"

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

/* USER CODE BEGIN PV */
// ***** Parámetros de la pantalla *****
const int width = 128;
const int height = 64;

// ***** Parámetros del menu *****
SSD1306_COLOR color = Black;	// Color de fondo del texto
int menu = 0;	// Indice del menu

// ***** Parámetros del juego *****
bool gameover;	// Bandera de Game Over
int x, y, score;	// Coordenadas de la pantalla y variable para los puntos
int fruitX[3], fruitY[3];	// Arrays con las coordenadas de las frutas
int snakeX[256], snakeY[256]; // Arrays con las coordenadas del cuerpo de la serpiente
int nTail;	// Número de pixeles que forman la cola de la serpiente
int state, prev_state;	// Estado en el que nos encontramos y el anterior a este

// ***** Variables del sistema de archivos Fat y del USB *****
FIL                     File;						// Handle del archivo
FATFS                   FatFs;						// Handle del sistema de archivos

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

// ***** Funciones de Audio *****
void AudioPlayer_Start(const TCHAR* file_name, bool bg_music, bool aflag);

// ***** Funciones del Juego *****
void Setup();		// Función para establecer las condiciones iniciales
void DrawOled();	// Función para dibujar la pantalla oled
void Algoritmo();	// Función que controla la ejecución del juego
void Fruta();		// Función para crear las frutas en la pantalla
void GameOver();	// Función para el estado de Game Over
void Leaderboard();	// Función para mostrar las mejores puntuaciones conseguidas
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_LCD_Init();
  MX_QUADSPI_Init();
  MX_SAI1_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  // Inicializamos los módulos BSP
  //BSP_COMPASS_Init();
  BSP_GYRO_Init();
  BSP_LCD_GLASS_Init();

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 
  /* Start scheduler */
  //osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // Mostramos la pantalla de inicio
	ssd1306_Init();	// Iniciamos la comunicación con la pantalla oled
	ssd1306_Fill(Black);	// Dibujamos toda la pantalla de negro
	ssd1306_SetCursor(24, 4);	// Situamos el cursor en las coordenadas indicadas
	ssd1306_WriteString("SNAKE", Font_16x26, White);	// Escribimos el string con la fuente
	ssd1306_SetCursor(24, 34);						 	// y el color indicado
	ssd1306_WriteString("START", Font_16x26, White);	// Repetimos para cada string
	ssd1306_UpdateScreen();	// Actualizamos la pantalla con lo que hemos dibujado

	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		// ***** Pantalla de Inicio *****
		if (menu == 0)
		{
			for(int i = 0; i < 100; i++)	// Realizamos una espera de 500 ms
			{
				JOYState_TypeDef JoyState = BSP_JOY_GetState();	// Obetenemos la entrada del DPAD

				if(JoyState == JOY_SEL){	// Si el usuario pulsa el botón central mostramos el menú
					menu = 1;	// Cambiamos el índice del menú para indicar que hemos entrado
					ssd1306_Fill(Black);	// Y dibujamos el menú de inicio
					ssd1306_SetCursor(42, 1);
					ssd1306_WriteString("MENU", Font_11x18, White);
					ssd1306_SetCursor(13, 20);
					ssd1306_WriteString("DPAD", Font_11x18, White);
					ssd1306_SetCursor(70, 20);
					ssd1306_WriteString("GYRO", Font_11x18, White);
					ssd1306_SetCursor(31, 40);
					ssd1306_WriteString("SCORES", Font_11x18, White);
					ssd1306_UpdateScreen();
					HAL_Delay(100);
					break;
				}
				HAL_Delay(5);
			}

			if(menu == 0)	// Cada 500 ms cambiamos el color de fondo del botón START
			{
				ssd1306_SetCursor(24, 34);
				ssd1306_WriteString("START", Font_16x26, color);
				ssd1306_UpdateScreen();
			}

			if (color == Black)	// Con este IF intercambiamos el color de fondo
				color = White;	// del botón de START para crear el efecto de intermitencia
			else
				color = Black;
		}
		else // ***** Menu principal *****
		{
			JOYState_TypeDef JoyState = BSP_JOY_GetState();

			switch(JoyState) {	// Leemos el valor de entrada del DPAD y cambiamos
			case JOY_LEFT:		// la selección del menú de acuerdo a eso
				if(menu == 2)
					menu = 1;
				break;
			case JOY_RIGHT:
				if(menu == 1)
					menu = 2;
				break;
			case JOY_UP:
				if(menu == 3)
					menu = 1;
				break;
			case JOY_DOWN:
				if(menu == 1 || menu == 2)
					menu = 3;
				break;
			case JOY_SEL:
				if(menu == 1)			// Si estamos en la opción 1 ejecutamos el juego usando
				{						// como entrada el DPAD para jugar
					HAL_Delay(100);
					Setup();
					DrawOled();			// Dibujamos la pantalla de juego
					osKernelStart();	// e iniciamos el planificador
				}
				else if(menu == 2)		// Si estamos en la opción 2 ejecutamos el juego usando
				{						// como entrada el Giroscopio o el Acelerómetro para jugar
					HAL_Delay(100);
					Setup();
					DrawOled();			// Dibujamos la pantalla de juego
					osKernelStart();	// e iniciamos el planificador
				}
				else if(menu == 3)		// Si estamos en la opción 3 ejecutamos la función para
				{						// mostrar las mejores puntuaciones que estén guardadas
					HAL_Delay(100);
					Leaderboard();
				}
				break;
			default:	// Si no hay entrada, no ejecutamos nada
				break;
			}

			if(menu == 1)	// Mostramos la opción de DPAD como seleccionada
			{
				ssd1306_Fill(Black);
				ssd1306_SetCursor(42, 1);
				ssd1306_WriteString("MENU", Font_11x18, White);
				ssd1306_SetCursor(13, 20);
				ssd1306_WriteString("DPAD", Font_11x18, Black);
				ssd1306_SetCursor(70, 20);
				ssd1306_WriteString("GYRO", Font_11x18, White);
				ssd1306_SetCursor(31, 40);
				ssd1306_WriteString("SCORES", Font_11x18, White);
				ssd1306_UpdateScreen();
			}
			else if(menu == 2)	// Mostramos la opción de GYRO como seleccionada
			{
				ssd1306_Fill(Black);
				ssd1306_SetCursor(42, 1);
				ssd1306_WriteString("MENU", Font_11x18, White);
				ssd1306_SetCursor(13, 20);
				ssd1306_WriteString("DPAD", Font_11x18, White);
				ssd1306_SetCursor(70, 20);
				ssd1306_WriteString("GYRO", Font_11x18, Black);
				ssd1306_SetCursor(31, 40);
				ssd1306_WriteString("SCORES", Font_11x18, White);
				ssd1306_UpdateScreen();
			}
			else if(menu == 3)	// Mostramos la opción de SCORES como seleccionada
			{
				ssd1306_Fill(Black);
				ssd1306_SetCursor(42, 1);
				ssd1306_WriteString("MENU", Font_11x18, White);
				ssd1306_SetCursor(13, 20);
				ssd1306_WriteString("DPAD", Font_11x18, White);
				ssd1306_SetCursor(70, 20);
				ssd1306_WriteString("GYRO", Font_11x18, White);
				ssd1306_SetCursor(31, 40);
				ssd1306_WriteString("SCORES", Font_11x18, Black);
				ssd1306_UpdateScreen();
			}
		}

		// Comprobamos si se ha conectado el USB, y en caso de que sí iniciamos el scheduler
		if ((HAL_GPIO_ReadPin(USB_VBUS_GPIO_Port, USB_VBUS_Pin) != GPIO_PIN_RESET) && menu == 0)
		{
			osKernelStart();
		}

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_SAI1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_USB;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 48;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV17;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV4;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK|RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration 
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */
void Setup()	//  Función de inicialización de las variables del juego
{
	char string[100];

	// Variables
	gameover = false;
	state = 0;
	prev_state = 0;
	x = width / 2;
	y = height / 2;
	nTail = 0;

	// Reseteamos la puntuación
	score = 0;
	sprintf(string, "%d", score);
	BSP_LCD_GLASS_DisplayString((uint8_t *)string);

	// Reseteamos las coordenadas de las frutas
	fruitX[0] = 0; fruitY[0] = 0;
	fruitX[1] = 0; fruitY[1] = 0;
	fruitX[2] = 0; fruitY[2] = 0;
	Fruta();	// Generamos 3 nuevas frutas
}

void DrawOled(){
	ssd1306_Fill(Black);	// Limpiamos la pantalla

	// Dibujamos los muros
	for (int i = 0; i < width ; i++){
		for (int j = 0; j < height; j++)
		{
			if (i == 0)
				ssd1306_DrawPixel(i, j, White);
			else if (i == width - 1)
				ssd1306_DrawPixel(i, j, White);
			else if (j == 0)
				ssd1306_DrawPixel(i, j, White);
			else if (j == height - 1)
				ssd1306_DrawPixel(i, j, White);
		}
	}

	// Dibujamos las frutas y la serpiente
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i == x && j == y)
				ssd1306_DrawPixel(i, j, White); // Cola
			else if ((i == fruitX[0] && j == fruitY[0]) || (i == fruitX[1] && j == fruitY[1]) || (i == fruitX[2] && j == fruitY[2]))
				ssd1306_DrawPixel(i, j, White); // Coincide con alguna de las frutas
			else
			{
				for (int k = 0; k < nTail; k++)
				{
					if (snakeX[k] == i && snakeY[k] == j)
					{
						ssd1306_DrawPixel(i, j, White);
					}
				}
			}
		}
	}

	// Juego en pausa cuando se pulsa el botón central
	if (state == 5 && prev_state!=0 && gameover==false)
	{
		ssd1306_SetCursor(36, 22);
		ssd1306_WriteString("PAUSA", Font_11x18, White);
	}

	ssd1306_UpdateScreen();
}

void Algoritmo(){	// Función principal que define el funcionamiento
	int prevX, prevY, prev2X, prev2Y;
	char string[100];

	// Actualizamos la cola
	if (state != 5)
	{
		prevX = snakeX[0];
		prevY = snakeY[0];
		snakeX[0] = x;
		snakeY[0] = y;

		for (int i = 1; i < nTail; i++)
		{
			prev2X = snakeX[i];
			prev2Y = snakeY[i];
			snakeX[i] = prevX;
			snakeY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}
	}

	// Izquierda - Variación en el eje X
	if(state == 1){
		x -= 1;
		prev_state = 1;
	}//Derecha
	else if(state == 2){
		x += 1;
		prev_state = 2;
	}// Arriba - Variación en el eje Y
	else if(state == 3){
		y -= 1;
		prev_state = 3;
	}// Abajo
	else if(state == 4){
		y += 1;
		prev_state = 4;
	}

	// Situaciones de Game Over
	if (x >= (width-1)) // Chocarse con las paredes laterales
		gameover = true;
	else if (x <= 0)
		gameover = true;

	if (y >= (height-1)) // Chocarse con la pared superior o inferior
		gameover = true;
	else if (y <= 0)
		gameover = true;

	for (int i = 0; i < nTail; i++) // Choca con su propia cola
	{
		if (snakeX[i] == x && snakeY[i] == y)
			gameover = true;
	}

	// Comprobamos si se ha comido una fruta
	for (int i = 0; i < 3; i++)
	{
		if (x == fruitX[i] && y == fruitY[i])
		{
			AudioPlayer_Start("fruit.wav", 0, 1);	// Reproducimos el audio asignada a las frutas
			score += 10;	// Sumamos 10 puntos a la puntuación
			sprintf(string, "%d", score);
			BSP_LCD_GLASS_DisplayString((uint8_t *)string);	// Actualizamos la puntuación en el LCD
			Fruta();	// Generamos una nueva fruta
			nTail++;	// Aumentamos longitud de la cola
		}
	}
}

void Fruta(){	// Genera una fruta en una posición aleatoria

	for( int i=0; i < 3; i++)	// Comprobamos cuantas frutas faltan y si se encuentran dentro de los límites
	{
		if(fruitX[i] <= 0 || fruitY[i] <= 0 || fruitX[i] >= height || fruitX[i] >= width || (fruitX[i] == x && fruitY[i] == y)){
			fruitX[i] = (rand()%width);
			fruitY[i] = (rand()%height);
			i--;
		}
	}
}

void GameOver(){		// Pantalla de GameOver

	char string[100];
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";	// String para obtener las letras en función de su posición
	int char_sel[3] = { 0, 0, 0};	// Variable para guardar la selección de letras
	int p = 0;

	FIL File;	// Establecemos la variable para abrir el archivo
	uint br;

	char players[80];	// String para cargar las puntuaciones guardadas
	char player[7];		// String del jugador actual
	char buffer[7];
	int ctrl = 0;

	color = Black;		// Declaramos el color de la fuente de texto como negro

	// Dibujamos la pantalla principal de Game Over
	ssd1306_Fill(Black);
	ssd1306_SetCursor(14, 1);
	ssd1306_WriteString("GAME OVER", Font_11x18, White);
	ssd1306_SetCursor(2, 22);
	ssd1306_WriteString("Name:", Font_11x18, White);
	ssd1306_SetCursor(74, 22);
	ssd1306_WriteString("A", Font_11x18, White);
	ssd1306_SetCursor(87, 22);
	ssd1306_WriteString("A", Font_11x18, White);
	ssd1306_SetCursor(100, 22);
	ssd1306_WriteString("A", Font_11x18, White);
	ssd1306_SetCursor(2, 42);
	sprintf(string, "Score: %d", score);
	ssd1306_WriteString(string, Font_11x18, White);
	ssd1306_UpdateScreen();

	// Introducción de las iniciales del jugador
	while(1)
	{
		for (int t=0; t<5; t++)
		{
			JOYState_TypeDef JoyState = BSP_JOY_GetState();

			switch(JoyState) {	// Leemos la entrada del DPAD y aumentamos o disminuimos
				case JOY_LEFT:	// el índice de cada de las letras
					if(p > 0)
						p--;
					break;
				case JOY_RIGHT:
					if(p < 2)
						p++;
					break;
				case JOY_UP:
					if(char_sel[p] < 25)
						char_sel[p]++;
					break;
				case JOY_DOWN:
					if(char_sel[p] > 0)
						char_sel[p]--;
					break;
				case JOY_SEL:	// Si el usuario presiona el boton central, comprobamos la puntuación

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

					/* FatFS read */
					if (f_open(&File, "score.txt", FA_READ) == FR_OK) { // Abrimos el archivo de puntuaciones
					  f_read(&File, players, 72, &br);	// Leemos el contenido y lo guardamos en el buffer
					  f_close(&File);
					}

					// Corregimos el string si es menor de 100, ya que tiene un dígito menos
					if (score >= 100)
						sprintf(player, "%c%c%c %d", alphabet[char_sel[0]], alphabet[char_sel[1]], alphabet[char_sel[2]], score);
					else
						sprintf(player, "%c%c%c  %d", alphabet[char_sel[0]], alphabet[char_sel[1]], alphabet[char_sel[2]], score);

					/* FatFS file write - Escribimos las puntuaciones en el archivo de nuevo */
					if (f_open(&File, "score.txt", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {

						// Este algoritmo comprobará la puntuación obtenida con las guardadas
						// y en el caso de que sea más alta la añadirá a la lista
						for(int i=0; i<8; i++){
							for(int j=0; j<7; j++){
								buffer[j] = players[j+(i-ctrl)*9];
							}

							// Comprobamos si la puntuación es más alta
							if ((player[4] > buffer[4]) && ctrl==0)
							{
								for(int k=0; k<7; k++)
									buffer[k] = player[k];
								ctrl = 1;
							}
							else if ((player[4] == buffer[4]) && ctrl==0)
							{
								if(player[5] > buffer[5])
								{
									for(int k=0; k<7; k++)
										buffer[k] = player[k];
									ctrl = 1;
								}
								else if (player[5] == buffer[5])
								{
									if(player[6] >= buffer[6])
									{
										for(int k=0; k<7; k++)
											buffer[k] = player[k];
										ctrl = 1;
									}
								}
							}

							// Añadimos al jugador
							f_printf(&File, buffer);
							f_printf(&File, "\n");
						}

						f_close(&File);
					}

					// Reiniciamos a la pantalla de inicio, como en las máquinas arcade antiguas
					NVIC_SystemReset();
					break;
				default:
					break;
			}

			// Impresión en pantalla de las iniciales
			for (int i=0; i<3; i++)
			{
				char letter = alphabet[char_sel[i]];

				if (i == p)
				{
					ssd1306_SetCursor(74+i*13, 22);
					ssd1306_WriteString(&letter, Font_11x18, color);
				}
				else
				{
					ssd1306_SetCursor(74+i*13, 22);
					ssd1306_WriteString(&letter, Font_11x18, White);
				}
			}

			ssd1306_UpdateScreen();
			HAL_Delay(50);
		}

		// Rotación de color de las letras
		if (color == Black)
			color = White;
		else
			color = Black;
	}
}

void Leaderboard(){	// Función para mostrar las puntuaciones guardadas
	char players[80];
	char buffer[10];

	// Variables para el control del dibujado en la pantalla
	int uix = 7;
	int uiy = 21;
	int ctrl = 0;

	// Variables del archivo de puntuaciones
	FIL File;
	FATFS FatFs;
	uint br;

	/* init code for USB_DEVICE - Iniciamos el periférico USB */
	MX_USB_DEVICE_Init();

	/* Register the file system object to the FatFs module */
	if (f_mount(&FatFs, (TCHAR const*) USERPath, 1) != FR_OK) {
	  /* FatFs Initialization Error */
	  if (f_mkfs((TCHAR const*)USERPath, 0, 128, NULL, 0) != FR_OK) {
		  Error_Handler();
	  } else {
		  /* Second trial to register the file system object */
		  if (f_mount(&FatFs, (TCHAR const*) USERPath, 1) != FR_OK) {
			  Error_Handler();
		  }
	  }
	}

	/* FatFS read - Leemos el archivo de puntuaciones*/
	if (f_open(&File, "score.txt", FA_READ) == FR_OK) {
	  f_read(&File, players, 72, &br);
	  puts(players);
	  f_close(&File);
	}

	// Mostramos las puntuaciones guardadas
	ssd1306_Fill(Black);
	ssd1306_SetCursor(3, 1);
	ssd1306_WriteString("BEST SCORES", Font_11x18, White);

	for(int i=0; i<8; i++){
		for(int j=0; j<7; j++){
			buffer[j] = players[j+(i-ctrl)*9];
		}

		// Imprimimos la puntuación
		ssd1306_SetCursor(uix, uiy);
		ssd1306_WriteString(buffer, Font_7x10, White);

		// Variamos la posición del cursor
		if (i<3)
			uix = 7;
		else
			uix = 63;

		if (uiy <51)
			uiy = uiy+10;
		else
			uiy = 21;
	}

	ssd1306_UpdateScreen();

	// Si presionamos el botón central volvemos al menu
	while(1)
	{
		JOYState_TypeDef JoyState = BSP_JOY_GetState();
		if(JoyState == JOY_SEL){
			HAL_Delay(100);
			return;
		}
	}
}

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
