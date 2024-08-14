示例目的：
          演示定时器外部触发启动ADC转换，定时1s采样芯片温度。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. GTIM1的时钟源为PCLK，256分频
          5. PB09设置为PUSH-PULL输出，驱动LED1
          6. ADC的时钟源为PCLK，2分频

演示说明：
          通用定时器GTIM1定时1s自动触发ADC模块进行转换，ADC通道为MCU内部温度传感器TS。
          LED1将0.5Hz的频率闪烁。

使用说明：
+ EWARM
          1. 打开adc_sgl_ext_trig.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sgl_ext_trig.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)