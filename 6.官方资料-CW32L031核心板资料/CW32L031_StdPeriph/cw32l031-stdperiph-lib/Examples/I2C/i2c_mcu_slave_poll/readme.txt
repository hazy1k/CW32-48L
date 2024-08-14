示例目的：
          利用I2C接口， 实现I2C从设备功能，和I2C主机进行数据通信。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz

演示说明：
1、采用默认时钟配置，初始化I2C为主接口，500KHz，不使能I2C中断。
2、主从通信从设备I2C接收时序：根据I2C模块状态变化，接收保存数据并回应ACK，直到主动回应NACK或者收到STOP信号。
3、主从通信从设备I2C发送时序：根据I2C模块状态变化，发送数据，直到收到NACK信号或者STOP信号。
4、程序流程：程序完成I2C从设备配置后，循环持续等待I2C模块的状态改变，收到SLA+W字节后，循环接收主机发送的所有数据的接收。然后启动数据接收流程：循环持续等待I2C模块的状态改变，收到SLA+R之后，循环发送数据，直到收到NACK信号或者STOP信号，完成所有字节发送。然后重复下一次测试过程。
5、注意实际测试时需要更改I2C/GPIO口线、时钟配置以及GPIO配置都需要修改。
6、注意主从通信时，需要连接从机单板对应I2C总线到单板的EEPROM接口上(标号CN11,VDDE/SCL/SDA三根口线都要连接，以给I2C提供上拉电阻)
7、I2C从机设备地址目前统一在cw32l031_i2c.h文件中进行定义，在来回测试EEPROM读写和两块单板进行主从通信时需要注意修改。


使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开i2c_mcu_slave_poll.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
