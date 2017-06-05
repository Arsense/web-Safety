; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutHelpDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "modem.h"

ClassCount=3
Class1=CModemApp
Class2=CModemDlg

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDD_MODEM_DIALOG (Chinese (P.R.C.))
Resource3=IDD_MODEM_DIALOG
Class3=CAboutHelpDlg
Resource4=IDD_DIALOGHELP

[CLS:CModemApp]
Type=0
HeaderFile=modem.h
ImplementationFile=modem.cpp
Filter=N
LastObject=CModemApp

[CLS:CModemDlg]
Type=0
HeaderFile=modemDlg.h
ImplementationFile=modemDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CModemDlg



[DLG:IDD_MODEM_DIALOG (Chinese (P.R.C.))]
Type=1
Class=CModemDlg
ControlCount=11
Control1=IDC_EDIT1,edit,1350631552
Control2=IDcall,button,1342242817
Control3=IDABOUT,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDhangup,button,1342242816
Control6=IDC_EDIT2,edit,1342244992
Control7=IDC_STATIC,static,1342177283
Control8=IDC_EDIT3,edit,1342244992
Control9=IDC_EDIT4,edit,1342244992
Control10=IDC_PLAY,button,1342242816
Control11=IDC_STOP,button,1342242816

[DLG:IDD_MODEM_DIALOG]
Type=1
Class=CModemDlg
ControlCount=9
Control1=IDC_EDIT1,edit,1350639744
Control2=IDcall,button,1342242817
Control3=ID_OK,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=ID_HANGUP,button,1342242816
Control6=IDC_EDIT2,edit,1342244992
Control7=IDC_EDIT3,edit,1342244992
Control8=IDC_EDIT4,edit,1342244992
Control9=ID_HELP,button,1342242816

[DLG:IDD_DIALOGHELP]
Type=1
Class=CAboutHelpDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342308352

[CLS:CAboutHelpDlg]
Type=0
HeaderFile=AboutHelpDlg.h
ImplementationFile=AboutHelpDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAboutHelpDlg

