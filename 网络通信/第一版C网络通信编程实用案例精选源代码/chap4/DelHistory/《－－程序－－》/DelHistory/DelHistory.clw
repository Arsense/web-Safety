; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDelHistoryDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DelHistory.h"

ClassCount=4
Class1=CDelHistoryApp
Class2=CDelHistoryDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DELHISTORY_DIALOG

[CLS:CDelHistoryApp]
Type=0
HeaderFile=DelHistory.h
ImplementationFile=DelHistory.cpp
Filter=N

[CLS:CDelHistoryDlg]
Type=0
HeaderFile=DelHistoryDlg.h
ImplementationFile=DelHistoryDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=DelHistoryDlg.h
ImplementationFile=DelHistoryDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DELHISTORY_DIALOG]
Type=1
Class=CDelHistoryDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDC_DEL_HISTORY,button,1342242816
Control3=IDC_DEL_INTERNET_TEMP_FILE,button,1342242816
Control4=IDC_DEL_HISTORY_ADDRESS,button,1342242816
Control5=IDC_DEL_HISTORY_PASSWORD,button,1342242816
Control6=IDC_DEL_FAVORITE,button,1342242816
Control7=IDC_DEL_COOKIES,button,1342242816
Control8=IDC_BUTTON_HELP,button,1342242816

