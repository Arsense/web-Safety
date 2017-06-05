// Request.h: interface for the CRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REQUEST_H__8F736945_170C_4079_915A_FD55E3D42048__INCLUDED_)
#define AFX_REQUEST_H__8F736945_170C_4079_915A_FD55E3D42048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRequest : public CObject  
{
public:
	int  StuffString( const CString& strData );
	void StuffHead(CString method, CString lpszURL);
	CRequest();
	virtual ~CRequest();
	CByteArray  m_buf;
	CString     m_strLine;
	LONG        m_cbOut;
};

#endif // !defined(AFX_REQUEST_H__8F736945_170C_4079_915A_FD55E3D42048__INCLUDED_)
