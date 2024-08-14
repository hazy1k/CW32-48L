示例目的：
          GTIM和BTIM内部互联，可实现定时器的任意分频。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，不分频，48MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=48MHz
          4. BTIM1由PCLK提供计数时钟，设置ARR为48000-1，则BTIM1 1ms溢出一次；          
          5. GTIM1的计数时钟由BTIM1的溢出信号OV提供，设置ARR为1000-1，则GTIM1
             1s溢出一次。
          6. 设置PB09为PUSH-PULL输出,驱动LED1

演示说明：
          程序运行后,LED1将1s亮1s灭。
          注：计数时钟为TRS时，分频值不能为0，否则计数异常
          
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
