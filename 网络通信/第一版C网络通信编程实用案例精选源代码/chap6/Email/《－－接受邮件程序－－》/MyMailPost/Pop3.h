// Pop3.h: interface for the CPop3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POP3_H__C7D744E6_CB2C_44AC_9E1D_71D0D9F1D1CC__INCLUDED_)
#define AFX_POP3_H__C7D744E6_CB2C_44AC_9E1D_71D0D9F1D1CC__INCLUDED_

#include "Pop3Socket.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPop3  
{
public:
	BOOL Reset(void);
	// 获得邮件主题
	LPCTSTR GetMailHeader(UINT nIndex);
	// 发送NOOP命令，进行一次空操作
	BOOL Noop(void);
	// 获得出错信息
	LPCTSTR GetErrorString(void);
	// 发送STAT命令，获得邮件总体的状态信息
	BOOL GetState(void);
	// 发送LIST命令，获得邮件的具体状态信息
	BOOL List(void);
	// 发送RETR命令，接收邮件正文
	BOOL RetrieveMail(UINT nIndex,CString &strMsg);
	// 发送DELE命令，将所选邮件标记为删除
	BOOL Delete(UINT nIndex);
	// 发送TOP命令，获得所有的邮件主题
	BOOL ListHeader(void);
	// 私有变量接口函数，返回给定索引的邮件大小
	DWORD GetMsgSize(UINT nID);
	// 私有变量接口函数，返回邮件数目
	UINT GetMailCount(void) const;
	// 发送命令
	BOOL Send(LPCTSTR lpszSend,UINT nLength);
	// 断开连接，发送QUIT命令，并请求真正删除所要求的邮件
	BOOL Disconnect(void);
	// 建立与服务器的连接，其中包括登录的过程
	BOOL Connect(LPCTSTR strAddress,LPCTSTR strUser,LPCTSTR strPassword,UINT nPort = 110);
	CPop3();
	virtual ~CPop3();

protected:
	// 所有邮件的总体大小
	UINT m_nSize;
	// 通信的套接字
	SOCKET m_sock;
	// 邮件数目，以GetMailCount为接口函数
	UINT m_nMails;
	// 记录是否发送过有效的LIST命令
	BOOL m_bList;
	// 记录是否发送过有效的STAT命令
	BOOL m_bState;
	// 记录每个邮件大小的MFC数组变量
	CDWordArray m_arSizes;
	// 记录每个邮件主题的MFC数组变量
	CStringArray m_arHeaders;
	// 记录是否已经与服务器建立连接
	BOOL m_bConnected;
	// 记录回应的消息
	CString m_sResponse;
	// 确定是否超时，只要用去的时间超过该值就表示超时
	// 应该采取相应措施
	DWORD m_dwTimeout;

private:
	// 记录出错信息，以GetErrorString为接口函数
	CString m_sError;
	// 去除开头的+OK标识，获得回应的有效信息
	// 从返回的信息中提取数目和大小信息，主要用于LIST命令的回应信息
	BOOL ParseNumAndSize(UINT &nNum,UINT &nSize,LPCTSTR lpszBuf,UINT &nEnd);
	// 读取回应信息，与GetResponse函数不同，它进行实际的工作
	BOOL ReadResponse(TCHAR *pChar,UINT nSize,BOOL bDouble);
	BOOL GetResponse(void);
	// 读取针对LIST命令的返回信息
	// 由于对LIST命令的返回信息较特殊，所以用单独的函数
	BOOL ReadList(void);
	// 对STAT命令的返回信息，原因同上
	BOOL ReadState(void);
};

#endif // !defined(AFX_POP3_H__C7D744E6_CB2C_44AC_9E1D_71D0D9F1D1CC__INCLUDED_)
