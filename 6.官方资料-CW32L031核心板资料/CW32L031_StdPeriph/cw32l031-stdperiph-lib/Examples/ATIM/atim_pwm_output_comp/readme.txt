示例目的：
          演示ATIM的PWM互补输出功能

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，8分频，计数时钟1MHz,ARR设置为99，100us溢出,
             输出比较, 使用ATIM的CH1A和CH1B通道输出互补的PWM信号。
          5. 串口UART1作为调试串口,打印调试信息,波特率115200
          6. PB13设置为ATIM_CH1B通道的输出.
          7. PB02设置为ATIM_CH1A通道的输出.

演示说明：
          PB13与PB02输出带死区的互补波形.

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
