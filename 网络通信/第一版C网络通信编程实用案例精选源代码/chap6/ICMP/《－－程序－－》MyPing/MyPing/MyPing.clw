; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyPingDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyPing.h"

ClassCount=4
Class1=CMyPingApp
Class2=CMyPingDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYPING_DIALOG

[CLS:CMyPingApp]
Type=0
HeaderFile=MyPing.h
ImplementationFile=MyPing.cpp
Filter=N

[CLS:CMyPingDlg]
Type=0
HeaderFile=MyPingDlg.h
ImplementationFile=MyPingDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MyPingDlg.h
ImplementationFile=MyPingDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYPING_DIALOG]
Type=1
Class=CMyPingDlg
ControlCount=6
Control1=IDOK,button,1342242816
Control2=IDC_HOST,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_PING,button,1342242817
Control5=IDC_EDIT2,edit,1353783492
Control6=IDC_CHECK_ROUTE,button,1342242819

