; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMailPostDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MailPost.h"

ClassCount=3
Class1=CMailPostApp
Class2=CMailPostDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MAILPOST_DIALOG

[CLS:CMailPostApp]
Type=0
HeaderFile=MailPost.h
ImplementationFile=MailPost.cpp
Filter=N

[CLS:CMailPostDlg]
Type=0
HeaderFile=MailPostDlg.h
ImplementationFile=MailPostDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMailPostDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MailPostDlg.h
ImplementationFile=MailPostDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MAILPOST_DIALOG]
Type=1
Class=CMailPostDlg
ControlCount=16
Control1=IDC_RECEIVE,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308866
Control4=IDC_ADDRESS,edit,1350631552
Control5=IDC_STATIC,static,1342308866
Control6=IDC_STATIC,static,1342308866
Control7=IDC_USERNAME,edit,1350631552
Control8=IDC_PASSWORD,edit,1350631584
Control9=IDC_STATIC,static,1342308353
Control10=IDC_EDIT_TEXT,edit,1353779396
Control11=IDC_LIST_MAIL,SysListView32,1350633473
Control12=IDC_DELETE,button,1342242816
Control13=IDC_REVERSE,button,1342242816
Control14=IDC_STATIC,static,1342308866
Control15=IDC_PORT,edit,1350631553
Control16=IDC_DISCONNECT,button,1342242816

