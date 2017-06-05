#if !defined(AFX_CONFIGUREDLG_H__2DDBAD08_313E_4895_AA8C_1D481D6EF3C0__INCLUDED_)
#define AFX_CONFIGUREDLG_H__2DDBAD08_313E_4895_AA8C_1D481D6EF3C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureDlg dialog

class CConfigureDlg : public CDialog
{
// Construction
public:
	CConfigureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigureDlg)
	enum { IDD = IDD_PARAMETER };
	UINT	m_COMPort;
	BOOL	m_DTREnable;
	BOOL	m_HandShaking;
	UINT	m_InBufferSize;
	long	m_InputMode;
	BOOL	m_NULLDiscard;
	UINT	m_OutBufferSize;
	CString	m_ParityReplace;
	short	m_RThreshold;
	BOOL	m_RTSEnable;
	CString	m_Setting;
	short	m_SThreshold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREDLG_H__2DDBAD08_313E_4895_AA8C_1D481D6EF3C0__INCLUDED_)
