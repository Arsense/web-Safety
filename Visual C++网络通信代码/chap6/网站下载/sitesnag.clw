; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProgress
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SiteSnag.h"
LastPage=0

ClassCount=11
Class1=CSnaggerApp
Class2=CSnaggerDoc
Class3=CSnaggerView
Class4=CSnaggerFrame

ResourceCount=12
Resource1=IDD_PROGRESS
Resource2=IDD_OPTIONS (English (U.S.))
Class5=CAboutDlg
Resource3=IDR_MAINFRAME (English (U.S.))
Resource4=IDD_SNAG
Class6=COptionsDlg
Resource5=IDD_ABOUTBOX
Class7=CProgress
Class8=CProjectDlg
Resource6=IDR_MAINFRAME
Class9=CProjectEdit
Class10=CSnagWait
Class11=CURLDlg
Resource7=IDD_PROJECT (English (U.S.))
Resource8=IDD_WAIT (English (U.S.))
Resource9=IDD_ABOUTBOX (English (U.S.))
Resource10=IDD_PROGRESS (English (U.S.))
Resource11=IDD_SNAG (English (U.S.))
Resource12=IDD_OPTIONS

[CLS:CSnaggerApp]
Type=0
HeaderFile=SiteSnag.h
ImplementationFile=SiteSnag.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=ID_HELP_CONTENTS

[CLS:CSnaggerDoc]
Type=0
HeaderFile=Document.h
ImplementationFile=Document.cpp
Filter=N
LastObject=CSnaggerDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CSnaggerView]
Type=0
HeaderFile=View.h
ImplementationFile=View.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_FILE_NEW

[CLS:CSnaggerFrame]
Type=0
HeaderFile=Frame.h
ImplementationFile=Frame.cpp
Filter=T
LastObject=ID_APP_ABOUT



[CLS:CAboutDlg]
Type=0
HeaderFile=SiteSnag.cpp
ImplementationFile=SiteSnag.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CSnaggerFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_SITEINPUT
Command8=ID_PARA
Command9=ID_DELETE
Command10=ID_RENAME
Command11=ID_HELP_CONTENTS
Command12=ID_APP_ABOUT
CommandCount=12

[ACL:IDR_MAINFRAME]
Type=1
Class=CSnaggerFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_REMOVEALL
Command5=ID_SNAG
Command6=ID_STOP
Command7=ID_FILE_OPTIONS
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_PROGRESS]
Type=1
Class=CProgress
ControlCount=11
Control1=IDC_ACTION,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_QUEUED_FILES,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_FILES_DOWNLOADED,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_KB_DOWNLOADED,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_PAGES_DOWNLOADED,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_LEVEL,static,1342308352

[DLG:IDD_OPTIONS]
Type=1
Class=COptionsDlg
ControlCount=19
Control1=IDC_MAX_LEVELS,edit,1350631552
Control2=IDC_SPIN1,msctls_updown32,1342177330
Control3=IDC_MAX_PAGES,edit,1350631552
Control4=IDC_SPIN2,msctls_updown32,1342177330
Control5=IDC_OFFSITE_LINKS,button,1342242819
Control6=IDC_GET_MEDIA,button,1342242819
Control7=IDC_FIXUP_HTML,button,1342242819
Control8=IDC_GENERATE_TOC,button,1342242819
Control9=IDC_STATIC,static,1342181383
Control10=IDC_PROXY_SERVER,edit,1350631552
Control11=IDOK,button,1342242817
Control12=IDCANCEL,button,1342242816
Control13=IDC_NO_LIMIT,button,1342242819
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342181383
Control17=IDC_STATIC,static,1342308352
Control18=IDC_USE_PROXY,button,1342242819
Control19=IDC_STATIC,static,1342308352

[CLS:COptionsDlg]
Type=0
HeaderFile=Options.h
ImplementationFile=Options.cpp
BaseClass=CDialog
Filter=D
LastObject=COptionsDlg
VirtualFilter=dWC

