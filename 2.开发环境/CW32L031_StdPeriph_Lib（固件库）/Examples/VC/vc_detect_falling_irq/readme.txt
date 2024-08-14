示例目的：
          演示VC模块的输出信号下降沿触发中断功能。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. VC的滤波时钟源为RC150K
          5. PB08/09设置为PUSH-PULL输出

演示说明：
          VC输入P通道为PA00，N通道为BGR基准电压1.2V，VC输出端口为PA11。
          外接可调直流电源，当VC输入P通道电压低于N通道电压时刻，PB09(LED1)输出电平翻转一次。
		  保持P通道电压低于N通道电压，或者调节P通道电压高于N通道电压时，PB09输出电平不变；
          
使用说明：
+ EWARM
          1. 打开vc_detect_falling_irq.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开vc_detect_falling_irq.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)