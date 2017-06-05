// ConfigureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSCOMM.h"
#include "ConfigureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog


CConfigureDlg::CConfigureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureDlg)
	m_COMPort = 0;
	m_DTREnable = FALSE;
	m_HandShaking = FALSE;
	m_InBufferSize = 0;
	m_InputMode = 0;
	m_NULLDiscard = FALSE;
	m_OutBufferSize = 0;
	m_ParityReplace = _T("");
	m_RThreshold = 0;
	m_RTSEnable = FALSE;
	m_Setting = _T("");
	m_SThreshold = 0;
	//}}AFX_DATA_INIT
}


void CConfigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureDlg)
	DDX_Text(pDX, IDC_EDIT1, m_COMPort);
	DDV_MinMaxUInt(pDX, m_COMPort, 1, 4);
	DDX_Text(pDX, IDC_EDIT2, m_DTREnable);
	DDX_Text(pDX, IDC_EDIT3, m_HandShaking);
	DDX_Text(pDX, IDC_EDIT4, m_InBufferSize);
	DDV_MinMaxUInt(pDX, m_InBufferSize, 10, 100000);
	DDX_Text(pDX, IDC_EDIT5, m_InputMode);
	DDV_MinMaxLong(pDX, m_InputMode, 0, 10000);
	DDX_Text(pDX, IDC_EDIT6, m_NULLDiscard);
	DDX_Text(pDX, IDC_EDIT7, m_OutBufferSize);
	DDV_MinMaxUInt(pDX, m_OutBufferSize, 0, 100000);
	DDX_Text(pDX, IDC_EDIT8, m_ParityReplace);
	DDX_Text(pDX, IDC_EDIT9, m_RThreshold);
	DDX_Text(pDX, IDC_EDIT10, m_RTSEnable);
	DDX_Text(pDX, IDC_EDIT11, m_Setting);
	DDX_Text(pDX, IDC_EDIT12, m_SThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigureDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigureDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg message handlers

void CConfigureDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
