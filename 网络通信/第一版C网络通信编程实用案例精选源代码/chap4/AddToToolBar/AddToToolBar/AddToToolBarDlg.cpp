// AddToToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddToToolBar.h"
#include "AddToToolBarDlg.h"
#include "objbase.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddToToolBarDlg dialog

CAddToToolBarDlg::CAddToToolBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddToToolBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddToToolBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAddToToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddToToolBarDlg)
	DDX_Control(pDX, IDC_BIG_ICON, m_BigIconButton);
	DDX_Control(pDX, IDC_SMALL_ICON, m_SmallIconButton);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddToToolBarDlg, CDialog)
	//{{AFX_MSG_MAP(CAddToToolBarDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddToToolBarDlg message handlers

BOOL CAddToToolBarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAddToToolBarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAddToToolBarDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAddToToolBarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAddToToolBarDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	if(dlg.DoModal()==IDOK)
	{
		SetDlgItemText(IDC_EDIT1,dlg.GetPathName());
		SetDlgItemText(IDC_EDIT2,dlg.GetFileName());
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
}

void CAddToToolBarDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CString valuename;
	CString filename;
	CString pathname;
	GetDlgItemText(IDC_EDIT1,pathname);
	GetDlgItemText(IDC_EDIT2,filename);


	HICON smalls;
	HICON larges;
	ExtractIconEx(LPCSTR(filename),0,&larges,&smalls,1);
	this->SetIcon(smalls,FALSE);
	this->m_BigIconButton.SetIcon(larges);
	this->m_SmallIconButton.SetIcon(smalls);

	CLSID pclsid;
	unsigned short *a= new unsigned short[pathname.GetLength()];
	for(int i=0;i<filename.GetLength();i++)
	{
		a[i]=(unsigned short)(filename[i]);
	}
	if(GetClassFile(a,&pclsid)==MK_E_CANTOPENFILE )
	{
		int k=0;
		k=k+1;
	}

	filename.TrimRight(".exe");
	/*
	CRegKey key;
	char  value[80];
	unsigned long count=80;
	CString temp="SOFTWARE\\Classes\\"+filename+"\\CLSID";
	key.Open(HKEY_LOCAL_MACHINE,(LPCSTR)temp);
	key.QueryValue(value,"",&count);	
	*/
	if(filename.IsEmpty())
	{
		AfxMessageBox("请首先选择添加的程序");
		return;
	}
	GenerateRadomName(valuename);
	AddEventSource(valuename,pathname,filename);
}

// 8-4-4-4-12
void CAddToToolBarDlg::GenerateRadomName(CString &valuename)
{

	char a[3];
	int i=0;
	valuename="{";

	srand( (unsigned)time( NULL ) );	//以时间为种子

	while(i<8)
	{
		i++;
		a[0]=rand()%('9'-'0'+1)+'0';
		a[1]=rand()%('z'-'a'+1)+'a';
		a[2]=rand()%('Z'-'A'+1)+'A';
		valuename=valuename+CString(a[rand()%3]);
	}
	
	valuename+="-";
	i=0;
	while(i<4)
	{
		i++;
		a[0]=rand()%('9'-'0'+1)+'0';
		a[1]=rand()%('z'-'a'+1)+'a';
		a[2]=rand()%('Z'-'A'+1)+'A';
		valuename=valuename+CString(a[rand()%3]);
	}
	valuename+="-";
	i=0;
	while(i<4)
	{
		i++;
		a[0]=rand()%('9'-'0'+1)+'0';
		a[1]=rand()%('z'-'a'+1)+'a';
		a[2]=rand()%('Z'-'A'+1)+'A';
		valuename=valuename+CString(a[rand()%3]);
	}
	valuename+="-";
	i=0;
	while(i<4)
	{
		i++;
		a[0]=rand()%('9'-'0'+1)+'0';
		a[1]=rand()%('z'-'a'+1)+'a';
		a[2]=rand()%('Z'-'A'+1)+'A';
		valuename=valuename+CString(a[rand()%3]);
	}

	valuename+="-";
	i=0;
	while(i<12)
	{
		i++;
		a[0]=rand()%('9'-'0'+1)+'0';
		a[1]=rand()%('z'-'a'+1)+'a';
		a[2]=rand()%('Z'-'A'+1)+'A';
		valuename=valuename+CString(a[rand()%3]);
	}
	valuename+="}";
}

