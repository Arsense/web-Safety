#pragma once
// CBasePop3Socket 命令目标


#define ACCEPT 0 
#define CLOSE 1
#define CONNECT 2
#define RECEIVE 3
#define SEND 4
class CBasePop3Socket : public CAsyncSocket
{
public:
	CBasePop3Socket();
	virtual ~CBasePop3Socket();
	//设定接收消息窗体
	void SetMsgWnd(CDialog * pWnd);
	CDialog * m_pWnd; //指向父窗体

	


	
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};


