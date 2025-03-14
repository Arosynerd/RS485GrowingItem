#include "esp8266.h"
#include "spi.h"
#include "usart.h"
#include "string.h"
#include "gpio.h" //复位PB13
#include "rtc.h"//复位计时

uint8_t esp8266_buf[MAX]; // esp8266_buffer 接收esp返回的信息，使用完记得清空 incase errors occur
uint16_t esp8266_len = 0;
uint8_t IsConnect = 0;
uint8_t esp8266_reconnect_count = 0;
uint8_t esp8266_checkonline_count = 0;
extern uint8_t hascache;
extern uint32_t written_pointer;
void esp8266_checkonline(void)
{
    if (IsConnect == 1)
    {
        uint8_t received_buf[35];
        // test begin
        printf("esp8266_checkonline_count = %d\r\n", esp8266_checkonline_count);
        // test end
        if (esp8266_checkonline_count++ > 3)//测试内容
        // if (esp8266_checkonline_count++ > 60)//每5分钟检查
        { // 每5分钟检测一次
            esp8266_checkonline_count = 0;
            espPrintf("+++"); // 退出透传
            HAL_Delay(1500);
            espPrintf("AT+CIPSTATUS\r\n");
            HAL_UART_Receive(&huart3, received_buf, 35, 1500);
            if (strstr((char *)received_buf, "STATUS:3") == 0)
            {
                IsConnect = 0;
                printf("esp8266 offline\r\n");
                printf((char *)received_buf);
                RTC_settime();
            }
            else
            { // 还原现场
                memset(received_buf, 0, 35);//好像没有必要
                espPrintf("AT+CIPMODE=1\r\n");
                // test begin
                HAL_UART_Receive(&huart3, received_buf, 35, 1000);
                if (strstr((char *)received_buf, "OK\r\n") == 0)
                {
                    printf("error");
                    printf((char *)received_buf);
                }
                else
                    printf("CIPMODE=1 success\r\n");
                HAL_Delay(250);

                memset(received_buf, 0, 35);//好像没有必要
                espPrintf("AT+CIPSEND\r\n");
                HAL_UART_Receive(&huart3, received_buf, 35, 1000);
                if (strstr((char *)received_buf, ">") == 0)
                {
                    printf("error");
                    printf((char *)received_buf);
                }
                else
                    printf("CIPSEND success\r\n");
            }
        }
    }
}
void espPrintf(char *fmt)
{
    HAL_UART_Transmit(&huart3, (uint8_t *)fmt, strlen(fmt), HAL_MAX_DELAY);
}
// 重新连接
void esp8266_Reconnect(void)
{
    if (IsConnect == 0) // 未连接才重连
    {
        printf(" count = %d \r\n", esp8266_reconnect_count);
        if (esp8266_reconnect_count++ > 2)
        {
            esp8266_reconnect_count = 0;
            //复位esp8266
            esp8266_reset();
            if (esp8266_init())
            {
                IsConnect = 1;
                printf("Reconnect success\r\n");
                if (hascache == HASCACHE)
                {
                    uint32_t nowpointer = 0;
                    uint8_t spibuf[50];
                    uint8_t block, sector, page, offset;

                    printf("Reconnect send cache\r\n");
                    // test begin
                    printf("written_pointer = %d\r\n", written_pointer);
                    printf("nowpointer = %d\r\n", nowpointer);
                    // test end
                    while (1)
                    {
                        if (nowpointer != written_pointer)
                        {
                            convert_address_to_w25q32(nowpointer, &block, &sector, &page, &offset);
                            if (offset > 250 - 1)
                            {
                                offset = 0;
                                page += 1;        // 移动到下一页
                                nowpointer += 56; // 空6个字节不用
                            }
                            else
                                nowpointer += 50;
                            W25Q32_Read(block, sector, page, offset, spibuf, 50);
                            printf((char *)spibuf);

                            espPrintf((char *)spibuf); // 发送信息
                            HAL_Delay(500);
                        }
                        else
                            break;
                    }
                    //复位
                    //擦除存储器
                    W25Q32_EraseChip();
                    written_pointer = 0;

                }
                else
                    printf("Reconnect fail\r\n");
            }
        }
    }
}
_Bool esp8266_init(void)
{
    uint8_t buf1[35];

    // test begin
    printf("esp8266_init\r\n");
    // test end
    MX_USART3_UART_Init();
    espPrintf("AT+RST\r\n");
    HAL_Delay(1000);

    espPrintf("AT\r\n");

    HAL_UART_Receive(&huart3, buf1, 12, 1000);
    if (strstr((char *)buf1, "OK\r\n") == 0)
    {
        printf("error");
        printf((char *)buf1);
        return 0;
    }
    else
        printf("AT success\r\n");
    memset(buf1, 0, 12);

    HAL_Delay(1000);

    espPrintf("AT+CWJAP=\"huawei123\",\"111222333\"\r\n");
    if (ESP8266_SendCmd("GOT IP\r\n"))
        printf("ATCWJAP success\r\n");
    else
    {
        printf("WIFIerror\r\n");
        return 0;
    }
    HAL_Delay(250);

    espPrintf("AT+CIPSTART=\"TCP\",\"172.20.10.3\",8080\r\n");
    if (ESP8266_SendCmd("CONNECT\r\n"))
        printf("ATTCP success\r\n");
    else
    {
        printf("TCPCONNECTerror\r\n");
        // test begin
        printf("%s\r\n", (char *)esp8266_buf);
        // test end
        return 0;
    }
    HAL_Delay(250);

    espPrintf("AT+CIPMODE=1\r\n");
    // test begin
    HAL_UART_Receive(&huart3, buf1, 35, 1000);
    if (strstr((char *)buf1, "OK\r\n") == 0)
    {
        printf("error");
        printf((char *)buf1);
        return 0;
    }
    else
        printf("AT success\r\n");

    // test end

    // if(ESP8266_SendCmd("OK\r\n")) printf("ATCIPMODE success\r\n");
    // else { printf("CIPMODEerror\r\n");
    //      //test begin
    //      printf("%s\r\n", (char *)esp8266_buf);
    //      //test end
    //     return; }
    HAL_Delay(250);

    espPrintf("AT+CIPSEND\r\n");
    if (ESP8266_SendCmd(">"))
        printf("ATCIPSEND success\r\n");
    else
    {
        printf("CIPSENDerror\r\n");
        return 0;
    }
    HAL_Delay(250);

    // test begin

    // char str[32];
    // float light = 800.89;
    // for(uint8_t i = 0; i < 100; i++){
    // light += 1;
    // sprintf(str, "%s%.2f", "2022120912000036.36889.789", light);
    // espPrintf(str);
    // HAL_Delay(3000);
    // }

    // test end

    printf("done\r\n");
    IsConnect = 1;
    return 1;
}

_Bool ESP8266_SendCmd(char *res)
{
    uint8_t timeOut = 15;
    while (timeOut--)
    {
        if (HAL_UARTEx_ReceiveToIdle(&huart3, esp8266_buf, MAX, &esp8266_len, 2000) == HAL_OK) // 如果收到数据
        {
            if (strstr((const char *)esp8266_buf, res) != 0) // 如果检索到关键词
            {
                memset(esp8266_buf, 0, MAX); // clear esp8266_buffer								//清空缓存

                return 1;
            }
        }

        HAL_Delay(10);
    }
    return 0;
}
void esp8266_reset(void)
{
    reset_pin_init();
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_Delay(100);
    reset_pin_deinit();
}
