#include "stdafx.h"
#include "MyCom.h"



CComStatus::CComStatus()
{
	m_hCom = NULL;
	m_bComId = (char)1;//COM1

	m_bByteSize=8;
	m_bStopBits=ONESTOPBIT;
	m_bParity=NOPARITY;
	m_dwBaudRate=9600;

	m_bEvtChar=EVENTCHAR;
	m_fBinary=1;

	m_bConnected = FALSE;
	m_bFlowCtrl = FC_XONXOFF ;
	m_fXonXoff = FALSE;
}

void CComStatus::Set(BYTE bComId,BYTE bByteSize,BYTE bStopBits,BYTE bParity,
		DWORD dwBaudRate,char bEvtChar,DWORD fBinary)
{
	m_hCom = NULL;
	m_bComId = bComId;

	m_bByteSize=bByteSize;
	m_bStopBits=bStopBits;
	m_bParity=bParity;
	m_dwBaudRate=dwBaudRate;

	m_bEvtChar=bEvtChar;
	m_fBinary=fBinary;

	m_bConnected = FALSE;
	m_bFlowCtrl = FC_XONXOFF ;
	m_fXonXoff = FALSE;

}
BOOL CComStatus::OpenConnection()
{
	char csCom[10];
    COMMTIMEOUTS  CommTimeOuts ;

    if((m_bComId < 0) || (m_bComId > 4))
		return FALSE;//从COM1到COM4
	if(m_hCom)//if already open
		return FALSE;
    //OVERLAPPED包含异步I/O信息
	m_rdos.Offset = 0;
	m_rdos.OffsetHigh = 0;
	m_rdos.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_rdos.hEvent == NULL)
		return FALSE;
	m_wtos.Offset = 0;
	m_wtos.OffsetHigh = 0;
	m_wtos.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_wtos.hEvent == NULL)
	{
		CloseHandle(m_rdos.hEvent);
		return FALSE;
	}
	

	wsprintf(csCom,"COM%d",m_bComId);
	m_hCom = CreateFile(csCom,GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
    NULL
	);

	if(m_hCom == INVALID_HANDLE_VALUE) {
		//dwError = GetLastError();
	    // handle error 
		return FALSE;
	}
	else
	{
      // get any early notifications

      SetCommMask( m_hCom, EV_RXCHAR ) ;

      // setup device buffers

      SetupComm( m_hCom, 4096, 4096 ) ;

      // purge any information in the buffer

      PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT |
                                      PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

      // set up for overlapped I/O
	  DWORD dwTemp = 1000 / (this->m_dwBaudRate / 8);
      CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
      CommTimeOuts.ReadTotalTimeoutMultiplier = 0;//((dwTemp > 0) ? dwTemp : 1);
      CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
      // CBR_9600 is approximately 1byte/ms. For our purposes, allow
      // double the expected time per character for a fudge factor.
      
      CommTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/this->m_dwBaudRate;//( npTTYInfo ) ;
      CommTimeOuts.WriteTotalTimeoutConstant = 0;//1000 ;
      SetCommTimeouts( m_hCom, &CommTimeOuts ) ;
	}
	if(!SetupConnection())
	{
		CloseConnection();
		return FALSE;
	}
	EscapeCommFunction( m_hCom, SETDTR );
	m_bConnected = TRUE;
	return TRUE;
}
BOOL CComStatus::CloseConnection()
{
   if (NULL == m_hCom)
      return ( TRUE ) ;

   // set connected flag to FALSE

	m_bConnected = FALSE;
   // disable event notification and wait for thread
   // to halt

   SetCommMask( m_hCom, 0 ) ;

   // block until thread has been halted

   //while(THREADID(npTTYInfo) != 0);

   // kill the focus

   //KillTTYFocus( hWnd ) ;

   // drop DTR

   EscapeCommFunction( m_hCom, CLRDTR ) ;

   // purge any outstanding reads/writes and close device handle

   PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT |
                                   PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
   CloseHandle( m_hCom ) ;
	m_hCom = NULL;

	CloseHandle(m_rdos.hEvent);
	CloseHandle(m_wtos.hEvent);

   return ( TRUE ) ;
}

BOOL CComStatus::SetupConnection()
{
   BOOL       fRetVal ;
   BYTE       bSet ;
   DCB        dcb ;
	if(m_hCom == NULL)
		return FALSE; 
   dcb.DCBlength = sizeof( DCB ) ;

   GetCommState( m_hCom, &dcb ) ;

   dcb.BaudRate = this->m_dwBaudRate;
   dcb.ByteSize = this->m_bByteSize;
   dcb.Parity =  this->m_bParity;
   dcb.StopBits = this->m_bStopBits ;
	dcb.EvtChar = this->m_bEvtChar ;
   // setup hardware flow control

   bSet = (BYTE) ((m_bFlowCtrl & FC_DTRDSR) != 0) ;
   dcb.fOutxDsrFlow = bSet ;
   if (bSet)
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE ;
   else
      dcb.fDtrControl = DTR_CONTROL_ENABLE ;

   bSet = (BYTE) ((m_bFlowCtrl & FC_RTSCTS) != 0) ;
	dcb.fOutxCtsFlow = bSet ;
   if (bSet)
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;
   else
      dcb.fRtsControl = RTS_CONTROL_ENABLE ;

   // setup software flow control

   bSet = (BYTE) ((m_bFlowCtrl & FC_XONXOFF) != 0) ;

   dcb.fInX = dcb.fOutX = bSet ;
   dcb.XonChar = ASCII_XON ;
   char xon = ASCII_XON ;
   dcb.XoffChar = ASCII_XOFF ;
    char xoff = ASCII_XOFF ;
    dcb.XonLim = 100 ;
   dcb.XoffLim = 100 ;


   dcb.fBinary = TRUE ;
   dcb.fParity = TRUE ;

   fRetVal = SetCommState( m_hCom, &dcb ) ;

   return ( fRetVal ) ;

} 

