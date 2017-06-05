//=============================================================================================
/*
	XLogFile.h
	File operate function of log file

	Project	: XFILTER 1.0 Personal Firewall
	Author	: Tony Zhu
	Create Date	: 2001/08/23
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.
*/
//=============================================================================================
// Log file

#ifndef XLOGFILE_H
#define XLOGFILE_H

typedef struct _LOG_HEADER
{
	TCHAR	Singnature[16];
	DWORD	RecordCount;
	DWORD	CurrentPosition;
} LOG_HEADER, *PLOG_HEADER;

typedef struct _LOG_FIND
{
	IN	CTime	tStartTime;
	IN	CTime	tEndTime;
	OUT DWORD	ulStartPosition;
	OUT DWORD	ulRecordCount;
	OUT DWORD	ulChangeDirectionIndex;
} LOG_FIND, *PLOG_FIND;

#define LOG_HEADER_LENTH			sizeof(LOG_HEADER)
#define FILE_SIZE_1M_BYTES			1048576
#define RESULT_ONE_RECORD_LENTH		LOG_HEADER_LENTH + SESSION_LENTH

class CXLogFile
{
private:
	CRITICAL_SECTION	gCriticalSectionLog;

private:
	int		CreateLog(const TCHAR *sPathName);
	int		CloseLog();
	int		OpenLog();
	int		WriteHeader();

public:
	CXLogFile();
	virtual ~CXLogFile();

	int		AddLog(SESSION *session, long MaxLogSize = 5);
	int		FindLog(LOG_FIND *logfind,  long MaxLogSize = 5);

public:
	LOG_HEADER		m_LogHeader;
	CFileStatus		m_FileStatus;
	CFile			m_LogFile;
	CString			m_sPathName;
};

#endif