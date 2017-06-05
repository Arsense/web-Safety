//this is base class for CPop3
//mostly, you don't use it directly (exceptions are Set, and OnConnect)
//CPop3 will find this class useful

#if !defined(AFX_GNIAZDO_H__03185426_9BCB_420B_ADA4_4561790828A8__INCLUDED_)
#define AFX_GNIAZDO_H__03185426_9BCB_420B_ADA4_4561790828A8__INCLUDED_


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Gniazdo.h : header file
//

#define DLG CPop3Dlg* //change it to your CDialog-based class

/////////////////////////////////////////////////////////////////////////////
// CBase4Pop3 command target

// CBase4Pop3 socket messages:
#define S_ACCEPT 0 
#define S_CLOSE 1
#define S_CONNECT 2
#define S_RECEIVE 3
#define S_SEND 4


class CBase4Pop3 : public CAsyncSocket
{
public:
	CBase4Pop3();
	virtual ~CBase4Pop3();

// Overrides
public:
	//设定接收消息窗体
	void Set(CDialog * pWnd);
	CDialog * m_pWnd; //指向父窗体

// Implementation
protected:
	//几个socket处理函数
	virtual void OnReceive(int);
	virtual void OnClose(int);
	virtual void OnSend(int);
	virtual void OnConnect(int);
	virtual void OnAccept(int);
private:
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GNIAZDO_H__03185426_9BCB_420B_ADA4_4561790828A8__INCLUDED_)
