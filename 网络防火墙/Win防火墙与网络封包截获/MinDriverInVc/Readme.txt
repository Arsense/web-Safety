
在 VC 环境下编译驱动程序(MinDriver)的示例
========================================================

系统需求：Windows 2000
开发工具：VC6.0
DDK 工具：DDK For Windows 2000

编译方法：
--------------------------------------------------------
1. 利用 VC 打开工作区文件MinDriverInVc.dsw
2.“Project”菜单->“Settings”菜单，弹出Project Settings对话框
3. 将 Build Command Line 的最后两个参数“d: ” 和
  “D:\tony\Xfilter_Disk\DdkSource\MinDriverInVc\MinDriver”
   修改为正确的路径(源代码所在的盘符和路径)
4. 按 Build 按钮编译驱动程序