#include "dth11.h"
// #include "stm32f1xx.h"

void delay_us(uint16_t us){     //微秒延时
	uint16_t differ = 0xffff-us-5;				
	__HAL_TIM_SET_COUNTER(&htim1,differ);	//设定TIM1计数器起始值
	HAL_TIM_Base_Start(&htim1);		//启动定时器	
	
	while(differ < 0xffff-5){	//判断
		differ = __HAL_TIM_GET_COUNTER(&htim1);		//查询计数器的计数值
	}
	HAL_TIM_Base_Stop(&htim1);
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    
uint8_t DHT11_Init(void)
{
    DHT11_Rst();
    return DHT11_Check();
}
void DHT11_Rst(void)
{
    DHT11_IO_OUT();  // SET OUTPUT
    DHT11_DQ_OUT(0); // 拉低DQ
    HAL_Delay(18);   // 拉低至少18ms
    DHT11_DQ_OUT(1); // DQ=1
    delay_us(30);   // 主机拉高20~40us
}
uint8_t DHT11_Check(void){
    uint8_t retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else return 1;
	return 0;	    
}



