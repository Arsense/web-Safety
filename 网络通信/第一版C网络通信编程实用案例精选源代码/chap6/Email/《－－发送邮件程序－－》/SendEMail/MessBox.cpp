// MessBox.cpp : implementation file
//

#include "stdafx.h"
#include "SendEMail.h"
#include "MessBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessBox dialog


CMessBox::CMessBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMessBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessBox)
	m_sMessage = _T("");
	//}}AFX_DATA_INIT
}


void CMessBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessBox)
	DDX_Text(pDX, IDC_MESSAGE, m_sMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessBox, CDialog)
	//{{AFX_MSG_MAP(CMessBox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessBox message handlers
