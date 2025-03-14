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
uint8_t send_data[50];
uint8_t timecnt = 0;
uint8_t timeout = 0;
uint8_t isWorking = 0; // 检测系统是否正常运行
uint8_t hascache = NOCACHE;
extern uint8_t IsConnect;
#include "string.h"
#include "esp8266.h"
#include "spi.h"
#include "rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
RTC_DateTypeDef GetData; // 获取日期结构体

RTC_TimeTypeDef GetTime; // 获取时间结构体

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

void TXRT_Swap(void)
{
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == TX_STATUS)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, RX_STATUS);
  else
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, TX_STATUS);
}
void make_send_data(uint8_t index)
{
  send_data[14 + index * 18] = (buf[0] / 10) + '0';
  send_data[15 + index * 18] = (buf[0] % 10) + '0';

  send_data[20 + index * 18] = (buf[1] / 10) + '0';
  send_data[21 + index * 18] = (buf[1] % 10) + '0';

  send_data[26 + index * 18] = (buf[2] / 10) + '0';
  send_data[27 + index * 18] = (buf[2] % 10) + '0';
}
void make_send_data_time(void)
{
  HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);
  send_data[0] = '2';
  send_data[1] = '0';
  send_data[2] = (GetData.Year / 10) + '0';
  send_data[3] = (GetData.Year % 10) + '0';
  send_data[4] = (GetData.Month / 10) + '0';
  send_data[5] = (GetData.Month % 10) + '0';
  send_data[6] = (GetData.Date / 10) + '0';
  send_data[7] = (GetData.Date % 10) + '0';
  send_data[8] = (GetTime.Hours / 10) + '0';
  send_data[9] = (GetTime.Hours % 10) + '0';
  send_data[10] = (GetTime.Minutes / 10) + '0';
  send_data[11] = (GetTime.Minutes % 10) + '0';
  send_data[12] = (GetTime.Seconds / 10) + '0';
  send_data[13] = (GetTime.Seconds % 10) + '0';
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
  MX_RTC_Init();
  MX_SPI1_Init();
  if (esp8266_init())
    printf("esp8266 init ok\r\n");
  else
  {
    printf("esp8266 init error\r\n");
    RTC_settime(); // 从1号00：00：00开始计时
  }
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // test resetpin begin测试成功
  //  uint8_t tempbuf[50];
  //  while(1){
  //    HAL_UARTEx_ReceiveToIdle_IT(&huart2, tempbuf, 50);
  //    if(begin){
  //      esp8266_reset();
  //      begin = 0;
  //    }
  //    HAL_Delay(100);

  // }

  // test resetpin end

  // test ontlinecheck begin

  // while(1){
  //   esp8266_checkonline();
  // }

  // test ontlinecheck end

  HAL_Delay(200);
  // test begin
  memset(send_data, 0x20, 50); // resset

  // rtc test begin测试成功
  // RTC_settime();
  // printf("hello world\n");
  // while (1)
  // {

  //   HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
  //   /* Get the RTC current Date */
  //   HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);

  //   /* Display date Format : yy/mm/dd */
  //   printf("%02d/%02d/%02d\r\n", 2000 + GetData.Year, GetData.Month, GetData.Date);
  //   /* Display time Format : hh:mm:ss */
  //   printf("%02d:%02d:%02d\r\n", GetTime.Hours, GetTime.Minutes, GetTime.Seconds);

  //   printf("\r\n");

  //   HAL_Delay(1000);
  // }
  // rtc test end
  uint8_t mid;
  uint16_t did;
  readID(&mid, &did);
  printf("helloworld\n");
  printf("mid: %#x\n", mid);
  printf("did: %#x\n", did);
  char datastr[50] = "2022112720303036.78965.687345.0036.78965.687345.00";

  printf("%s\r\n", datastr);
  // 3. 段擦除

  //W25Q32_EraseSector(0, 0);
  W25Q32_EraseChip();//一次擦除整一片
  //  for (uint8_t i = 0; i < 10; i++)
  //  {
  //    add_data((uint8_t *)datastr);
  //  }
  // 4. 页写
  //  // W25Q32_PageWrite(0, 0, 0, "12345678", 8);

  // //5. 读取
  // uint8_t buffer[50] = {0};
  // for (uint8_t i = 0; i < 5; i++)
  // {
  //   W25Q32_Read(0, 0, 0, i * 50, buffer, 50);
  //   printf("Read %d: %s\n", i, buffer);
  // }
  // for (uint8_t i = 0; i < 5; i++)
  // {
  //   W25Q32_Read(0, 0, 1, i * 50, buffer, 50);
  //   printf("Read %d: %s\n", i + 5, buffer);
  // }
  // W25Q32_Read(0, 0, 0, 0, buffer, 8);

  // // printf("buffer = %s\n", buffer);
  // while (1)
  // {
  //   //gpio test begin

  //   //gpio test end

  // }
  // test end

  // 发送从机1
  HAL_UART_Transmit(&huart1, "2", 1, 1000);
  TXRT_Swap(); // 切换为接收
  HAL_TIM_Base_Start_IT(&htim2);
  while (1)
  {
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, buf, 1000);
    if (begin)
    {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      // test begin
      printf("temp humi:%d %d %d\r\n", buf[0], buf[1], buf[2]);
      make_send_data(0);
      // test end
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
  TXRT_Swap(); // 切换为发送
  HAL_Delay(200);
  // 发送从机2
  HAL_UART_Transmit(&huart1, "1", 1, 1000);
  HAL_TIM_Base_Start_IT(&htim2);
  TXRT_Swap(); // 切换为接收
  while (1)
  {

    HAL_UARTEx_ReceiveToIdle_IT(&huart1, buf, 1000);
    if (begin)
    {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

      // test begin
      printf("temp humi:%d %d %d\r\n", buf[0], buf[1], buf[2]);
      make_send_data(1);
      // test end
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
  TXRT_Swap(); // 切换为发送

  // test begin

  for (uint8_t i = 0; i < 50; i++)
  {
    printf("%d:%c\r\n", i, send_data[i]);
  }
  espPrintf((char *)send_data);

  // test end
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
    // 发送从机1
    HAL_UART_Transmit(&huart1, "2", 1, 1000);
    TXRT_Swap(); // 切换为接收
    HAL_TIM_Base_Start_IT(&htim2);
    while (1)
    {
      HAL_UARTEx_ReceiveToIdle_IT(&huart1, buf, 1000);
      if (begin)
      {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        // test begin
        printf("temp humi:%d %d %d\r\n", buf[0], buf[1], buf[2]);
        make_send_data(0);
        // test end
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
    TXRT_Swap(); // 切换为发送
    HAL_Delay(1000);
    // 发送从机2
    HAL_UART_Transmit(&huart1, "1", 1, 1000);
    HAL_TIM_Base_Start_IT(&htim2);
    TXRT_Swap(); // 切换为接收
    while (1)
    {

      HAL_UARTEx_ReceiveToIdle_IT(&huart1, buf, 1000);
      if (begin)
      {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        // test begin
        printf("temp humi:%d %d %d\r\n", buf[0], buf[1], buf[2]);
        make_send_data(1);
        // test end
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
    TXRT_Swap(); // 切换为发送

    //组成数据
    make_send_data_time();

    //发送数据
    if (!IsConnect)
    {
      printf("add\r\n");
      printf((char *)send_data);
      hascache = HASCACHE;
      add_data(send_data); // 无连接的情况下储存到存储器
      esp8266_Reconnect();
    }
    else{
      espPrintf((char *)send_data); // 连接发送
      esp8266_checkonline();                  // 每五分钟检测一次是否在线
    }

    // test begin
    // for (uint8_t i = 0; i < 50; i++)
    // {
    //   printf("%d:%c\r\n", i, send_data[i]);
    // }
    // test end

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12); // 系统是否正常运行
    
    HAL_Delay(5000);
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

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
