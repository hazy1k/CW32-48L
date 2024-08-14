示例目的：
          SPI读写外部FLASH（W25Q64）。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟2分频，24MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz
          4. SPI，波特率为PCLK的8分频，3Mbps
		  5. UART的传输时钟设置为PCLK
		  6. PB09（LED1）设置为PUSH-PULL输出
		  7. PB08（LED2）设置为PUSH-PULL输出
		  

演示说明：
          读外部FLASH的DeviceID、ManufactDeviceID、JedecID、UniqueID，并通过UART打印到串口助手。
		  
		  当读取的JedecID正确时：
		      写入TxBuffer缓冲区中的数据到FLASH第1页。
		      读取FLASH第1页数据，并存储到RxBuffer缓冲区。
		      检查写入的数据与读出的数据是否一致：
		          一致，打印“FLASH Success”，LED1亮，
		          否则，打印“FLASH Error 1”，LED2亮。
		      擦除4KB扇区，读取FLASH第1页数据，并存储到RxBuffer缓冲区。
		      RxBuffer缓冲区中数据存在‘1’时，打印“FLASH Error 2”，LED2亮。
			  
		  当读取的JedecID不正确时：
		      打印“FLASH Error 3”，LED2亮。

硬件连接：
          SPI1_SCK  (PA10) -- FLASH SCK  
          SPI1_MISO (PA11) -- FLASH MISO
		  SPI1_MOSI (PA12) -- FLASH MOSI 
          SPI1_CS   (PA15) -- FLASH CS
		  
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
