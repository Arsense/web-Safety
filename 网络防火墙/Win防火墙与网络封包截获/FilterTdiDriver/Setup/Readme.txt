============================================================================

传输层过滤驱动：FilterTdiDriver

适应环境：	Windows 2000
作者：		朱雁辉
创建时间：	2001-07-10
Email：		xstudio@371.net、xstudio@xfilt.com
网址：		http://www.xfilt.com
版权所有：	x 工作室

说明：传输层过滤驱动程序，用它截获网络封包，这个例子直接将封包转发到了底层。

============================================================================

测试方法：

1. 双击FilterTdiDriver.reg文件导入注册表配置
   并将FilterTdiDriver.sys复制到winnt\system32目录下。
2. 重新启动计算机。
3. 运行dbgview.exe(查看调试信息工具，本书没有附带，www.sysinternals.com)。
4. 进入命令行模式。
5. 在命令行模式执行命令：net start filtertdidriver，加载驱动程序
6. 在dbgview.exe的显示区会出现filtertdidriver.sys输出的调试信息，
   表示入口函数DriverEntry以被调用。
7. 启动IE或者其它网络程序，发出上网动作。
8. 通过Dbgview来查看调试信息，会发现封包传输函数已被截获
9. 在命令行模式执行命令：net stop filtertdidriver，停止驱动程序
10.Dbgview.exe 会出现filtertdidriver.sys输出的DriverUnload的调试信息。


