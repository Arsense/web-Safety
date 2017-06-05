// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__AFACDE45_FA46_4C98_BA35_A71F1A3FAEC9__INCLUDED_)
#define AFX_LOGFILE_H__AFACDE45_FA46_4C98_BA35_A71F1A3FAEC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _FSTREAM_

#include <fstream>
using namespace std;

#endif

class CLogFile  
{
public:
	void Write(TCHAR* formatString, ...);
	char m_szLogPath[MAX_PATH];
	void Open(LPCTSTR lpszFileName);
	void Open();
	char m_FileName[256];
	void SetLogLevel(int nLevel);
	void Clear();
	void Date();
	void Time();
	void Close();
	int m_nCurrentLevel;
	int m_nLevel;
	CLogFile();
	virtual ~CLogFile();

	CLogFile& operator <<(long				lVal);
	CLogFile& operator <<(const		TCHAR*	str);
	CLogFile& operator <<(TCHAR				tch);
	CLogFile& operator <<(int				nVal);
	CLogFile& operator <<(unsigned	long	ulVal);
	CLogFile& operator <<(double			dVal);
	CLogFile& operator <<(unsigned	int		unVal);

	CLogFile& operator <<(CLogFile& (*_f)(CLogFile&));

	friend CLogFile& error_lvl(CLogFile& os) 
	{
		os.m_nCurrentLevel = 1;
		return os;
	}

	friend CLogFile& warning_lvl(CLogFile& os) 
	{
		os.m_nCurrentLevel = 2;
		return os;
	}
	
	friend CLogFile& trace_lvl(CLogFile& os) 
	{
		os.m_nCurrentLevel = 3;
		return os;
	}
private:
	WORD m_OldDate;
	void Output(const TCHAR* data);
	ofstream* m_File;
};

// manipulator for writing time to logfile
CLogFile& time(CLogFile& os);

// manipulator for writing date to logfile
CLogFile& date(CLogFile& os);

// manipulator for writing end of line to logfile
CLogFile& endl(CLogFile& os);

// manipulator for settings log level to error
CLogFile& error_lvl(CLogFile& os);
// manipulator for settings log level to trace
CLogFile& trace_lvl(CLogFile& os);
// manipulator for settings log level to warning
CLogFile& warning_lvl(CLogFile& os);

#endif // !defined(AFX_LOGFILE_H__AFACDE45_FA46_4C98_BA35_A71F1A3FAEC9__INCLUDED_)
