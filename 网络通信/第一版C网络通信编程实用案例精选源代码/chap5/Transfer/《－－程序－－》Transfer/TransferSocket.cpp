// TransferSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Transfer.h"
#include "TransferSocket.h"
#include "TransferDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferSocket

CTransferSocket::CTransferSocket()
{
	m_aSessionIn=NULL;
	m_aSessionOut=NULL;
	m_sfSocketFile=NULL;
	m_bInit=false;
	m_bClose=false;
}

CTransferSocket::~CTransferSocket()
{
	if(m_aSessionIn)
		delete m_aSessionIn;
	if(m_aSessionOut)
		delete m_aSessionOut;
	if(m_sfSocketFile)
		delete m_sfSocketFile;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CTransferSocket, CSocket)
	//{{AFX_MSG_MAP(CTransferSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CTransferSocket member functions

void CTransferSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
		CMessg msg;
		msg.Serialize(*m_aSessionIn);

		if(msg.m_iCommand==0)
		{
			m_dlg->m_RemoteShareFiles.RemoveAll();
			CListBox* list=(CListBox*)(m_dlg->GetDlgItem(IDC_REMOTE_FILE_LIST));
			list->ResetContent();
			CString temp,name;
			int i;
			temp=msg.m_strText;
			TRACE("%s\n",msg.m_strText);
			temp.TrimLeft("!");//!
			while((i=temp.Find("!"))!=-1)
			{

				name=temp.Left(i);
				TRACE("%s\n",name);
				TRACE("%s\n",temp);
				temp=temp.Mid(name.GetLength()+1);
				m_dlg->m_RemoteShareFiles.Add(name);
				list->AddString(name);
			}
			m_dlg->m_RemoteShareFiles.Add(temp);
			list->AddString(temp);
			return;
		}

		if(msg.m_iCommand==1)
		{
			CString temp,name;
			int i;
			temp=msg.m_strText;
			temp.TrimLeft("!");//!
			while((i=temp.Find("!"))!=-1)
			{
				name=temp.Left(i);
				temp=temp.Mid(name.GetLength()+1);
				m_dlg->m_NeedTransferFiles.Add(name);
			}
			m_dlg->m_NeedTransferFiles.Add(temp);
			m_dlg->SetWindowText("需要传输的文件名称处理完毕!");
			for(i=0;i<m_dlg->m_NeedTransferFiles.GetSize();i++)
			{
				CMessg m;
				TRACE("%s\n",m_dlg->m_NeedTransferFiles[i]);
				CFile file(m_dlg->m_NeedTransferFiles[i],CFile::modeRead|CFile::shareDenyWrite);
				m_dlg->SetWindowText("正在传输"+m_dlg->m_NeedTransferFiles[i]);
				byte *buffer=NULL;
				long count=file.GetLength();
				buffer= new byte[count];
				file.Read(buffer,count);
				m.m_iCommand=2;
				m.m_strText=buffer;
				m_dlg->m_TransferSocket.SendMessage(&m);
				m_dlg->SetWindowText(m_dlg->m_NeedTransferFiles[i]+"传输完毕!");
				file.Close();
			}
			m_dlg->m_NeedTransferFiles.RemoveAll();
			return;
		}

		//接收文件
		if(msg.m_iCommand==2)
		{
			int k=m_dlg->m_NeedReceiveFiles[0].ReverseFind('\\');
			CString tt;
			tt=m_dlg->m_NeedReceiveFiles[0].Mid(k+1);
			tt="E:\\"+tt;
			if(m_file.Open(tt,CFile::modeWrite|CFile::modeCreate)==FALSE)
			{
				AfxMessageBox("Failed to open the file!");
				m_file.Close();
			}
			m_file.Write(msg.m_strText,msg.m_strText.GetLength());
			m_file.Close();
			AfxMessageBox(m_dlg->m_NeedReceiveFiles[0]+"下载完毕!");
			return;
		}
	CSocket::OnReceive(nErrorCode);
}

void CTransferSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_bClose=true;
	CloseSocket();
//	m_dlg->CloseSessionSocket();	
	CSocket::OnClose(nErrorCode);	
}

void CTransferSocket::Init(CTransferDlg * dlg)
{
	m_sfSocketFile= new CSocketFile(this);
	m_aSessionIn=new CArchive(m_sfSocketFile,CArchive::load);
	m_aSessionOut=new CArchive(m_sfSocketFile,CArchive::store);
	m_bClose=false;
	this->m_dlg=dlg;
}

BOOL CTransferSocket::SendMessage(CMessg * msg)
{
	if (m_aSessionOut != NULL)
	{
		msg->Serialize(*m_aSessionOut);
		m_aSessionOut->Flush();
		return TRUE;
	}
	else
	{
		//对方关闭了连接
		m_bClose=true;
		CloseSocket();
//		m_dlg->CloseSessionSocket();
		return FALSE;
	}
}


void CTransferSocket::CloseSocket()
{
	if(m_aSessionIn)
	{
		delete m_aSessionIn;
		m_aSessionIn=NULL;
	}
	if(m_aSessionOut)
	{
		delete m_aSessionOut;
		m_aSessionOut=NULL;
	}
	if(m_sfSocketFile)
	{
		delete m_aSessionOut;
		m_sfSocketFile=NULL;
	}
	Close();
	m_bInit=false;
	m_bClose=true;
}