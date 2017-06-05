// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ftpserver.h"
#include "LogFile.h"
#include <strstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{
	m_File = NULL;
	m_nLevel = 0;
	m_nCurrentLevel = 0;
}

CLogFile::~CLogFile()
{
	Write("*** END OF APPLICATION ***");
	// close file, if still opened
	Close();
}

//Private Member function: Output data to file.				
//This is the only place where is actually written to the file.
void CLogFile::Output(const TCHAR *data)
{
	//file not opened
	if(m_File == NULL)
		return;

	//do we need to log this data?
	if(m_nCurrentLevel <= m_nLevel)
	{
		//write data to file
		m_File->write(data,_tcslen(data));
		m_File->flush();
	}
}

//Close current logfile.	
void CLogFile::Close()
{
	//file not open...
	if(m_File == NULL)
		return;

	//close file
	m_File->close();
	//delete class
	delete m_File;
	m_File = NULL;
}

//Used for logging the time.
void CLogFile::Time()
{
	TCHAR szTime[15];
    SYSTEMTIME sysTime;

    GetLocalTime(&sysTime);

    wsprintf(szTime, "%02d:%02d:%02d.%03d ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
    Output(szTime);
}

void CLogFile::Date()
{
	TCHAR szDate[12];
    SYSTEMTIME sysTime;

    GetLocalTime(&sysTime);

    wsprintf(szDate, "%02d/%02d/%02d ", sysTime.wDay, sysTime.wMonth, sysTime.wYear);
    Output(szDate);
}

//Clear current logfile
void CLogFile::Clear()
{
	//close file before deleting
    Close();
    //remove log file
    DeleteFile((LPCTSTR)m_FileName);
    //create new file
	Open(m_FileName);
    Write("*** CLEARED LOGFILE ***");   
}

void CLogFile::SetLogLevel(int nLevel)
{
	m_nLevel = nLevel;
}

//Open File using date as part of the filename.	
void CLogFile::Open()
{
	SYSTEMTIME sysTime;

    GetLocalTime(&sysTime);

    // save current date
    m_OldDate = sysTime.wDay;
    
	// create the log directory if it does not already exists
	CreateDirectory(m_szLogPath, NULL);	
 
	// create file name
    wsprintf(m_FileName,"%s%04d%02d%02d.txt", m_szLogPath, sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	Open(m_FileName);
}

//Open File with name from parameter. 	
void CLogFile::Open(LPCTSTR lpszFileName)
{
    // if already open close !
	if (m_File != NULL)
		Close();

	lstrcpy(m_FileName, lpszFileName);

	// create ofstream class for buffered disk file I/O
	m_File = new ofstream(lpszFileName, ios_base::out | ios::app);	
    // successfull opened ?
	if (!m_File->is_open())
    {
		// failed to open file
		char strValue[256]; 

        wsprintf(strValue, "Cannot create or open file: '%s'", lpszFileName);
        MessageBox(NULL,(LPCTSTR)strValue,"Error in CLogFile::Open()",MB_OK | MB_ICONSTOP);
        delete m_File;
        m_File = NULL;
    }
	Write("*** START OF APPLICATION ***");
}

/****************************************************************/
/*																*/
/* Operator: Used for logging 'unsigned int' types.				*/
/*																*/
/****************************************************************/
CLogFile& CLogFile::operator <<(unsigned int unVal)
{
    strstream tmp;
    tmp << unVal;
    tmp << '\0';

    TCHAR* output = tmp.str();

    Output(output);

    tmp.freeze(false);
	return *this;
}


/****************************************************************/
/*																*/
/* Operator: Used for logging 'long' types.						*/
/*																*/
/****************************************************************/
CLogFile& CLogFile::operator <<(long lVal)
{
    strstream tmp;
    tmp << lVal;
    tmp << '\0';

    TCHAR* output = tmp.str();

    Output(output);

    tmp.freeze(false);

	return *this;
}


/****************************************************************/
/*																*/
/* Operator: Used for logging 'char[]' types.					*/
/*																*/
/****************************************************************/
CLogFile& CLogFile::operator <<(const TCHAR* str)
{
    Output(str);
	return *this;
}


/****************************************************************/
/*																*/
/* Operator: Used for logging 'char' types.						*/
/*																*/
/****************************************************************/
CLogFile& CLogFile::operator <<(TCHAR	tch)
{
    TCHAR szCh[2];
    szCh[0] = tch;
    szCh[1] = '\0';
    Output(szCh);
    return *this;
}


/****************************************************************/
/*																*/
/* Operator: Used for logging 'int' types.						*/
/*																*/
/****************************************************************/
CLogFile& CLogFile::operator <<(int nVal)
{
    strstream tmp;
    tmp << nVal;
    tmp << '\0';

    TCHAR* output = tmp.str();

    Output(output);

    tmp.freeze(false);
    return *this;
}


/****************************************************************/
/*																*/
/* Operator: Used for logging 'unsigned long' types.			*/
/*																*/
/****************************************************************/
CLogFile& CLogFile::operator <<(unsigned long ulVal)
{
    strstream tmp;
    tmp << ulVal;
    tmp << '\0';

    TCHAR* output = tmp.str();
    Output(output);
    tmp.freeze(false);
	return *this;
}


/****************************************************************/
/*																*/
/* Operator: Used for logging 'double' types.					*/
/*																*/
/****************************************************************/
CLogFile& CLogFile::operator <<(double dVal)
{
    strstream tmp;
    tmp << dVal;
    tmp << '\0';

    TCHAR* output = tmp.str();
    Output(output);
    tmp.freeze(false);
	return *this;
}



//Operator: Necessary for manipulators 'time' and 'endl'		
CLogFile& CLogFile::operator <<(CLogFile& (*_f)( CLogFile& ) ) 
{
    (*_f)(*this);
    return *this;
}

//Write line to log file, supports printf formatting.
void CLogFile::Write(TCHAR* formatString, ...)
{
	m_nCurrentLevel = 1;

	// Insert the current date
	Date();
	// Insert the current time
	Time();
	// Parse the format string and write to the file
	if (formatString == NULL)
	{
        // No point in continuiing
		return;
	}
		
	va_list argList;
	// Set va_list to the beginning of optional arguments
	va_start(argList, formatString);
	TCHAR* ptr = formatString;

	while(*ptr != '\0')
	{
		TCHAR* str			= NULL;
		int nInteger		= 0;
		unsigned int unInt	= 0;
		long lLong			= 0;
		unsigned long ulLong= 0;
		double	dDoub		= 0;

		if(*ptr == '%')
		{
			switch(*(ptr+1))
			{
			case 's':
				str = va_arg(argList, TCHAR*);

				if( NULL == str)
					break;
				*this << str;
				ptr++;
				break;

			case 'd':
				nInteger = va_arg(argList, int);
				*this << nInteger;
				ptr++;
				break;

			case 'u':
				unInt = va_arg(argList, unsigned int);
				*this << unInt;
				ptr++;
				break;

			case 'l':
				ptr++;
				if(*(ptr+1) == 'd')
				{
					lLong = va_arg(argList, long);
					*this << lLong;
					ptr++;
				}
				else if(*(ptr+1) == 'u')
				{
					ulLong = va_arg(argList, unsigned long);
					*this << ulLong;
					ptr++;
				}
				break;
			case 'f':
				dDoub = va_arg(argList, double);
				*this << dDoub;
				ptr++;
				break;
			default:
				*this << *ptr;
			}
		} //  if(*ptr == '%')
		else
		{
			*this << *ptr;
		}
		// Increment pointer..
		ptr++;
	}
	*this << '\n';
}

/****************************************************************/
/*																*/
/* Manipulator to insert the time into log file.				*/
/*																*/
/****************************************************************/
CLogFile& time(CLogFile& os) 
{
    TCHAR szTime[15];
    SYSTEMTIME sysTime;
    
	GetLocalTime(&sysTime);

    wsprintf(szTime, "%02d:%02d:%02d.%03d ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	return os << szTime;
}


/****************************************************************/
/*																*/
/* Manipulator to insert the date into log file.				*/
/*																*/
/****************************************************************/
CLogFile& date(CLogFile& os) 
{
    TCHAR szDate[12];
    SYSTEMTIME sysTime;

    GetLocalTime(&sysTime);

    wsprintf(szDate, "%02d/%02d/%02d ", sysTime.wDay, sysTime.wMonth, sysTime.wYear);
	return os << szDate;
}


/****************************************************************/
/*																*/
/* Manipulator to insert end of line into log file.				*/
/*																*/
/****************************************************************/
CLogFile& endl(CLogFile& os) 
{
	return os << '\n';
}


