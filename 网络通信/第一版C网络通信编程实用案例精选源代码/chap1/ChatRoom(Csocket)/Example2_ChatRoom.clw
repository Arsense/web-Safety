; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CServerSocket
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Example2_ChatRoom.h"

ClassCount=5
Class1=CExample2_ChatRoomApp
Class2=CExample2_ChatRoomDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CServerSocket
Class5=CClientSocket
Resource3=IDD_EXAMPLE2_CHATROOM_DIALOG

[CLS:CExample2_ChatRoomApp]
Type=0
HeaderFile=Example2_ChatRoom.h
ImplementationFile=Example2_ChatRoom.cpp
Filter=N

[CLS:CExample2_ChatRoomDlg]
Type=0
HeaderFile=Example2_ChatRoomDlg.h
ImplementationFile=Example2_ChatRoomDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CONNECTSERVER

[CLS:CAboutDlg]
Type=0
HeaderFile=Example2_ChatRoomDlg.h
ImplementationFile=Example2_ChatRoomDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_EXAMPLE2_CHATROOM_DIALOG]
Type=1
Class=CExample2_ChatRoomDlg
ControlCount=9
Control1=IDOK,button,1342242816
Control2=IDC_SHOWTEXT,edit,1352730692
Control3=IDC_INPUTTEXT,edit,1350635652
Control4=IDC_IPADDRESS,SysIPAddress32,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_PORT,edit,1350639744
Control8=IDC_CONNECTSERVER,button,1342242817
Control9=IDC_SETSERVER,button,1342242816

[CLS:CServerSocket]
Type=0
HeaderFile=ServerSocket.h
ImplementationFile=ServerSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CServerSocket

[CLS:CClientSocket]
Type=0
HeaderFile=ClientSocket.h
ImplementationFile=ClientSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CClientSocket

