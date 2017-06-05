// Request.cpp: implementation of the CRequest class.
// Download by http://www.codefans.net
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HTTPClient.h"
#include "Request.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRequest::CRequest()
{
	m_buf.SetSize( 1024 );
	m_cbOut = 0;
}

CRequest::~CRequest()
{

}

void CRequest::StuffHead(CString method, CString lpszURL)
{
	if(!method.CompareNoCase("GET")){
		StuffString("GET /");
		StuffString(lpszURL);
		StuffString(" HTTP/1.1\n");
		StuffString("Accept: */*\n");
		StuffString("Accept-Language: zh-cn\n");
		StuffString("Accept-Encoding: gzip, deflate\n");
		StuffString("User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows 98)\n");
		StuffString("host: ");
		StuffString("127.0.0.1");
		StuffString("\n\x0d\x0a");
	}
	if(!method.CompareNoCase("HEAD")){
		StuffString("HEAD /");
		StuffString(lpszURL);
		StuffString(" HTTP/1.1\n");
		StuffString("Accept: */*\n");
		StuffString("Accept-Language: zh-cn\n");
		StuffString("Accept-Encoding: gzip, deflate\n");
		StuffString("User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows 98)\n");
		StuffString("host: ");
		StuffString("127.0.0.1");
		StuffString("\n\x0d\x0a");
	}
	if(!method.CompareNoCase("POST")){
		HANDLE hFile = CreateFile(lpszURL, GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,	NULL );
		DWORD length;
		if(hFile != INVALID_HANDLE_VALUE)
			length = GetFileSize(hFile, NULL);
		StuffString("POST /");
		StuffString("post.bat");
		StuffString(" HTTP/1.1\n");
		StuffString("Accept: */*\n");
		StuffString("Accept-Language: zh-cn\n");
		StuffString("Accept-Encoding: gzip, deflate\n");
		StuffString("User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows 98)\n");
		StuffString("host: 127.0.0.1\n");
		CString s;
		s.Format("Content-Length: %d\n", length);
		StuffString(s);
		StuffString("\n\x0d\x0a");
		DWORD dwRead;
		ReadFile(hFile, m_buf.GetData() + m_cbOut, 1024 - m_cbOut, &dwRead, NULL);
		CloseHandle(hFile);
	}
}

int CRequest::StuffString(const CString &strData)
{
	int nLen = strData.GetLength()*sizeof(TCHAR);
	// make sure there's enough room....
	if ( m_cbOut + nLen > m_buf.GetSize() )
	{
		int nChunks = nLen/1024 + 1;
		m_buf.SetSize( m_cbOut + nChunks*1024 );
	}
	// copy the data....
	MoveMemory( m_buf.GetData() + m_cbOut, (LPCSTR)strData, nLen );
	m_cbOut += nLen;
	// return amount of space left....
	return (m_buf.GetSize() - m_cbOut);
}
