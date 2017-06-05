; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFTPSOCKETDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FTPSOCKET.h"

ClassCount=3
Class1=CFTPSOCKETApp
Class2=CFTPSOCKETDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_FTPSOCKET_DIALOG

[CLS:CFTPSOCKETApp]
Type=0
HeaderFile=FTPSOCKET.h
ImplementationFile=FTPSOCKET.cpp
Filter=N

[CLS:CFTPSOCKETDlg]
Type=0
HeaderFile=FTPSOCKETDlg.h
ImplementationFile=FTPSOCKETDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CHECK_PASV

[CLS:CAboutDlg]
Type=0
HeaderFile=FTPSOCKETDlg.h
ImplementationFile=FTPSOCKETDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_FTPSOCKET_DIALOG]
Type=1
Class=CFTPSOCKETDlg
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBOBOXEX_LOCAL,ComboBoxEx32,1344340226
Control4=IDC_LIST_LOCALFILE,SysListView32,1350631937
Control5=IDC_COMBOBOXEX_SERVER,ComboBoxEx32,1344340226
Control6=IDC_LIST_SERVERFILE,SysListView32,1350631937
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_SERVER,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_PORT,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_UID,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_PWD,edit,1350631552
Control16=IDC_STATIC,button,1342177287
Control17=IDC_EDIT_MESSAGE,edit,1352728580
Control18=IDC_BUTTON_CONNECT,button,1342242816
Control19=IDC_CHECK_PASV,button,1342242819

