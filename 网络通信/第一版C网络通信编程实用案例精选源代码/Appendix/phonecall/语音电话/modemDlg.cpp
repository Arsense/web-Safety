// modemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modem.h"
#include "modemDlg.h"
#include "AboutHelpDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL bClicked = FALSE;



/////////////////////////////////////////////////////////////////////////////
// CModemDlg dialog

CModemDlg::CModemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModemDlg::IDD, pParent),TIMER_ID1(1),TIMER_ID2(2)
{
	//{{AFX_DATA_INIT(CModemDlg)
	m_editchar = _T("");
	m_modem = _T("");
	m_com = _T("");
	m_phone = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModemDlg)
	DDX_Control(pDX, IDhangup, m_hangup);
	DDX_Control(pDX, IDcall, m_call);
	DDX_Control(pDX, IDC_EDIT1, m_editctrl);
	DDX_Text(pDX, IDC_EDIT1, m_editchar);
	DDV_MaxChars(pDX, m_editchar, 40);
	DDX_Text(pDX, IDC_EDIT3, m_modem);
	DDX_Text(pDX, IDC_EDIT2, m_com);
	DDX_Text(pDX, IDC_EDIT4, m_phone);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CModemDlg, CDialog)
	//{{AFX_MSG_MAP(CModemDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDcall, Oncall)
	ON_BN_CLICKED(IDhangup, Onhangup)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDEXIT, OnExit)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_BN_CLICKED(ID_HANGUP, Onhangup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModemDlg message handlers

BOOL CModemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetDlgItem(IDcall)->EnableWindow(FALSE);
    GetDlgItem(IDhangup)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);

    
	 vcon=0;
	 vconcount=0;
	 ring=0;
	 
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CModemDlg::OnPaint() 
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
HCURSOR CModemDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CModemDlg::OnChangeEdit1() 
{

	UpdateData();
	if(m_editchar.GetLength())
		GetDlgItem(IDcall)->EnableWindow(TRUE);
	else 
		GetDlgItem(IDcall)->EnableWindow(FALSE);//无输入时禁止按钮
}

void CModemDlg::Oncall() 
{
	GetDlgItem(IDhangup)->EnableWindow(TRUE);
	GetDlgItem(IDcall)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

	m_editctrl.GetWindowText(m_editchar);  

	char strOutMsg[50]; 
	DWORD dwBytesWritten;
	wsprintf(strOutMsg,"ATDT%s\r",m_editchar);

	BOOL bReturn=WriteFile(hCommDev,"AT#CLS=8#VLS=8#VGT=131#VSR=8000\r",strlen("AT#CLS=8#VLS=8#VGT=131#VSR=11025\r"),&dwBytesWritten,NULL);
	if(!bReturn)
	{ 
		m_phone="语音模式设置......失败!";
	}
	else 
		m_phone="语音模式设置......成功!";
	Sleep(500);

	bReturn=WriteFile(hCommDev,strOutMsg,strlen(strOutMsg),&dwBytesWritten,NULL);
	if(!bReturn)
	{
		m_phone="拨号连接......失败!"; 
		UpdateData(FALSE);
		return;
	}
	m_phone="正在呼叫连接...  "+m_editchar;
	UpdateData(FALSE);
}

void CModemDlg::Onhangup() 
{
	GetDlgItem(IDhangup)->EnableWindow(FALSE);
    GetDlgItem(IDcall)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
    m_editctrl.EnableWindow(TRUE);	
    m_editctrl.GetWindowText(m_editchar);  

    DWORD dwBytesWritten;

    BOOL bReturn=WriteFile(hCommDev,"\r",strlen("\r"),&dwBytesWritten,NULL);
	if(!bReturn)
	{
		m_phone="挂断连接......失败!"; 
		UpdateData(FALSE);
		return;
	}
	Sleep(1000);

	bReturn=WriteFile(hCommDev,"+++",strlen("+++"),&dwBytesWritten,NULL);
	if(!bReturn)
	{
		m_phone="无法与调制解调器通讯,挂断连接......失败!"; 
		UpdateData(FALSE);
		return;
	}

	m_phone="正在挂断连接...  "+m_editchar;
	UpdateData(FALSE);
	Sleep(1000);
	bReturn=WriteFile(hCommDev,"ATZ\r",strlen("ATZ\r"),&dwBytesWritten,NULL);
	if(!bReturn)
	{
		m_phone="无法与调制解调器通讯,挂断连接......失败!"; 
		UpdateData(FALSE);
		return;
	}
	vconcount=0;

}





int CModemDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	DCB dcb; 
	char strOutNumber[50]; 
	DWORD dwBytesWritten;
	BOOL bReturn,flagcom=FALSE,flagmodem=FALSE;
	char strOutMsg[20]="ATS0=0V1M0\r";						 

	hCommDev=CreateFile("COM1",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
	if(hCommDev!=(HANDLE)-1)
	{ 
		flagcom=TRUE;

		BuildCommDCB("COM1:115200,n,8,1",&dcb);
		SetCommState(hCommDev,&dcb);   //以上为初始化串口

		COMMTIMEOUTS CommTimeOuts;
		CommTimeOuts.ReadIntervalTimeout=MAXDWORD;
		CommTimeOuts.ReadTotalTimeoutMultiplier=0;
		CommTimeOuts.ReadTotalTimeoutConstant=0;
		CommTimeOuts.WriteTotalTimeoutMultiplier=0;
		CommTimeOuts.WriteTotalTimeoutConstant=1000;
		SetCommTimeouts(hCommDev,&CommTimeOuts);
		PurgeComm(hCommDev,PURGE_TXCLEAR);
		PurgeComm(hCommDev,PURGE_RXCLEAR); //设置读写串口超时时间

		bReturn=WriteFile(hCommDev,strOutMsg,strlen(strOutMsg),&dwBytesWritten,NULL);
		if(!bReturn)
			flagmodem=FALSE;
		else 
			flagmodem=TRUE;
	} 
	else 
		flagcom=FALSE;

	if(flagcom==FALSE||flagmodem==FALSE)
    {
		CloseHandle(hCommDev);
		hCommDev=CreateFile("COM2",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
		if(hCommDev!=(HANDLE)-1)
		{ 
			flagcom=TRUE;
			BuildCommDCB("COM2:115200,n,8,1",&dcb);
			SetCommState(hCommDev,&dcb);   //以上为初始化串口

			COMMTIMEOUTS CommTimeOuts;
			CommTimeOuts.ReadIntervalTimeout=MAXDWORD;
			CommTimeOuts.ReadTotalTimeoutMultiplier=0;
			CommTimeOuts.ReadTotalTimeoutConstant=0;
			CommTimeOuts.WriteTotalTimeoutMultiplier=0;
			CommTimeOuts.WriteTotalTimeoutConstant=1000;
			SetCommTimeouts(hCommDev,&CommTimeOuts);
			PurgeComm(hCommDev,PURGE_TXCLEAR);
			PurgeComm(hCommDev,PURGE_RXCLEAR); //设置读写串口超时时间

			bReturn=WriteFile(hCommDev,strOutMsg,strlen(strOutMsg),&dwBytesWritten,NULL);
			if(!bReturn)
				flagmodem=FALSE;
			else 
				flagmodem=TRUE;
		} 
        else 
			flagcom=FALSE;
	}

	if(flagcom==FALSE||flagmodem==FALSE)
    {
		CloseHandle(hCommDev);
		hCommDev=CreateFile("COM3",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
		if(hCommDev!=(HANDLE)-1)
		{ 
			flagcom=TRUE;
			BuildCommDCB("COM1:115200,n,8,1",&dcb);
			SetCommState(hCommDev,&dcb);   //以上为初始化串口

			COMMTIMEOUTS CommTimeOuts;
			CommTimeOuts.ReadIntervalTimeout=MAXDWORD;
			CommTimeOuts.ReadTotalTimeoutMultiplier=0;
			CommTimeOuts.ReadTotalTimeoutConstant=0;
			CommTimeOuts.WriteTotalTimeoutMultiplier=0;
			CommTimeOuts.WriteTotalTimeoutConstant=1000;
			SetCommTimeouts(hCommDev,&CommTimeOuts);
			PurgeComm(hCommDev,PURGE_TXCLEAR);
			PurgeComm(hCommDev,PURGE_RXCLEAR); //设置读写串口超时时间

			bReturn=WriteFile(hCommDev,strOutMsg,strlen(strOutMsg),&dwBytesWritten,NULL);
			if(!bReturn)
				flagmodem=FALSE;
			else 
				flagmodem=TRUE;
		} 
		else 
			flagcom=FALSE;
	}

	if(flagcom==FALSE||flagmodem==FALSE)
	{
		CloseHandle(hCommDev);
		hCommDev=CreateFile("COM4",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
		if(hCommDev==(HANDLE)-1)
		{	
			flagcom=FALSE;
		}
		else 
			flagcom=TRUE;

		BuildCommDCB("COM2:115200,n,8,1",&dcb);
		SetCommState(hCommDev,&dcb);   //以上为初始化串口
		COMMTIMEOUTS CommTimeOuts;
		CommTimeOuts.ReadIntervalTimeout=MAXDWORD;
		CommTimeOuts.ReadTotalTimeoutMultiplier=0;
		CommTimeOuts.ReadTotalTimeoutConstant=0;
		CommTimeOuts.WriteTotalTimeoutMultiplier=0;
		CommTimeOuts.WriteTotalTimeoutConstant=1000;
		SetCommTimeouts(hCommDev,&CommTimeOuts);
		PurgeComm(hCommDev,PURGE_TXCLEAR);
		PurgeComm(hCommDev,PURGE_RXCLEAR); //设置读写串口超时时间

		bReturn=WriteFile(hCommDev,strOutMsg,strlen(strOutMsg),&dwBytesWritten,NULL);
		if(!bReturn)
		{
			flagmodem=FALSE;
		}
		else 
			flagmodem=TRUE;
	}    //若MODEM不在COM1口上,则在COM2口上搜索MODEM

	if(flagcom==FALSE)
		m_com="初始化串口......失败!";
	else  
		m_com="初始化串口......成功!";

	if(flagmodem==FALSE)
		m_modem="初始化调制解调器......失败!";
	else 
		m_modem="初始化调制解调器......成功!";

    SetTimer(TIMER_ID1,45,NULL);     //设置55毫秒定时器,用于读取串口数据
    SetTimer(TIMER_ID2,400,NULL); 
	return 0;
}

void CModemDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
       
	CloseHandle(hCommDev);
	KillTimer(TIMER_ID1);
	KillTimer(TIMER_ID2);
	CDialog::OnClose();
}


void CModemDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

    DWORD dwBytesWritten;
	if(nIDEvent==TIMER_ID1)
	{
		char strCommRecvMsg[50];
		DWORD dwBytesRead;
		BOOL bReturn=ReadFile(hCommDev,strCommRecvMsg,50,&dwBytesRead,NULL);
		if(!bReturn)
			return;
		if(dwBytesRead<1)
			return;

		CString strTmp,strTmp1; 
		strTmp=strCommRecvMsg;	 
		strTmp1=strTmp.Left(dwBytesRead);	 
       
		if(strstr(strTmp1,"VCON")!=NULL)
		{
			vcon=1;
			vconcount++;
			if(vconcount==1) 
				m_phone="检测到对方摘机!";
			else 
				m_phone="挂断电话!";
		}    
		else if(strstr(strTmp1,"RING")!=NULL)
			{
				m_phone="检测到振铃信号!";
				ring++;
			}    
		else if(strstr(strTmp1,"BUSY")!=NULL)
			m_phone="检测到忙音!";  
		else if(strstr(strTmp1,"CONNECT")!=NULL)
			m_phone="开始语音传送!";
		else if(strstr(strTmp1,"NO DIALTONE")!=NULL)
			m_phone="无拨号音!";
		UpdateData(FALSE);	
	}
	else if(nIDEvent==TIMER_ID2)
	{
		if(vcon==1&&vconcount==1)
		{ 
			vcon=0;
			WriteFile(hCommDev,"AT#VTX\r",strlen("AT#VTX\r"),&dwBytesWritten,NULL);
        }       		
	}

/*       switch(key)
	   {  
	     
	
		  case 12:if(vconcount==1){
			  //    DWORD dwBytesWritten;
                  WriteFile(hCommDev,"\r",strlen("\r"),&dwBytesWritten,NULL);
	              Sleep(500);
                  WriteFile(hCommDev,"ATZ\r",strlen("ATZ\r"),&dwBytesWritten,NULL);
				  vconcount=0;
                 GetDlgItem(IDhangup)->EnableWindow(FALSE);
                 GetDlgItem(IDcall)->EnableWindow(TRUE);
                  }
                  break;
		
		  default:break;
	   }	  

*/
	CDialog::OnTimer(nIDEvent);
}




void CModemDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	OnClose();
	EndDialog(1);	
}

void CModemDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	
}

void CModemDlg::OnHelp() 
{
	// TODO: Add your control notification handler code here
	CAboutHelpDlg a;
	a.DoModal();
}