[CLS:CProgress]
Type=0
HeaderFile=progress.h
ImplementationFile=progress.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CProgress

[CLS:CProjectDlg]
Type=0
HeaderFile=Project.h
ImplementationFile=Project.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_SNAG]
Type=1
Class=CURLDlg
ControlCount=12
Control1=IDC_EDIT,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_MAX_LEVELS,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_MAX_PAGES,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_GET_MEDIA,static,1342308352
Control11=IDC_STATIC,static,1342181383
Control12=IDC_STATIC,static,1342308352

[CLS:CProjectEdit]
Type=0
HeaderFile=Project.h
ImplementationFile=Project.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CProjectEdit

[CLS:CSnagWait]
Type=0
HeaderFile=Wait.h
ImplementationFile=Wait.cpp
BaseClass=CDialog
Filter=D
LastObject=CSnagWait
VirtualFilter=dWC

[CLS:CURLDlg]
Type=0
HeaderFile=urldlg.h
ImplementationFile=urldlg.cpp
BaseClass=CDialog
LastObject=CURLDlg
Filter=D
VirtualFilter=dWC

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_SNAG
Command5=ID_FILE_OPTIONS
Command6=ID_FILE_REMOVEALL
Command7=ID_FILE_RENAME
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_VIEW_TOOLBAR
Command11=ID_VIEW_STATUS_BAR
Command12=ID_SITEINPUT
Command13=ID_PARA
Command14=ID_DELETE
Command15=ID_RENAME
Command16=ID_HELP_CONTENTS
Command17=ID_APP_ABOUT
CommandCount=17

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_REMOVEALL
Command5=ID_SNAG
Command6=ID_STOP
Command7=ID_FILE_OPTIONS
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
ControlCount=9
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342177287
Control9=IDC_STATIC,static,1476397070

[DLG:IDD_PROGRESS (English (U.S.))]
Type=1
ControlCount=11
Control1=IDC_ACTION,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_QUEUED_FILES,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_FILES_DOWNLOADED,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_KB_DOWNLOADED,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_PAGES_DOWNLOADED,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_LEVEL,static,1342308352

[DLG:IDD_SNAG (English (U.S.))]
Type=1
ControlCount=14
Control1=IDC_EDIT,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_HELPBTN,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_MAX_LEVELS,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_MAX_PAGES,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_GET_MEDIA,static,1342308352
Control13=IDC_STATIC,static,1342181383
Control14=IDC_STATIC,static,1342308352

[DLG:IDD_OPTIONS (English (U.S.))]
Type=1
ControlCount=20
Control1=IDC_MAX_LEVELS,edit,1350631552
Control2=IDC_SPIN1,msctls_updown32,1342177330
Control3=IDC_MAX_PAGES,edit,1350631552
Control4=IDC_SPIN2,msctls_updown32,1342177330
Control5=IDC_OFFSITE_LINKS,button,1342242819
Control6=IDC_GET_MEDIA,button,1342242819
Control7=IDC_FIXUP_HTML,button,1342242819
Control8=IDC_GENERATE_TOC,button,1342242819
Control9=IDC_STATIC,static,1342181383
Control10=IDC_PROXY_SERVER,edit,1350631552
Control11=IDOK,button,1342242817
Control12=IDCANCEL,button,1342242816
Control13=IDC_HELPBTN,button,1342242816
Control14=IDC_NO_LIMIT,button,1342242819
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342181383
Control18=IDC_STATIC,static,1342308352
Control19=IDC_USE_PROXY,button,1342242819
Control20=IDC_STATIC,static,1342308352

[DLG:IDD_PROJECT (English (U.S.))]
Type=1
ControlCount=5
Control1=IDC_PROJECT_NAME,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_HELPBTN,button,1342242816
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_WAIT (English (U.S.))]
Type=1
ControlCount=2
Control1=IDC_STATIC,static,1342177283
Control2=IDC_MESSAGE,static,1342308352

