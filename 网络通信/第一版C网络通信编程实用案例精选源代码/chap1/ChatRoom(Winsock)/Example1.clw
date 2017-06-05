; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CExample1Dlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Example1.h"

ClassCount=4
Class1=CExample1App
Class2=CExample1Dlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_EXAMPLE1_DIALOG
Class4=CChangeNameDlg
Resource4=IDD_SETNAME

[CLS:CExample1App]
Type=0
HeaderFile=Example1.h
ImplementationFile=Example1.cpp
Filter=N

[CLS:CExample1Dlg]
Type=0
HeaderFile=Example1Dlg.h
ImplementationFile=Example1Dlg.cpp
Filter=D
LastObject=CExample1Dlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=Example1Dlg.h
ImplementationFile=Example1Dlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_EXAMPLE1_DIALOG]
Type=1
Class=CExample1Dlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_SERVERIP,SysIPAddress32,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PORT,edit,1350639744
Control6=IDC_SHOWTEXT,edit,1352730692
Control7=IDC_INPUTTEXT,edit,1350635652
Control8=IDC_CONNECT,button,1342242817
Control9=IDC_SETSERVER,button,1342242816

[DLG:IDD_SETNAME]
Type=1
Class=CChangeNameDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_NAME,edit,1350631552
Control3=IDC_STATIC,static,1342308352

[CLS:CChangeNameDlg]
Type=0
HeaderFile=ChangeNameDlg.h
ImplementationFile=ChangeNameDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

