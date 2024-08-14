示例目的：
          利用I2C接口， 实现I2C主设备功能，和I2C从机进行数据通信。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz

演示说明：
1、采用默认时钟配置，初始化I2C为主接口，500KHz，不使能I2C中断。
2、主从通信主设备I2C发送时序：依次发送START信号、SLA+W字节、多个数据字节，最后发送STOP信号。
3、主从通信主设备I2C接收时序：依次发送START信号、SLA+R字节，然后接收多个数据字节，最后发送STOP信号。
4、程序流程：程序完成I2C主设备配置后，先将u8Senddata数组中的内容发给从机：发送START信号后，I2C模块会产生状态改变，程序持续查询I2C模块状态并根据不同状态值进行不同处理，直到完成SLA+W、所有待发送的数据字节以及STOP信号的发送。然后启动数据接收流程：发送启动信号后，I2C模块会产生状态改变，程序持续查询I2C模块状态并根据不同状态值进行不同处理，直到完成SLA+R、所有待接收字节的接收，以及STOP信号发送。然后重新初始化u8Senddata数组内容，重复下一次测试过程。完成WriteReadCycle变量设置的测试次数后退出。
5、注意实际测试时需要更改I2C/GPIO口线、时钟配置以及GPIO配置都需要修改。
6、注意主从通信时，需要连接从机单板对应I2C总线到单板的EEPROM接口上(标号CN11,VDDE/SCL/SDA三根口线都要连接，以给I2C提供上拉电阻)
7、I2C从机设备地址目前统一在cw32l031_i2c.h文件中进行定义，在来回测试EEPROM读写和两块单板进行主从通信时需要注意修改。
8、注意主机完成发送流程后，需要等待从机准备好后再开始接收流程，具体需要等待的时间取决于从机的处理速度。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开Template.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
