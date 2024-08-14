示例目的：
          模拟软件发生故障时，独立看门狗IWDT复位系统的功能。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HS，PCLK、HCLK不分频，PCLK=HCLK=SysClk=48MHz
          4. IWDT由独立时钟RC10K经4分频，即0.4ms计数一次
          5. PB09设置为PUSH-PULL输出，驱动LED1
          6. PB08设置为PUSH-PULL输出，驱动LED2
          7. PA02设置为输入，作为按键KEY1的输入
          8. 系统节拍定时器设置为250ms产生一次中断

演示说明：
          系统时钟由HSI提供，频率48MHz。配置IWDT的ARR为699，即IWDT的计时时常为
          （699 + 1）* 0.4ms = 280ms，当IWDT计数溢出后，系统复位。
          系统节拍配置为每 250 ms 产生一个中断，中断复位程序中闪烁 LED1，并喂狗。
          按键KEY1配置为在其下降沿上产生中断。
          在 NVIC 中，将KEY1对应引脚的中断向量且优先级等于0，并且 SysTick 中断向量优先级设置为1
          （KEY1的中断优先于 SysTick 中断）。
          按钮KEY1用于模拟软件故障：KEY1按下后，进入对应的中断服务程序，且中断标志
          位不清零，CPU不会进入系统节拍中断服务程序，因此不会重载IWDT计数器。
          当IWDT计数器溢出时，IWDT将复位系统。程序发生IWDT复位后，LED2亮起。
          
          
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
