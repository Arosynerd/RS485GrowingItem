#ifndef DTH11_H
#define DTH11_H

#include "gpio.h"
#include "tim.h"


//PA11
// 修改为使用HAL库函数配置输入模式
// 修改为使用HAL库函数配置输出模式
//IO操作函数
// 修改为使用HAL库函数设置输出电平
#define DHT11_DQ_OUT(X) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, (GPIO_PinState)X)
// 修改为使用HAL库函数读取输入电平
#define DHT11_DQ_IN HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

uint8_t DHT11_Init(void);
void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);
void delay_us(uint16_t us);



#endif
