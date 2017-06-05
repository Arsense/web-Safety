// Page8.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page8.h"
#include ".\page8.h"
#include "Iphlpapi.h"


// CPage8 对话框

IMPLEMENT_DYNAMIC(CPage8, CDialog)
CPage8::CPage8(CWnd* pParent /*=NULL*/)
	: CDialog(CPage8::IDD, pParent)
	, m_MACAddress(_T(""))
	, m_InterfaceName(_T(""))
	, m_dwIndex(0)
	, m_dwType(0)
	, m_dwAddressLength(0)
{
}

CPage8::~CPage8()
{
}

void CPage8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddress);
	DDX_Text(pDX, IDC_EDIT3, m_InterfaceName);
}


BEGIN_MESSAGE_MAP(CPage8, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// 获取计算机信息

int CPage8::GetInfo(void)
{
	PIP_ADAPTER_INFO pinfo=NULL;
	unsigned long len=0;

	if (pinfo!=NULL)
		delete (pinfo);
	unsigned  long nError;

	//获得本地计算机的网络信息
	nError	=	GetAdaptersInfo(pinfo,&len);
	if (nError==0)
	{
		m_MACAddress.Format("%d:%d:%d:%d:%d:%d",
			pinfo->Address[0],
			pinfo->Address[1],
			pinfo->Address[2],
			pinfo->Address[3],
			pinfo->Address[4],
			pinfo->Address[5]);
		m_InterfaceName	=	pinfo->Description;
		m_dwIndex = pinfo->Index;
		m_dwType = pinfo->Type;
		m_dwAddressLength = pinfo->AddressLength;
		
		 PIP_ADDR_STRING pAddressList = &(pinfo->IpAddressList);
		 CString ipaddr = pAddressList->IpAddress.String;
		 CString temp = ipaddr;
		 int i_dotoldposition=0;
		 int i_dotposition = ipaddr.Find('.',0);
		 int a=atoi(temp.Mid(i_dotoldposition,i_dotposition));
		 i_dotoldposition = i_dotposition;
		 temp = ipaddr;
		 i_dotposition = ipaddr.Find('.',i_dotposition);

		 int b=atoi(temp.Mid(i_dotoldposition,i_dotposition));
		 i_dotoldposition = i_dotposition;
		 temp = ipaddr;
		 i_dotposition = ipaddr.Find('.',i_dotposition);

		 int c=atoi(temp.Mid(i_dotoldposition,i_dotposition));
		 i_dotoldposition = i_dotposition;
		 temp = ipaddr;
		 i_dotposition = ipaddr.Find('.',i_dotposition);

		 int d=atoi(temp.Mid(i_dotoldposition,i_dotposition));

		 m_IPAddress.SetAddress(a,b,c,d);
		 UpdateData(False);
//		  do 
//		  {
//			  m_IpAddress	+=	pAddressList->IpAddress.String;
//			 pAddressList = pAddressList->Next;
//			 if (pAddressList != NULL)
//				m_IpAddress	+="\r\n";
//		  } while (pAddressList != NULL);
	}
	if (nError==ERROR_NO_DATA)
	{
		AfxMessageBox("No adapter information exists for the local computer");
	}
	if (nError==ERROR_NOT_SUPPORTED)
	{
		AfxMessageBox("GetAdaptersInfo is not supported by the operating system running on the local computer");
	}
	if (nError==ERROR_BUFFER_OVERFLOW)
	{
		pinfo= (PIP_ADAPTER_INFO)malloc(len);
		nError	=	GetAdaptersInfo(pinfo,&len);
		if (nError==0)
		{
		m_MACAddress.Format("%d:%d:%d:%d:%d:%d",
			pinfo->Address[0],
			pinfo->Address[1],
			pinfo->Address[2],
			pinfo->Address[3],
			pinfo->Address[4],
			pinfo->Address[5]);
		m_InterfaceName	=	pinfo->Description;
		m_dwIndex = pinfo->Index;
		m_dwType = pinfo->Type;
		m_dwAddressLength = pinfo->AddressLength;
		
		 PIP_ADDR_STRING pAddressList = &(pinfo->IpAddressList);
		 CString ipaddr = pAddressList->IpAddress.String;
		 CString temp = ipaddr;
		 int i_dotoldposition=0;
		 int i_dotposition = ipaddr.Find('.',0);
		 int a=atoi(temp.Mid(i_dotoldposition,i_dotposition));
		 
		 i_dotoldposition = i_dotposition;
		 temp = ipaddr;
		 i_dotposition = ipaddr.Find('.',i_dotposition+1);
		 int b=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

		 i_dotoldposition = i_dotposition;
		 temp = ipaddr;
		 i_dotposition = ipaddr.Find('.',i_dotposition+1);
		 int c=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

		 i_dotoldposition = i_dotposition;
		 temp = ipaddr;
		 i_dotposition = ipaddr.GetLength();
		 int d=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

		 m_IPAddress.SetAddress(a,b,c,d);
		 UpdateData(False);
		}
	}

	if (pinfo!=NULL)
		delete (pinfo);
	return 0;
}

BOOL CPage8::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetInfo();
	UpdateData(False);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage8::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dw_IPAddress;
	m_IPAddress.GetAddress(dw_IPAddress);
	MIB_IPNETROW *p_mib_ipnetrow = new MIB_IPNETROW;
	
	CString temp = m_MACAddress;
	int i_dotoldposition=0;
	int i_dotposition = m_MACAddress.Find(':',0);
	int a=atoi(temp.Mid(i_dotoldposition,i_dotposition));
	
	i_dotoldposition = i_dotposition;
	temp = m_MACAddress;
	i_dotposition = m_MACAddress.Find(':',i_dotposition+1);
	int b=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

	i_dotoldposition = i_dotposition;
	temp = m_MACAddress;
	i_dotposition = m_MACAddress.Find(':',i_dotposition+1);
	int c=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

	i_dotoldposition = i_dotposition;
	temp = m_MACAddress;
	i_dotposition = m_MACAddress.Find(':',i_dotposition+1);
	int d=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

	i_dotoldposition = i_dotposition;
	temp = m_MACAddress;
	i_dotposition = m_MACAddress.Find(':',i_dotposition+1);
	int e=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

	i_dotoldposition = i_dotposition;
	temp = m_MACAddress;
	i_dotposition = m_MACAddress.GetLength();
	int f=atoi(temp.Mid(i_dotoldposition+1,i_dotposition));

	p_mib_ipnetrow->bPhysAddr[0] = a;
	p_mib_ipnetrow->bPhysAddr[1] = b;
	p_mib_ipnetrow->bPhysAddr[2] = c;
	p_mib_ipnetrow->bPhysAddr[3] = d;
	p_mib_ipnetrow->bPhysAddr[4] = e;
	p_mib_ipnetrow->bPhysAddr[5] = f;

	p_mib_ipnetrow->dwAddr = dw_IPAddress;
	p_mib_ipnetrow->dwIndex = m_dwIndex;
	p_mib_ipnetrow->dwPhysAddrLen = m_dwAddressLength;
	p_mib_ipnetrow->dwType = m_dwType;
	if( SetIpNetEntry(p_mib_ipnetrow) != NO_ERROR)
		AfxMessageBox("出错！");;
	delete p_mib_ipnetrow;
}
