/****************************************************************************************
* ///////////////////////////////////////////////////////////////////////////////////////
*	Original Filename: 	Log.cpp
*
*	History:
*	Created/Modified by				Date			Main Purpose/Changes
*	Souren M. Abeghyan				2001/05/25		Implementation of the CLog class.
*	
*	Comments:	
* \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
****************************************************************************************/
#include "stdafx.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
	InitializeCriticalSection(&cs);
}

CLog::~CLog()
{
	DeleteCriticalSection(&cs);
}



BOOL CLog::LogMessage(const char *szFolder, const char *szMsg, const char *szMsg1, long nNumber)
{
	EnterCriticalSection(&cs);
	time(&ltime);

	if((!strlen(szFolder)) || (!strlen(szMsg)))
		return FALSE;
	
	if(!GetWindowsDirectory(szLogFilePath, MAX_PATH))
	{
		LeaveCriticalSection(&cs);
		return FALSE;
	}
	
	if(szLogFilePath[0] != '\\')
		strcat(szLogFilePath, "\\");
	strcat(szLogFilePath, szFolder);

	m_f = fopen(szLogFilePath, "a");
	if(m_f != NULL)				
	{
		newtime = localtime(&ltime);
		strftime(szDT, 128,
					"%a, %d %b %Y %H:%M:%S", newtime);
		
		if(szMsg1 != NULL)
			sprintf(szMessage, "%s - %s.\t[%s]\t[%d]\n", szDT, szMsg, szMsg1, nNumber);
		else
			sprintf(szMessage, "%s - %s.\t[%d]\n", szDT, szMsg, nNumber);

		int n = fwrite(szMessage, sizeof(char), strlen(szMessage), m_f);
		if(n != strlen(szMessage))
		{
			LeaveCriticalSection(&cs);
			fclose(m_f);
			return FALSE;
		}

		fclose(m_f);
		LeaveCriticalSection(&cs);
		return TRUE;
	}

	LeaveCriticalSection(&cs);
	return FALSE;
}



BOOL CLog::ClearLog(const char *szFolder)
{
	if(!strlen(szFolder))
		return FALSE;
	
	if(!GetWindowsDirectory(szLogFilePath, MAX_PATH))
		return FALSE;

	if(szLogFilePath[0] != '\\')
		strcat(szLogFilePath, "\\");
	strcat(szLogFilePath, szFolder);

	return 	DeleteFile(szLogFilePath);
}
