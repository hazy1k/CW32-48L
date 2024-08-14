示例目的：
          演示ATIM的基本计数用法。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，不分频，ARR设置为9999，溢出周期1.25ms
          5. PA00设置为PUSH-PULL输出

演示说明：
          ATIM以8MHz时钟进行计数，设置ARR寄存器为9999，则ATIM将每1.25ms溢出一次，并触发
          中断服务程序。中断服务程序中，PA00输出电平翻转。

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
