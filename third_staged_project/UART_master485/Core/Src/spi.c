/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    spi.c
 * @brief   This file provides code for the configuration
 *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */
uint32_t written_pointer = 0;
/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (spiHandle->Instance == SPI1)
  {
    /* USER CODE BEGIN SPI1_MspInit 0 */

    /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN SPI1_MspInit 1 */

    /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

  if (spiHandle->Instance == SPI1)
  {
    /* USER CODE BEGIN SPI1_MspDeInit 0 */

    /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    /* USER CODE BEGIN SPI1_MspDeInit 1 */

    /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint8_t SPI_SwapByte(uint8_t byte)
{
  uint8_t result;
  HAL_SPI_TransmitReceive(&hspi1, &byte, &result, 1, 1000);
  return result;
}
void readID(uint8_t *mid, uint16_t *did)
{
  SPI1_CS_LOW();
  SPI_SwapByte(0x9F);
  *mid = SPI_SwapByte(0x9f);
  *did = 0;
  *did |= SPI_SwapByte(0xff) << 8;
  *did |= SPI_SwapByte(0xff);
  SPI1_CS_HIGH();
}
// 开启写使能
void W25Q32_WriteEnable(void)
{
  SPI1_CS_LOW();
  SPI_SwapByte(0x06);
  SPI1_CS_HIGH();
}

// 关闭写使能
void W25Q32_WriteDisable(void)
{
  SPI1_CS_LOW();
  SPI_SwapByte(0x04);
  SPI1_CS_HIGH();
}

// 等待状态不为忙（busy）
void W25Q32_WaitNotBusy(void)
{
  SPI1_CS_LOW();

  // 发送读取状态寄存器指令
  SPI_SwapByte(0x05);

  // 等待收到的数据末位变成0
  while (SPI_SwapByte(0xff) & 0x01)
  {
  }

  SPI1_CS_HIGH();
}

// 擦除段（sector erase）,地址只需要块号和段号
void W25Q32_EraseSector(uint8_t block, uint8_t sector)
{
  // 首先等待状态不为忙
  W25Q32_WaitNotBusy();

  // 开启写使能
  W25Q32_WriteEnable();

  // 计算要发送的地址（段首地址）
  uint32_t addr = (block << 16) + (sector << 12);

  SPI1_CS_LOW();

  // 发送指令
  SPI_SwapByte(0x20);

  SPI_SwapByte(addr >> 16 & 0xff); // 第一个字节
  SPI_SwapByte(addr >> 8 & 0xff);  // 第二个字节
  SPI_SwapByte(addr >> 0 & 0xff);  // 第三个字节

  SPI1_CS_HIGH();

  W25Q32_WriteDisable();
}

// 写入（页写）
void W25Q32_PageWrite(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t *data, uint16_t len)
{
  // 首先等待状态不为忙
  W25Q32_WaitNotBusy();

  // 开启写使能
  W25Q32_WriteEnable();

  // 计算要发送的地址（页首地址）
  uint32_t addr = (block << 16) + (sector << 12) + (page << 8) + innerAddr;

  SPI1_CS_LOW();

  // 发送指令
  SPI_SwapByte(0x02);

  // 发送24位地址
  SPI_SwapByte(addr >> 16); // 第一个字节
  SPI_SwapByte(addr >> 8);  // 第二个字节
  SPI_SwapByte(addr >> 0);  // 第三个字节

  //  依次发送数据
  for (uint16_t i = 0; i < len; i++)
  {
    SPI_SwapByte(data[i]);
  }

  SPI1_CS_HIGH();

  W25Q32_WriteDisable();
}

// 读取
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t innerAddr, uint8_t *buffer, uint16_t len)
{
  // 首先等待状态不为忙
  W25Q32_WaitNotBusy();

  // 计算要发送的地址
  uint32_t addr = (block << 16) + (sector << 12) + (page << 8) + innerAddr;

  SPI1_CS_LOW();

  // 发送指令
  SPI_SwapByte(0x03);

  // 发送24位地址
  SPI_SwapByte(addr >> 16 & 0xff); // 第一个字节
  SPI_SwapByte(addr >> 8 & 0xff);  // 第二个字节
  SPI_SwapByte(addr >> 0 & 0xff);  // 第三个字节

  //  依次读取数据
  for (uint16_t i = 0; i < len; i++)
  {
    buffer[i] = SPI_SwapByte(0xff);
  }

  SPI1_CS_HIGH();
}
/**
 * @brief 将一个uint32_t类型的地址转换为W25Q32的块号、区号、页号和页内地址。
 * @param addr 输入的uint32_t类型的地址。
 * @param block 指向存储块号的uint8_t变量的指针。
 * @param sector 指向存储区号的uint8_t变量的指针。
 * @param page 指向存储页号的uint8_t变量的指针。
 * @param offset 指向存储页内地址的uint8_t变量的指针。
 */
void convert_address_to_w25q32(uint32_t addr, uint8_t *block, uint8_t *sector, uint8_t *page, uint8_t *offset)
{
  // 假设每个块有16个区，每个区有16个页，每页有256字节
  *block = (addr >> 16) & 0xFF;
  *sector = (addr >> 12) & 0x0F;
  *page = (addr >> 8) & 0x0F;
  *offset = addr & 0xFF;
}
void add_data(uint8_t *data)
{
  uint8_t block, sector, page, offset;
  convert_address_to_w25q32(written_pointer, &block, &sector, &page, &offset);

  if (offset > 250 - 1)
  {
    offset = 0;
    page += 1;// 移动到下一页
    written_pointer += 56;  //空6个字节不用
  }
  else written_pointer += 50;

  W25Q32_PageWrite(block, sector, page, offset, data, 50);

}

/* USER CODE END 1 */
