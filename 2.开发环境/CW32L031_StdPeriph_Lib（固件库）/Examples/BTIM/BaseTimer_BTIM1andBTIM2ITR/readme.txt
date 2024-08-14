示例目的：
          控制基本定时器BTIM1与BTIM2内部级联，扩展定时器的计数/计时范围

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. BTIM1的时钟设置为PCLK，8分频
          5. BTIM2的时钟为BTIM1的OV信号，不分频
          6. PB09设置为PUSH-PULL输出，驱动LED1
          7. PB08设置为PUSH-PULL输出，驱动LED2
          8. PA05设置为BTIM2的TOGP输出

演示说明：
          BTIM1以1MHz时钟进行计数，设置ARR寄存器为49999，则BTIM1将每50ms溢出一次，并触发
          中断服务程序。中断服务程序中，PB09输出电平翻转，LED1将10Hz的频率闪烁。
          BTIM2以BTIM1的溢出信号OV进行计数，设置ARR寄存器为9，则BTIM1将每500ms溢出一次，并触发
          中断服务程序。中断服务程序中，PB08输出电平翻转，LED2将1Hz的频率闪烁。

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
