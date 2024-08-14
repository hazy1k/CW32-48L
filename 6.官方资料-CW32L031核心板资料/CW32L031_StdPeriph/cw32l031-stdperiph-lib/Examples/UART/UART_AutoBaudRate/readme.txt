示例目的：
          UART自动波特率检测。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK

演示说明：
          PC（主机）发送以0xF8作为数据头的数据流，UART从机等待从主机接收数据，
		  然后开始自动波特率检测，更新波特率设置，并回传接收到的数据至PC。

硬件连接：
          UART1_TXD (PA08) -- PCRX
          UART1_RXD (PA09) -- PCTX

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
