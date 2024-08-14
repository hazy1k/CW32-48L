用于EWARM工具的单片机Flash烧写配置文件，请将该文件下的arm文件加拷贝到EWARM的安装目录下。

1. 拷贝arm文件到EWARM的安装路径：
    如EWARM安装路径为D:\Program Files (x86)\IAR Systems\Embedded Workbench 8.2
    则将arm文件夹拷贝到D:\Program Files (x86)\IAR Systems\Embedded Workbench 8.2\目录下，覆盖同名的arm文件夹。

2. 使用：
    1) EWARM界面->"Project"->"Options..."->"Linker"->"Config"->在显示的标签栏中选择“Override default"，点击"..."在弹出的对话框中选择"CW32l031.icf"->点击"打开"
    2)EWARM界面->"Project"->"Options..."->"Debugger"->"Download"->在显示的标签栏中选择“Override default .board file"，点击"..."在弹出的对话框中选择"WHXY"文件夹下的"FlashLoader_CW32L031xxxxRAM24K.board"->点击"打开"

