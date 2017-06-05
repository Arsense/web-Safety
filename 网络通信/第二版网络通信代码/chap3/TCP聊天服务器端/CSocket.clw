; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCSocketDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CSocket.h"

ClassCount=3
Class1=CCSocketApp
Class2=CCSocketDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CSOCKET_DIALOG

[CLS:CCSocketApp]
Type=0
HeaderFile=CSocket.h
ImplementationFile=CSocket.cpp
Filter=N

[CLS:CCSocketDlg]
Type=0
HeaderFile=CSocketDlg.h
ImplementationFile=CSocketDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=CSocketDlg.h
ImplementationFile=CSocketDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CSOCKET_DIALOG]
Type=1
Class=CCSocketDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_LIST1,SysListView32,1350631425
Control4=IDC_BUTTON1,button,1342242817

