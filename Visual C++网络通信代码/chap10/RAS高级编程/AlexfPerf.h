///////////////////////////////////////////////////////////////////////
// If this program works, it was written by Alexander Fedorov.
// If not, I don't know who wrote it.
// mailto:lamer2000@hotmail.com
// Home Page: http://members.xoom.com/lamer2000/
// This file is part of Alexf Dialer.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
///////////////////////////////////////////////////////////////////////

// AlexfPerf.h: interface for the CAlexfPerf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALEXFPERF_H__D9B4586B_025F_11D3_9864_E21B940F3AD0__INCLUDED_)
#define AFX_ALEXFPERF_H__D9B4586B_025F_11D3_9864_E21B940F3AD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAlexfPerf95
{
protected:
	BOOL m_bSuccess;
	CString m_sCounterName;
	CString m_sComputerName;

public:
	CAlexfPerf95(LPCTSTR pszObjName, LPCTSTR pszCounterName, LPCTSTR pszComputerName = NULL);
	virtual ~CAlexfPerf95();
	BOOL IsOK() {return m_bSuccess;}
	BOOL Collect(DWORD& dwData);
	DWORD GetData();
};

class CAlexfPerfNT
{
protected:
	BOOL m_bSuccess;
	CString m_SObj, m_sCounter;
	CString m_sComputerName;

	BOOL GetIndex(const char * pszCounter, char * szIndex);

public:
	CAlexfPerfNT(LPCTSTR pszObjName, LPCTSTR pszCounterName, LPCTSTR pszComputerName = NULL);
	virtual ~CAlexfPerfNT();
	BOOL IsOK() {return m_bSuccess;}
	BOOL Collect(DWORD& dwData);
	DWORD GetData();
};

#define ALEXF_PERF_DIALUP_CONNECTSPEED	0
#define ALEXF_PERF_DIALUP_BYTESRECVD	1
#define ALEXF_PERF_DIALUP_BYTESXMIT		2

class CAlexfPerf
{
protected:
	CAlexfPerf95 * m_p95;
	CAlexfPerfNT * m_pNT;
	BOOL m_bSuccess;
	BOOL bInit95(int iPerfType);
	BOOL bInitNT(int iPerfType);
	int m_iPerfType;
public:
	CAlexfPerf(int iPerfType);
	virtual ~CAlexfPerf();
	BOOL IsOK() {return m_bSuccess;}
	BOOL Collect(DWORD& dwData);
	DWORD GetData();
	static CString GetRegKey(HKEY hOpen, CString key1,CString key2);
};

#endif // !defined(AFX_ALEXFPERF_H__D9B4586B_025F_11D3_9864_E21B940F3AD0__INCLUDED_)
