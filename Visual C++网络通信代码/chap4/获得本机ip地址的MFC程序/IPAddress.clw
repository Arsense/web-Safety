; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIPAddressDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IPAddress.h"

ClassCount=3
Class1=CIPAddressApp
Class2=CIPAddressDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_IPADDRESS_DIALOG

[CLS:CIPAddressApp]
Type=0
HeaderFile=IPAddress.h
ImplementationFile=IPAddress.cpp
Filter=N

[CLS:CIPAddressDlg]
Type=0
HeaderFile=IPAddressDlg.h
ImplementationFile=IPAddressDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CIPAddressDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=IPAddressDlg.h
ImplementationFile=IPAddressDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_IPADDRESS_DIALOG]
Type=1
Class=CIPAddressDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_HOSTNAME,edit,1342244992
Control5=IDC_IPADDRESS,edit,1342244992

