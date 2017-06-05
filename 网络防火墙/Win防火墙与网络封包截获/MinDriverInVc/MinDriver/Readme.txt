============================================================================

最小化的驱动程序：MinDriver

适应环境：	Windows 2000
作者：		朱雁辉
创建时间：	2001-07-10
Email：		xstudio@371.net
网址：		www.xfilt.com
版权所有：	x 工作室

说明：这个驱动程序是一个最小化的驱动程序，它仅仅只有入口函数和退出函数

============================================================================

测试方法：

1. 双击MinDriver.reg文件导入注册表配置
   并将MinDriver.sys复制到winnt\system32目录下。
2. 重新启动计算机。
3. 运行dbgview.exe(查看调试信息工具，本书没有附带，www.sysinternals.com)。
4. 进入命令行模式。
5. 在命令行模式执行命令：net start mindriver，加载驱动程序
6. 在dbgview.exe的显示区会出现MinDriver.sys输出的调试信息，
   表示入口函数DriverEntry已被调用。
7. 在命令行模式执行命令：net stop mindriver，停止驱动程序
8. Dbgview.exe 会出现MinDriver.sys输出的DriverUnload的调试信息。


