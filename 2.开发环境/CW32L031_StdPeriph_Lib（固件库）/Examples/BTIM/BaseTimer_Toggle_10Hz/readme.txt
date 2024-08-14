示例目的：
          演示基本定时器BTIM的TOG输出功能，不需要中断和口线寄存器的操作，完全由硬件实现方波的输出。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSE，16MHz
          3. 系统时钟设置为HSE，PCLK、HCLK不分频，PCLK=HCLK=SysClk=16MHz
          4. BTIM1的时钟设置为PCLK，256分频，
          5. PA07,PA09,PA10设置为PUSH-PULL输出, 复用为BTIM1的TOG输出
          6. PB09设置为PUSH-PULL输出，驱动LED1
          7. PB08设置为PUSH-PULL输出，驱动LED2



演示说明：
          BTIM1的计数时钟为16MHz/256=62500Hz，设置ARR寄存器为62500 / 20 - 1，BTIM1每50ms溢出一次；
          溢出后，触发中断服务程序。中断服务程序中，PB09输出翻转，LED1闪烁，用于指示。
          PA07和PA09输出TOGP，PA10输出TOGN，可以示波器观测波形。
          

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
