示例目的：
          AWT采用LSI作为工作时钟，进行AWT的休眠唤醒测试

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. AWT的时钟设置为LSI，32768分频

演示说明：
          AWT以1Hz时钟进行计数，设置ARR寄存器为4，则AWT将每5S溢出一次，产生溢出中断标志。
          程序完成定时器相关参数设置，并打开定时器溢出中断，然后进入低功耗，等待被AWT溢出中断唤醒。
          当5S时间到后，AWT溢出中断标志产生，唤醒MCU，进入中断处理，然后接着处理主循环中睡眠指令后面的任务。

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
