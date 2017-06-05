; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAccountDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyFtpServer.h"

ClassCount=4
Class1=CMyFtpServerApp
Class2=CMyFtpServerDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYFTPSERVER_DIALOG
Class4=CAccountDialog
Resource4=IDD_ACCOUNT

[CLS:CMyFtpServerApp]
Type=0
HeaderFile=MyFtpServer.h
ImplementationFile=MyFtpServer.cpp
Filter=N

[CLS:CMyFtpServerDlg]
Type=0
HeaderFile=MyFtpServerDlg.h
ImplementationFile=MyFtpServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMyFtpServerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MyFtpServerDlg.h
ImplementationFile=MyFtpServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYFTPSERVER_DIALOG]
Type=1
Class=CMyFtpServerDlg
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDC_ACCOUNT_LIST,listbox,1353777409
Control3=IDC_STATIC,button,1342177287
Control4=IDC_ADD_ACCOUNT,button,1342242816
Control5=IDC_DEL_ACCOUNT,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342177287
Control8=IDC_PORT,edit,1350639744
Control9=IDC_STATIC,static,1342308352
Control10=IDC_REFRESH_PORT,button,1342242816
Control11=IDC_STATIC,button,1342177287
Control12=IDC_DEFAULT_DIRECTORY,edit,1350631552
Control13=IDC_REFRESH_DEFAULTDIRECTORY,button,1342242816
Control14=IDC_START_SERVER,button,1342242816
Control15=IDC_ACCOUNT_INFOR,button,1342242816
Control16=IDC_DATALOG,edit,1353781444
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,button,1342177287
Control19=IDC_WELCOME,edit,1353781444

[DLG:IDD_ACCOUNT]
Type=1
Class=CAccountDialog
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_USER,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PASSWORD,edit,1350631584
Control6=IDC_STATIC,static,1342308352
Control7=IDC_DIRECTORY,edit,1350631552

[CLS:CAccountDialog]
Type=0
HeaderFile=AccountDialog.h
ImplementationFile=AccountDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

