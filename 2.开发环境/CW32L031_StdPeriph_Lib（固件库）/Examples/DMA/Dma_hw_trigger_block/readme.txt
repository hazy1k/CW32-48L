示例目的：
          通过ADC转换完成标志触发DMA方式实现外设到内存的DMA传输：无CPU干涉情况下将ADC转换数据传输到目的地址BUFFER的DMA传输。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz

演示说明：
        配置DMA的传输方式为硬件触发，DMA硬件触发源为ADC转换完成标志，DMA传输完成中断并使能。
        配置ADC为单次单通道转换模式，ADC转换完成中断并使能。
        ADC转换完成后，产生ADC转换完成标志，触发DMA将数据传到到目标BUFFER，同时产生ADC完成中断，在ADC转换完成中断服务程序中重新启动下一次ADC转换。
        16次ADC转换完成（程序设置，用户可自行配置）并触发DMA传输后，产生DMA传输完成中断标志，在DMA传输完成中断中重新配置DMA的计数寄存器并重新使能DMA，开启新一轮的数据传输。
        在DMA循环传输过程中，LED灯持续闪烁，如发生传输错误则MCU被挂起，LED不再闪烁。

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
