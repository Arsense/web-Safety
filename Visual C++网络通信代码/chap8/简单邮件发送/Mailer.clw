; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMailerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Mailer.h"

ClassCount=2
Class1=CMailerApp
Class2=CMailerDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_MAILER_DIALOG

[CLS:CMailerApp]
Type=0
HeaderFile=Mailer.h
ImplementationFile=Mailer.cpp
Filter=N

[CLS:CMailerDlg]
Type=0
HeaderFile=MailerDlg.h
ImplementationFile=MailerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_MESSAGE



[DLG:IDD_MAILER_DIALOG]
Type=1
Class=CMailerDlg
ControlCount=10
Control1=IDOK,button,1342275584
Control2=IDCANCEL,button,1342275584
Control3=IDC_STATIC,static,1342308363
Control4=IDC_EDIT_SERVER,edit,1342242944
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_FROM,edit,1342242944
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_TO,edit,1342242944
Control9=IDC_EDIT_MESSAGE,edit,1344340100
Control10=IDC_ABOUT,button,1342275584

