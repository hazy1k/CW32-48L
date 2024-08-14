示例目的：
          演示GTIM的TOGGLE输出功能。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，不分频，48MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=48MHz
          4. GTIM1的分频系数设置为48000，ARR设置为200-1，则GTIM1每
             200ms溢出一次。          
          5. PB12，PB13复用为GTIM1_TOGN，GTIM1_TOGP
          6. 设置PB09为PUSH-PULL输出,驱动LED1

演示说明：
          程序运行后,PB12和PB13以2.5Hz的频率输出互补的方波，PB13的波形和PB09的
          波形的相位一致。LED1以200ms的间隔闪烁。
          
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
