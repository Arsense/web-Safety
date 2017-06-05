// Page1.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page1.h"
#include ".\page1.h"

// CPage1 对话框

IMPLEMENT_DYNAMIC(CPage1, CDialog)
CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CPage1::IDD, pParent)
	, m_GetComputerName(_T(""))
	, m_gethostname(_T(""))
	, m_IPAddress(_T(""))
{
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_GetComputerName);
	DDX_Text(pDX, IDC_EDIT2, m_gethostname);
	DDX_Text(pDX, IDC_EDIT3, m_IPAddress);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
END_MESSAGE_MAP()


// CPage1 消息处理程序

BOOL CPage1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 获取计算机名称和IP地址
int CPage1::GetInfo(void)
{
	#define BUFSIZE 1024

	LPTSTR lpszSystemInfo;      // pointer to system information string 
	DWORD cchBuff = BUFSIZE;    // size of computer
	TCHAR tchBuffer[BUFSIZE];   // buffer for string

	lpszSystemInfo = tchBuffer; 

	// Get and display the name of the computer. 
	if( GetComputerName(lpszSystemInfo, &cchBuff) )
	{
		m_GetComputerName.AppendFormat("%s\t//GetComputerName()", lpszSystemInfo);
		UpdateData(False);
	}

	WSADATA     WSAData;
	int WSAReturn;
	//在使用WinSock API之前，必须调用WSAStartup函数，
	//只有该函数成功返回（表示与WinSock库成功地建立起连接），
	//才可以调用其他Windows Sockets DLL中的函数
	WSAReturn = WSAStartup( 0x0101, &WSAData );
	if( WSAReturn == 0 ){
		if( gethostname(lpszSystemInfo, cchBuff) )
		{
			m_gethostname.AppendFormat("%s\t//gethostname()", lpszSystemInfo);
			UpdateData(False);
		}

		struct hostent *p_HostEnt;
		p_HostEnt = gethostbyname(lpszSystemInfo);
		if(p_HostEnt != NULL)
		{
			char  Hostaddress[20]; // 主机IP地址
			wsprintf( Hostaddress, " %d. %d. %d. %d",
			( p_HostEnt->h_addr_list[0][0] & 0x00ff ),
			( p_HostEnt->h_addr_list[0][1] & 0x00ff ),
			( p_HostEnt->h_addr_list[0][2] & 0x00ff ),
			( p_HostEnt->h_addr_list[0][3] & 0x00ff ) );
			m_IPAddress.Format(Hostaddress);
			UpdateData(False);
			//调用WSACleanup函数进行WinSock的清理工作，以便释放其占用的资源
			WSACleanup();
		}
	}

	return 0;
}
