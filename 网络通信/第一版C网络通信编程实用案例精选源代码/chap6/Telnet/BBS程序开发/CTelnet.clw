; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHostDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CTelnet.h"
LastPage=0

ClassCount=5
Class1=CCTelnetApp
Class2=CCTelnetDoc
Class3=CHostDialog
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDD_HOST
Class5=CAboutDlg
Resource3=IDR_MAINFRAME
Resource4=IDD_DIALOG1

[CLS:CCTelnetApp]
Type=0
HeaderFile=CTelnet.h
ImplementationFile=CTelnet.cpp
Filter=N

[CLS:CCTelnetDoc]
Type=0
HeaderFile=CTelnetDoc.h
ImplementationFile=CTelnetDoc.cpp
Filter=N

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_SET_FONT
BaseClass=CFrameWnd
VirtualFilter=fWC



[CLS:CAboutDlg]
Type=0
HeaderFile=CTelnet.cpp
ImplementationFile=CTelnet.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308480
Control2=IDOK,button,1342373889
Control3=IDR_MAINFRAME,static,1342177283

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_SET_FRONT_COLOR
Command6=ID_SET_BACK_COLOR
Command7=ID_SET_FONT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
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
Command2=ID_SET_BACK_COLOR
Command3=ID_SET_FRONT_COLOR
Command4=ID_SET_FONT
Command5=ID_APP_ABOUT
CommandCount=5

[DLG:IDD_HOST]
Type=1
Class=CHostDialog
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308866
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PORT,edit,1350631552

[CLS:CHostDialog]
Type=0
HeaderFile=HostDialog.h
ImplementationFile=HostDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PORT

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

