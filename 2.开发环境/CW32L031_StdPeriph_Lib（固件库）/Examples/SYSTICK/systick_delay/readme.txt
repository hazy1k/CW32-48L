示例目的：
          利用systick定时器完成100mS定时。

硬件资源：
          1. CW32L031CxTx StarKit  
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟2分频，24MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz

演示说明：
        采用默认时钟配置，并初始化systick为1mS定时，在systick中对32位的uwTick变量进行持续加1（计满后变为0）。在主程序中通过读取uwTick的差值来定时，本程序中利用systick实现100mS的定时进行LED灯闪烁。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开systick_delay.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
