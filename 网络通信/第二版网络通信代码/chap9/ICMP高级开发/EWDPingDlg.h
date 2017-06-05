// EWDPingDlg.h : header file
//

#include "Icmp.h"

/////////////////////////////////////////////////////////////////////////////

#define	MAXHOSTNAME			128	  // maximum size of a host name buffer

/////////////////////////////////////////////////////////////////////////////
//
//					Local Message Interrupts to Process
//
/////////////////////////////////////////////////////////////////////////////

#define WSA_PING_ASYNC		WM_USER + 200

#define IDT_TIMER1			WSA_PING_ASYNC + 1

#define MAX_MESSAGE			IDT_TIMER1 + 1

/////////////////////////////////////////////////////////////////////////////

#define MAX_INTERVAL_TIME	500

/////////////////////////////////////////////////////////////////////////////
// CEWDPingDlg dialog

class CEWDPingDlg : public CDialog
{

	enum ImageType
	{
		Icon_Blank,
		Icon_BlueArrow
	};


// Construction
private:
	//////////////////////////////////////////////////
	//												//
	//			Timer support routines				//
	//												//
	//////////////////////////////////////////////////

	void StopTimer();
	void StartTimer();
	void ChangeIconState(void);

	//////////////////////////////////////////////////
	//												//
	//			Icmp Socket Initialization			//
	//												//
	//////////////////////////////////////////////////

	BOOL InitSockets();

	//////////////////////////////////////////////////
	//												//
	//			Registry Support Routines			//
	//												//
	//////////////////////////////////////////////////

	BOOL FetchWinsockSettings();
	BOOL LoadRegValues(void);
	void SaveRegValues();

	//////////////////////////////////////////////////
	//												//
	//		ICMP Ping and IP Address Routines		//
	//												//
	//////////////////////////////////////////////////

	void SendPing(void);
	unsigned long HostIPAddress();

	//////////////////////////////////////////////////
	//												//
	//		ListCtrl ICMP Trace Routines			//
	//												//
	//////////////////////////////////////////////////

	void UpdateTrace ();
	void DisplayTrace(LPCSTR TripTimeMessage, LPCSTR IPAddressMessage, LPCSTR HostMessage);
	void EndTrace(void);
	
	//////////////////////////////////////////////////
	//												//
	//		ListCtrl Image/Display Routines			//
	//												//
	//////////////////////////////////////////////////

	void InitImageList();
	int AddListColumn(int column, int lfmt, int lmaxwidth, LPSTR ltext, int lsubitem);
	void DisplayBlankLine(void);
	void SetTraceSequence(int Seq, int FocusItem, ImageType FocusImage);
	void SetTraceFocus(int FocusItem, int FocusSubItem);
	void SetDisplayImage(int FocusItem, enum ImageType FocusImage);
	void TraceComment(CString Comment);

public:
	CEWDPingDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	//{{AFX_DATA(CEWDPingDlg)
	enum { IDD = IDD_EWDPING_DIALOG };
	CListCtrl	m_TraceList;
	CString	m_Host;
	CString	m_LocalHost;
	CString	m_NameServer;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEWDPingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	LRESULT OnPINGAsynch(WPARAM wParam, LPARAM lParam);  // PING Asynchronous Message Processor

	// Generated message map functions
	//{{AFX_MSG(CEWDPingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuit();
	afx_msg void OnPingButton();
	afx_msg void OnTraceButton();
	afx_msg void OnOptionsButton();
	afx_msg void OnTimer(UINT);
	afx_msg void OnClearButton();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CIcmp				PingSocket;						// Icmp Object

	WSADATA				wsaData;						// Windows Socket Information structure

	CString				LocalNameServer;				// Default name server (from Registry)
	CString				LocalDomainName;				// Domain name (from Registry)
	CString				DefHost;						// Default Host Name (from Registry)

	char				HostName[MAXHOSTNAME];			// Host name entered in dialog
	char				CurrentHostName[MAXHOSTNAME];	// Current host being Ping-ed

	struct in_addr		TraceTarget;					// Endpoint for trace operation

	BOOL				TimerActive;					// TRUE = timer is running
	int					TimerNumber;					// Assigned by system, used to access the timer
	int					TimerInterval;					// How long to set the timer for (from Registry)
	unsigned long		icmpIntervalCount;				// Current interval being counted
	unsigned long		icmpMaxIntervals;				// Maximum intervals (icmpMaxIntervals * icmpIntervalCount = TimerInterval)

	int					IconState;						// Next icon to display in icon animation sequence

	CImageList			smallImageList;					// List of small images for ListCtrl
	CImageList			largeImageList;					// List of large images for ListCtrl

	char				icmpBuffer[MAX_PACKET];			// General purpose buffer for Icmp operations
	LPSTR				pIcmpBuffer;					// FAR pointer to icmpBuffer
	int					icmpDataLen;					// Number of bytes in ICMP outgoing message (not counting header)
	int					icmpPingTTL;					// TTL value - used for TRACE operation
	BOOL				icmpTracing;					// TRUE = TRACE operation, FALSE = PING operation
	BOOL				PingSent;						// TRUE = Waiting for ECHO REPLY, FALSE = ignore input
};
