示例目的：
          演示基本定时器BTIM的触发启动功能。设置BTIM的EN为0，当触发信号有效时，定时器启动计数。

硬件资源：
          1. CW32L031CxTx StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. BTIM1的时钟设置为PCLK，512分频，触发启动模式，触发类型下降沿
          5. PA12复用功能配置为BTIM_ETR，作为外部触发信号的输入端口
          6. PB09设置为PUSH-PULL输出，驱动LED1
          7. PB08设置为PUSH-PULL输出，驱动LED2
          8. PA01设置为中断输入口，中断类型下降沿


演示说明：
          BTIM1配置为触发启动模式，单次运行，以15625Hz时钟频率计数，设置ARR寄存器为15624；
          BTIM1计时满1s后溢出，溢出后，触发中断服务程序。中断服务程序中，PB09输出低电平，
          LED1灭。
          使用杜邦线将PA01脚与PA12脚连接，当按下KEY1时，将触发BTIM1启动计数，同时GPIOA的
          中断服务程序中，PB09输出高电平，LED1亮。
          每按下KEY1一次，LED1亮1s后熄灭。
          LED2常亮作为电压指示等。
          

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
