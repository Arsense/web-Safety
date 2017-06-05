; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyPortScannerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyPortScanner.h"

ClassCount=3
Class1=CMyPortScannerApp
Class2=CMyPortScannerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYPORTSCANNER_DIALOG

[CLS:CMyPortScannerApp]
Type=0
HeaderFile=MyPortScanner.h
ImplementationFile=MyPortScanner.cpp
Filter=N

[CLS:CMyPortScannerDlg]
Type=0
HeaderFile=MyPortScannerDlg.h
ImplementationFile=MyPortScannerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMyPortScannerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MyPortScannerDlg.h
ImplementationFile=MyPortScannerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYPORTSCANNER_DIALOG]
Type=1
Class=CMyPortScannerDlg
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDC_IPADDRESS_BEGIN,SysIPAddress32,1342242816
Control3=IDC_IPADDRESS_END,SysIPAddress32,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_PORT_BEGIN,edit,1350639744
Control8=IDC_PORT_END,edit,1350639744
Control9=IDC_BEGIN_SCAN,button,1342242816
Control10=IDC_STOP_SCAN,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_THREAD_NUM,edit,1350639744
Control13=IDC_RESULT,edit,1353779396
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,button,1342177287

