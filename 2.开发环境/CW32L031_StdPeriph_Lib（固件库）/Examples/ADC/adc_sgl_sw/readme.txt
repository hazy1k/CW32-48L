示例目的：
          演示软件启动ADC单通道转换，支持单通道单次、多次、连续等3种转换模式。

硬件资源：
          1. CW32L031CxTx StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ADC的时钟源为PCLK，1或者4分频
          5. PB08/09设置为PUSH-PULL输出

演示说明：
          ADC通道为AIN1:PA01。
          ADC转换完成，PB09输出电平翻转, LED1闪烁。
          多次转换模式时，如果实际完成转换次数与设定值不等时，LED1和LED2全部点亮。
          注意：修改main.h中的转换模式宏定义，即可实现各种转换模式的测试。

使用说明：
+ EWARM
          1. 打开adc_sgl_sw.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sgl_sw.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)