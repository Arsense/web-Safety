// Page7.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page7.h"
#include ".\page7.h"
#include "winsock2.h"

// CPage7 对话框

IMPLEMENT_DYNAMIC(CPage7, CDialog)
CPage7::CPage7(CWnd* pParent /*=NULL*/)
	: CDialog(CPage7::IDD, pParent)
	, m_EnumResource(_T(""))
{
}

CPage7::~CPage7()
{
}

void CPage7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_EnumResource);
}


BEGIN_MESSAGE_MAP(CPage7, CDialog)
END_MESSAGE_MAP()


// 获取计算机信息

int CPage7::GetInfo(void)
{
	struct hostent *host; 
	struct in_addr *ptr; // 检索IP地址  

	DWORD dwScope = RESOURCE_CONTEXT; 
	NETRESOURCE *NetResource = NULL; 
	HANDLE hEnum; 
	WNetOpenEnum( dwScope, NULL, NULL,  
				NULL, &hEnum ); 

	WSADATA wsaData; 
	WSAStartup(MAKEWORD(1,1),&wsaData); 

	if ( hEnum ) 
	{ 
		DWORD Count = 0xFFFFFFFF; 
		DWORD BufferSize = 2048; 
		LPVOID Buffer = new char[2048]; 
		WNetEnumResource( hEnum, &Count, Buffer, &BufferSize ); 
		NetResource = (NETRESOURCE*)Buffer; 

		char szHostName[200]; 
		unsigned int i; 

		for ( i = 0; i < BufferSize/sizeof(NETRESOURCE); i++, NetResource++ ) 
		{ 
			if ( NetResource->dwUsage == RESOURCEUSAGE_CONTAINER &&  
				NetResource->dwType == RESOURCETYPE_ANY ) 
			{ 
				if ( NetResource->lpRemoteName ) 
				{ 
					CString strFullName = NetResource->lpRemoteName; 
					if ( 0 == strFullName.Left(2).Compare("\\\\") )    
						strFullName = strFullName.Right(strFullName.GetLength()-2); 

					gethostname( szHostName, strlen( szHostName ) ); 
					host = gethostbyname(strFullName); 

					if(host == NULL) continue;  
					ptr = (struct in_addr *)host->h_addr_list[0];                     

					// =. 分隔开IP:211.40.35.76.              
					int a = ptr->S_un.S_un_b.s_b1;  // 211            
					int b = ptr->S_un.S_un_b.s_b2;  // 40 
					int c = ptr->S_un.S_un_b.s_b3;  // 35 
					int d = ptr->S_un.S_un_b.s_b4;  // 76 

					m_EnumResource.AppendFormat("计算机名称：%s-->IP地址：%d.%d.%d.%d\r\n", strFullName,a,b,c,d); 
				} 
			} 
		} 

		delete Buffer; 
		WNetCloseEnum( hEnum );  
	} 

	WSACleanup(); 
	return 0;
}

BOOL CPage7::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetInfo();
	UpdateData(False);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
