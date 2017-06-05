// Pop3.cpp: implementation of the CPop3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MailPost.h"
#include "Pop3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPop3::CPop3()
{
	// 初始化变量
	m_sError = _T("");
	m_sock = INVALID_SOCKET;
	m_nMails = 0;
	m_nSize = 0;
	m_bList = FALSE;
	m_bState = FALSE;
	m_arSizes.RemoveAll();
	m_arHeaders.RemoveAll();
	m_bConnected = FALSE;
	m_dwTimeout = 2000;
}

CPop3::~CPop3()
{

}

BOOL CPop3::Connect(LPCTSTR strAddress, LPCTSTR strUser, LPCTSTR strPassword,UINT nPort)
{
	// 初始化本地套接字
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		m_sError = _T("初始化套接字失败");
		return FALSE;
	}

	// 初始化远端套接字
	SOCKADDR_IN sockAddr;
	memset(&sockAddr,'\0',sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons((u_short)nPort);
	sockAddr.sin_addr.s_addr = inet_addr(strAddress);

	// 如果主机名不是IP地址，用gethostbyname获得其IP地址
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(strAddress);
		if (lphost)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			m_sError = _T("无法解析地址!"); 
			return FALSE;
		}
	}
	
	// 尝试与服务器连接
	if (connect(m_sock,(SOCKADDR *)(&sockAddr),sizeof(sockAddr)) == SOCKET_ERROR)
	{
		m_sError = _T("无法与服务器建立连接!");
		return FALSE;
	}

	// 接收回应信息
	if (!GetResponse())
	{
		// 回应失败
		m_sError = _T("无法收到回应信息或收到错误信息!");
		Disconnect();
		return FALSE;
	}

	// 发送用户名
	CString strTemp;
	strTemp.Format("USER %s\r\n",strUser);
    if (!Send((LPCTSTR)strTemp,strTemp.GetLength()))
    {
		// 发送用户名失败
		m_sError = _T("发送用户名失败!");
		return FALSE;
    }

	// 接收回应信息
    if (!GetResponse())
    {
		// 接收回应信息失败
		m_sError = _T("无法收到回应信息或收到无效信息!");
		Disconnect();
		return FALSE;
    } 

    // 发送密码
	strTemp.Format("PASS %s\r\n",strPassword);
    if (!Send((LPCTSTR)strTemp,strTemp.GetLength()))
    {
		// 发送密码失败
		m_sError = _T("发送密码失败!");
		Disconnect();
		return FALSE;
    }

	// 接收回应信息
    if (!GetResponse())
    {
		// 接收回应信息失败
		m_sError = _T("无法收到回应信息或收到无效信息!");
		Disconnect();
		return FALSE;
    } 

	m_bConnected = TRUE;
	return TRUE;
}

BOOL CPop3::GetResponse(void)
{
	TCHAR chArray[1188];
	if (!ReadResponse(chArray,1188,FALSE))
	{
		return FALSE;
	}
	m_sResponse = chArray;
	return TRUE;
}

BOOL CPop3::ReadResponse(TCHAR *pChar, UINT nSize, BOOL bDouble)
{
	CString strTemp;

	// 读取回应信息
	BOOL bEnd = FALSE;
	UINT nReceived = 0;
	DWORD dwStart = ::GetTickCount();
	while (!bEnd)
	{
		// 尝试时间到
		if ((::GetTickCount() - dwStart) >	m_dwTimeout)
		{
			pChar[nReceived] = '\0';
			m_sError = _T("超时");
			// 保存当前回应的消息
			m_sResponse = pChar;
			return FALSE;
		}

		// 看套接字是否可读
		timeval timeout = {0, 0};
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(m_sock,&fds);
		int nStatus = select(0,&fds,NULL,NULL,&timeout);

		if (nStatus == SOCKET_ERROR)
		{
			// 套接字不可读
			m_sError = _T("套接字不可达");
			return FALSE;
		}
		else if (!nStatus)
		{
			// 没有接收到数据
			::Sleep(688);
			continue;
		}

		// 从套接字中接收数据
		::Sleep(288);
		nStatus = recv(m_sock,pChar + nReceived,nSize,0);
		if (nStatus == SOCKET_ERROR)
		{
			pChar[nReceived] = '\0';
			// 套接字错误
			m_sError = _T("未能从套接字中收到数据!");
			// 保存当前回应信息
			m_sResponse = pChar; 
			return FALSE; 
		}
		else if (nStatus)
		{
			// 重置计时器
			dwStart = ::GetTickCount();
			// 已收到的数据又增加了
			nReceived += nStatus;
		}

		// 将pChar设为字符串，并赋给CString型的变量
		pChar[nReceived] = '\0';
		strTemp = pChar;
								
		// 检查是否收到了结束标志
		LPCTSTR lpszComp = bDouble ? "\r\n.\r\n" : "\r\n";
		bEnd = (strTemp.Find(lpszComp) != -1);
	}

	// 去掉结束标志
	nReceived -= bDouble ? 3 : 0;
	pChar[nReceived] = '\0';

	// 检查回应信息是否有效
	strTemp = pChar;
	strTemp.MakeUpper();
	int nStart = strTemp.Find("+OK");
	if (nStart == -1)
	{
		// 收到无效信息
		m_sError = _T("回应信息无效");
		return FALSE;
	}
	// 提取有效信息
	strTemp = pChar;
	m_sResponse = strTemp.Right(strTemp.GetLength() - nStart - 3);
	
	return TRUE;
}

