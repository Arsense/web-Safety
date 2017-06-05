; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFiveChessView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FiveChess.h"
LastPage=0

ClassCount=7
Class1=CFiveChessApp
Class2=CFiveChessDoc
Class3=CFiveChessView
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SETSERVER
Class5=CAboutDlg
Class6=CClientDlg
Class7=CServerDlg
Resource4=IDD_SETCLIENT

[CLS:CFiveChessApp]
Type=0
HeaderFile=FiveChess.h
ImplementationFile=FiveChess.cpp
Filter=N
LastObject=CFiveChessApp

[CLS:CFiveChessDoc]
Type=0
HeaderFile=FiveChessDoc.h
ImplementationFile=FiveChessDoc.cpp
Filter=N
LastObject=CFiveChessDoc

[CLS:CFiveChessView]
Type=0
HeaderFile=FiveChessView.h
ImplementationFile=FiveChessView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CFiveChessView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_START




[CLS:CAboutDlg]
Type=0
HeaderFile=FiveChess.cpp
ImplementationFile=FiveChess.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_SETSERVER
Command3=ID_SETCLIENT
Command4=ID_APP_ABOUT
CommandCount=4

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_SETSERVER]
Type=1
Class=CServerDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PORT,edit,1350639744
Control6=IDC_IP,edit,1350633600

[DLG:IDD_SETCLIENT]
Type=1
Class=CClientDlg
ControlCount=5
Control1=IDC_IPADDRESS,SysIPAddress32,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_SERVERPORT,edit,1350639744
Control5=IDC_BUTTON,button,1342242816

[CLS:CClientDlg]
Type=0
HeaderFile=ClientDlg.h
ImplementationFile=ClientDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CClientDlg
VirtualFilter=dWC

[CLS:CServerDlg]
Type=0
HeaderFile=ServerDlg.h
ImplementationFile=ServerDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CServerDlg
VirtualFilter=dWC

