; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMSCOMMDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MSCOMM.h"

ClassCount=4
Class1=CMSCOMMApp
Class2=CMSCOMMDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_MSCOMM_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CConfigureDlg
Resource4=IDD_PARAMETER

[CLS:CMSCOMMApp]
Type=0
HeaderFile=MSCOMM.h
ImplementationFile=MSCOMM.cpp
Filter=N

[CLS:CMSCOMMDlg]
Type=0
HeaderFile=MSCOMMDlg.h
ImplementationFile=MSCOMMDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_MSCOMM1

[CLS:CAboutDlg]
Type=0
HeaderFile=MSCOMMDlg.h
ImplementationFile=MSCOMMDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MSCOMM_DIALOG]
Type=1
Class=CMSCOMMDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control3=IDC_START_RECEIVE,button,1342242816
Control4=IDC_STOP_RECEIVE,button,1342242816
Control5=IDC_SEND_DATA,button,1342242816
Control6=IDC_SENDDATA_EDIT,edit,1350631552
Control7=IDC_RECEIVEDATA_EDIT,edit,1484849284
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,button,1342177287
Control10=IDC_SET_COMM,button,1342242816

[DLG:IDD_PARAMETER]
Type=1
Class=CConfigureDlg
ControlCount=25
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT4,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT5,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT6,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT7,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT8,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT9,edit,1350631552
Control20=IDC_STATIC,static,1342308352
Control21=IDC_EDIT10,edit,1350631552
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT11,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT12,edit,1350631552

[CLS:CConfigureDlg]
Type=0
HeaderFile=ConfigureDlg.h
ImplementationFile=ConfigureDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

