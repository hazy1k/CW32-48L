示例目的：
          RTC时钟测试，初始化日历，使用间隔中断0.5秒通过Log输出日期时间

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI， LSE使能
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. RTC使用LSE作为时钟输入
          5. 串口UART1作为调试串口,打印调试信息,波特率115200
          

演示说明：
          每0.5秒通过Log输出日期时间.

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
