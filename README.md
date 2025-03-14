# RS485GrowingItem

## 大概介绍

主机发送从机，每隔一段时间询问从机温湿度及光照，从机应答。主机再发送数据到tcpserver

tcpserver保存数据到数据库。若没有连接到tcpserver，暂时保存在主机的外部存储器，连接后再发送给tcpserver。

主机从机都有一个频闪指示灯代表正常运行。

## first_project

最开始的，较为简单

## second_staged_project

增加了tcpserver显示和存储数据

## third_staged_project

1.增加断连tcpserver时，存储采集数据到外部存储器

重新连接后发送给tcpserver保存在数据库

2.主从机增加了频闪灯，表示设备正常运行

## four_staged_project

1.对esp8266增加rst控制线，解决正常连接然后断连，esp8266仍然处于透传模式，重连时无法重连的问题。

2.整理了代码解决了其他小问题

### Problems existed

1.最多可以存储3天数据，未测试长时间断连是否可以正常存储发送

2.从机不稳定，有时发不回数据

