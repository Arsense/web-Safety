; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHostNameDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Neighbor.h"

ClassCount=6
Class1=CNeighborApp
Class2=CNeighborDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_NEIGHBOR_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_DIRECTDRIVER
Class4=CDirectDriver
Resource4=IDD_LOCALNAME
Class5=CLocalNameDlg
Resource5=IDD_ABOUTBOX
Class6=CHostNameDlg
Resource6=IDD_HOSTNAME

[CLS:CNeighborApp]
Type=0
HeaderFile=Neighbor.h
ImplementationFile=Neighbor.cpp
Filter=N

[CLS:CNeighborDlg]
Type=0
HeaderFile=NeighborDlg.h
ImplementationFile=NeighborDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CNeighborDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=NeighborDlg.h
ImplementationFile=NeighborDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342308480
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_NEIGHBOR_DIALOG]
Type=1
Class=CNeighborDlg
ControlCount=18
Control1=IDC_LISTBUTTON,button,1342242816
Control2=IDC_NAVIGATOR,button,1342242816
Control3=IDC_REDIRECT,button,1342242816
Control4=IDC_DISCONNECTDIRECT,button,1342242816
Control5=IDC_IPBUTTON,button,1342242816
Control6=IDC_IPTOHOSTNAME,button,1342242816
Control7=IDC_NETSEND,button,1342242816
Control8=IDC_MESSAGE,edit,1350631620
Control9=IDOK,button,1342242817
Control10=IDC_LIST,listbox,1353777409
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_SEND_ADDRESS,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352

[DLG:IDD_DIRECTDRIVER]
Type=1
Class=CDirectDriver
ControlCount=9
Control1=IDC_REMOTENAME,edit,1350631552
Control2=IDC_LOCALNAME,edit,1350631552
Control3=IDC_USERNAME,edit,1350631552
Control4=IDC_PASSWORD,edit,1350631552
Control5=IDOK,button,1342242817
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[CLS:CDirectDriver]
Type=0
HeaderFile=DirectDriver.h
ImplementationFile=DirectDriver.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDirectDriver

[DLG:IDD_LOCALNAME]
Type=1
Class=CLocalNameDlg
ControlCount=3
Control1=IDC_EDIT1,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDC_STATIC,static,1342308352

[CLS:CLocalNameDlg]
Type=0
HeaderFile=LocalNameDlg.h
ImplementationFile=LocalNameDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CLocalNameDlg

[DLG:IDD_HOSTNAME]
Type=1
Class=CHostNameDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATICEDIT,static,1342308352

[CLS:CHostNameDlg]
Type=0
HeaderFile=HostNameDlg.h
ImplementationFile=HostNameDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CHostNameDlg

