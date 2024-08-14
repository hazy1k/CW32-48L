示例目的：
          UART单线半双工通信示例。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK
          5. PB09（LED1）设置为PUSH-PULL输出
	      6. PC02（LED2）设置为PUSH-PULL输出

演示说明：
          USARTy查询方式（TXE）发送TxBuffer1缓冲区中的数据，
		  USARTz查询方式（RC）接收数据，并存储到RxBuffer2缓冲区。
		  
		  USARTz查询方式（TXE）发送TxBuffer2缓冲区中的数据，
		  USARTy查询方式（RC）接收数据，并存储到RxBuffer1缓冲区。
		  
		  比较TxBuffer1和RxBuffer2、TxBuffer2和RxBuffer1，
		  如果数据一致，则LED1亮，否则LED2亮。

硬件连接：
          UART1_TXD (PA08) -- UART2_TXD (PA06)

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