void CAddToToolBarDlg::AddEventSource(CString keyname,CString pathname,CString filename)
{
    HKEY hk; 
	CString buffer;
	CString CLSID="{1FBA04EE-3024-11D2-8F1F-0000F87ABD16}";	

 
    if (RegCreateKey(HKEY_LOCAL_MACHINE, 
            "Software\\Microsoft\\Internet Explorer\\Extensions\\"+keyname, &hk)!=ERROR_SUCCESS)
	{
		AfxMessageBox("Cannot Create Key !");
		return;
	}
 
	// button text
	buffer="ButtonText";
    if (RegSetValueEx(hk,             // subkey handle 
            buffer,       // value name 
            0,                        // must be zero 
            REG_SZ,            // value type 
            (CONST BYTE *)((LPCSTR)filename),           // pointer to value data 
            filename.GetLength())!=ERROR_SUCCESS)       // length of value data 
	{
		AfxMessageBox("Cannot Set Value");
	}

	//CLSID
    if (RegSetValueEx(hk,             // subkey handle 
            "CLSID",       // value name 
            0,                        // must be zero 
            REG_SZ,            // value type 
            (CONST BYTE *)((LPCSTR)CLSID),           // pointer to value data 
            CLSID.GetLength())!=ERROR_SUCCESS)       // length of value data 
	{
		AfxMessageBox("Cannot Set Value");
	}
 
	//	Default visible
	buffer="Yes";
    if (RegSetValueEx(hk,		// subkey handle 
            "Default visible",			// value name 
            0,					// must be zero 
            REG_SZ,			// value type 
            (CONST BYTE *)((LPCSTR)buffer),	// pointer to value data 
            buffer.GetLength())!=ERROR_SUCCESS)    // length of value data 
	{
		AfxMessageBox("Cannot Set Value !");
	} 

	//	Exec
    if (RegSetValueEx(hk,		// subkey handle 
            "Exec",			// value name 
            0,					// must be zero 
            REG_SZ,			// value type 
            (CONST BYTE *)((LPCSTR)pathname),	// pointer to value data 
            pathname.GetLength())!=ERROR_SUCCESS)    // length of value data 
	{
		AfxMessageBox("Cannot Set Value !");
	} 
	
	//	HotIcon
    if (RegSetValueEx(hk,		// subkey handle 
            "HotIcon",			// value name 
            0,					// must be zero 
            REG_SZ,				// value type 
            (CONST BYTE *)((LPCSTR)(pathname+",001")),	// pointer to value data 
            pathname.GetLength())!=ERROR_SUCCESS)    // length of value data 
	{
		AfxMessageBox("Cannot Set Value !");
	} 
	
	//	Icon
    if (RegSetValueEx(hk,		// subkey handle 
            "Icon",			// value name 
            0,					// must be zero 
            REG_SZ,			// value type 
            (CONST BYTE *)((LPCSTR)(pathname+",002")),	// pointer to value data 
            pathname.GetLength())!=ERROR_SUCCESS)    // length of value data 
	{
		AfxMessageBox("Cannot Set Value !");
	} 

	//	MenuStatusBar
    if (RegSetValueEx(hk,		// subkey handle 
            "MenuStatusBar",			// value name 
            0,					// must be zero 
            REG_SZ,			// value type 
            (CONST BYTE *)((LPCSTR)filename),	// pointer to value data 
            filename.GetLength())!=ERROR_SUCCESS)    // length of value data 
	{
		AfxMessageBox("Cannot Set Value !");
	} 

	//	MenuText
    if (RegSetValueEx(hk,		// subkey handle 
            "MenuText",			// value name 
            0,					// must be zero 
            REG_SZ,			// value type 
            (CONST BYTE *)((LPCSTR)filename),	// pointer to value data 
            filename.GetLength())!=ERROR_SUCCESS)    // length of value data 
	{
		AfxMessageBox("Cannot Set Value !");
	} 

//	AfxMessageBox("Success !");
    RegCloseKey(hk); 
} 


