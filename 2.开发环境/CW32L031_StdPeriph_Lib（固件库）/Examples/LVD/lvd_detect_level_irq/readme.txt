示例目的：
          演示LVD模块的电平触发中断功能。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟2分频，24MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz
          4. LVD的滤波时钟源为RC150K
          5. PB08/09设置为PUSH-PULL输出

演示说明：
          LVD输入通道为PB11，输出端口为PA08，门限电压为1.8V。
          外接可调直流电源，当LVD输入通道电压低于门限电压时，PB09输出电平翻转。
		  延时1s之后，重新监测LVD输入通道电压。
		  如果仍然低于门限电压时，PB09输出电平继续翻转；如果高于门限电压时，PB09输出电平不变。
          
使用说明：
+ EWARM
          1. 打开lvd_detect_falling_irq.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开lvd_detect_falling_irq.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)