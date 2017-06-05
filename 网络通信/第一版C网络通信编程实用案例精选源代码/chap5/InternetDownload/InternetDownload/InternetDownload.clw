; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInternetDownloadDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "InternetDownload.h"

ClassCount=4
Class1=CInternetDownloadApp
Class2=CInternetDownloadDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_INTERNETDOWNLOAD_DIALOG

[CLS:CInternetDownloadApp]
Type=0
HeaderFile=InternetDownload.h
ImplementationFile=InternetDownload.cpp
Filter=N

[CLS:CInternetDownloadDlg]
Type=0
HeaderFile=InternetDownloadDlg.h
ImplementationFile=InternetDownloadDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=InternetDownloadDlg.h
ImplementationFile=InternetDownloadDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_INTERNETDOWNLOAD_DIALOG]
Type=1
Class=CInternetDownloadDlg
ControlCount=10
Control1=IDOK,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_DOWNLOAD,button,1342242817
Control4=IDC_FILE_URL,edit,1350631552
Control5=IDC_LOCAL_DIRECTORY,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SPEED_TEXT,static,1342308352
Control8=IDC_PERCENT_TEXT,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352

