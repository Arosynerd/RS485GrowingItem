/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
uint8_t begin = 0;
uint16_t len = 0;
uint8_t buf[100];
uint8_t timecnt = 0;
uint8_t timeout = 0;
#include "string.h"
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
void TXRT_Swap(void){
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == TX_STATUS)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, RX_STATUS); 
  else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, TX_STATUS); 
}
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(200);

  // 发送从机1
  HAL_UART_Transmit(&huart1, "2", 1, 1000);
  TXRT_Swap();// 切换为接收
  HAL_TIM_Base_Start_IT(&htim2);
  while (1)
  {
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, buf, 1000);
    if (begin)
    {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      //test begin
     
      //test end
      printf("%s\r\n", buf);
      begin = 0;
      break;
    }
    else
    {
      if (timeout == 1)
      {
        timeout = 0;
        printf("timeout\r\n");
        break;
      }
    }
  }
  timecnt = 0;
  // 关闭定时器
  HAL_TIM_Base_Stop_IT(&htim2);
  TXRT_Swap();// 切换为发送
  HAL_Delay(200);
  // 发送从机2
  HAL_UART_Transmit(&huart1, "1", 1, 1000);
  HAL_TIM_Base_Start_IT(&htim2);
  TXRT_Swap();// 切换为接收
  while (1)
  {
    
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, buf, 1000);
    if (begin)
    {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      printf("%s\r\n", buf);
      //test begin
      printf("test\r\n");
      printf("len is %d\r\n", len);
      if(len == 2)printf("humi temp is %d %d\r\n", buf[0], buf[1]);
      //test end
      begin = 0;
      break;
    }
    else
    {
      if (timeout == 1)
      {
        timeout = 0;
        printf("timeout\r\n");
        break;
      }
    }
  }
  timecnt = 0;
  // 关闭定时器
  HAL_TIM_Base_Stop_IT(&htim2);
  TXRT_Swap();// 切换为发送

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
     HAL_UARTEx_ReceiveToIdle_IT(&huart1, buf, 1000);
    if (begin)
    {
      // HAL_UART_Transmit(&huart2, buf, len, 1000);
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      printf("%s\r\n", buf);
      begin = 0;
    }
    // HAL_UART_Transmit(&huart1, "1", 1, 1000);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    /* USER CODE BEGIN 3 */
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

#ifdef USE_FULL_ASSERT
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
