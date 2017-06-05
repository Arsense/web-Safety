#if !defined(_COMM_ACCESS_FUNCTIONS_AND_DATA)
#define _COMM_ACCESS_FUNCTIONS_AND_DATA

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define EVENTCHAR               0x0d
#define MAXBLOCKLENGTH          59//130


// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04


// ascii definitions

#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

class CComStatus
{
public:
	HANDLE m_hCom;
	BYTE m_bComId;

	BYTE m_bByteSize;
	BYTE m_bStopBits;
	BYTE m_bParity;
	DWORD m_dwBaudRate;
	char m_bEvtChar;
	DWORD m_fBinary;

	BOOL m_bConnected;
	BOOL m_fXonXoff;
	BOOL m_bFlowCtrl;

	OVERLAPPED	m_rdos;
	OVERLAPPED  m_wtos;

	CComStatus();
	void Set(BYTE bComId=1,BYTE bByteSize=8,BYTE bStopBits=ONESTOPBIT,BYTE bParity=NOPARITY,DWORD dwBaudRate=9600,char bEvtChar=EVENTCHAR,DWORD fBinary=1);
	BOOL OpenConnection();
	BOOL CloseConnection();
	BOOL SetupConnection();
	BOOL IsConnected();

  }; 
/******************************************************
GLOBAL FUNCTIONS
******************************************************/

BOOL WriteCommBlock( CComStatus& comDev, LPSTR lpByte , DWORD dwBytesToWrite);
int ReadCommBlock(CComStatus& comDev,LPSTR lpszBlock, int nMaxLength );

#endif