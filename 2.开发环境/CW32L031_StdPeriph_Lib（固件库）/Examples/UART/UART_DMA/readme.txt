示例目的：
          UART通过DMA发送数据示例。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK
		  5. DMA，硬件触发 BLOCK 传输模式
		  6. PB09（LED1）设置为PUSH-PULL输出

演示说明：
          通过for循环初始化TxRxBuffer缓冲区，
          UART通过DMA发送TxRxBuffer缓冲区中的数据至串口调试助手，无需CPU的干预，
		  此时，LED1同时闪烁。

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
