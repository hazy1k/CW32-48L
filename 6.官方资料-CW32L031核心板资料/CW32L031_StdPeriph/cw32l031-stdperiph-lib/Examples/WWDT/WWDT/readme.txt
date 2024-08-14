示例目的：
          模拟软件发生故障时，窗口看门狗WWDT复位系统的功能。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟由HSI提供，48MHz
          3. PCLK、HCLK不分频，PCLK=HCLK=SysClk=48MHz
          4. WWDT由PCLK经524288分频，即约11ms计数一次
          5. PB08设置为PUSH-PULL输出，驱动LED1
          6. PB08设置为PUSH-PULL输出，驱动LED2
          7. PA02设置为输入，作为按键KEY1的输入
          8. 系统节拍定时器设置为200ms产生一次中断

演示说明：
          系统时钟由HSI提供，频率48MHz。配置WWDT的WCNT为0x60，WINR为0x50，喂狗的窗口为176ms~352ms，允许预溢出中断。
          系统节拍配置为每 200 ms 产生一个中断，中断复位程序中闪烁 LED1，并喂狗。
          按键KEY1配置为在其下降沿上产生中断。
          在 NVIC 中，将KEY1对应引脚的中断向量且优先级等于0，并且 SysTick 中断向量优先级设置为1（KEY1的中断优先于 SysTick 中断）。
          按钮KEY1用于模拟软件故障：KEY1按下后，进入对应的中断服务程序，且中断标志位不清零，CPU不会进入系统节拍中断服务程序，因此不会重载WWDT计数器。
          当WWDT计数器预溢出时，WWDT将产生一个POV中断。
          程序发生WWDT中断后，LED1长亮，延后2个喂狗周期后，系统复位。
          若系统发生过WWDT复位，则LED2亮。
          
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
