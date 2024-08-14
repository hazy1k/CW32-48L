示例目的：
          控制基本定时器BTIM1以固定的时间间隔产生中断，并在中断中控制口线电平翻转

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. BTIM1的时钟设置为PCLK，8分频
          5. PB09设置为PUSH-PULL输出，驱动LED1

演示说明：
          BTIM1以1MHz时钟进行计数，设置ARR寄存器为49999，则BTIM1将每50ms溢出一次，并触发
          中断服务程序。中断服务程序中，PB09输出电平翻转，LED1将10Hz的频率闪烁。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开project.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
