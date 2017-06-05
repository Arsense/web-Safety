; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMessBox
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SendEMail.h"

ClassCount=5
Class1=CSendEMailApp
Class2=CSendEMailDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_SENDEMAIL_DIALOG
Resource2=IDR_MAINFRAME
Class4=CESocket
Resource3=IDD_ABOUTBOX
Class5=CMessBox
Resource4=IDD_MESSAGE

[CLS:CSendEMailApp]
Type=0
HeaderFile=SendEMail.h
ImplementationFile=SendEMail.cpp
Filter=N

[CLS:CSendEMailDlg]
Type=0
HeaderFile=SendEMailDlg.h
ImplementationFile=SendEMailDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=SendEMailDlg.h
ImplementationFile=SendEMailDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SENDEMAIL_DIALOG]
Type=1
Class=CSendEMailDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDC_SMTP_ADDRESS,edit,1350631552
Control3=IDC_FROM_ADDRESS,edit,1350631552
Control4=IDC_TO_ADDRESS,edit,1350631552
Control5=IDC_EMAIL_CONTENT,edit,1353781444
Control6=ID_HELP,button,1342242817
Control7=ID_SEND,button,1342242817
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=ID_SHOW,button,1342242817

[CLS:CESocket]
Type=0
HeaderFile=ESocket.h
ImplementationFile=ESocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CESocket

[DLG:IDD_MESSAGE]
Type=1
Class=CMessBox
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_MESSAGE,edit,1353777348

[CLS:CMessBox]
Type=0
HeaderFile=MessBox.h
ImplementationFile=MessBox.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_MESSAGE

