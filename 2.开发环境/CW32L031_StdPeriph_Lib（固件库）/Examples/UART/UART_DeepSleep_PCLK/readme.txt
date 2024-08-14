示例目的：
          UART深度休眠模式示例（传输时钟为PCLK）。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK
		  5. PB09（LED1）设置为PUSH-PULL输出
		  6. UART1_RXD(PA09)下降沿中断

演示说明：
          程序启动一段时间后进入深度休眠模式，
		  PC发送数据可唤醒MCU，唤醒后UART轮询接收数据，并存储到TxRxBuffer缓冲区，
		  UART接收到'\n'后不再接收数据，然后将TxRxBuffer缓冲区中的数据回传至PC。
		  传输结束后，LED1闪烁5s，并再次进入深度休眠模式。

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
