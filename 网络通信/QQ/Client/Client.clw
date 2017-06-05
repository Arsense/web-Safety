; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Client.h"

ClassCount=4
Class1=CClientApp
Class2=CClientDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_CLIENT_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CLogin
Resource4=IDD_DIALOG_LOGIN

[CLS:CClientApp]
Type=0
HeaderFile=Client.h
ImplementationFile=Client.cpp
Filter=N

[CLS:CClientDlg]
Type=0
HeaderFile=ClientDlg.h
ImplementationFile=ClientDlg.cpp
Filter=D
LastObject=CClientDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ClientDlg.h
ImplementationFile=ClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CLIENT_DIALOG]
Type=1
Class=CClientDlg
ControlCount=20
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC_GROUP,button,1342177287
Control3=IDC_LIST_USER,listbox,1352728835
Control4=IDC_STATIC_SEND,static,1342308352
Control5=IDC_COMBO_USER,combobox,1344340226
Control6=IDC_EDIT_SHOW,edit,1352730756
Control7=IDC_BUTTON_TRANS,button,1342275712
Control8=IDC_BUTTON_SHAKE,button,1342275712
Control9=IDC_CHECK_ONLY,button,1342242819
Control10=IDC_CHECK_ME,button,1342242819
Control11=IDC_EDIT_SEND,edit,1350631552
Control12=IDC_BUTTON_RECV,button,1073840128
Control13=IDC_BUTTON_END,button,1073840128
Control14=IDC_BUTTON_SEND,button,1342242816
Control15=IDC_PROGRESS,msctls_progress32,1082130432
Control16=IDC_STATIC_RECV,static,1073872896
Control17=IDC_STATIC_FILENAME,static,1073872896
Control18=IDC_STATIC_FILESIZE,static,1073872896
Control19=IDC_STATIC,static,1073872896
Control20=IDC_STATIC_SPEED,static,1073872896

[DLG:IDD_DIALOG_LOGIN]
Type=1
Class=CLogin
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_IPADDRESS,SysIPAddress32,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT,edit,1350631552

[CLS:CLogin]
Type=0
HeaderFile=Login.h
ImplementationFile=Login.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_IPADDRESS
VirtualFilter=dWC

