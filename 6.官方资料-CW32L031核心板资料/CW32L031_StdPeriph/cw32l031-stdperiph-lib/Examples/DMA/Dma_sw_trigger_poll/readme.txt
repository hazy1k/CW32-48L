示例目的：
          通过软件触发DMA方式实现内存到内存的DMA传输：无CPU干涉情况下的源地址BUFFER到目的地址BUFFER的DMA传输。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz

演示说明：
          正确配置DMA的传输方式，不配置中断，通过软件设置启动DMA传输。
          主循环中进行传输完成标志判断，获取到传输完成标志后，比较目的地址BUFFER和源地址BUFFER的内容是否相同，相同则闪灯，否则不闪灯。

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
