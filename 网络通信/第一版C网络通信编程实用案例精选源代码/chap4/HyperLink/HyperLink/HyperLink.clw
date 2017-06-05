; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHyperLinkDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "HyperLink.h"

ClassCount=4
Class1=CHyperLinkApp
Class2=CHyperLinkDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CHyperLinker
Resource3=IDD_HYPERLINK_DIALOG

[CLS:CHyperLinkApp]
Type=0
HeaderFile=HyperLink.h
ImplementationFile=HyperLink.cpp
Filter=N

[CLS:CHyperLinkDlg]
Type=0
HeaderFile=HyperLinkDlg.h
ImplementationFile=HyperLinkDlg.cpp
Filter=D
LastObject=IDC_LINKER
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=HyperLinkDlg.h
ImplementationFile=HyperLinkDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_HYPERLINK_DIALOG]
Type=1
Class=CHyperLinkDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_LINKER,static,1342308352
Control3=IDC_NET,static,1342308352
Control4=IDC_LOCALFILE,static,1342308352

[CLS:CHyperLinker]
Type=0
HeaderFile=HyperLinker.h
ImplementationFile=HyperLinker.cpp
BaseClass=CStatic
Filter=W
LastObject=CHyperLinker

