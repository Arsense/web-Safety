// MyTapiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTapi.h"
#include "MyTapiDlg.h"

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
// CMyTapiDlg dialog

CMyTapiDlg::CMyTapiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyTapiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyTapiDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyTapiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyTapiDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyTapiDlg, CDialog)
	//{{AFX_MSG_MAP(CMyTapiDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ASSIT_PHONE, OnAssitPhone)
	ON_BN_CLICKED(IDC_BASIC_PHONE, OnBasicPhone)
	ON_BN_CLICKED(IDC_HANG_UP, OnHangUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTapiDlg message handlers

BOOL CMyTapiDlg::OnInitDialog()
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
	char c[10]={'\0'},sc[10]={'\0'};
	tapiGetLocationInfo(LPSTR(c),LPSTR(sc));
	char buffer[200];
	sprintf(buffer,"您的国家代号是：%s ,你所在地区或城市的区号是：%s",c,sc);
	SetDlgItemText(IDC_ZIP_CODE,buffer);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyTapiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyTapiDlg::OnPaint() 
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
HCURSOR CMyTapiDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyTapiDlg::OnAssitPhone() 
{
	// TODO: Add your control notification handler code here
	CString phonenum;
	GetDlgItemText(IDC_ASSIT_PHONE,phonenum);
	if(phonenum=="")
	{
		AfxMessageBox("please input the phone num!");
	}
	else
	{
		LONG lResult;
		lResult=tapiRequestMakeCall(phonenum,"TAPI","TAPI","简单的TAPI");
		if(lResult!=0)
		{
			AfxMessageBox("Sorry,tapiRequestMakeCall error!");
		}
	}
}

void CMyTapiDlg::InitLine()
{
	m_bError=FALSE;
	LONG result=lineInitialize(m_hTAPI,AfxGetInstanceHandle(),TapiCallbackFunc,"MyTapi",&m_dwNumLines);
	if(result)
	{
		AfxMessageBox("tapi init error!");
	}
}

void  CALLBACK CMyTapiDlg::TapiCallbackFunc(DWORD device, DWORD msg, DWORD callbackinstance, DWORD param1, DWORD param2, DWORD param3)
{
	CMyTapiDlg *pctapi=(CMyTapiDlg *)callbackinstance;
	switch(msg)
	{
	case LINE_CALLSTATE:
		//displayCallStates(
		break;
	case LINE_CLOSE:
		if(pctapi)
		{
			pctapi->ShutDown();
		}
		break;
	case LINE_REPLY:
		if(pctapi&&(param1==pctapi->m_lRequestID))
		{
			if(param2)
			{

			}
			else
			{
				pctapi->m_bhCallValid=TRUE;
			}
		}
		break;
	case LINE_CREATE:
		AfxMessageBox("Line Create");
	default:
		AfxMessageBox("TapiCallBackFuck 消息被屏蔽了!");
		break;
	}
}

void CMyTapiDlg::InitLineEx()
{
	m_bError=FALSE;
	LONG result;
	DWORD version=0x00020002;
	LINEINITIALIZEEXPARAMS LIP;
	memset(&LIP,0,sizeof(LIP));
	LIP.dwTotalSize=LIP.dwUsedSize=sizeof(LIP);
	LIP.dwOptions=LINEINITIALIZEEXOPTION_USEHIDDENWINDOW;
	result=lineInitializeEx(m_hTAPI,AfxGetInstanceHandle(),TapiCallbackFunc,"MyTapi",&m_dwNumLines,&version,&LIP);
	if(result)
	{
		AfxMessageBox("tapi init error!");
		m_bError=TRUE;
	}
	else
	{
		m_dwTapiVersion=version;
	}
}

void CMyTapiDlg::NegotiateTAPIVersion()
{
	if(m_bError)	return;
	LONG result;
	LINEEXTENSIONID	extensionid;
	DWORD tapiversion;
	char buffer[1024];
	BOOL having =FALSE;
	if(m_hTAPI)
	{
		for(DWORD line=0;line<m_dwNumLines;line++)
		{
			result=lineNegotiateAPIVersion(*m_hTAPI,line,0x00010003,0x00020002,&tapiversion,&extensionid);
			if(result)
			{
				continue;
			}
			having=TRUE;
			LPLINEDEVCAPS lpLineDevCaps =GetDevCaps(line,tapiversion);
			if((lpLineDevCaps)&&(lpLineDevCaps->dwLineNameSize)&&(lpLineDevCaps->dwLineNameOffset)&&
				(lpLineDevCaps->dwStringFormat==STRINGFORMAT_ASCII))
			{
				char* linename=((char *)lpLineDevCaps)+lpLineDevCaps->dwLineNameOffset;
				sprintf(buffer,"Device: %d  name: %s 协商的TAPI版本号: 0x%lx",line,linename,tapiversion);
				AfxMessageBox(buffer);
			}
			else
			{
				sprintf(buffer,"Device: %d no name 协商的TAPI版本号:0x%lx",line,tapiversion);
				AfxMessageBox(buffer);
			}
			if((lpLineDevCaps)&&(lpLineDevCaps->dwBearerModes&LINEBEARERMODE_VOICE)&&
				(lpLineDevCaps->dwLineFeatures&LINEFEATURE_MAKECALL)&&
				(lpLineDevCaps->dwMediaModes&LINEMEDIAMODE_INTERACTIVEVOICE))
			{
				m_dwLineToUse=line;
				m_dwTapiVersion=tapiversion;
			}
			free(lpLineDevCaps);
			return;
		}
	}
}


LPLINEDEVCAPS CMyTapiDlg::GetDevCaps(DWORD device, DWORD tapiversion)
{
	if(m_bError)	return NULL;
	LPLINEDEVCAPS	lpLineDevCaps=NULL;
	size_t	need=sizeof(LINEDEVCAPS);
	LONG result=0;
	for(int i=0;i<2;i++)
	{
		lpLineDevCaps=(LPLINEDEVCAPS)calloc(need,1);
		if(!lpLineDevCaps)
		{
			return NULL;
		}
		lpLineDevCaps->dwTotalSize=need;
		result=lineGetDevCaps(*m_hTAPI,device,tapiversion,0,lpLineDevCaps);
		if(result)
		{
			AfxMessageBox("sorry,error in call lineGetDevCaps!");
			free(lpLineDevCaps);
			return NULL;
		}
		if(lpLineDevCaps->dwNeededSize<=lpLineDevCaps->dwTotalSize)
			break;
		need=lpLineDevCaps->dwNeededSize;
		free(lpLineDevCaps);
		lpLineDevCaps=NULL;
	}
	if(lpLineDevCaps)
		free(lpLineDevCaps);
	return NULL;
}



void CMyTapiDlg::OpenLine()
{
	if(m_bError)	return;
	LONG result;
	result=lineOpen(*m_hTAPI,m_dwLineToUse,m_hLine,m_dwTapiVersion,0,(DWORD)this,
		LINECALLPRIVILEGE_NONE,LINEMEDIAMODE_INTERACTIVEVOICE,0);
	if(result)
	{
		AfxMessageBox("error in calling lineopen!");
	}
}


void CMyTapiDlg::Call(CString number)
{
	if(m_bError)	return;
	LPLINETRANSLATEOUTPUT LTO;
	char dialstr[256];
	LONG result;
	LTO=(LPLINETRANSLATEOUTPUT)calloc(512,1);
	if(!LTO)
		return;
	LTO->dwTotalSize=512;
	result=lineTranslateAddress(*m_hTAPI,m_dwLineToUse,m_dwTapiVersion,number,0,0,LTO);
	if(LTO->dwDialableStringSize>=256)
		return;
	strncpy(dialstr,&((const char*)LTO)[LTO->dwDialableStringOffset],
		LTO->dwDialableStringSize);
	dialstr[LTO->dwDialableStringSize]='\0';
	free(LTO);

	LINECALLPARAMS lcp;
	memset(&lcp,0,sizeof(LINECALLPARAMS));
	lcp.dwTotalSize=sizeof(LINECALLPARAMS);
	lcp.dwBearerMode=LINEBEARERMODE_VOICE;
	lcp.dwMediaMode=LINEMEDIAMODE_INTERACTIVEVOICE;
	result=lineMakeCall(*m_hLine,m_hCall,dialstr,0,&lcp);
	if(result<0)
	{
		AfxMessageBox("call error!");
	}
	if(result>0)
	{
		m_lRequestID=result;
	}

}

void CMyTapiDlg::GetID()
{
	if(m_bError)	return;
	LPVARSTRING str=NULL;
	size_t need=sizeof(VARSTRING);
	LONG result;

	for(int i=0;i<2;i++)
	{
		str=(LPVARSTRING)calloc(need,1);
		if(!str)
			return;
		result=lineGetID(*m_hLine,0,NULL,LINECALLSELECT_LINE,str,"comm/datamodem");
		if(result)
		{
			free(str);
			return;
		}
		if(str->dwNeededSize<=str->dwTotalSize)
			break;
		need=str->dwNeededSize;
		free(str);
		str=NULL;
	}
	m_hModem=*(LPHANDLE)(str+str->dwStringOffset);
	if(str)
		free(str);
}


void CMyTapiDlg::Drop()
{
	if(m_bError)	return;
	LONG result=0;
	if(m_bhCallValid&&*m_hCall)
		result=lineDrop(*m_hCall,NULL,0);
	if(result<0)
	{
		AfxMessageBox("error in drop!");
	}
}

void CMyTapiDlg::ShutDown()
{
	if(m_bError)	return;
	LONG result=0;
	Drop();
	if(*m_hTAPI)
	{
		result=lineShutdown(*m_hTAPI);
	}
	if(result)
	{
		AfxMessageBox("error in lineShutDown");
	}
	*m_hTAPI=NULL;
	m_dwNumLines=0;
	m_dwTapiVersion=0;
	m_dwLineToUse=0;
	*m_hLine=0;
	*m_hCall=0;
	m_bhCallValid=FALSE;
}

void CMyTapiDlg::OnBasicPhone() 
{
	// TODO: Add your control notification handler code here
	InitLineEx();
	NegotiateTAPIVersion();
	OpenLine();
	CString phone;
	GetDlgItemText(IDC_PHONE_NUM,phone);
	Call(phone);
}

void CMyTapiDlg::OnHangUp() 
{
	// TODO: Add your control notification handler code here
	ShutDown();
}
