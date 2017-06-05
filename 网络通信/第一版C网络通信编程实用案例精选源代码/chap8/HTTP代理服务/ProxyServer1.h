// ProxyServer1.h: interface for the CProxyServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYSERVER1_H__10E4037F_FB74_44A6_9CF7_C18DDCC1B062__INCLUDED_)
#define AFX_PROXYSERVER1_H__10E4037F_FB74_44A6_9CF7_C18DDCC1B062__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <io.h>
#include "afxinet.h"

#define AGM_OUTPUT				0x500
#define REQUEST_UNAUTHORIZED	1
#define VIRTUAL_RESOURCE		2
#define PARTIAL_CONTENT			4
#define BUFFER_SIZE				2048
#define SITEBASE "ProxyServer"

struct WORKPARAM{
	SOCKET		sckClient;
	sockaddr_in	addr;
};

struct HTTPHEADER{
	DWORD dwHttpStatus;
	int nVerMajor;
	int nVerMinor;
	char pszStatus[256];
	char pszType[256];
	char pszLocation[256];
	DWORD dwContSize;
	char * pContBegin;
};

struct REQUESTHEADER{
	DWORD dwFlag;
	char*		pszAuthBegin;
	char*		pszAuthEnd;
	char*		pszVirtualRes;
	char		pszUrl[512];
	char		pszUserPass[16];
	char*		pszContBegin;

	DWORD		dwRange;
	DWORD		dwContSize;
};

struct ProxyInfo{
	HWND	hWnd;
	
	int		nPort2nd;
	CString	strUser;
	CString strPasswd;
    
	CString strLocalAddr;
	CString strProxyAddr;
	int		nPort;	// local
};

typedef int(*ProcessCommand)(SOCKET, char*, char*);

class CProxyServer  
{
public:
	ProcessCommand  ProCommand;
	CProxyServer();
	virtual ~CProxyServer();
public:
	int		m_nConnectionCount;
	HANDLE	m_hConnectionCount;
	unsigned long	m_ulIpRefused[100];
	int		m_nRefused;
	ProxyInfo m_Info;
	WORKPARAM * pWorkParam;
	
	
public:
	BYTE GetSixBits(char* pszStr, int& i);
	BYTE Table(BYTE ot);
	void ContentType(char* pszFile, char* pszType);
	void UnCh(CProxyServer* pServer,char* pszStr);
	void Unbase64(CProxyServer* pServer,char* pszText, char* pszCode);
	DWORD RecvHeader(CProxyServer* pServer,SOCKET socket, char *buf, DWORD dwSize, HTTPHEADER* pHeader);
	void Base64(char* pszSource, char* pszCode);
	void HttpService(CProxyServer* pServer,char * pszUrl, SOCKET sck, char* buf, REQUESTHEADER* pRequestHeader);
	int RecvRequest(CProxyServer* pServer,SOCKET sck, char * buf, DWORD dwSize, REQUESTHEADER * pRequestHeader);
	void OUTPUT(CProxyServer* pServer,LPCTSTR strMsg);
	static UINT ListenThread(LPVOID lpVoid);
	static UINT WorkingThread(LPVOID lpVoid);
};

#endif // !defined(AFX_PROXYSERVER1_H__10E4037F_FB74_44A6_9CF7_C18DDCC1B062__INCLUDED_)