BOOL CComStatus::IsConnected()
{
	return m_bConnected;
}


//  Description:
//     Reads a block from the COM port and stuffs it into
//     the provided buffer.
//
//  Parameters:
//     HWND hWnd
//        handle to TTY window
//
//     LPSTR lpszBlock
//        block used for storage
//
//     int nMaxLength
//        max length of block to read

int ReadCommBlock(CComStatus& comDev,LPSTR lpszBlock, int nMaxLength )
{
   BOOL       fReadStat ;

   COMSTAT    ComStat ;
   DWORD      dwErrorFlags;
   DWORD      dwLength;
   DWORD      dwError;
   char       szError[ 10 ] ;

   // only try to read number of bytes in queue
   ClearCommError( comDev.m_hCom, &dwErrorFlags, &ComStat ) ;
   dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

   if (dwLength > 0)
   {
      fReadStat = ReadFile( comDev.m_hCom, lpszBlock,
		                    dwLength, &dwLength, &(comDev.m_rdos) ) ;
      if (!fReadStat)
      {
         if (GetLastError() == ERROR_IO_PENDING)
         {
            OutputDebugString("\n\rIO Pending");
            // We have to wait for read to complete.
            // This function will timeout according to the
            // CommTimeOuts.ReadTotalTimeoutConstant variable
            // Every time it times out, check for port errors
            while(!GetOverlappedResult( comDev.m_hCom ,
               &(comDev.m_rdos), &dwLength, TRUE ))
            {
               dwError = GetLastError();
               if(dwError == ERROR_IO_INCOMPLETE)
                  // normal result if not finished
                  continue;
               else
               {
                  // an error occurred, try to recover
                  wsprintf( szError, "<CE-%u>", dwError ) ;
                  ClearCommError( comDev.m_hCom , &dwErrorFlags, &ComStat ) ;
                  break;
               }

            }

	      }
         else
         {
            // some other error occurred
            dwLength = 0 ;
            ClearCommError( comDev.m_hCom , &dwErrorFlags, &ComStat ) ;
         }
      }
   }

   return ( dwLength ) ;

}


//     Writes a block of data to the COM port specified in the associated
//     TTY info structure.
//
//  Parameters:
//     HWND hWnd
//        handle to TTY window
//
//     BYTE *pByte
//        pointer to data to write to port

BOOL WriteCommBlock( CComStatus& comDev, LPSTR lpByte , DWORD dwBytesToWrite)
{

   BOOL        fWriteStat ;
   DWORD       dwBytesWritten ;
   DWORD       dwErrorFlags;
   DWORD   		dwError;
   DWORD       dwBytesSent=0;
   COMSTAT     ComStat;
   char        szError[ 128 ] ;

   fWriteStat = WriteFile( comDev.m_hCom , lpByte, dwBytesToWrite,
                           &dwBytesWritten, &( comDev.m_wtos) ) ;

   // Note that normally the code will not execute the following
   // because the driver caches write operations. Small I/O requests
   // (up to several thousand bytes) will normally be accepted
   // immediately and WriteFile will return true even though an
   // overlapped operation was specified

   if (!fWriteStat)
   {
      if(GetLastError() == ERROR_IO_PENDING)
      {
         // We should wait for the completion of the write operation
         // so we know if it worked or not

         // This is only one way to do this. It might be beneficial to
         // place the write operation in a separate thread
         // so that blocking on completion will not negatively
         // affect the responsiveness of the UI

         // If the write takes too long to complete, this
         // function will timeout according to the
         // CommTimeOuts.WriteTotalTimeoutMultiplier variable.
         // This code logs the timeout but does not retry
         // the write.

         while(!GetOverlappedResult( comDev.m_hCom,
            &(comDev.m_wtos), &dwBytesWritten, TRUE ))
         {
            dwError = GetLastError();
            if(dwError == ERROR_IO_INCOMPLETE)
            {
               // normal result if not finished
               dwBytesSent += dwBytesWritten;
               continue;
            }
            else
            {
               // an error occurred, try to recover
               wsprintf( szError, "<CE-%u>", dwError ) ;
               ClearCommError( comDev.m_hCom, &dwErrorFlags, &ComStat ) ;
               break;
            }
         }

         dwBytesSent += dwBytesWritten;

         if( dwBytesSent != dwBytesToWrite )
             wsprintf(szError,"\nProbable Write Timeout: Total of %ld bytes sent", dwBytesSent);
         else
             wsprintf(szError,"\n%ld bytes written", dwBytesSent);

         OutputDebugString(szError);

      }
      else
      {
         // some other error occurred
         ClearCommError( comDev.m_hCom, &dwErrorFlags, &ComStat ) ;
         return ( FALSE );
      }
   }
   return ( TRUE ) ;

}
