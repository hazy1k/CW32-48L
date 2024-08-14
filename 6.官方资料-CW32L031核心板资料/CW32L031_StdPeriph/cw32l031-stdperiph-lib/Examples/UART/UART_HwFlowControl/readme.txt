示例目的：
          UART硬件流控（RTS、CTS）示例。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK

演示说明：
          UART发送TxBuffer缓冲区中的数据至PC；
		  PC发送数据，UART接收数据并存储到RxBuffer缓冲区，
		  UART将RxBuffer缓冲区中的数据回传至PC。
		  当UART接收到'\n'，或接收数据量超过RxBufferSize后，传输结束。

硬件连接：
          UART1_TXD (PA08) -- PCRX
          UART1_RXD (PA09) -- PCTX
          UART1_CTS (PA10) -- CTS
          UART1_RTS (PA11) -- RTS

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
