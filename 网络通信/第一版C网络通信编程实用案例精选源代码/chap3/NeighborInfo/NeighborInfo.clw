; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNeighborInfoDlg
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NeighborInfo.h"

ClassCount=4
Class1=CNeighborInfoApp
Class2=CNeighborInfoDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CMessSocket
Resource3=IDD_NEIGHBORINFO_DIALOG

[CLS:CNeighborInfoApp]
Type=0
HeaderFile=NeighborInfo.h
ImplementationFile=NeighborInfo.cpp
Filter=N

[CLS:CNeighborInfoDlg]
Type=0
HeaderFile=NeighborInfoDlg.h
ImplementationFile=NeighborInfoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LISTINFOR

[CLS:CAboutDlg]
Type=0
HeaderFile=NeighborInfoDlg.h
ImplementationFile=NeighborInfoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_NEIGHBORINFO_DIALOG]
Type=1
Class=CNeighborInfoDlg
ControlCount=9
Control1=IDOK,button,1342242816
Control2=IDC_IPADDRESSSTART,SysIPAddress32,1342242816
Control3=IDC_IPADDRESSEND,SysIPAddress32,1342242816
Control4=IDC_BEGINSCAN,button,1342242817
Control5=IDC_STOPSCAN,button,1342242816
Control6=IDC_LISTINFOR,SysListView32,1350631424
Control7=IDC_MYHELP,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_EMPTY,button,1342242816

[CLS:CMessSocket]
Type=0
HeaderFile=MessSocket.h
ImplementationFile=MessSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CMessSocket

