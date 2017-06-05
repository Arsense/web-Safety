; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyTraceRouteDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyTraceRoute.h"

ClassCount=3
Class1=CMyTraceRouteApp
Class2=CMyTraceRouteDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYTRACEROUTE_DIALOG

[CLS:CMyTraceRouteApp]
Type=0
HeaderFile=MyTraceRoute.h
ImplementationFile=MyTraceRoute.cpp
Filter=N

[CLS:CMyTraceRouteDlg]
Type=0
HeaderFile=MyTraceRouteDlg.h
ImplementationFile=MyTraceRouteDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_SPIN

[CLS:CAboutDlg]
Type=0
HeaderFile=MyTraceRouteDlg.h
ImplementationFile=MyTraceRouteDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYTRACEROUTE_DIALOG]
Type=1
Class=CMyTraceRouteDlg
ControlCount=8
Control1=IDOK,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_HOST,edit,1350631552
Control4=IDC_TRACERT,button,1342242817
Control5=IDC_SPIN,msctls_updown32,1342177312
Control6=IDC_STATIC,static,1342308352
Control7=IDC_ROUTE_COUNT,edit,1350639744
Control8=IDC_RESULT,edit,1353783492

