# RS485GrowingItem

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

