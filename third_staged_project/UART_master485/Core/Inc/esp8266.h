#ifndef ESP8266_H
#define ESP8266_H
#define MAX 100

_Bool esp8266_init(void);
void espPrintf(char *fmt);
_Bool ESP8266_SendCmd(char *res);
void esp8266_Reconnect(void);



#endif

