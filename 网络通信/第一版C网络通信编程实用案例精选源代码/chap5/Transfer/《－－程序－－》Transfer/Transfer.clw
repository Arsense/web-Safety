; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTransferSocket
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Transfer.h"

ClassCount=5
Class1=CTransferApp
Class2=CTransferDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CServerSocket
Class5=CTransferSocket
Resource3=IDD_TRANSFER_DIALOG

[CLS:CTransferApp]
Type=0
HeaderFile=Transfer.h
ImplementationFile=Transfer.cpp
Filter=N

[CLS:CTransferDlg]
Type=0
HeaderFile=TransferDlg.h
ImplementationFile=TransferDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=TransferDlg.h
ImplementationFile=TransferDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TRANSFER_DIALOG]
Type=1
Class=CTransferDlg
ControlCount=17
Control1=IDOK,button,1342242817
Control2=IDC_ADDSHAREFILE,button,1342242816
Control3=IDC_DELSHAREFILE,button,1342242816
Control4=IDC_RADIO1,button,1342177289
Control5=IDC_RADIO2,button,1342177289
Control6=IDC_IPADDRESS,SysIPAddress32,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_PORT,edit,1350639744
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,button,1342177287
Control12=IDC_DOWNLOADSHAREFILE,button,1342242816
Control13=IDC_STATIC,button,1342177287
Control14=IDC_LOCAL_FILE_LIST,listbox,1353781515
Control15=IDC_REMOTE_FILE_LIST,listbox,1353781515
Control16=IDC_SETNETWORK,button,1342242816
Control17=IDC_BUTTON4,button,1342242816

[CLS:CServerSocket]
Type=0
HeaderFile=ServerSocket.h
ImplementationFile=ServerSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CServerSocket

[CLS:CTransferSocket]
Type=0
HeaderFile=TransferSocket.h
ImplementationFile=TransferSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CTransferSocket

