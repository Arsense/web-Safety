// ipmonDlg.h : header file
//

#if !defined(AFX_IPMONDLG_H__47076949_0324_11D5_924F_00010219EDF4__INCLUDED_)
#define AFX_IPMONDLG_H__47076949_0324_11D5_924F_00010219EDF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct _PROTN2T
{ 
	int  proto ;
	char *pprototext ;
}PROTN2T ; 
						
#define PROTO_NUM  11 

/* The IP header */
typedef struct _IPHEADER {
        unsigned char  header_len:4;
        unsigned char  version:4;   
        unsigned char  tos;            // type of service
        unsigned short total_len;      // length of the packet
        unsigned short ident;          // unique identifier
        unsigned short flags;          
        unsigned char  ttl;            
        unsigned char  proto;          // protocol ( IP , TCP, UDP etc)
        unsigned short checksum;       
        unsigned int   sourceIP;
        unsigned int   destIP;

}IPHEADER;

/*-------------------zhuwei add(2002.11.9)-----------------------------*/


#define UDP_HEAD_LEN 8	/* UDP head length */

#define PSEUDO_HEAD_LEN 12	/* Pseudo head length */

#define ICMP_HEAD_LEN 4	/* ICMP head length */

struct TCPPacketHead {
	WORD SourPort;
	WORD DestPort;
	DWORD SeqNo;
	DWORD AckNo;
	BYTE HLen;
	BYTE Flag;
	WORD WndSize;
	WORD ChkSum;
	WORD UrgPtr;
};

struct ICMPPacketHead {
	BYTE Type;
	BYTE Code;
	WORD ChkSum;
};

struct UDPPacketHead {
	WORD SourPort;
	WORD DestPort;
	WORD Len;
	WORD ChkSum;
};


/////////////////////////////////////////////////////////////////////////////
// CIpmonDlg dialog

class CIpmonDlg : public CDialog
{
// Construction
public:
	void AddData(CString s0,CString s1,CString s2,CString s3,CString s4,CString s5,CString s6);
	CIpmonDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIpmonDlg)
	enum { IDD = IDD_IPMON_DIALOG };
	CListCtrl	m_ctrList;
	CButton	m_start;
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpmonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	DWORD  m_ipsource     ; // IP in net format
	DWORD  m_iphostsource ; // same IP in host format
	DWORD  m_iphost;
	DWORD  m_ipcheckedhost;
	SOCKET m_s            ;
	DWORD  m_threadID     ;
	BOOL   m_Multihomed   ;
	BOOL   m_Local        ;
	CDWordArray m_IPArr   ;

	friend UINT threadFunc ( LPVOID p ) ;


	// Generated message map functions
	//{{AFX_MSG(CIpmonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLookUp();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPMONDLG_H__47076949_0324_11D5_924F_00010219EDF4__INCLUDED_)
