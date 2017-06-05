// Page3.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page3.h"
#include ".\page3.h"
#include "Iphlpapi.h"

// CPage3 对话框

IMPLEMENT_DYNAMIC(CPage3, CDialog)
CPage3::CPage3(CWnd* pParent /*=NULL*/)
	: CDialog(CPage3::IDD, pParent)
	, m_DNS(_T(""))
{
}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_DNS);
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
END_MESSAGE_MAP()


// 获取计算机信息

int CPage3::GetInfo(void)
{
   FIXED_INFO * FixedInfo;
   ULONG    ulOutBufLen;
   DWORD    dwRetVal;
   IP_ADDR_STRING * pIPAddr;

   FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, sizeof( FIXED_INFO ) );
   ulOutBufLen = sizeof( FIXED_INFO );
   
   if( ERROR_BUFFER_OVERFLOW == GetNetworkParams( FixedInfo, &ulOutBufLen ) ) {
      GlobalFree( FixedInfo );
      FixedInfo = (FIXED_INFO *)GlobalAlloc( GPTR, ulOutBufLen );
   }

   if ( dwRetVal = GetNetworkParams( FixedInfo, &ulOutBufLen ) ) {
        printf( "Call to GetNetworkParams failed. Return Value: %08x\n", dwRetVal );
   }
   else {
	   //获得计算机名称
	   //printf( "Host Name: %s\n", FixedInfo -> HostName );
	   //获得计算机域名
	   //printf( "Domain Name: %s\n", FixedInfo -> DomainName );
      
      //获得DNS服务器设置
	  m_DNS.Format("%s\r\n",FixedInfo -> DnsServerList.IpAddress.String);

      pIPAddr = FixedInfo -> DnsServerList.Next;
      while ( pIPAddr ) {
         m_DNS.AppendFormat( "%s\r\n", pIPAddr ->IpAddress.String );
         pIPAddr = pIPAddr ->Next;
      }
      UpdateData(False);
   }
	return 0;
}

BOOL CPage3::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
