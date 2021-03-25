/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "oled.h"
#include "drv\commDrv\Re_commDrv.h"
#include "service\input\keypad\Re_keypad.h"
#include "as608.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t input[20], password[20];
uint8_t Length = 0, flags = 0;
char demo[] = "---------------;";
char sys[20] = {0};
//dsda
SysPara p = {0};
uint8_t read_ans=0;
__IO uint8_t readSta=0;
uint8_t status_step=0;
uint16_t pageID;
uint16_t score;

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
void callback(unsigned char x, unsigned char y)
{
  switch (x)
  {
  case 0:
    switch (y)
    {
    case 0:
      input[Length] = 0;
      sys[Length] = '*';
      Length++;
      break;
    case 1:
      input[Length] = 1;
      sys[Length] = '*';
      Length++;
      break;
    case 2:
      input[Length] = 2;
      sys[Length] = '*';
      Length++;
      break;
    case 3:
      input[Length] = 3;
      sys[Length] = '*';
      Length++;
      break;
    }
    break;
  case 1:
    switch (y)
    {
    case 0:
      input[Length] = 4;
      sys[Length] = '*';
      Length++;
      break;
    case 1:
      input[Length] = 5;
      sys[Length] = '*';
      Length++;
      break;
    case 2:
      input[Length] = 6;
      sys[Length] = '*';
      Length++;
      break;
    case 3:
      input[Length] = 7;
      sys[Length] = '*';
      Length++;
      break;
    }
    break;
  case 2:
    switch (y)
    {
    case 0:
      input[Length] = 8;
      sys[Length] = '*';
      Length++;
      break;
    case 1:
      input[Length] = 9;
      sys[Length] = '*';
      Length++;
      break;
    }
    break;
  case 3:
    switch (y)
    {
    case 3:
      verification();
      break;
    }
    break;
  }
}
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

  /* USER CODE BEGIN SysInit */*

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //keypad_init(Pullup, 100, callback);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 8);
  OLED_Init();
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
  //PS_BetterEnroll(3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
   	  	if (readSta)
	     {
	  		if(PS_GetImage()) {printf("no\r\n");}
	     	PS_GenChar(1);
	     	PS_Search(1,0,15,&pageID,&score);
	     	if(score >=60)
	     	{
	     		flags = 1;
	     	}
	     	else
	     	{
	     		flags = 2;
	     	}
	     }

	    if (flags == 1)
	    {
	      rightdoing();
	      __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,17);
	      myDelayMS(4000);
	      __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,8);
	      myDelayMS(1000);
	      flags = 0;
	      Length = 0;
	      memset(password, 0, sizeof(password));
	      memset(input, 0, sizeof(input));
	      memcpy(sys, demo, 15);
	      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
	    }
	    else if(flags == 2)
	    {
	      wrongdoing();
	      //Menu_display();
	      flags = 0;
	      Length = 0;
	      memset(password, 0, sizeof(password));
	      memset(input, 0, sizeof(input));
	      memcpy(sys, demo, 15);
	      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
	    }
	    else
	    {
	    	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void Menu_display()
{
  OLED_ShowCHinese(0, 0, 0);   //??
  OLED_ShowCHinese(18, 0, 1);  //??
  OLED_ShowCHinese(36, 0, 2);  //??
  OLED_ShowCHinese(54, 0, 3);  //??
  OLED_ShowCHinese(72, 0, 4);  //??
  OLED_ShowCHinese(90, 0, 5);  //??
  OLED_ShowCHinese(108, 0, 6); //??year
  OLED_Clear();
  myDelayMS(500);
//  HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
//  HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);
//  OLED_ShowNum(0, 3, GetData.Year + 2021, 4, 12);
//  OLED_ShowString(25, 3, "-", 12);
//  OLED_ShowNum(30, 3, GetData.Month, 2, 12);
//  OLED_ShowString(45, 3, "-", 12);
//  OLED_ShowNum(50, 3, GetData.Date, 2, 12);
//  OLED_ShowNum(70, 3, GetTime.Hours, 2, 12);
//  OLED_ShowString(85, 3, ":", 12);
//  OLED_ShowNum(90, 3, GetTime.Minutes, 2, 12);
//  OLED_ShowString(105, 3, ":", 12);
//  OLED_ShowNum(110, 3, GetTime.Seconds, 2, 12);
//  OLED_ShowString(6, 5, "Enter Password", 12);
//  OLED_ShowString(0, 7, sys, 12);
}
void wrongdoing()
{
  OLED_Clear();
  OLED_ShowCHinese(0, 0, 0);   //??
  OLED_ShowCHinese(18, 0, 1);  //??
  OLED_ShowCHinese(36, 0, 2);  //??
  OLED_ShowCHinese(54, 0, 3);  //??
  OLED_ShowCHinese(72, 0, 4);  //??
  OLED_ShowCHinese(90, 0, 5);  //??
  OLED_ShowCHinese(108, 0, 6); //??year
  OLED_ShowCHinese(24, 4, 7);
  OLED_ShowCHinese(42, 4, 8);
  OLED_ShowCHinese(60, 4, 9);
  OLED_ShowCHinese(78, 4, 10);
  myDelayMS(800);
  OLED_Clear();
  myDelayMS(20);
}

void rightdoing()
{
  OLED_Clear();
  OLED_ShowCHinese(0, 0, 0);   //??
  OLED_ShowCHinese(18, 0, 1);  //??
  OLED_ShowCHinese(36, 0, 2);  //??
  OLED_ShowCHinese(54, 0, 3);  //??
  OLED_ShowCHinese(72, 0, 4);  //??
  OLED_ShowCHinese(90, 0, 5);  //??
  OLED_ShowCHinese(108, 0, 6); //??year
  OLED_ShowCHinese(24, 4, 11);
  OLED_ShowCHinese(42, 4, 12);
  OLED_ShowCHinese(60, 4, 13);
  OLED_ShowCHinese(78, 4, 14);
  myDelayMS(1000);
  OLED_Clear();
  myDelayMS(20);
}
void verification()
{
  if (Length != 8)
  {
    flags = 2;
  }
//  password[0] = GetTime.Hours / 10;
//  password[1] = GetTime.Hours % 10;
//  password[2] = GetTime.Minutes / 10;
//  password[3] = GetTime.Minutes % 10;
  password[4] = 6;
  password[5] = 0;
  password[6] = 1;
  password[7] = 1;
  for (int i = 0; i < Length; i++)
  {
    if (password[i] != input[i])
    {
      flags = 2;
    }
  }
  if (flags != 2)
  {
    flags = 1;
  }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == AS608_Pin)
	{
		readSta = 1;
		SystemClock_Config();
		__HAL_GPIO_EXTI_CLEAR_IT(AS608_Pin);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
