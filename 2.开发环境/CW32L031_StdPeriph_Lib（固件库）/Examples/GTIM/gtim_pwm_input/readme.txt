示例目的：
          演示GTIM的输入捕获功能，测量PWM的频率和脉宽。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，6分频，8MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=8MHz
          4. GTIM1不分频       
          5. 设置PA06复用为GTIM1_CH1, 上升沿触发捕获
          6. 设置PA07复用为GTIM1_CH2，下降沿触发捕获。
  
演示说明：
          使用GTIM1的CH1和CH2两个通道对PWM输入信号进行测量，CH1测量周期，
          CH2测量脉宽。 将同一个PWM输入信号引入到PA06和PA07上。
          
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
