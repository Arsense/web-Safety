// connectd.cpp : implementation file
//

#include "stdafx.h"
#include "wsterm.h"
#include "connectd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// .INI File Section and entries
static const char szSection[] = "ConnectInfo";
static const char szHostName[] = "HostName";
static const char szPort[] = "Port";

/////////////////////////////////////////////////////////////////////////////
// CConnectDialog dialog


CConnectDialog::CConnectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDialog)
	m_strHostName = "";
	m_strPort = "";
	//}}AFX_DATA_INIT
}

void CConnectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDialog)
	DDX_Text(pDX, IDC_HOSTNAME, m_strHostName);
	DDX_CBString(pDX, IDC_PORT, m_strPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConnectDialog, CDialog)
	//{{AFX_MSG_MAP(CConnectDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConnectDialog message handlers

BOOL CConnectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Restore previous selections
	m_strHostName = AfxGetApp()->GetProfileString(szSection, 
								     szHostName);
	m_strPort     = AfxGetApp()->GetProfileString(szSection, 
								     szPort);
	UpdateData(FALSE);
	return TRUE;
}

// Convert m_strPort and
// store in m_nPort in HOST order
void CConnectDialog::OnOK()
{
	// Service entry structure
    LPSERVENT	pServEntry;

	UpdateData(TRUE);
	
    m_nPort = atoi(m_strPort);
	// Is it already a number?
    if (m_nPort == 0)
    {             
	    // Use WinSock database routine
    	// for service resolution  
    	// First, make copy and 
    	// convert to lower case
    	CString strCopy(m_strPort);
    	strCopy.MakeLower();
		pServEntry = getservbyname(strCopy, "tcp");
		if (pServEntry == NULL)
		{
			AfxMessageBox("Unknown service name");
			return;
		}                    
		// Port already in network order
		// Socket classes assume we will
		// use host order, so we convert
		m_nPort = ntohs(pServEntry->s_port);
	}
	// Save entries to .ini for next use
	AfxGetApp()->WriteProfileString(szSection, 
								    szHostName, 
								    m_strHostName);
	AfxGetApp()->WriteProfileString(szSection, 
								    szPort, 
								    m_strPort);
	CDialog::OnOK();
}

