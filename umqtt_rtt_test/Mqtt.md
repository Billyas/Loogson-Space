## MQTT移植方案

1. 基于龙芯IDE环境下的rtthread加载umqtt，也是本文档的方案，umqtt特别精简，仅需少量依赖即可工作，已成功编译运行。
2. 基于RT-THread官方的裁剪编译环境加载包，虽然成功，但是缺少所需网卡驱动，移植网卡驱动太过于复杂，未解决。
3. 基于Free RTOS的mqtt移植过程，这个方法具体采用[【野火】](https://doc.embedfire.com/net/lwip/stm32/zh/latest/doc/chapter21/chapter21.html)教程，已有相关源码，基础部分编译通过，验证部分还未编写。



## umqtt移植

![image-20220519230858057](.\pic\image-20220519230858057.png)



初步编译成功，执行失败是由于没有配置对应的服务器，后续配置，问题不大。

![image-20220519231048759](.\pic\image-20220519231048759.png)



使用测试服务器后测试成功

![image-20220520000421553](.\pic\image-20220520000421553.png)
