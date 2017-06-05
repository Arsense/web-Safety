#if !defined(AFX_CHOOSEDLG_H__3D1E71AA_95CB_4C36_8804_4A69F83A3BD4__INCLUDED_)
#define AFX_CHOOSEDLG_H__3D1E71AA_95CB_4C36_8804_4A69F83A3BD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChooseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChooseDlg dialog

class CChooseDlg : public CDialog
{
// Construction
public:
	void AddToList(CString s);
	CChooseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChooseDlg)
	enum { IDD = IDD_MSGCHOOSE };
	CComboBox	ctlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChooseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChooseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSEDLG_H__3D1E71AA_95CB_4C36_8804_4A69F83A3BD4__INCLUDED_)
