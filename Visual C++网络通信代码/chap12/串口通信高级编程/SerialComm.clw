; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSerialCommDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SerialComm.h"

ClassCount=3
Class1=CSerialCommApp
Class2=CSerialCommDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SERIALCOMM_DIALOG

[CLS:CSerialCommApp]
Type=0
HeaderFile=SerialComm.h
ImplementationFile=SerialComm.cpp
Filter=N

[CLS:CSerialCommDlg]
Type=0
HeaderFile=SerialCommDlg.h
ImplementationFile=SerialCommDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_SEND

[CLS:CAboutDlg]
Type=0
HeaderFile=SerialCommDlg.h
ImplementationFile=SerialCommDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERIALCOMM_DIALOG]
Type=1
Class=?
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_OPEN,button,1342242816
Control4=IDC_WRITE,button,1342242816
Control5=IDC_READAVAILABLE,button,1342242816
Control6=IDC_SEND,edit,1352728580
Control7=IDC_READ_1,button,1342242816
Control8=IDC_READ_10,button,1342242816
Control9=IDC_READ_UPTO,button,1342242816
Control10=IDC_DATARX,edit,1351682052

