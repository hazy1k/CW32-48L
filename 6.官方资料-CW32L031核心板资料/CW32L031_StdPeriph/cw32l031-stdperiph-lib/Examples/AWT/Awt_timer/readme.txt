示例目的：
          控制AWT定时器以固定的时间间隔产生中断，并在中断中控制口线电平翻转

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. AWT的时钟设置为HSIOSC，32768分频
          5. PB02、PB09设置为PUSH-PULL输出，驱动LED1

演示说明：
          AWT以48/32768 MHz时钟进行计数，设置ARR寄存器为145，则AWT将每100ms溢出一次，并产生溢出标志。程序循环查询溢出溢出标志，溢出标志有效后，清除标志，并进行PB08和PB09端口翻转。

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
