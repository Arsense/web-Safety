; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEWDPingDlg
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EWDPing.h"

ClassCount=11
Class1=CEWDPingApp
Class2=CEWDPingDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_EWDPING_DIALOG
Class4=CIcmp
Class5=CPingOptionsDlg
Class6=CRegistry
Class7=RegKey
Class8=CTraceList
Class9=SysTCPIP
Class10=CNameServ
Class11=NSLookUp
Resource4=IDD_PingOptions

[CLS:CEWDPingApp]
Type=0
HeaderFile=EWDPing.h
ImplementationFile=EWDPing.cpp
Filter=N

[CLS:CEWDPingDlg]
Type=0
HeaderFile=EWDPingDlg.h
ImplementationFile=EWDPingDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_Stop

[CLS:CAboutDlg]
Type=0
HeaderFile=EWDPingDlg.h
ImplementationFile=EWDPingDlg.cpp
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

[DLG:IDD_EWDPING_DIALOG]
Type=1
Class=CEWDPingDlg
ControlCount=13
Control1=IDC_PingButton,button,1342242817
Control2=IDC_TraceButton,button,1342242816
Control3=IDC_ClearButton,button,1342242816
Control4=IDC_Stop,button,1342242816
Control5=IDC_DEST,edit,1350631552
Control6=IDC_OptionsButton,button,1342242816
Control7=IDQUIT,button,1342242816
Control8=IDC_TraceList,SysListView32,1350565889
Control9=IDC_STATIC,static,1342308352
Control10=IDC_CommLink,static,1342177283
Control11=IDC_LocalHost,static,1342308352
Control12=IDC_NameServer,static,1342308352
Control13=IDC_STATIC,static,1342308352

[CLS:CIcmp]
Type=0
HeaderFile=Icmp.h
ImplementationFile=Icmp.cpp
BaseClass=CSocket
Filter=N

[DLG:IDD_PingOptions]
Type=1
Class=CPingOptionsDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_DefaultHost,edit,1350631552
Control5=IDC_STATIC,static,1342308354
Control6=IDC_BufferSize,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308354
Control9=IDC_PingTimeout,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308354
Control12=IDC_MaxHops,edit,1350631552

[CLS:CPingOptionsDlg]
Type=0
HeaderFile=PingOptionsDlg.h
ImplementationFile=PingOptionsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_MaxHops

[CLS:CRegistry]
Type=0
HeaderFile=Registry.h
ImplementationFile=Registry.cpp
BaseClass=generic CWnd
Filter=W

[CLS:RegKey]
Type=0
HeaderFile=RegKey.h
ImplementationFile=RegKey.cpp
BaseClass=generic CWnd
Filter=W

[CLS:CTraceList]
Type=0
HeaderFile=TraceList.h
ImplementationFile=TraceList.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CTraceList

[CLS:SysTCPIP]
Type=0
HeaderFile=SysTCPIP.h
ImplementationFile=SysTCPIP.cpp
BaseClass=generic CWnd
Filter=W

[CLS:CNameServ]
Type=0
HeaderFile=NameServ.h
ImplementationFile=NameServ.cpp
BaseClass=generic CWnd
Filter=W

[CLS:NSLookUp]
Type=0
HeaderFile=NSLookUp.h
ImplementationFile=NSLookUp.cpp
BaseClass=generic CWnd
Filter=W

