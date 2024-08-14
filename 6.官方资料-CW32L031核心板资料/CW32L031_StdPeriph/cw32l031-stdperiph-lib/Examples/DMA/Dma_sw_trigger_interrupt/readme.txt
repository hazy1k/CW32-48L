示例目的：
          通过软件触发DMA方式实现内存到内存的DMA传输：无CPU干涉情况下的源地址BUFFER到目的地址BUFFER的DMA传输。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz

演示说明：
          正确配置DMA的传输方式，配置DMA传输完成中断并使能，通过软件设置启动DMA传输。
          DMA传输完成中断服务程序中置传输完成标志TransOverFlag为1。
          主循环中进行传输完成标志TransOverFlag判断：未完成传输时LED灯快闪，已完成传输则比较目的地址BUFFER和源地址BUFFER的内容是否相同，相同则LED灯慢闪，否则不闪灯。
          注意：由于DMA传输过程非常快，LED灯快闪过程基本看不见，属于正常现象。

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