BOOL CPop3::Disconnect()
{
	BOOL bSuccess;
	
	// 如果还保持连接状态则断开
	if (m_bConnected)
	{
		CString strSend = "QUIT\r\n";
		// 发送断开请求
		if (!this->Send((LPCTSTR)strSend,strSend.GetLength()))
		{
			AfxMessageBox(_T("发送断开请求失败"));
			return FALSE;
		}

		bSuccess = this->GetResponse();

		// 将有关的变量复位
	    m_bConnected = FALSE;
		m_bList = FALSE;
	    m_bState = FALSE;
		m_nMails = 0;
		m_nSize = 0;
		m_arHeaders.RemoveAll();
		m_arSizes.RemoveAll();

		// 关闭套接字
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;
	}

	return bSuccess;
}

BOOL CPop3::Send(LPCTSTR lpszSend, UINT nLength)
{
	return send(m_sock,lpszSend,nLength,0) != SOCKET_ERROR;
}

BOOL CPop3::ReadState()
{
	// 获得返回的状态信息
	if (!GetResponse())
	{
		// 接收返回信息失败
		m_sError = _T("接收返回信息失败!");
		return FALSE;
	}

	// 看返回的信息是否有效
	m_sResponse.MakeUpper();
	UINT nEnd = 0;
	if (m_sResponse.Find("+OK") == -1)
	{
		// 收到无效的回应信息
		m_sError = _T("返回信息无效!");
		return FALSE;
	}
	else
	{ 
		// 分析邮件数和大小，首先分析数目
		return this->ParseNumAndSize(m_nMails,m_nSize,(LPCTSTR)m_sResponse,nEnd);
	}
}

UINT CPop3::GetMailCount() const
{
	return m_nMails;
}

BOOL CPop3::ParseNumAndSize(UINT &nNum, UINT &nSize,LPCTSTR lpszBuf,UINT &nEnd)
{
	BOOL bNum = TRUE;
	for (int i = nEnd,j = -1;lpszBuf[i];i++)
	{
		if (isdigit(lpszBuf[i]))
			j = (j == -1) ? i : j;
		else
		{	
			if (j == -1)
			{
				// 还未读入数字
				continue;
			}

			// 分析完一个字段
			else if (bNum)
			{
				// 分析数目
				m_nMails = atoi(&lpszBuf[j]);
				nNum = m_nMails;
				bNum = FALSE;
				j = -1;
			}

			else
			{
				// 分析大小
				m_nSize = atoi(&lpszBuf[j]);
				nSize = m_nSize;
				nEnd = i;
				return TRUE;
			}
		}
	}
	nEnd = i;
	return FALSE;
}

BOOL CPop3::ReadList()
{
	UINT nBuf = m_nMails * 18 + 188;
	char *pBuf = new char[nBuf];
	if (!pBuf)
	{
		return FALSE;
	}
	if (!ReadResponse(pBuf,nBuf,TRUE))
	{
		return FALSE;
	}
	m_arSizes.RemoveAll();
	m_arSizes.SetSize(m_nMails);
	UINT nNum,nSize,nEnd = 0;
	while (ParseNumAndSize(nNum,nSize,m_sResponse,nEnd))
	{
		m_arSizes.SetAt(nNum - 1,nSize);
	}

	return TRUE;
}

DWORD CPop3::GetMsgSize(UINT nID) 
{
	BOOL bSuccess;
	if (!m_bList)
		bSuccess = this->List();
	if (!bSuccess)
		return 0;

	return m_arSizes.GetAt(nID);
}

