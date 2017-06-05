; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPortScanView
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PortScan.h"
LastPage=0

ClassCount=7
Class1=CPortScanApp
Class2=CPortScanDoc
Class3=CPortScanView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Class6=CTheSocket
Class7=CSplashWnd
Resource3=IDD_PORTSCAN_FORM

[CLS:CPortScanApp]
Type=0
HeaderFile=PortScan.h
ImplementationFile=PortScan.cpp
Filter=N
LastObject=ID_APP_ABOUT

[CLS:CPortScanDoc]
Type=0
HeaderFile=PortScanDoc.h
ImplementationFile=PortScanDoc.cpp
Filter=N

[CLS:CPortScanView]
Type=0
HeaderFile=PortScanView.h
ImplementationFile=PortScanView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=CPortScanView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=PortScan.cpp
ImplementationFile=PortScan.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_SAVE
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_PORTSCAN_FORM]
Type=1
Class=CPortScanView
ControlCount=17
Control1=IDC_IP_ADDRESS,SysIPAddress32,1342242816
Control2=IDC_RADIO_SINGLE,button,1342177289
Control3=IDC_EDIT_SINGLE_PORT,edit,1350639744
Control4=IDC_RADIO_RANGE,button,1342177289
Control5=IDC_EDIT_SINGLE_PORT_FROM,edit,1350639744
Control6=IDC_EDIT_SINGLE_PORT_TO,edit,1350639744
Control7=IDC_EDIT_ATTEMPTS,edit,1350639744
Control8=IDC_BUTTON_SCAN,button,1342242817
Control9=IDC_BUTTON_STOP,button,1342242816
Control10=IDC_LIST_RESULT,SysListView32,1350631425
Control11=IDC_STATIC_IP,static,1342308352
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC_DUE,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_PROGRESS,msctls_progress32,1350565888

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_SAVE
Command2=ID_APP_ABOUT
CommandCount=2

[CLS:CTheSocket]
Type=0
HeaderFile=TheSocket.h
ImplementationFile=TheSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CTheSocket

[CLS:CSplashWnd]
Type=0
HeaderFile=Splash.h
ImplementationFile=Splash.cpp
BaseClass=CWnd
LastObject=CSplashWnd

