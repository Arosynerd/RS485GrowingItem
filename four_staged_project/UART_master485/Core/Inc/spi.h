/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */
#define SPI1_CS_LOW() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)
#define SPI1_CS_HIGH() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)
/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */
uint8_t SPI_SwapByte(uint8_t byte);
void readID(uint8_t *mid, uint16_t *did);
// 开启写使能
void W25Q32_WriteEnable(void);

// 关闭写使能
void W25Q32_WriteDisable(void);

// 等待状态不为忙（busy）
void W25Q32_WaitNotBusy(void);

// 擦除段（sector erase）,地址只需要块号和段号
void W25Q32_EraseSector(uint8_t block, uint8_t sector);
//擦除整片芯片
void W25Q32_EraseChip(void);

// 写入（页写）
void W25Q32_PageWrite(uint8_t block, uint8_t sector, uint8_t page,uint8_t innerAddr, uint8_t * data, uint16_t len);

// 读取
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t * buffer, uint16_t len);
/* USER CODE END Prototypes */
void convert_address_to_w25q32(uint32_t addr, uint8_t *block, uint8_t *sector, uint8_t *page, uint8_t *offset);

void add_data(uint8_t *data);
#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