BOOL CPop3::ListHeader()
{
	m_arHeaders.RemoveAll();
	m_arHeaders.SetSize(m_nMails);
	CString strTemp;
	for (UINT nMsg = 1;nMsg <= m_nMails;nMsg++)
	{
		strTemp.Format("TOP %d 0\r\n",nMsg);
		if (!Send((LPCTSTR)strTemp,strTemp.GetLength()))
		{
			m_sError = _T("发送TOP命令失败!");
			return FALSE;
		}

		if (!GetResponse())
		{
			m_sError = _T("列举邮件主题失败!");
			return FALSE;
		}
		
		strTemp = m_sResponse;
		strTemp.MakeUpper();
		
		// 查找SUBJECT段
		UINT nStart = strTemp.Find("\r\nSUBJECT:");
		if (nStart == -1)
		{
			m_arHeaders.SetAt(nMsg - 1,_T(""));			
			return TRUE;
		}
		strTemp = m_sResponse.Right(m_sResponse.GetLength() - nStart - 10);
		nStart = strTemp.Find("\r\n");
		if (nStart != -1)
			strTemp = strTemp.Left(nStart);
		strTemp.TrimLeft();
		strTemp.TrimRight();
		m_arHeaders.SetAt(nMsg - 1,strTemp);
	}

	return TRUE;
}

BOOL CPop3::Delete(UINT nIndex)
{
	CString strSend;
	strSend.Format("DELE %d\r\n",nIndex);

	// 发送DELE命令
	if (!Send((LPCTSTR)strSend,strSend.GetLength()))
	{
		// 未能成功发送
		m_sError = _T("删除邮件失败!");
		return FALSE;
	}

	// 接收返回信息
	if (!GetResponse())
	{
		m_sError = _T("收到无效信息!");
		return FALSE;
	}
	
	// 上一次STAT和LIST命令得到的信息无效
	m_bState = FALSE;
	m_bList = FALSE;
	return TRUE;
}

BOOL CPop3::RetrieveMail(UINT nIndex,CString &strMsg)
{
	CString strSend;
	strSend.Format("RETR %d\r\n",nIndex);
	if (!Send((LPCTSTR)strSend,strSend.GetLength()))
	{
		m_sError = _T("收取邮件失败!");
		return FALSE;
	}

	if (!GetResponse())
	{
		m_sError = _T("无法收到送回的邮件!");
		return FALSE;
	}

	// 以CONTENT-DISPOSITION: INLINE为起始标志
	strMsg = m_sResponse;
	strMsg.MakeUpper();
	int nStart = strMsg.Find("CONTENT-DISPOSITION: INLINE\r\n");
	strMsg = m_sResponse.Right(m_sResponse.GetLength() - nStart - 29);

	// 空行表示邮件正文
	nStart = strMsg.Find("\r\n\r\n");
	strMsg = strMsg.Right(strMsg.GetLength() - nStart - 4);
	// 空行表示邮件正文结束
	nStart = strMsg.Find("\r\n\r\n");
	if (nStart != -1)
		strMsg = strMsg.Left(nStart);
	return TRUE;
}

BOOL CPop3::List()
{
	CString strTemp = "LIST\r\n";
	if (m_bList)
		return TRUE;
	if (!Send((LPCTSTR)strTemp,strTemp.GetLength()))
	{
		// 发送失败
		m_sError = _T("查询邮箱状态失败!");
		Disconnect();
		return FALSE;
	}

	if (!ReadList())
	{
		// 接收回应信息失败
		m_sError = _T("无法收到邮件状态信息或收到无效信息!");
		Disconnect();
		return FALSE;
    }

	return TRUE;
}

BOOL CPop3::GetState()
{
	CString strTemp = "STAT\r\n";
	if (m_bState)
		return TRUE;
	if (!Send((LPCTSTR)strTemp,strTemp.GetLength()))
	{
		// 发送失败
		m_sError = _T("查询邮箱状态失败!");
		Disconnect();
		return FALSE;
	}

    if (!ReadState())
    {
		// 接收回应信息失败
		m_sError = _T("无法收到邮箱状态信息或收到无效信息!");
		Disconnect();
		return FALSE;
    }

	return TRUE;
}

LPCTSTR CPop3::GetErrorString()
{
	return (LPCTSTR)m_sError;
}

BOOL CPop3::Noop()
{
	CString strTemp = "NOOP\r\n";

	// 发送NOOP命令
	if (!Send((LPCTSTR)strTemp,strTemp.GetLength()))
	{
		// 发送失败
		m_sError = _T("进行空操作失败!");
		Disconnect();
		return FALSE;
	}

	// 接收回应信息
    if (!GetResponse())
    {
		// 接收回应信息失败
		m_sError = _T("无法收到回应信息或收到无效信息!");
		Disconnect();
		return FALSE;
    }

	return TRUE;
}

LPCTSTR CPop3::GetMailHeader(UINT nIndex)
{
	return (LPCTSTR)m_arHeaders.GetAt(nIndex);
}


BOOL CPop3::Reset()
{
	CString strTemp = "RSET\r\n";

	// 发送RSET命令
	if (!Send((LPCTSTR)strTemp,strTemp.GetLength()))
	{
		m_sError = _T("发送RSET命令失败!");
		return FALSE;
	}
	
	// 接收回应信息
	if (!GetResponse())
	{
		// 接收回应信息失败
		m_sError = _T("回应失败!");
		return FALSE;
	}

	return TRUE;
}
