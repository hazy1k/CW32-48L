示例目的：
          演示ATIM作为从设备，配合BTIM主设备的使用

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，4MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=4MHz
          4. ATIM边沿计数，上计数模式，不分频，计数时钟4MHz,ARR设置为39999，10ms溢出,
             单次运行。溢出中断程序中翻转PB09的电平输出。
          5. 串口UART1作为调试串口,打印调试信息,波特率115200
          6. PB09设置为PUSH-PULL输出，驱动LED1.
          7. PB00设置为BTIM1的TOGN.
          8. BTIM1使用内部时钟PCLK计数，4分频，频率为1MHz，单次运行，50ms溢出，触发ATIM运行

演示说明：
          程序运行后，BTIM1每50ms溢出触发ATIM启动一次计时，ATIM计时10ms后停止，
          BTIM1每次溢出后PB00的电平翻转一次，ATIM溢出后，计时停止，并翻转PB09的电平
          因PB00和PB09的初始电平相同，PB09的波形比PB00延时10ms

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
