## 功能

主机发送从机，每隔一段时间询问从机温湿度及光照，从机应答。主机再发送数据到tcpserver

tcpserver保存数据到数据库。若没有连接到tcpserver，暂时保存在主机的外部存储器，连接后再发送给tcpserver。

主机从机都有一个频闪指示灯代表正常运行。

## Connection

## ![image-20250314144332574](readme.assets/image-20250314144332574.png)

### 主机发送指令 ：

主机名：如1、2 一个字节

### 从机返回信息

【temp】【humi】【light】三个字节

## 逻辑过程

#### 主机逻辑

![image-20250314144624331](readme.assets/image-20250314144624331.png)

## TCPServer

![abc](readme.assets/abc.png)

![image-20250310160702313](readme.assets/image-20250310160702313.png)

## 实物连接

![image-20250314144458985](readme.assets/image-20250314144458985.png)

## 串口1数据说明

![屏幕截图 2025-03-14 155346](readme.assets/屏幕截图 2025-03-14 155346.png)

## 其他说明

从机1、2、3....的代码都是RS485_slave,将对应名字改成1、2、3......即可。最多255个，8个字节

