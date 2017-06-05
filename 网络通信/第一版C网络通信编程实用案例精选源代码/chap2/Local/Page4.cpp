// Page4.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page4.h"
#include ".\page4.h"
#include "iphlpapi.h"

// 因为是通过NetAPI来获取网卡信息,所以需要包含其题头文件nb30.h
#include <nb30.h>


// CPage4 对话框

IMPLEMENT_DYNAMIC(CPage4, CDialog)
CPage4::CPage4(CWnd* pParent /*=NULL*/)
	: CDialog(CPage4::IDD, pParent)
	, m_MACAddress(_T(""))
	, m_MACAddress2(_T(""))
{
}

CPage4::~CPage4()
{
}

void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_MACAddress);
	DDX_Text(pDX, IDC_EDIT4, m_MACAddress2);
}


BEGIN_MESSAGE_MAP(CPage4, CDialog)
END_MESSAGE_MAP()


// 获取网卡MAC地址

int CPage4::GetInfo(void)
{
	//方法1：NetApi32
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER
		NameBuff [30];
	} ASTAT, * PASTAT;

	ASTAT Adapter;
	// 定义一个存放返回网卡信息的变量

      NCB Ncb;
      UCHAR uRetCode;
      char NetName[50];
      LANA_ENUM   lenum;
      int      i;

	  memset( &Ncb, 0, sizeof(Ncb) );
      Ncb.ncb_command = NCBENUM;
      Ncb.ncb_buffer = (UCHAR *)&lenum;
      Ncb.ncb_length = sizeof(lenum);
      uRetCode = Netbios( &Ncb );
      //printf( "The NCBENUM return code is: 0x%x \n", uRetCode );

      for(i=0; i < lenum.length ;i++)
      {
          memset( &Ncb, 0, sizeof(Ncb) );
          Ncb.ncb_command = NCBRESET;
          Ncb.ncb_lana_num = lenum.lana[i];

          uRetCode = Netbios( &Ncb );
          //printf( "The NCBRESET on LANA %d return code is: 0x%x \n",
          //        lenum.lana[i], uRetCode );

          memset( &Ncb, 0, sizeof (Ncb) );
          Ncb.ncb_command = NCBASTAT;
          Ncb.ncb_lana_num = lenum.lana[i];

          strcpy( (char*)Ncb.ncb_callname,  "*               " );
          Ncb.ncb_buffer = (PUCHAR) &Adapter;
          Ncb.ncb_length = sizeof(Adapter);

          uRetCode = Netbios( &Ncb );
          //printf( "The NCBASTAT on LANA %d return code is: 0x%x \n",
          //        lenum.lana[i], uRetCode );
          if ( uRetCode == 0 )
          {
             m_MACAddress.AppendFormat( "%02X-%02X-%02X-%02X-%02X-%02X\t",
                  Adapter.adapt.adapter_address[0],
                  Adapter.adapt.adapter_address[1],
                  Adapter.adapt.adapter_address[2],
                  Adapter.adapt.adapter_address[3],
                  Adapter.adapt.adapter_address[4],
                  Adapter.adapt.adapter_address[5] );
          }
       }
	  m_MACAddress.AppendFormat("%s","//调用NetApi32实现");
	  UpdateData(False);
 
	  //方法2：GetAdaptersInfo
	PIP_ADAPTER_INFO pinfo=NULL;
	unsigned long len=0;

	if (pinfo!=NULL)
		delete (pinfo);
	unsigned  long nError;

	//获得本地计算机的网络信息
	nError	=	GetAdaptersInfo(pinfo,&len);
	if (nError==0)
	{
		do
		{
			m_MACAddress2.AppendFormat("%02X-%02X-%02X-%02X-%02X-%02X\t",
				pinfo->Address[0],
				pinfo->Address[1],
				pinfo->Address[2],
				pinfo->Address[3],
				pinfo->Address[4],
				pinfo->Address[5]);
		}
		while(pinfo->Next != NULL);
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
				do
				{
					m_MACAddress2.AppendFormat("%02X-%02X-%02X-%02X-%02X-%02X\t",
						pinfo->Address[0],
						pinfo->Address[1],
						pinfo->Address[2],
						pinfo->Address[3],
						pinfo->Address[4],
						pinfo->Address[5]);
				}
				while(pinfo->Next != NULL);
			}
	}

	m_MACAddress2.AppendFormat("%s","//调用GetAdaptersInfo()实现");
	UpdateData(False);

	if (pinfo!=NULL)
		delete (pinfo);	

	return 0;
}

BOOL CPage4::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
