// Server.h: interface for the CServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVER_H__BF20FB89_D02B_4628_884C_35576F24D6BC__INCLUDED_)
#define AFX_SERVER_H__BF20FB89_D02B_4628_884C_35576F24D6BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "stdlib.h"
#include "winsock2.h"

#define WSA_RECV         0
#define WSA_SEND         1

#define DATA_BUFSIZE    8192
#define MAX_NAME_LEN    128
#define MAX_PWD_LEN     128
#define MAX_RESP_LEN    1024
#define MAX_REQ_LEN     256
#define MAX_ADDR_LEN    80

#define FTP_PORT        21     // FTP 控制端口
#define DATA_FTP_PORT   20     // FTP 数据端口

#define USER_OK         331
#define LOGGED_IN       230
#define LOGIN_FAILED    530
#define CMD_OK          200
#define OPENING_AMODE   150
#define TRANS_COMPLETE  226
#define CANNOT_FIND     550
#define FTP_QUIT        221
#define CURR_DIR        257
#define DIR_CHANGED     250
#define OS_TYPE         215
#define REPLY_MARKER    504
#define PASSIVE_MODE    227

//#define FTP_USER		"toldo"
//#define FTP_PASS		"toldo"
//#define DEFAULT_HOME_DIR    "F:\\"

#define MAX_FILE_NUM        1024

#define MODE_PORT       0
#define MODE_PASV       1

#define PORT_BIND   1821

typedef struct {
   CHAR   buffRecv[DATA_BUFSIZE];
   CHAR   buffSend[DATA_BUFSIZE];
   WSABUF wsaBuf;
   SOCKET s;
   WSAOVERLAPPED o;
   DWORD dwBytesSend;
   DWORD dwBytesRecv;
   int   nStatus;
} SOCKET_INF, *LPSOCKET_INF;

typedef struct {
	TCHAR    szFileName[MAX_PATH];
	DWORD    dwFileAttributes; 
	FILETIME ftCreationTime; 
	FILETIME ftLastAccessTime; 
	FILETIME ftLastWriteTime; 
	DWORD    nFileSizeHigh; 
	DWORD    nFileSizeLow; 
} FILE_INF, *LPFILE_INF;

extern UINT ServerThread(LPVOID lpParameter);

class CServer  
{
public:
	CServer();
	virtual ~CServer();
public:

	char m_User[128];
	char m_Password[128];
	char m_Directory[128];
	char m_WelcomeInfor[1024];
	UINT m_Port;
	BOOL m_bStop;

	void SetSerConfigInfor(const char *user,const char *password,const char *directory,const char * welcomeinfor,UINT port);
	
	BOOL WelcomeInfo( SOCKET s );
	int LoginIn( LPSOCKET_INF pSocketInfo  );
	int SendRes( LPSOCKET_INF pSI );
	int RecvReq( LPSOCKET_INF pSI );
	int DealCommand( LPSOCKET_INF pSI );
	int DataRecv( SOCKET s, const char* szFileName );
	DWORD WriteToFile( SOCKET s , const char* szFile );
	SOCKET DataAccept( SOCKET& s );
	int DataSend( SOCKET s, char* buff,int nBufSize );
	UINT FileListToString( char* buff, UINT nBuffSize,BOOL bDetails );

	char* ConvertCommaAddress( char* szAddress, WORD wPort );
	int ConvertDotAddress( char* szAddress, LPDWORD pdwIpAddr, LPWORD pwPort);
	DWORD ReadFileToBuffer( const char* szFile, char *buff, DWORD nFileSize );
	int CombindFileNameSize( const char* szFileName,char* szFileNS );
	int	DataConn( SOCKET& s, DWORD dwIp, WORD wPort, int nMode);



	int GetFileList( LPFILE_INF pFI, UINT nArraySize, const char* szPath  );
	char* GetLocalAddress();
	char* HostToNet( char* szPath ) ;
	char* NetToHost( char* szPath ) ;
	char* RelativeDirectory( char* szDir );
	char* AbsoluteDirectory( char* szDir );

};

#endif // !defined(AFX_SERVER_H__BF20FB89_D02B_4628_884C_35576F24D6BC__INCLUDED_)
