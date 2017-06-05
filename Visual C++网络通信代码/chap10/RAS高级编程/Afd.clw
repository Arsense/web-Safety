; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPage1
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "afd.h"
LastPage=0

ClassCount=12
Class1=CAfdApp
Class2=CAfdDlg
Class3=CAFDialPanel
Class4=CFaceWnd
Class5=CHyperLink
Class6=CPage1
Class7=CPage2
Class8=CPage3
Class9=CPage4
Class10=CPage5
Class11=CPhoneNumberDlg
Class12=CRunProgDlg

ResourceCount=7
Resource1=IDR_TRAY_MENU1
Resource2=IDD_DIALOG2
Resource3=IDD_PHONE_NUMBER
Resource4=IDD_DIALOG3
Resource5=IDD_DIALOG4
Resource6=IDD_DIALOG1
Resource7=IDD_AFD_DIALOG

[CLS:CAfdApp]
Type=0
BaseClass=CWinApp
HeaderFile=Afd.h
ImplementationFile=Afd.cpp
LastObject=IDC_FACE_FRAME

[CLS:CAfdDlg]
Type=0
BaseClass=CDialog
HeaderFile=AfdDlg.h
ImplementationFile=AfdDlg.cpp

[CLS:CAFDialPanel]
Type=0
BaseClass=CStatic
HeaderFile=AFDialPanel.h
ImplementationFile=AFDialPanel.cpp

[CLS:CFaceWnd]
Type=0
BaseClass=CStatic
HeaderFile=FaceWnd.h
ImplementationFile=FaceWnd.cpp

[CLS:CHyperLink]
Type=0
BaseClass=CStatic
HeaderFile=HyperLink.h
ImplementationFile=HyperLink.cpp

[CLS:CPage1]
Type=0
BaseClass=CPropertyPage
HeaderFile=Page1.h
ImplementationFile=Page1.cpp
LastObject=IDC_LIST_PHONES
Filter=D
VirtualFilter=idWC

[CLS:CPage2]
Type=0
BaseClass=CPropertyPage
HeaderFile=Page2.h
ImplementationFile=Page2.cpp

[CLS:CPage3]
Type=0
BaseClass=CPropertyPage
HeaderFile=Page3.h
ImplementationFile=Page3.cpp

[CLS:CPage4]
Type=0
BaseClass=CPropertyPage
HeaderFile=Page4.h
ImplementationFile=Page4.cpp

[CLS:CPage5]
Type=0
BaseClass=CPropertyPage
HeaderFile=Page5.h
ImplementationFile=Page5.cpp
Filter=D
VirtualFilter=idWC

[CLS:CPhoneNumberDlg]
Type=0
BaseClass=CDialog
HeaderFile=PhoneNumberDlg.h
ImplementationFile=PhoneNumberDlg.cpp

[CLS:CRunProgDlg]
Type=0
BaseClass=CDialog
HeaderFile=RunProgDlg.h
ImplementationFile=RunProgDlg.cpp

[DLG:IDD_AFD_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDC_HIDE,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDOK,button,1073807361

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=16
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COMBO_PROVIDER,combobox,1344340227
Control3=IDC_BUTTON_PROV_ADD,button,1342242816
Control4=IDC_BUTTON_PROV_EDIT,button,1342242816
Control5=IDC_BUTTON_PROV_DEL,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_LIST_PHONES,SysListView32,1342275597
Control8=IDC_BUTTON_TEL_ADD,button,1342242816
Control9=IDC_BUTTON_TEL_EDIT,button,1342242816
Control10=IDC_BUTTON_TEL_DEL,button,1342242816
Control11=IDC_DIAL,button,1342242816
Control12=IDC_STATIC,static,1342308865
Control13=IDC_STATUS,static,1342308352
Control14=IDC_STATIC,static,1342308865
Control15=IDC_LAST_CALL,static,1342308352
Control16=IDC_PROGRESS_TIMEOUT,msctls_progress32,1350565888

[DLG:IDD_DIALOG2]
Type=1
Class=?
ControlCount=7
Control1=IDC_STATIC_NAME,static,1342308865
Control2=IDC_STATIC,static,1342308866
Control3=IDC_EDIT_USER_NAME,edit,1350631552
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308866
Control6=IDC_EDIT_PASSWORD,edit,1350631552
Control7=IDC_HIDEPWD,button,1342242819

[DLG:IDD_DIALOG3]
Type=1
Class=?
ControlCount=33
Control1=IDC_STATIC,static,1342308865
Control2=IDC_RADIO1,button,1342308361
Control3=IDC_RADIO2,button,1342177289
Control4=IDC_STATIC,button,1342308359
Control5=IDC_CHECK_TIMEOUT,button,1342242819
Control6=IDC_EDIT_TIMEOUT,edit,1350639744
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,button,1342177287
Control9=IDC_CHECK_SPEED,button,1342242819
Control10=IDC_EDIT_SPEED,edit,1350639744
Control11=IDC_STATIC,static,1342308864
Control12=IDC_STATIC,button,1342177287
Control13=IDC_CHECK_PING,button,1342242819
Control14=IDC_EDIT_PING,edit,1350639744
Control15=IDC_STATIC,static,1342308864
Control16=IDC_STATIC,button,1342177287
Control17=IDC_CHECK_TERMINATE,button,1342242819
Control18=IDC_EDIT_TERMINATE,edit,1350639744
Control19=IDC_STATIC,static,1342308864
Control20=IDC_EDIT_TERMTIME,edit,1350639744
Control21=IDC_STATIC,static,1342308864
Control22=IDC_STATIC,button,1342177287
Control23=IDC_CHECK_REDIAL,button,1342242819
Control24=IDC_CHECK_HANGUP,button,1342242819
Control25=IDC_STATIC,button,1342177287
Control26=IDC_CHECK_LOG,button,1342242819
Control27=IDC_BUTTON_FILE_LOG,button,1342242816
Control28=IDC_EDIT_LOG,edit,1350631552
Control29=IDC_BUTTON_VEW_LOG,button,1342242816
Control30=IDC_STATIC,button,1342177287
Control31=IDC_CHECK_INTERFACE_START_MINIMIZED,button,1342242819
Control32=IDC_CHECK_INTERFACE_START_MINIMIZED2,button,1342242819
Control33=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG4]
Type=1
Class=?
ControlCount=15
Control1=IDC_STATIC_NAME,static,1342308865
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308865
Control4=IDC_EDIT_TIME_CUR,edit,1350633600
Control5=IDC_STATIC,static,1342308865
Control6=IDC_EDIT_TIME_TOT,edit,1350633600
Control7=IDC_CLEAN,button,1342242816
Control8=IDC_STATIC,static,1342308865
Control9=IDC_EDIT_SENT,edit,1350633600
Control10=IDC_STATIC,static,1342308865
Control11=IDC_EDIT_RECV,edit,1350633600
Control12=IDC_STATIC,static,1342308865
Control13=IDC_EDIT_SPEED,edit,1350633600
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC_PANEL,static,1342308352

[DLG:IDD_PHONE_NUMBER]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_EDIT2,edit,1350631552

[MNU:IDR_TRAY_MENU1]
Type=1
Class=?
Command1=IDM_SHOW
Command2=IDM_TRAY_DIAL
Command3=IDCANCEL
CommandCount=3

