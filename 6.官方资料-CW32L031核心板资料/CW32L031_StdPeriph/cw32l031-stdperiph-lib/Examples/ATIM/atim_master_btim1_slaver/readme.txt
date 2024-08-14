示例目的：
          演示ATIM作为主机，BTIM作为从机的级联使用

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，8分频，ARR设置为99，溢出周期100us, 主模式
          5. BTIM1使用ATIM的软件更新位UG作为计数时钟的来源，即100us计数一次，ARR设置为99，
             则BTIM1每10ms溢出一次。
          6. PB00设置为BTIM1TOGN的输出。
          7. 串口UART1作为调试串口,打印调试信息,波特率115200
演示说明：
          ATIM以1MHz时钟进行计数，设置ARR寄存器为99，则ATIM将每100us溢出一次，并触发
          中断服务程序。中断服务程序中，对CR.UG位写1,向BTIM1提供计时信号。PB00将每10ms

          翻转一次.

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
