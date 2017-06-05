//
// TERMSOCK.H -- Socket derived from CAsyncSocket
//

#ifndef __TERMSOCK_H__
#define __TERMSOCK_H__

//网络连接类
class CTermSocket : public CAsyncSocket
{
	DECLARE_DYNAMIC(CTermSocket);

// Implementation
protected:
	//连接
	virtual void OnConnect(int nErrorCode);
	//发送数据
	virtual void OnSend(int nErrorCode);
	//接收数据
	virtual void OnReceive(int nErrorCode);
	//关闭
	virtual void OnClose(int nErrorCode);
};
#endif // __TERMSOCK_H__
