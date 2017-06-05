// Page6.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page6.h"
#include ".\page6.h"


// CPage6 对话框

IMPLEMENT_DYNAMIC(CPage6, CDialog)
CPage6::CPage6(CWnd* pParent /*=NULL*/)
	: CDialog(CPage6::IDD, pParent)
	, m_ServiceName(_T(""))
	, m_ServiceName2(_T(""))
{
}

CPage6::~CPage6()
{
}

void CPage6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_ServiceName);
	DDX_Text(pDX, IDC_EDIT4, m_ServiceName2);
}


BEGIN_MESSAGE_MAP(CPage6, CDialog)
END_MESSAGE_MAP()


// CPage6 消息处理程序

BOOL CPage6::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetInfo();
	UpdateData(False);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 获取计算机信息
int CPage6::GetInfo(void)
{
	struct servent *servrec;
	struct sockaddr_in serv_addr;

	WSADATA     WSAData;
	int WSAReturn;
	//在使用WinSock API之前，必须调用WSAStartup函数，
	//只有该函数成功返回（表示与WinSock库成功地建立起连接），
	//才可以调用其他Windows Sockets DLL中的函数
	WSAReturn = WSAStartup( 0x0101, &WSAData );
	if( WSAReturn == 0 ){
		//使用getservbyname获得服务
		m_ServiceName.AppendFormat("例：使用getservbyname获取ftp服务\r\n");
		servrec = getservbyname("ftp", "tcp");
		if (servrec != NULL) {
			m_ServiceName.AppendFormat("正规的服务名：%s\r\n",servrec->s_name);
			m_ServiceName.AppendFormat("可选服务名队列：%s\r\n",servrec->s_aliases);
			m_ServiceName.AppendFormat("连接该服务时需要用到的端口号（按网络字节序）：%d\r\n",servrec->s_port);
			m_ServiceName.AppendFormat("连接该服务时用到的协议名：%s\r\n",servrec->s_proto);
		} 
		//根据getservbyport获得服务
		m_ServiceName2.AppendFormat("\r\n例：使用getservbyport获取ftp服务\r\n");
		servrec = getservbyport(servrec->s_port, "tcp");
		if (servrec != NULL) {
			m_ServiceName2.AppendFormat("正规的服务名：%s\r\n",servrec->s_name);
			m_ServiceName2.AppendFormat("可选服务名队列：%s\r\n",servrec->s_aliases);
			m_ServiceName2.AppendFormat("连接该服务时需要用到的端口号（按网络字节序）：%d\r\n",servrec->s_port);
			m_ServiceName2.AppendFormat("连接该服务时用到的协议名：%s\r\n",servrec->s_proto);
		} 

	}
	//调用WSACleanup函数进行WinSock的清理工作，以便释放其占用的资源
	WSACleanup();

	return 0;
}
