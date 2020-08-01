#This is for FLY's FuLiYe new project
#2020年新工科联盟-Xilinx暑期学校（Summer School）项目

title: 开发板 +AWS +Web 实现摇摇乐计数小游戏：“是男人就摇10s!”

## Let's Go
---
### 开发板准备
1. 为最新的[FPGADemo](https://github.com/DoneSEA/SEA)添加QSPI模块，支持串口数据输出。
2. 使用[Vivado 2018.3](https://china.xilinx.com)进行编译生成bit文件。
3. 将bit文件移入tf卡的overlay文件夹
---
### Arduino程序编写
1. 对开发板进行烧写。
2. 使用串口监视器进行陀螺仪原始数据进行观测。
3. 编写数据处理模块处理原始数据。
4. 编写计数器模块进行摇动计数。

---
### [AWS Iot](https://www.awseducate.com)
1. 创建事物、策略等。
2. 通过互联网接收数据。
3. 使用MQTT订阅主题观察数据情况。

---
### Web
1. 使用aws SDK等组件从AWS平台获取数据。
2. HTML+CSS+js搭建web实现摇摇乐小游戏。

