/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "spi.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  /* USER CODE BEGIN 2 */

  /*
  GPIO_init(GPIOA);

  GPIO_PinCfg_TypeDef pinCfg;
  pinCfg.MODE = GPIO_MODE_OUTPUT;
  pinCfg.OTYPE = GPIO_TYPE_PP;
  pinCfg.OSPEED = GPIO_SPEED_LOW;
  pinCfg.PUPD = GPIO_PUPD_NONE;
  pinCfg.AF = 0;
  GPIO_config(GPIOA, 7, &pinCfg);

  pinCfg.MODE = GPIO_MODE_INPUT;
  pinCfg.OTYPE = GPIO_TYPE_PP;
  pinCfg.OSPEED = GPIO_SPEED_LOW;
  pinCfg.PUPD = GPIO_PUPD_NONE;
  pinCfg.AF = 0;
  GPIO_config(GPIOA, 9, &pinCfg);
  */

  /* Config GPIO for SPI1 */
  GPIO_init(GPIOA);

  GPIO_PinCfg_TypeDef pinCfg;
  pinCfg.MODE = GPIO_MODE_ALTERNATE;
  pinCfg.OTYPE = GPIO_TYPE_PP;
  pinCfg.OSPEED = GPIO_SPEED_HIGH;
  pinCfg.PUPD = GPIO_PUPD_NONE;
  pinCfg.AF = 5;
  GPIO_config(GPIOA, GPIO_Pin5, &pinCfg); // PA5 - SPI1_SCLK
  GPIO_config(GPIOA, GPIO_Pin6, &pinCfg); // PA6 - SPI1_MISO
  GPIO_config(GPIOA, GPIO_Pin7, &pinCfg); // PA7 - SPI1_MOSI

  pinCfg.MODE = GPIO_MODE_OUTPUT;
  pinCfg.AF = 0;
  GPIO_config(GPIOA, GPIO_Pin8, &pinCfg); // SS

  /* Initialize and configure SPI1 */
  SPI_init(SPI1);

  SPI_Config_TypeDef spiCfg = {0};
  spiCfg.baud = SPI_CLKDIV_4;
  spiCfg.cpha = 1;
  spiCfg.cpol = 0;
  spiCfg.dataSize = SPI_DS_16;
  spiCfg.lsbFirst = 0;

  SPI_config(SPI1, &spiCfg);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /*
  uint32_t i = 0;
  GPIO_writePin(GPIOA, 7, GPIO_PIN_LOW);
  GPIO_writePin(GPIOA, 9, GPIO_PIN_HIGH);
  */

  SPI_enable(SPI1);
  GPIO_writePin(GPIOA, GPIO_Pin8, 1);
  uint8_t i = 0;
  uint8_t j = 0;
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0x8000));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0x8800));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0x9000));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0x9800));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0xA000));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0xA800));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0xB000));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	for(i = 0; i < 5; ++i);
	SPI_send16(SPI1, (uint16_t)(0xB800));
	for(i = 0; i < 5; ++i);
	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	/*
	j = 0;
	  for(j = 0; j < 2; ++j) {
		// Write all registers
		GPIO_writePin(GPIOA, GPIO_Pin8, 0);
		SPI_send16(SPI1, (uint16_t)(0b0001000010000000));
		// for(i = 0; i < 100; ++i);
		GPIO_writePin(GPIOA, GPIO_Pin8, 1);

		GPIO_writePin(GPIOA, GPIO_Pin8, 0);
		SPI_send16(SPI1, (uint16_t)(0b0001101100110011));
		// for(i = 0; i < 100; ++i);
		GPIO_writePin(GPIOA, GPIO_Pin8, 1);

		GPIO_writePin(GPIOA, GPIO_Pin8, 0);
		SPI_send16(SPI1, (uint16_t)(0b0010011100110011));
		// for(i = 0; i < 100; ++i);
		GPIO_writePin(GPIOA, GPIO_Pin8, 1);

		GPIO_writePin(GPIOA, GPIO_Pin8, 0);
		SPI_send16(SPI1, (uint16_t)(0b0010100101101101));
		// for(i = 0; i < 100; ++i);
		GPIO_writePin(GPIOA, GPIO_Pin8, 1);

		GPIO_writePin(GPIOA, GPIO_Pin8, 0);
		SPI_send16(SPI1, (uint16_t)(0b0011001010000011));
		// for(i = 0; i < 100; ++i);
		GPIO_writePin(GPIOA, GPIO_Pin8, 1);

		GPIO_writePin(GPIOA, GPIO_Pin8, 0);
		SPI_send16(SPI1, (uint16_t)(0b0011100000000000));
		// for(i = 0; i < 100; ++i);
		GPIO_writePin(GPIOA, GPIO_Pin8, 1);

		for(i = 0; i < 100; ++i);

	// Read all registers
	  GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1000000000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	  	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1000100000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	  	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1001000000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	  	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1001100000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	  	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1010000000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	  	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1010100000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	  	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1011000000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);

	  	GPIO_writePin(GPIOA, GPIO_Pin8, 0);
	  	SPI_send16(SPI1, (uint16_t)(0b1011100000000000));
	  	// for(i = 0; i < 100; ++i);
	  	GPIO_writePin(GPIOA, GPIO_Pin8, 1);
	  }
	*/

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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
