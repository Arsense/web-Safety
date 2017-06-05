; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyTapiDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyTapi.h"

ClassCount=4
Class1=CMyTapiApp
Class2=CMyTapiDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYTAPI_DIALOG

[CLS:CMyTapiApp]
Type=0
HeaderFile=MyTapi.h
ImplementationFile=MyTapi.cpp
Filter=N

[CLS:CMyTapiDlg]
Type=0
HeaderFile=MyTapiDlg.h
ImplementationFile=MyTapiDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MyTapiDlg.h
ImplementationFile=MyTapiDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYTAPI_DIALOG]
Type=1
Class=CMyTapiDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDC_PHONE_NUM,edit,1350631552
Control3=IDC_ASSIT_PHONE,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BASIC_PHONE,button,1342242816
Control6=IDC_HANG_UP,button,1342242816
Control7=IDC_ZIP_CODE,static,1342308352

