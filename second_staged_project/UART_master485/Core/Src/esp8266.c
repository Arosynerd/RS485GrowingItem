#include "esp8266.h"
#include "usart.h"
#include "string.h"
uint8_t esp8266_buf[MAX];//esp8266_buffer
uint16_t esp8266_len = 0;
void espPrintf(char *fmt){
    HAL_UART_Transmit(&huart3, (uint8_t *)fmt, strlen(fmt), HAL_MAX_DELAY);
}
void esp8266_init(void){
    uint8_t buf1[35];
    
    //test begin
    printf("esp8266_init\r\n");
    //test end
    MX_USART3_UART_Init();
    espPrintf("AT+RST\r\n");
    HAL_Delay(1000);

    espPrintf("AT\r\n");
    //test begin
    HAL_UART_Receive(&huart3, buf1, 12, 1000);
    if(strstr((char *)buf1, "OK\r\n") == 0) { printf("error"); printf((char *)buf1); return;}
    else printf("AT success\r\n");
    memset(buf1, 0, 12);

    // if(strstr((char *)esp8266_buf, "OK\r\n") == 0) { printf("error"); printf((char *)esp8266_buf); }
    // else printf("AT success\r\n");
    // while(HAL_UARTEx_ReceiveToIdle(&huart3, esp8266_buf, MAX, &esp8266_len, 100) != HAL_OK);
    // if(strstr((char *)esp8266_buf, "OK\r\n") == 0) { printf("error"); printf((char *)esp8266_buf); }
    // else printf("AT success\r\n");
    // while(HAL_UARTEx_ReceiveToIdle(&huart3, esp8266_buf, MAX, &esp8266_len, 100) != HAL_OK);
    // if(strstr((char *)esp8266_buf, "OK\r\n") == 0) { printf("error"); printf((char *)esp8266_buf); }
    // else printf("AT success\r\n");

    //test end


    HAL_Delay(1000);

    espPrintf("AT+CWJAP=\"huawei123\",\"111222333\"\r\n");
    if(ESP8266_SendCmd("GOT IP\r\n")) printf("ATCWJAP success\r\n");
    else { printf("WIFIerror\r\n"); return; }
    HAL_Delay(250);

    espPrintf("AT+CIPSTART=\"TCP\",\"172.20.10.3\",8080\r\n");
    if(ESP8266_SendCmd("CONNECT\r\n")) printf("ATTCP success\r\n");
    else { printf("TCPCONNECTerror\r\n");
        //test begin
        printf("%s\r\n", (char *)esp8266_buf);
        //test end
        return; }
    HAL_Delay(250);

    espPrintf("AT+CIPMODE=1\r\n");
    //test begin
    HAL_UART_Receive(&huart3, buf1, 35, 1000);
    if(strstr((char *)buf1, "OK\r\n") == 0) { printf("error"); printf((char *)buf1); return;}
    else printf("AT success\r\n");



    //test end


    // if(ESP8266_SendCmd("OK\r\n")) printf("ATCIPMODE success\r\n");
    // else { printf("CIPMODEerror\r\n");
    //      //test begin
    //      printf("%s\r\n", (char *)esp8266_buf);
    //      //test end
    //     return; }
    HAL_Delay(250);

    espPrintf("AT+CIPSEND\r\n");
    if(ESP8266_SendCmd(">")) printf("ATCIPSEND success\r\n");
    else { printf("CIPSENDerror\r\n"); return; }
    HAL_Delay(250);
    
    //test begin

    // char str[32];
    // float light = 800.89;
    // for(uint8_t i = 0; i < 100; i++){
    // light += 1;
    // sprintf(str, "%s%.2f", "2022120912000036.36889.789", light);
    // espPrintf(str);
    // HAL_Delay(3000);
    // }
    
    //test end
    
    printf("done\r\n");

}

_Bool ESP8266_SendCmd(char *res)
{
	uint8_t timeOut = 15;
    while(timeOut--){
		if(HAL_UARTEx_ReceiveToIdle(&huart3, esp8266_buf, MAX, &esp8266_len, 2000) == HAL_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, res) != 0)		//如果检索到关键词
			{
				memset(esp8266_buf, 0, MAX);//clear esp8266_buffer								//清空缓存
				
				return 1;
			}
		}
		
		HAL_Delay(10);
    }
	return 0;

}

