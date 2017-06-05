; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CStopPopupDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "StopPopup.h"

ClassCount=4
Class1=CStopPopupApp
Class2=CStopPopupDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_STOPPOPUP_DIALOG

[CLS:CStopPopupApp]
Type=0
HeaderFile=StopPopup.h
ImplementationFile=StopPopup.cpp
Filter=N

[CLS:CStopPopupDlg]
Type=0
HeaderFile=StopPopupDlg.h
ImplementationFile=StopPopupDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=StopPopupDlg.h
ImplementationFile=StopPopupDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_STOPPOPUP_DIALOG]
Type=1
Class=CStopPopupDlg
ControlCount=2
Control1=IDC_STOP,button,1342242816
Control2=IDC_GO,button,1342242816

