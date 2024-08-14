示例目的：
          利用HCLK时钟输出功能将HCLK时钟从PA04引脚输出。

硬件资源：
          1. CW32L031CxTx StarKit  
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz

演示说明：
        采用默认时钟配置，并设置HCLK时钟输出，用示波器观测PA04引脚的波形，默认程序为8MHz。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开rcc_hclkout.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
