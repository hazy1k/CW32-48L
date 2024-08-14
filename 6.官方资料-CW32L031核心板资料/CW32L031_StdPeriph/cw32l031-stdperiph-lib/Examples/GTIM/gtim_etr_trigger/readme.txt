示例目的：
          演示GTIM的触发启动计数的功能。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，不分频，48MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=48MHz
          4. GTIM1的分频系数设置为48000，ARR设置为200-1，则GTIM1每
             200ms溢出一次。       
          5. 设置PB2为输入复用，GTIM1_ETR, 
          6. 设置PB09为PUSH-PULL输出,驱动LED1
          7. 设置GTIM1_ETR的极性为上升沿有效
          8. 设置PA01为输入。
          9. 使能GTIM1的中断溢出。
          10. 设置PA02为输入。

演示说明：
          使用杜邦线将PA01与PB2连接，按下KEY2键后释放，则LED1以200ms的间隔闪烁。
          按下KEY1后，GTIM1停止计数，LED1不闪烁。
          
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
