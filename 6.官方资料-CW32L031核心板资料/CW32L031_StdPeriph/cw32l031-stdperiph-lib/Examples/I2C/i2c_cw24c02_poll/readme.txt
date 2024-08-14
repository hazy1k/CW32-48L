示例目的：
          利用I2C接口，采用轮询方式读写EEPROM芯片（CW24C02）。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz

演示说明：
1、采用默认时钟配置，初始化I2C为主接口，500KHz，不使能I2C中断。
2、I2C写时序：根据CW24C02手册，采用页写模式：依次发送START信号、SLA+W字节、目标地址字节、以及最多8字节（受CW24C02每页8字节限制）数据，最后发送STOP信号。
3、I2C读时序：根据CW24C02手册，采用顺序读模式：先通过字节写模式完成SLA+W字节以及目标地址发送，然后发送重复START信号、SLA+R，并根据需要读取的字节数产生对应字节的时钟信号，最后发送STOP信号。
4、程序流程：程序完成I2C主设备配置后，先将u8Senddata/u8Senddata2数组中的内容依次写入到EEPROM的第1/2页（CW24C02每页8字节）：发送START信号后，I2C模块会产生状态改变，程序持续查询I2C模块状态并根据不同状态值进行不同处理，直到完成CW24C02的页写模式所有数据字节以及STOP信号发送。然后启动从EERROM的起始地址开始的16字节（第1/2页）数据读取流程：发送启动信号后，I2C模块会产生状态改变，程序持续查询I2C模块状态并根据不同状态值进行不同处理，直到完成CW24C02的顺序读模式所有数据字节发送及接收，并发送STOP信号，完成读操作流程。然后重新初始化u8Senddata数组内容，重复下一次测试过程。完成WriteReadCycle变量设置的测试次数后退出。
5、注意实际测试时需要更改I2C/GPIO口线、时钟配置以及GPIO配置都需要修改。
6、注意由于EEPROM器件限制，在两次写入数据之间需要加入一定的等待延时，以等待EEPROM实际的写操作完成，否则后续的或者写操作会失败。
7、I2C从机设备地址目前统一在cw32l031_i2c.h文件中进行定义，在来回测试EEPROM读写和两块单板进行主从通信时需要注意修改。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开i2c_cw24c02_poll.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
