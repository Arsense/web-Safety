/*
Module : SMTP.CPP
Purpose: Implementation for a MFC class encapsulation of the SMTP protocol
Created: PJN / 22-05-1998
History: PJN / 15-06-1998 1) Fixed the case where a single dot occurs on its own
                             in the body of a message
				              	  2) Class now supports Reply-To Header Field
                          3) Class now supports file attachments
   		   PJN / 18-06-1998 1) Fixed a memory overwrite problem which was occurring 
	  		                 with the buffer used for encoding base64 attachments
         PJN / 27-06-1998 1) The case where a line begins with a "." but contains
                          other text is now also catered for. See RFC821, Section 4.5.2
                          for further details.
                          2) m_sBody in CSMTPMessage has now been made protected.
                          Client applications now should call AddBody instead. This
                          ensures that FixSingleDot is only called once even if the 
                          same message is sent a number of times.
                          3) Fixed a number of problems with how the MIME boundaries
                          were defined and sent.
                          4) Got rid of an unreferenced formal parameter 
                          compiler warning when doing a release build
         PJN / 11-09-1998 1) VC 5 project file is now provided
                          2) Attachment array which the message class contains now uses
                          references instead of pointers.
                          3) Now uses Sleep(0) to yield our time slice instead of Sleep(100),
                          this is the preferred way of writting polling style code in Win32
                          without serverly impacting performance.
                          4) All Trace statements now display the value as returned from
                          GetLastError
                          5) A number of extra asserts have been added
                          6) A AddMultipleRecipients function has been added which supports added a 
                          number of recipients at one time from a single string
                          7) Extra trace statements have been added to help in debugging
         PJN / 12-09-98   1) Removed a couple of unreferenced variable compiler warnings when code
                          was compiled with Visual C++ 6.0
                          2) Fixed a major bug which was causing an ASSERT when the CSMTPAttachment
                          destructor was being called in the InitInstance of the sample app. 
                          This was inadvertingly introduced for the 1.2 release. The fix is to revert 
                          fix 2) as done on 11-09-1998. This will also help to reduce the number of 
                          attachment images kept in memory at one time.
         PJN / 18-01-99   1) Full CC & BCC support has been added to the classes
         PJN / 22-02-99   1) Addition of a Get and SetTitle function which allows a files attachment 
                          title to be different that the original filename
                          2) AddMultipleRecipients now ignores addresses if they are empty.
                          3) Improved the reading of responses back from the server by implementing
                          a growable receive buffer
                          4) timeout is now 60 seconds when building for debug
         PJN / 25-03-99   1) Now sleeps for 250 ms instead of yielding the time slice. This helps 
                          reduce CPU usage when waiting for data to arrive in the socket
         PJN / 14-05-99   1) Fixed a bug with the way the code generates time zone fields in the Date headers.
         PJN / 10-09-99   1) Improved CSMTPMessage::GetHeader to include mime field even when no attachments
                          are included.
         PJN / 16-02-00   1) Fixed a problem which was occuring when code was compiled with VC++ 6.0.
         PJN / 19-03-00   1) Fixed a problem in GetHeader on Non-English Windows machines
                          2) Now ships with a VC 5 workspace. I accidentally shipped a VC 6 version in one of the previous versions of the code.
                          3) Fixed a number of UNICODE problems
                          4) Updated the sample app to deliberately assert before connecting to the author's SMTP server.
         PJN / 28-03-00   1) Set the release mode timeout to be 10 seconds. 2 seconds was causing problems for slow dial
                          up networking connections.
         PJN / 07-05-00   1) Addition of some ASSERT's in CSMTPSocket::Connect
		     PP  / 16-06-00   The following modifications were done by Puneet Pawaia
						              1) Removed the base64 encoder from this file
						              2) Added the base64 encoder/decoder implementation in a separate 
						              file. This was done because base64 decoding was not part of 
							            the previous implementation
						              3) Added support for ESMTP connection. The class now attempts to 
						              authenticate the user on the ESMTP server using the username and
							            passwords supplied. For this connect now takes the username and 
							            passwords as parameters. These can be null in which case ESMTP 
							            authentication is not attempted
						              4) This class can now handle AUTH LOGIN and AUTH LOGIN PLAIN authentication
						              schemes on 
		     PP  / 19-06-00   The following modifications were done by Puneet Pawaia
				         		      1) Added the files md5.* containing the MD5 digest generation code
						              after modifications so that it compiles with VC++ 6
						              2) Added the CRAM-MD5 login procedure.
         PJN / 10-07-00   1) Fixed a problem with sending attachments > 1K in size
                          2) Changed the parameters to CSMTPConnection::Connect
         PJN / 30-07-00   1) Fixed a bug in AuthLogin which was transmitting the username and password
                          with an extra "=" which was causing the login to failure. Thanks to Victor Vogelpoel for
                          finding this.
         PJN / 05-09-00   1) Added a CSMTP_NORSA preprocessor macro to allow the CSmtpConnection code to be compiled
                          without the dependence on the RSA code.
         PJN / 28-12-2000 1) Removed an unused variable from ConnectESMTP.
                          2) Allowed the hostname as sent in the HELO command to be specified at run time 
                          in addition to using the hostname of the client machine
                          3) Fixed a problem where high ascii characters were not being properly encoded in
                          the quoted-printable version of the body sent.
                          4) Added support for user definable charset's for the message body.
                          5) Mime boundaries are now always sent irrespective if whether attachments are included or
                          not. This is required as the body is using quoted-printable.
                          6) Fixed a bug in sendLines which was causing small message bodies to be sent incorrectly
                          7) Now fully supports custom headers in the SMTP message
                          8) Fixed a copy and paste bug where the default port for the SMTP socket class was 110.
                          9) You can now specify the address on which the socket is bound. This enables the programmer
                          to decide on which NIC data should be sent from. This is especially useful on a machine
                          with multiple IP addresses.
                          10) Addition of functions in the SMTP connection class to auto dial and auto disconnect to 
                          the Internet if you so desire.
                          11) Sample app has been improved to allow Auto Dial and binding to IP addresses to be configured.
         PJN / 26-02-2001 1)  Charset now defaults to ISO 8859-1 instead of us-ascii
                          2)  Removed a number of unreferrenced variables from the sample app.
                          3)  Headers are now encoded if they contain non ascii characters.
                          4)  Fixed a bug in getLine, Thanks to Lev Evert for spotting this one.
                          5)  Made the charset value a member of the message class instead of the connection class
                          6)  Sample app now fully supports specifying the charset of the message
                          7)  Added a AddMultipleAttachments method to CSMTPMessage
                          8)  Attachments can now be copied to each other via new methods in CSMTPAttachment
                          9)  Message class now contains copies of the attachments instead of pointers to them
                          10) Sample app now allows multiple attachments to be added
                          11) Removed an unnecessary assert in QuotedPrintableEncode
                          12) Added a Mime flag to the CSMTPMessage class which allows you to decide whether or not a message 
                          should be sent as MIME. Note that if you have attachments, then mime is assumed.
                          13) CSMTPAttachment class has now become CSMTPBodyPart in anticipation of full support for MIME and 
                          MHTML email support
                          14) Updated copright message in source code and documentation
                          15) Fixed a bug in GetHeader related to _tsetlocale usage. Thanks to Sean McKinnon for spotting this
                          problem.
                          16) Fixed a bug in SendLines when sending small attachments. Thanks to Deng Tao for spotting this
                          problem.
                          17) Removed the need for SendLines function entirely.
                          18) Now fully supports HTML email (aka MHTML)
                          19) Updated copyright messages in code and in documentation
         PJN / 17-06-2001 1) Fixed a bug in CSMTPMessage::HeaderEncode where spaces were not being interpreted correctly. Thanks
                          to Jim Alberico for spotting this.
                          2) Fixed 2 issues with ReadResponse both having to do with multi-line responses. Thanks to Chris Hanson 
                          for this update.
         PJN / 25-06-2001 1) Code now links in Winsock and RPCRT40 automatically. This avoids client code having to specify it in 
                          their linker settings. Thanks to Malte and Phillip for spotting this issue.
                          2) Updated sample code in documentation. Thanks to Phillip for spotting this.
                          3) Improved the code in CSMTPBodyPart::SetText to ensure lines are correctly wrapped. Thanks to 
                          Thomas Moser for this fix.
         PJN / 01-07-2001 1) Modified QuotedPrintableEncode to prevent the code to enter in an infinite loop due to a long word i.e. 
                          bigger than SMTP_MAXLINE, in this case, the word is breaked. Thanks to Manuel Gustavo Saraiva for this fix.
                          2) Provided a new protected variable in CSMTPBodyPart called m_bQuotedPrintable to bypass the 
                          QuotedPrintableEncode function in cases that we don't want that kind of correction. Again thanks to 
                          Manuel Gustavo Saraiva for this fix.
         PJN / 15-07-2001 1) Improved the error handling in the function CSMTPMessage::AddMultipleAttachments. In addition the 
                          return value has been changed from BOOL to int
         PJN / 11-08-2001 1) Fixed a bug in QuotedPrintableEncode which was wrapping encoding characters across multiple lines. 
                          Thanks to Roy He for spotting this.
                          2) Provided a "SendMessage" method which sends a email directly from disk. This allows you 
                          to construct your own emails and the use the class just to do the sending. This function also has the 
                          advantage that it efficiently uses memory and reports progress.
                          3) Provided support for progress notification and cancelling via the "OnSendProgress" virtual method.
         PJN / 29-09-2001 1) Fixed a bug in ReadResponse which occured when you disconnected via Dial-Up Networking
                          while a connection was active. This was originally spotted in my POP3 class.
                          2) Fixed a problem in CSMTPBodyPart::GetHeader which was always including the "quoted-printable" even when 
                          m_bQuotedPrintable for that body part was FALSE. Thanks to "jason" for spotting this.
         PJN / 12-10-2001 1) Fixed a problem where GetBody was reporting the size as 1 bigger than it should have been. Thanks
                          to "c f" for spotting this problem.
                          2) Fixed a bug in the TRACE statements when a socket connection cannot be made.
                          3) The sample app now displays a filter of "All Files" when selecting attachments to send
                          4) Fixed a problem sending 0 byte attachments. Thanks to Deng Tao for spotting this problem.
         PJN / 11-01-2002 1) Now includes a method to send a message directly from memory. Thanks to Tom Allebrandi for this
                          suggestion.
                          2) Change function name "IsReadible" to be "IsReadable". I was never very good at English!.
                          3) Fixed a bug in CSMTPBodyPart::QuotedPrintableEncode. If a line was exactly 76 characters long plus 
                          \r\n it produced an invalid soft linebreak of "\r=\r\n\n". Thanks to Gerald Egert for spotting this problem.
.



Copyright (c) 1998 - 2002 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

//////////////// Includes ////////////////////////////////////////////
#include "stdafx.h"
#include <locale.h>
#include "smtp.h"

#ifndef CSMTP_NORSA
#include "glob-md5.h"
#include "md5.h"
#endif

#include <wininet.h>

//////////////// Macros / Locals /////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SMTP_MAXLINE  76

//Link in Winsock dll automatically
#pragma comment(lib, "wsock32.lib")
//Link in RPC runtimes dll automatically
#pragma comment(lib, "rpcrt4.lib")



//////////////// Implementation //////////////////////////////////////

//Class which handles function which must be constructed at run time
//since we cannot absolutely gurantee wininet will be available. To avoid the loader
//bringing up a message such as "Failed to load due to missing export...", the
//functions are constructed using GetProcAddress. The SMTP functions then checks to 
//see if the function pointers are NULL and if it is it returns failure and sets the 
//error code ERROR_CALL_NOT_IMPLEMENTED which is what the OS would have done if it had 
//implemented a stub for it in the first place !!
class _WININET_DATA
{
public:
//Constructors /Destructors
  _WININET_DATA();
  ~_WININET_DATA();

//typedefs of the function pointers
  typedef BOOL (WINAPI INTERNETGETCONNECTEDSTATE)(LPDWORD, DWORD);
  typedef INTERNETGETCONNECTEDSTATE* LPINTERNETGETCONNECTEDSTATE;
  typedef BOOL (WINAPI INTERNETAUTODIALHANGUP)(DWORD);
  typedef INTERNETAUTODIALHANGUP* LPINTERNETAUTODIALHANGUP;
  typedef BOOL (WINAPI INTERNETATTEMPCONNECT)(DWORD);
  typedef INTERNETATTEMPCONNECT* LPINTERNETATTEMPCONNECT;


//Member variables
  HINSTANCE                   m_hWininet;  //Instance handle of the "Wininet.dll" which houses the 2 functions we want
  LPINTERNETGETCONNECTEDSTATE m_lpfnInternetGetConnectedState;
  LPINTERNETAUTODIALHANGUP    m_lpfnInternetAutoDialHangup;
  LPINTERNETATTEMPCONNECT     m_lpfnInternetAttemptConnect;
};

_WININET_DATA::_WININET_DATA()
{
  m_hWininet = LoadLibrary(_T("WININET.DLL"));
  if (m_hWininet)
  {
    m_lpfnInternetGetConnectedState = (LPINTERNETGETCONNECTEDSTATE) GetProcAddress(m_hWininet, "InternetGetConnectedState");
    m_lpfnInternetAutoDialHangup = (LPINTERNETAUTODIALHANGUP) GetProcAddress(m_hWininet, "InternetAutodialHangup");
    m_lpfnInternetAttemptConnect = (LPINTERNETATTEMPCONNECT) GetProcAddress(m_hWininet, "InternetAttemptConnect");
  }
}

_WININET_DATA::~_WININET_DATA()
{
  if (m_hWininet)
  {
    FreeLibrary(m_hWininet);
    m_hWininet = NULL;
  }
}



//The local variable which handle the function pointers

_WININET_DATA _WinInetData;



CSMTPSocket::CSMTPSocket()
{
  m_hSocket = INVALID_SOCKET; //default to an invalid scoket descriptor
}

CSMTPSocket::~CSMTPSocket()
{
  Close();
}

BOOL CSMTPSocket::Create()
{
  m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
  return (m_hSocket != INVALID_SOCKET);
}

BOOL CSMTPSocket::Connect(LPCTSTR pszHostAddress, int nPort, LPCTSTR pszLocalBoundAddress)
{
  ASSERT(pszHostAddress); //验证主机地址
  ASSERT(_tcslen(pszHostAddress)); 

  USES_CONVERSION;

  //验证是否socket被创建
  ASSERT(m_hSocket != INVALID_SOCKET);

  //绑定本地地址
  if (pszLocalBoundAddress && _tcslen(pszLocalBoundAddress))
  {
    LPSTR lpszAsciiLocalAddress = T2A((LPTSTR)pszLocalBoundAddress);

    SOCKADDR_IN sockLocalAddress;
    ZeroMemory(&sockLocalAddress, sizeof(sockLocalAddress));
    sockLocalAddress.sin_family = AF_INET;
    sockLocalAddress.sin_port = htons(0);
    sockLocalAddress.sin_addr.s_addr = inet_addr(lpszAsciiLocalAddress);

	  //域名转换
	  if (sockLocalAddress.sin_addr.s_addr == INADDR_NONE)
	  {
		  LPHOSTENT lphost;
		  lphost = gethostbyname(lpszAsciiLocalAddress);
		  if (lphost != NULL)
			  sockLocalAddress.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		  else
		  {
			  WSASetLastError(WSAEINVAL); 
			  return FALSE;
		  }
    }

    //绑定
    if (bind(m_hSocket, (sockaddr*) &sockLocalAddress, sizeof(sockLocalAddress)) == SOCKET_ERROR)
      return FALSE;
  }

  
	//远程服务器地址
	LPSTR lpszAsciiDestination = T2A((LPTSTR)pszHostAddress);

	SOCKADDR_IN sockDestinationAddr;
	ZeroMemory(&sockDestinationAddr, sizeof(sockDestinationAddr));
	sockDestinationAddr.sin_family = AF_INET;
	sockDestinationAddr.sin_port = htons((u_short)nPort);
	sockDestinationAddr.sin_addr.s_addr = inet_addr(lpszAsciiDestination);

	if (sockDestinationAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAsciiDestination);
		if (lphost != NULL)
			sockDestinationAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
      WSASetLastError(WSAEINVAL); 
			return FALSE;
		}
	}
	//调用另外一个多态函数connect连接到服务器
	return Connect((SOCKADDR*)&sockDestinationAddr, sizeof(sockDestinationAddr));
}

BOOL CSMTPSocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	return (connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR);
}

BOOL CSMTPSocket::Send(LPCSTR pszBuf, int nBuf)
{
  //must have been created first
  ASSERT(m_hSocket != INVALID_SOCKET);
  return (send(m_hSocket, pszBuf, nBuf, 0) != SOCKET_ERROR);
}

int CSMTPSocket::Receive(LPSTR pszBuf, int nBuf)
{
  //must have been created first
  ASSERT(m_hSocket != INVALID_SOCKET);

  return recv(m_hSocket, pszBuf, nBuf, 0); 
}

void CSMTPSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CSMTPSocket::IsReadable(BOOL& bReadible)
{
  timeval timeout = {0, 0};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(m_hSocket, &fds);
  int nStatus = select(0, &fds, NULL, NULL, &timeout);
  if (nStatus == SOCKET_ERROR)
  {
    return FALSE;
  }
  else
  {
    bReadible = !(nStatus == 0);
    return TRUE;
  }
}




CSMTPAddress::CSMTPAddress() 
{
}

CSMTPAddress::CSMTPAddress(const CSMTPAddress& address)
{
  *this = address;
}

CSMTPAddress::CSMTPAddress(const CString& sAddress) : 
              m_sEmailAddress(sAddress) 
{
  ASSERT(m_sEmailAddress.GetLength()); //An empty address is not allowed
}

CSMTPAddress::CSMTPAddress(const CString& sFriendly, const CString& sAddress) : 
              m_sFriendlyName(sFriendly), m_sEmailAddress(sAddress) 
{
  ASSERT(m_sEmailAddress.GetLength()); //An empty address is not allowed
}

CSMTPAddress& CSMTPAddress::operator=(const CSMTPAddress& r) 
{ 
  m_sFriendlyName = r.m_sFriendlyName; 
	m_sEmailAddress = r.m_sEmailAddress; 
	return *this;
}

CString CSMTPAddress::GetRegularFormat() const
{
  ASSERT(m_sEmailAddress.GetLength()); //Email Address must be valid

  CString sAddress;
  if (m_sFriendlyName.IsEmpty())
    sAddress = m_sEmailAddress;  //Just transfer the address across directly
  else
    sAddress.Format(_T("%s <%s>"), m_sFriendlyName, m_sEmailAddress);

  return sAddress;
}




CSMTPBodyPart::CSMTPBodyPart() : m_sCharset(_T("iso-8859-1")), m_sContentType(_T("text/plain")), m_pParentBodyPart(NULL), m_bQuotedPrintable(TRUE)
{
  //Automatically generate a unique boundary separator for this body part by creating a guid
  UUID uuid;
  UuidCreate(&uuid);
  
  //Convert it to a string
  #ifdef _UNICODE
  TCHAR* pszGuid = NULL;
  #else
  unsigned char* pszGuid = NULL;
  #endif
  UuidToString(&uuid, &pszGuid);

  m_sBoundary = pszGuid;

  //Free up the temp memory
  RpcStringFree(&pszGuid);
}

CSMTPBodyPart::CSMTPBodyPart(const CSMTPBodyPart& bodyPart)
{
  *this = bodyPart;
}

CSMTPBodyPart::~CSMTPBodyPart()
{
  //Free up the array memory
  for (int i=0; i<m_ChildBodyParts.GetSize(); i++)
    delete m_ChildBodyParts.GetAt(i);
  m_ChildBodyParts.RemoveAll();
}

CSMTPBodyPart& CSMTPBodyPart::operator=(const CSMTPBodyPart& bodyPart)
{
  m_sFilename        = bodyPart.m_sFilename;
  m_sText            = bodyPart.m_sText;       
  m_sTitle           = bodyPart.m_sTitle;      
  m_sContentType     = bodyPart.m_sContentType;
  m_sCharset         = bodyPart.m_sCharset;
  m_sContentBase     = bodyPart.m_sContentBase;
  m_sContentID       = bodyPart.m_sContentID;
  m_sContentLocation = bodyPart.m_sContentLocation;
  m_pParentBodyPart  = bodyPart.m_pParentBodyPart;
  m_sBoundary        = bodyPart.m_sBoundary;
  m_bQuotedPrintable = bodyPart.m_bQuotedPrintable;

  //Free up the array memory
  for (int i=0; i<m_ChildBodyParts.GetSize(); i++)
    delete m_ChildBodyParts.GetAt(i);
  m_ChildBodyParts.RemoveAll();
  //Now copy over the new object
  for (i=0; i<bodyPart.m_ChildBodyParts.GetSize(); i++)
  {
    CSMTPBodyPart* pBodyPart = new CSMTPBodyPart(*bodyPart.m_ChildBodyParts.GetAt(i));
    pBodyPart->m_pParentBodyPart  = this;
    m_ChildBodyParts.Add(pBodyPart);
  }

  return *this;
}

BOOL CSMTPBodyPart::SetFilename(const CString& sFilename)
{
  ASSERT(sFilename.GetLength());  //Empty Filename !

  //determine the file size
  CFileStatus fs;
  if (!CFile::GetStatus(sFilename, fs))
  {
    TRACE(_T("Failed to get the status for file %s, probably does not exist\n"), m_sFilename);
    return FALSE;
  }

	//Hive away the filename and form the title from the filename
  TCHAR sPath[_MAX_PATH];
  TCHAR sFname[_MAX_FNAME];
  TCHAR sExt[_MAX_EXT];
  _tsplitpath(sFilename, NULL, NULL, sFname, sExt);
  _tmakepath(sPath, NULL, NULL, sFname, sExt);
	m_sFilename = sFilename;
  m_sTitle = sPath;

  //Also sent the content type to be appropiate for an attachment
  m_sContentType = _T("application/octet-stream");

  return TRUE;
}

CString CSMTPBodyPart::Replace(const CString& sText, const CString& sToBeReplaced, const CString& sReplaceWith)
{
  //The string we will be returning
  CString sFind(sText);
  CString sReturn;

  int nFind = -1;
  int nToBeReplacedLength = sToBeReplaced.GetLength();
  do
  {
    nFind = sFind.Find(sToBeReplaced);
    if (nFind != -1)
    {
      sReturn += (sFind.Left(nFind) + sReplaceWith);
      sFind = sFind.Right(sFind.GetLength() - nFind - nToBeReplacedLength);
    }
  }
  while (nFind != -1);
  sReturn += sFind;

  return sReturn;
}

void CSMTPBodyPart::SetText(const CString& sText)
{
  m_sText = sText;

  //Ensure lines are correctly wrapped
  m_sText = Replace(m_sText, _T("\r\n"), _T("\n"));
  m_sText = Replace(m_sText, _T("\r"), _T("\n"));
  m_sText = Replace(m_sText, _T("\n"), _T("\r\n"));

  //Fix the case of a single dot on a line in the message body
  FixSingleDot(m_sText);

  //Also set the content type while we are at it
  m_sContentType = _T("text/plain");
}

void CSMTPBodyPart::SetContentID(const CString& sContentID) 
{
  m_sContentLocation.Empty();
  m_sContentID = sContentID; 
}

CString CSMTPBodyPart::GetContentID() const 
{ 
  return m_sContentID; 
}

void CSMTPBodyPart::SetContentLocation(const CString& sContentLocation) 
{ 
  m_sContentID.Empty();
  m_sContentLocation = sContentLocation; 
}

CString CSMTPBodyPart::GetContentLocation() const 
{ 
  return m_sContentLocation; 
}

char CSMTPBodyPart::HexDigit(int nDigit)
{
  if (nDigit < 10)
    return (char) (nDigit + '0');
  else
    return (char) (nDigit - 10 + 'A');
}

//Converts text to its Quoted printable equivalent according to RFC 2045
CString CSMTPBodyPart::QuotedPrintableEncode(const CString& sText)
{
  CString sTemp;
  int nSize = sText.GetLength();
  for (int i=0; i<nSize; i++)
  {
    BYTE c = (BYTE) sText[i];
    if (((c >= 33) && (c <= 60)) || ((c >= 62) && (c <= 126)) || (c == '\r') || (c == '\n') || (c == '\t') || (c == ' '))
      sTemp += TCHAR(c);
    else
    {
      //otherwise must quote the text
      sTemp += _T('=');
      sTemp += HexDigit((c & 0xF0) >> 4);
      sTemp += HexDigit(c & 0x0F);
    }
  }

  //Now insert soft line breaks where appropiate
  CString sOut;
  int nStartLine = 0;
  int nLen = sTemp.GetLength();
  for (i=0; i<nLen; i++)
  {
    BYTE c = (BYTE) sTemp[i];
    
    if (c == '\n' || c == '\r' || i == (nLen-1))
    {
      sOut += sTemp.Mid(nStartLine, i-nStartLine+1);
      nStartLine = i+1;
      continue;
    }

    if ((i - nStartLine) > SMTP_MAXLINE)
    {
      BOOL bInWord = TRUE;
      while (bInWord)
      {
        bInWord = (!isspace(c) && sTemp[i-2] != _T('='));
        if (bInWord)
        {
          --i;
          c = (BYTE) sTemp[i];
        }

		    if (i == nStartLine)
		    {
			    i = nStartLine + SMTP_MAXLINE;
			    break;
		    }
      }

      sOut += sTemp.Mid(nStartLine, i-nStartLine+1);
      sOut += _T("=\r\n");
      nStartLine = i+1;
    }
  }

  return sOut;
}

void CSMTPBodyPart::FreeHeader(LPSTR& pszHeader)
{
  //The CSMTPBodyPart class always allocates the memory for the header
  delete [] pszHeader;
  pszHeader = NULL;
}

void CSMTPBodyPart::FreeBody(LPSTR& pszBody)
{
  //The CSMTPBodyPart class allocates the memory for the body if it was not base 64 encoded
  if (pszBody)
  {
    delete [] pszBody;
    pszBody = NULL;
  }
}

void CSMTPBodyPart::FreeFooter(LPSTR& pszFooter)
{
  //The CSMTPBodyPart class always allocates the memory for the footer
  delete [] pszFooter;
  pszFooter = NULL;
}

BOOL CSMTPBodyPart::GetHeader(LPSTR& pszHeader, int& nHeaderSize)
{
  //For correct operation of the T2A macro, see MFC Tech Note 59
  USES_CONVERSION;

  //Assume the worst
  BOOL bSuccess = FALSE;
  CString sHeader;
  if (m_sFilename.GetLength())
  {
    //Ok, it's a file  

    //Form the header to go along with this body part
    if (GetNumberOfChildBodyParts())
		  sHeader.Format(_T("\r\n\r\n--%s\r\nContent-Type: %s; charset=%s; name=%s; Boundary=\"%s\"\r\nContent-Transfer-Encoding: base64\r\nContent-Disposition: attachment; filename=%s\r\n"), 
                     m_pParentBodyPart->m_sBoundary, m_sContentType, m_sCharset, m_sTitle, m_sBoundary, m_sTitle);
    else
		  sHeader.Format(_T("\r\n\r\n--%s\r\nContent-Type: %s; charset=%s; name=%s\r\nContent-Transfer-Encoding: base64\r\nContent-Disposition: attachment; filename=%s\r\n"), 
                     m_pParentBodyPart->m_sBoundary, m_sContentType, m_sCharset, m_sTitle, m_sTitle);

    bSuccess = TRUE;
  }
  else
  {
    //ok, it's some text

    //Form the header to go along with this body part
    ASSERT(m_pParentBodyPart);
    if (GetNumberOfChildBodyParts())
    {
      if (m_bQuotedPrintable)
        sHeader.Format(_T("\r\n--%s\r\nContent-Type: %s; charset=%s; Boundary=\"%s\"\r\nContent-Transfer-Encoding: quoted-printable\r\n"),
                       m_pParentBodyPart->m_sBoundary, m_sContentType, m_sCharset, m_sBoundary);
      else
        sHeader.Format(_T("\r\n--%s\r\nContent-Type: %s; charset=%s; Boundary=\"%s\"\r\n"),
                       m_pParentBodyPart->m_sBoundary, m_sContentType, m_sCharset, m_sBoundary);
    }
    else
    {
      if (m_bQuotedPrintable)
        sHeader.Format(_T("\r\n--%s\r\nContent-Type: %s; charset=%s\r\nContent-Transfer-Encoding: quoted-printable\r\n"),
                       m_pParentBodyPart->m_sBoundary, m_sContentType, m_sCharset);
      else
        sHeader.Format(_T("\r\n--%s\r\nContent-Type: %s; charset=%s\r\n"),
                       m_pParentBodyPart->m_sBoundary, m_sContentType, m_sCharset);
    }

    bSuccess = TRUE;
  }

  //Add the other headers
  if (m_sContentBase.GetLength())
  {
    CString sLine;
    sLine.Format(_T("Content-Base: %s\r\n"), m_sContentBase);
    sHeader += sLine;
  }
  if (m_sContentID.GetLength())
  {
    CString sLine;
    sLine.Format(_T("Content-ID: %s\r\n"), m_sContentID);
    sHeader += sLine;
  }
  if (m_sContentLocation.GetLength())
  {
    CString sLine;
    sLine.Format(_T("Content-Location: %s\r\n"), m_sContentLocation);
    sHeader += sLine;
  }
  sHeader += _T("\r\n");

  nHeaderSize = _tcslen(sHeader);
  pszHeader = new char[nHeaderSize+1];
  strcpy(pszHeader, T2A((LPTSTR) (LPCTSTR) sHeader));

  return bSuccess;
}

BOOL CSMTPBodyPart::GetBody(LPSTR& pszBody, int& nBodySize)
{
	USES_CONVERSION;
	BOOL bSuccess = FALSE;

	if (m_sFilename.GetLength())
	{
		//如果是附件
		//打开文件
		CFile infile;
		if (infile.Open(m_sFilename, CFile::modeRead | CFile::shareDenyWrite))
		{
			DWORD dwSize = infile.GetLength();

			if (dwSize)
			{
				//读入数据
				BYTE* pszIn = new BYTE[dwSize];
				try
				{
					infile.Read(pszIn, dwSize);
					bSuccess = TRUE;
				}
				catch(CFileException* pEx)
				{
					bSuccess = FALSE;
					pEx->Delete();
				}

				if (bSuccess)
				{
					//编码
					m_Coder.Encode(pszIn, dwSize);

					//删除了输入缓冲区
					delete [] pszIn;

					//关闭输入文件
					infile.Close();

					//形成编码后的发送内容
					LPSTR pszEncoded = m_Coder.EncodedMessage();
					int nEncodedSize = m_Coder.EncodedMessageSize();
					nBodySize = nEncodedSize + (((nEncodedSize/76)+1)*2) + 1;
					pszBody = new char[nBodySize];
					--nBodySize; 

					int nInPos = 0;
					int nOutPos = 0;
					while (nInPos < nEncodedSize)
					{
						int nThisLineSize = min(nEncodedSize - nInPos, SMTP_MAXLINE);
						CopyMemory(&pszBody[nOutPos], &pszEncoded[nInPos], nThisLineSize);
						nOutPos += nThisLineSize;
						CopyMemory(&pszBody[nOutPos], "\r\n", 2);
						nOutPos += 2;
						nInPos += nThisLineSize;
					}
					pszBody[nOutPos] = '\0'; //以空字符串结束
				}
			}
			else
			{
				bSuccess = TRUE;
				pszBody = NULL;
				nBodySize = 0;
			}
		}
		else
			TRACE(_T("No bodypart body text or filename specified!\n"));
	}
	else
	{
		//如果是文本内容
		if (m_bQuotedPrintable)
		{
			//形成发送体
			CString sBody = QuotedPrintableEncode(m_sText);
			nBodySize = _tcslen(sBody);
			pszBody = new char[nBodySize+1];
			strcpy(pszBody, T2A((LPTSTR) (LPCTSTR) sBody));
		}
		else
		{
			nBodySize = _tcslen(m_sText);
			pszBody = new char[nBodySize+1];
			strcpy(pszBody, T2A((LPTSTR) (LPCTSTR) m_sText));
		}
		bSuccess = TRUE;
	}
	return bSuccess;
}

BOOL CSMTPBodyPart::GetFooter(LPSTR& pszFooter, int& nFooterSize)
{
  //For correct operation of the T2A macro, see MFC Tech Note 59
  USES_CONVERSION;

  //Form the MIME footer
	CString sFooter;
  sFooter.Format(_T("\r\n--%s--"), m_sBoundary);
  nFooterSize = _tcslen(sFooter);
  pszFooter = new char[nFooterSize+1];
  strcpy(pszFooter, T2A((LPTSTR) (LPCTSTR) sFooter));

  return TRUE;  
}

int CSMTPBodyPart::GetNumberOfChildBodyParts() const
{
  return m_ChildBodyParts.GetSize();
}

int CSMTPBodyPart::AddChildBodyPart(CSMTPBodyPart& bodyPart)
{
  CSMTPBodyPart* pNewBodyPart = new CSMTPBodyPart(bodyPart);
  pNewBodyPart->m_pParentBodyPart = this;
  ASSERT(m_sContentType.GetLength()); 

  return m_ChildBodyParts.Add(pNewBodyPart);
}

void CSMTPBodyPart::RemoveChildBodyPart(int nIndex)
{
  CSMTPBodyPart* pBodyPart = m_ChildBodyParts.GetAt(nIndex);
  delete pBodyPart;
  m_ChildBodyParts.RemoveAt(nIndex);
}

CSMTPBodyPart* CSMTPBodyPart::GetChildBodyPart(int nIndex)
{
  return m_ChildBodyParts.GetAt(nIndex);
}

CSMTPBodyPart* CSMTPBodyPart::GetParentBodyPart()
{
  return m_pParentBodyPart;
}

void CSMTPBodyPart::FixSingleDot(CString& sBody)
{
  int nFind = sBody.Find(_T("\n."));
  if (nFind != -1)
  {
	  CString sLeft(sBody.Left(nFind+1));
	  CString sRight(sBody.Right(sBody.GetLength()-(nFind+1)));
	  FixSingleDot(sRight);
	  sBody = sLeft + _T(".") + sRight;
  }
}

CSMTPBodyPart* CSMTPBodyPart::FindFirstBodyPart(const CString sContentType)
{
  for (int i=0; i<m_ChildBodyParts.GetSize(); i++)
  {
    CSMTPBodyPart* pBodyPart = m_ChildBodyParts.GetAt(i);
    if (pBodyPart->m_sContentType == sContentType)
      return pBodyPart;
  }
  return NULL;
}



CSMTPMessage::CSMTPMessage() : m_sXMailer(_T("CSMTPConnection v2.17")), m_bMime(FALSE)
{
}

CSMTPMessage::~CSMTPMessage()
{
  //Free up the array memory
  for (int i=0; i<m_ToRecipients.GetSize(); i++)
    delete m_ToRecipients.GetAt(i);
  m_ToRecipients.RemoveAll();

  for (i=0; i<m_CCRecipients.GetSize(); i++)
    delete m_CCRecipients.GetAt(i);
  m_CCRecipients.RemoveAll();

  for (i=0; i<m_BCCRecipients.GetSize(); i++)
    delete m_BCCRecipients.GetAt(i);
  m_BCCRecipients.RemoveAll();
}

void CSMTPMessage::SetCharset(const CString& sCharset)
{
  m_RootPart.SetCharset(sCharset);
}

CString CSMTPMessage::GetCharset() const
{
  return m_RootPart.GetCharset();
}

int CSMTPMessage::GetNumberOfRecipients(RECIPIENT_TYPE RecipientType) const
{
  int nSize = 0;
  switch (RecipientType)
  {
    case TO:  nSize = m_ToRecipients.GetSize();  break;
    case CC:  nSize = m_CCRecipients.GetSize();  break;
    case BCC: nSize = m_BCCRecipients.GetSize(); break;
    default: ASSERT(FALSE);                      break;
  }

	return nSize;
}

int CSMTPMessage::AddRecipient(CSMTPAddress& recipient, RECIPIENT_TYPE RecipientType)
{
  int nIndex = -1;

  CSMTPAddress* pNewRecipient = new CSMTPAddress(recipient);

  switch (RecipientType)
  {
    case TO:  nIndex = m_ToRecipients.Add(pNewRecipient);  break;
    case CC:  nIndex = m_CCRecipients.Add(pNewRecipient);  break;
    case BCC: nIndex = m_BCCRecipients.Add(pNewRecipient); break;
    default: ASSERT(FALSE);                            break;
  }

  return nIndex;
}

void CSMTPMessage::RemoveRecipient(int nIndex, RECIPIENT_TYPE RecipientType)
{
  switch (RecipientType)
  {
    case TO:  
    {
      delete m_ToRecipients.GetAt(nIndex);
      m_ToRecipients.RemoveAt(nIndex);  
      break;
    }
    case CC:  
    {
      delete m_CCRecipients.GetAt(nIndex);
      m_CCRecipients.RemoveAt(nIndex);  
      break;
    }
    case BCC: 
    {
      delete m_BCCRecipients.GetAt(nIndex);
      m_BCCRecipients.RemoveAt(nIndex); 
      break;
    }
    default:  
    {
      ASSERT(FALSE);                    
      break;
    }
  }
}

CSMTPAddress* CSMTPMessage::GetRecipient(int nIndex, RECIPIENT_TYPE RecipientType)
{
  switch (RecipientType)
  {
    case TO:  return m_ToRecipients.GetAt(nIndex);   break;
    case CC:  return m_CCRecipients.GetAt(nIndex);   break;
    case BCC: return m_BCCRecipients.GetAt(nIndex);  break;
    default: ASSERT(FALSE); return (CSMTPAddress*) NULL; break;
  }
}

int CSMTPMessage::AddBodyPart(CSMTPBodyPart& Attachment)
{
	SetMime(TRUE); //Body parts implies Mime
	return m_RootPart.AddChildBodyPart(Attachment);
}

void CSMTPMessage::RemoveBodyPart(int nIndex)
{
	m_RootPart.RemoveChildBodyPart(nIndex);
}

CSMTPBodyPart* CSMTPMessage::GetBodyPart(int nIndex)
{
	return m_RootPart.GetChildBodyPart(nIndex);
}

int CSMTPMessage::GetNumberOfBodyParts() const
{
	return m_RootPart.GetNumberOfChildBodyParts();
}

void CSMTPMessage::AddCustomHeader(const CString& sHeader)
{
  m_CustomHeaders.Add(sHeader);
}

CString CSMTPMessage::GetCustomHeader(int nIndex)
{
  return m_CustomHeaders.GetAt(nIndex);
}

int CSMTPMessage::GetNumberOfCustomHeaders() const
{
  return m_CustomHeaders.GetSize();
}

void CSMTPMessage::RemoveCustomHeader(int nIndex)
{
  m_CustomHeaders.RemoveAt(nIndex);
}

CString CSMTPMessage::GetHeader()
{
  //Hive away the locale so that we can restore it later. We
  //require the English locale to ensure the date header is
  //formed correctly
  CString sOldLocale = _tsetlocale(LC_TIME, NULL);    
  _tsetlocale(LC_TIME, _T("english"));

  //Form the Timezone info which will form part of the Date header
  TIME_ZONE_INFORMATION tzi;
  int nTZBias;
  if (GetTimeZoneInformation(&tzi) == TIME_ZONE_ID_DAYLIGHT)
    nTZBias = tzi.Bias + tzi.DaylightBias;
  else
    nTZBias = tzi.Bias;
  CString sTZBias;
  sTZBias.Format(_T("%+.2d%.2d"), -nTZBias/60, nTZBias%60);

  //Create the "Date:" part of the header
  CTime now(CTime::GetCurrentTime());
  CString sDate(now.Format(_T("%a, %d %b %Y %H:%M:%S ")));
  sDate += sTZBias;

  //Create the "To:" part of the header
  CString sTo;
  for (int i=0; i<GetNumberOfRecipients(TO); i++)
  {
    CSMTPAddress* pRecipient = GetRecipient(i, TO);
    if (i)
 		  sTo += _T(",");
    ASSERT(pRecipient);
    sTo += pRecipient->GetRegularFormat();
  }

  //Create the "Cc:" part of the header
  CString sCc;
  for (i=0; i<GetNumberOfRecipients(CC); i++)
  {
    CSMTPAddress* pRecipient = GetRecipient(i, CC);
    if (i)
 		  sCc += _T(",");
    ASSERT(pRecipient);
    sCc += pRecipient->GetRegularFormat();
  }

  //No Bcc info added in header

  //Add the From and to fields
  CString sReply;
  sReply.Format(_T("From: %s\r\nTo: %s\r\n"),  HeaderEncode(m_From.GetRegularFormat()), HeaderEncode(sTo));
  CString sBuf(sReply);
  
  //Add the CC field if there is any
  if (sCc.GetLength())
  {
    sReply.Format(_T("Cc: %s\r\n"), HeaderEncode(sCc));
    sBuf += sReply;
  }

  //Add the subject, Date and X-Mailer fields
  sReply.Format(_T("Subject: %s\r\nDate: %s\r\nX-Mailer: %s\r\n"), HeaderEncode(m_sSubject), HeaderEncode(sDate), HeaderEncode(m_sXMailer));
  sBuf += sReply;

  //Add the Mime header if needed
  BOOL bHasChildParts = (m_RootPart.GetNumberOfChildBodyParts() != 0);
  if (bHasChildParts && m_bMime)
  {
    sReply.Format(_T("MIME-Version: 1.0\r\nContent-Type: %s; boundary=\"%s\"\r\n"), m_RootPart.GetContentType(), m_RootPart.GetBoundary());
    sBuf += sReply;
  }
  else if (!bHasChildParts && m_bMime)
  {
    sReply.Format(_T("MIME-Version: 1.0\r\nContent-Type: %s\r\n"), m_RootPart.GetContentType());
    sBuf += sReply;
  }
  sBuf += _T("Content-Transfer-Encoding: quoted-printable\r\n");


	//Add the optional Reply-To Field
	if (m_ReplyTo.m_sEmailAddress.GetLength())
	{
		CString sReply;
		sReply.Format(_T("Reply-To: %s\r\n"), m_ReplyTo.GetRegularFormat());
		sBuf += sReply;
	}

  //Add the custom headers
  int nCustomHeaders = m_CustomHeaders.GetSize();
  for (i=0; i<nCustomHeaders; i++)
  {
    CString sHeader = m_CustomHeaders.GetAt(i);
    sBuf += sHeader;
    
    //Add line separators for each header bar the final one
    if (i < (nCustomHeaders-1))
      sBuf += _T("\r\n");
  }

  //Add the line separator between the header and what will follow
  //i.e. the body
	sBuf += _T("\r\n");

  // restore original locale
  _tsetlocale(LC_TIME, sOldLocale);

	//Return the result
	return sBuf;
}

//Converts header text to its encoded form according to RFC 2047
CString CSMTPMessage::HeaderEncode(const CString& sText) const
{
  int nSize = sText.GetLength();

  //Determine if a translation is needed
  BOOL bTranslationNeeded = FALSE;
  for (int i=0; i<nSize && !bTranslationNeeded; i++)
  {
    BYTE c = (BYTE) sText[i];
    bTranslationNeeded = (c > 127);
  }

  CString sOut;
  if (bTranslationNeeded)
  {
    sOut.Format(_T("=?%s?q?"), m_RootPart.GetCharset());
    for (i=0; i<nSize; i++)
    {
      BYTE c = (BYTE) sText[i];
      if (c == ' ') // A space
        sOut += _T('_');
      else if ((c > 127) || (c == '=') || (c == '?') || (c == '_'))
      {
        //Must Quote the text
        sOut += _T('=');
        sOut += CSMTPBodyPart::HexDigit((c & 0xF0) >> 4);
        sOut += CSMTPBodyPart::HexDigit(c & 0x0F);
      }
      else
        sOut += TCHAR(c);
    }
    sOut += _T("?=");
  }
  else
  {
    //Just pass the text thro unmodified
    sOut = sText;
  }

  return sOut;
}

CSMTPAddressArray* CSMTPMessage::ParseMultipleRecipients(const CString& sRecipients)
{
	ASSERT(sRecipients.GetLength()); //An empty string is now allowed

  //What will be the return value
  CSMTPAddressArray* pRecipients = new CSMTPAddressArray;
	
	//Loop through the whole string, adding recipients as they are encountered
	int length = sRecipients.GetLength();
	TCHAR* buf = new TCHAR[length + 1];	// Allocate a work area (don't touch parameter itself)
	_tcscpy(buf, sRecipients);
	for (int pos=0, start=0; pos<=length; pos++)
	{
		//Valid separators between addresses are ',' or ';'
		if ((buf[pos] == _T(',')) || (buf[pos] == _T(';')) || (buf[pos] == 0))
		{
			buf[pos] = 0;	//Redundant when at the end of string, but who cares.
      CString sTemp(&buf[start]);

			// Now divide the substring into friendly names and e-mail addresses
      CSMTPAddress To;
			int nMark = sTemp.Find(_T('<'));
			if (nMark >= 0)
			{
				To.m_sFriendlyName = sTemp.Left(nMark);
				int nMark2 = sTemp.Find(_T('>'));
				if (nMark2 < nMark)
				{
          //An invalid string was sent in, fail the call
					delete [] buf;
          SetLastError(ERROR_INVALID_DATA);
          TRACE(_T("An error occurred while parsing the recipients string\n"));
          pRecipients->RemoveAll();
					return pRecipients;
				}
				// End of mark at closing bracket or end of string
				nMark2 > -1 ? nMark2 = nMark2 : nMark2 = sTemp.GetLength() - 1;
				To.m_sEmailAddress = sTemp.Mid(nMark + 1, nMark2 - (nMark + 1));
			}
			else
			{
				To.m_sEmailAddress = sTemp;
				To.m_sFriendlyName = _T("");
			}

      //Finally add the new recipient to the array of recipients
			To.m_sEmailAddress.TrimRight();
			To.m_sEmailAddress.TrimLeft();
			To.m_sFriendlyName.TrimRight();
			To.m_sFriendlyName.TrimLeft();
      if (To.m_sEmailAddress.GetLength())
        pRecipients->Add(To);

      //Move on to the next position
			start = pos + 1;
		}
	}
    
  //Tidy up the heap memory we have used
	delete [] buf;

	return pRecipients;
}

BOOL CSMTPMessage::AddMultipleRecipients(const CString& sRecipients, RECIPIENT_TYPE RecipientType)
{
  CSMTPAddressArray* pRecipients = ParseMultipleRecipients(sRecipients);

  //Add them to the correct array
  for (int i=0; i<pRecipients->GetSize(); i++)
  {
    CSMTPAddress& address = pRecipients->ElementAt(i);
    AddRecipient(address, RecipientType);
  }

  //Tidy up the heap memory
  delete pRecipients;

	return TRUE;
}

int CSMTPMessage::AddMultipleAttachments(const CString& sAttachments)
{
	BOOL nAttachments = 0;

	ASSERT(sAttachments.GetLength()); //允许空字符串
	
	//循环获得多个附件
	int length = sAttachments.GetLength();
	TCHAR* buf = new TCHAR[length + 1];	//申请空间
	_tcscpy(buf, sAttachments);
	for (int pos=0, start=0; pos<=length; pos++)
	{
		//每个附件之间通过","或者";"来分隔开的
		if ((buf[pos] == _T(',')) || (buf[pos] == _T(';')) || (buf[pos] == 0))
		{
			buf[pos] = 0;	
			CString sTemp(&buf[start]);

			//将附件增加到附件数组中
			CSMTPBodyPart attachment;
			sTemp.TrimRight();
			sTemp.TrimLeft();
			if (sTemp.GetLength())
			{
				BOOL bAdded = attachment.SetFilename(sTemp);
				if (bAdded)
				{
					++nAttachments;
					AddBodyPart(attachment);
				}
			}

			//移到下一个位置
			start = pos + 1;
		}
	}

	//清除临时缓冲区
	delete [] buf;

	return nAttachments;
}

CString CSMTPMessage::ConvertHTMLToPlainText(const CString& sHtml)
{
  //First pull out whats within the body tags
  CString sRet(sHtml);
  sRet.MakeUpper();
  int nStartCut = -1;
  int nStartBody = sRet.Find(_T("<BODY"));
  if (nStartBody != -1)
  {
    sRet = sHtml.Right(sHtml.GetLength() - nStartBody - 5);
    int nTemp = sRet.Find(_T('>'));
    nStartCut = nStartBody + nTemp + 6;
    sRet = sRet.Right(sRet.GetLength() - nTemp - 1);
  }
  sRet.MakeUpper();
  int nLength = sRet.Find(_T("</BODY"));

  //Finally do the actual cutting
  if (nLength != -1)
    sRet = sHtml.Mid(nStartCut, nLength);
  else
    sRet = sHtml;

  //Now strip all html tags
  int nStartTag = sRet.Find(_T('<'));
  int nEndTag = sRet.Find(_T('>'));
  while (nStartTag != -1 && nEndTag != -1)
  {
    sRet = sRet.Left(nStartTag) + sRet.Right(sRet.GetLength() - nEndTag - 1);
    nStartTag = sRet.Find(_T('<'));
    nEndTag = sRet.Find(_T('>'));
  }

  sRet.TrimLeft();
  sRet.TrimRight();
  return sRet;
}

void CSMTPMessage::AddTextBody(const CString& sBody)
{
  if (m_bMime)
  {
    CSMTPBodyPart* pTextBodyPart = m_RootPart.FindFirstBodyPart(_T("text/plain"));
    if (pTextBodyPart)
      pTextBodyPart->SetText(sBody);
    else
    {
      //Create a text body part
      CSMTPBodyPart oldRoot = m_RootPart;

      //Reset the root body part to be multipart/related
      m_RootPart.SetCharset(oldRoot.GetCharset());
      m_RootPart.SetText(_T("This is a multi-part message in MIME format"));
      m_RootPart.SetContentType(_T("multipart/mixed"));

      //Just add the text/plain body part (directly to the root)
      CSMTPBodyPart text;
      text.SetCharset(oldRoot.GetCharset());
      text.SetText(sBody);

      //Hook everything up to the root body part
      m_RootPart.AddChildBodyPart(text);
    }
  }
  else
  {
    //Let the body part class do all the work
    m_RootPart.SetText(sBody);
  }
}

void CSMTPMessage::AddHTMLBody(const CString& sBody, const CString& sContentBase)
{
  ASSERT(m_bMime); //You forgot to make this a MIME message using SetMime(TRUE)

  CSMTPBodyPart* pHtmlBodyPart = m_RootPart.FindFirstBodyPart(_T("text/html"));
  if (pHtmlBodyPart)
    pHtmlBodyPart->SetText(sBody);
  else
  {
    //Remember some of the old root settings before we write over it
    CSMTPBodyPart oldRoot = m_RootPart;

    //Reset the root body part to be multipart/related
    m_RootPart.SetCharset(oldRoot.GetCharset());
    m_RootPart.SetText(_T("This is a multi-part message in MIME format"));
    m_RootPart.SetContentType(_T("multipart/related"));

    //Just add the text/html body part (directly to the root)
    CSMTPBodyPart html;
    html.SetCharset(oldRoot.GetCharset());
    html.SetText(sBody);
    html.SetContentType(_T("text/html"));
    html.SetContentBase(sContentBase);

    //Hook everything up to the root body part
    m_RootPart.AddChildBodyPart(html);
  }
}

CString CSMTPMessage::GetHTMLBody()
{
  CString sRet;

  if (m_RootPart.GetNumberOfChildBodyParts())
  {
    CSMTPBodyPart* pHtml = m_RootPart.GetChildBodyPart(0);
    if (pHtml->GetContentType() == _T("text/html"))
      sRet = pHtml->GetText();
  }
  return sRet;
}

CString CSMTPMessage::GetTextBody()
{
  return m_RootPart.GetText();
}

void CSMTPMessage::SetMime(BOOL bMime)
{
  if (m_bMime != bMime)
  {
    m_bMime = bMime;

    //Reset the body body parts
    for (int i=0; i<m_RootPart.GetNumberOfChildBodyParts(); i++)
      m_RootPart.RemoveChildBodyPart(i);

    if (bMime)
    {
      CString sText = GetTextBody();

      //Remember some of the old root settings before we write over it
      CSMTPBodyPart oldRoot = m_RootPart;

      //Reset the root body part to be multipart/mixed
      m_RootPart.SetCharset(oldRoot.GetCharset());
      m_RootPart.SetText(_T("This is a multi-part message in MIME format"));
      m_RootPart.SetContentType(_T("multipart/mixed"));

      //Also readd the body if non - empty
      if (sText.GetLength())
        AddTextBody(sText);
    }
  }
}

BOOL CSMTPMessage::SaveToDisk(const CString& sFilename)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

  //Assume the worst
  BOOL bSuccess = FALSE;

  //Open the file for writing
  CFile outFile;
  if (outFile.Open(sFilename, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
  {
    try
    {
      //Write out the Message Header
      CString sHeader = GetHeader();
      char* pszHeader = T2A((LPTSTR) (LPCTSTR) sHeader);
      int nCmdLength = strlen(pszHeader);
      outFile.Write(pszHeader, nCmdLength);

	    //Write out the separator
      char* pszBodyHeader = "\r\n";
      nCmdLength = strlen(pszBodyHeader);
      outFile.Write(pszBodyHeader, nCmdLength);

      //Write out the rest of the message
      BOOL bHasChildParts = (m_RootPart.GetNumberOfChildBodyParts() != 0);
      if (bHasChildParts || m_bMime)
      {
        //Write the root body part (and all its children)
        WriteToDisk(outFile, &m_RootPart, TRUE);
      }
      else
      {
        //Send the body
        char* pszBody = T2A((LPTSTR) (LPCTSTR) m_RootPart.GetText());
        ASSERT(pszBody);
        nCmdLength = strlen(pszBody);

        //Send the body
        outFile.Write(pszBody, nCmdLength);
      }

      //Got to here, everything is ok
      bSuccess = TRUE;
    }
    catch(CFileException* pEx)
    {
      TRACE(_T("A file exception occured while trying to save the message to file %s\n"), sFilename);
      pEx->Delete();
    }
  }

  return bSuccess;
}

BOOL CSMTPMessage::WriteToDisk(CFile& file, CSMTPBodyPart* pBodyPart, BOOL bRoot)
{
  //Assume success
  BOOL bSuccess = TRUE;

  if (!bRoot)
  {
    //First send this body parts header
    LPSTR pszHeader = NULL;
    int nHeaderSize = 0;
    if (!pBodyPart->GetHeader(pszHeader, nHeaderSize))
    {
		  TRACE(_T("Failed in call to send body parts header, GetLastError returns: %d\n"), GetLastError());
		  return FALSE;
    }
		file.Write(pszHeader, nHeaderSize);

    //Free up the temp memory we have used
    pBodyPart->FreeHeader(pszHeader);
  }
  
  //Then the body parts body
  LPSTR pszBody = NULL;
  int nBodySize = 0;
  if (!pBodyPart->GetBody(pszBody, nBodySize))
  {
		TRACE(_T("Failed in call to send body parts body, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
  }
	file.Write(pszBody, nBodySize);

  //Free up the temp memory we have used
  pBodyPart->FreeBody(pszBody);

  //The recursively Send all the child body parts
  int nChildBodyParts = pBodyPart->GetNumberOfChildBodyParts();
  for (int i=0; i<nChildBodyParts && bSuccess; i++)
  {
    CSMTPBodyPart* pChildBodyPart = pBodyPart->GetChildBodyPart(i);
    bSuccess = WriteToDisk(file, pChildBodyPart, FALSE);
  }

  //Then the MIME footer if need be
  BOOL bSendFooter = (pBodyPart->GetNumberOfChildBodyParts() != 0);
  if (bSendFooter)
  {
    LPSTR pszFooter = NULL;
    int nFooterSize = 0;
    if (!pBodyPart->GetFooter(pszFooter, nFooterSize))
    {
		  TRACE(_T("Failed in call to send body parts footer, GetLastError returns: %d\n"), GetLastError());
		  return FALSE;
    }

	  file.Write(pszFooter, nFooterSize);

    //Free up the temp memory we have used
    pBodyPart->FreeFooter(pszFooter);
  }
  
  return bSuccess;
}




CSMTPConnection::CSMTPConnection()
{
  m_bConnected = FALSE;
#ifdef _DEBUG
  m_dwTimeout = 90000; //default timeout of 90 seconds when debugging
#else
  m_dwTimeout = 60000;  //default timeout of 60 seconds for normal release code
#endif
  m_nLastCommandResponseCode = 0;
  m_sHeloHostname = _T("auto");
}

CSMTPConnection::~CSMTPConnection()
{
  if (m_bConnected)
    Disconnect();
}

BOOL CSMTPConnection::Connect(LPCTSTR pszHostName, LoginMethod lm, LPCTSTR pszUsername, LPCTSTR pszPassword, int nPort, LPCTSTR pszLocalBoundAddress)
{
	USES_CONVERSION;

  //参数合法性检查
  ASSERT(pszHostName);

  //创建socket
  if (!m_SMTP.Create())
  {
    AfxMessageBox("不能创建socket");

	return FALSE;
  }

  //连接到服务器
  if (!m_SMTP.Connect(pszHostName, nPort, pszLocalBoundAddress))
  {
    if (pszLocalBoundAddress  && _tcslen(pszLocalBoundAddress))
	{
	  AfxMessageBox("不能连接服务器");
    }
	else
    {  
      AfxMessageBox("不能连接服务器");
	}
	return FALSE;
  }
  else
  {
    //连接成功
    m_bConnected = TRUE;

    //获得相应
    if (!ReadCommandResponse(220))
    {
      TRACE(_T("An unexpected SMTP login response was received\n"));
      Disconnect();
      return FALSE;
    }

	    //获得本地机器名
		if (m_sHeloHostname == _T("auto"))
		{
			//获得主机名
			char sHostName[100];
			gethostname(sHostName, sizeof(sHostName));
			TCHAR* pszHostName = A2T(sHostName);
			m_sHeloHostname = pszHostName;
		}

	  // 进行下一步连接，即进行SMTP会话
	  BOOL bConnectOk = FALSE;
	  if (lm != NoLoginMethod)
		  bConnectOk = ConnectESMTP(m_sHeloHostname, pszUsername, pszPassword, lm);
	  else
		  bConnectOk = ConnectSMTP(m_sHeloHostname);

	  // 如果失败，则断开连接
	  if (!bConnectOk)
	  {
      Disconnect();
      return FALSE;
	  }

    return TRUE;
  }
}

// 该函数是通过扩展SMTP会话方式连接服务器的
BOOL CSMTPConnection::ConnectESMTP(LPCTSTR pszLocalName, LPCTSTR pszUsername, LPCTSTR pszPassword, LoginMethod lm)
{
  //验证参数
	ASSERT(pszUsername);
	ASSERT(pszPassword);
	ASSERT(lm != NoLoginMethod);

	USES_CONVERSION;

	//发送EHLO命令
	CString sBuf;
	sBuf.Format(_T("EHLO %s\r\n"), pszLocalName);
	LPCSTR pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
	int nCmdLength = strlen(pszData);
	if (!m_SMTP.Send(pszData, nCmdLength))
	{
		return FALSE;
	}
	
	//检查Hello命令的响应
	if (!ReadCommandResponse(250))
	{
		return FALSE;
	}

	BOOL bLoginOk = FALSE;
	switch (lm)
	{
    #ifndef CSMTP_NORSA
    case CramMD5Method:
    {
      bLoginOk = CramLogin(pszUsername, pszPassword); // CRAM-MD5 authentication
      break;
    }
    #endif
    case AuthLoginMethod:
    {
      bLoginOk = AuthLogin(pszUsername, pszPassword); // LOGIN authentication
      break;
    }
    case LoginPlainMethod:
    {
      bLoginOk = AuthLoginPlain(pszUsername, pszPassword); // PLAIN authentication
      break;
    }
    default:
    {
      ASSERT(FALSE);
      break;
    }
  }
	return bLoginOk;
}

// This function connects using standard SMTP connection i.e. HELO
BOOL CSMTPConnection::ConnectSMTP(LPCTSTR pszLocalName)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

  //Send the HELO command
	CString sBuf;
	sBuf.Format(_T("HELO %s\r\n"), pszLocalName);
	LPCSTR pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
	int nCmdLength = strlen(pszData);
	if (!m_SMTP.Send(pszData, nCmdLength))
	{
	  TRACE(_T("An unexpected error occurred while sending the HELO command\n"));
	  return FALSE;
	}
//check the response to the HELO command
	return ReadCommandResponse(250);
}

BOOL CSMTPConnection::Disconnect()
{
  BOOL bSuccess = FALSE;      

  //disconnect from the SMTP server if connected 
  if (m_bConnected)
  {
    char sBuf[10];
    strcpy(sBuf, "QUIT\r\n");
    int nCmdLength = strlen(sBuf);
    if (!m_SMTP.Send(sBuf, nCmdLength))
      TRACE(_T("Failed in call to send QUIT command, GetLastError returns: %d\n"), GetLastError());

    //Check the reponse
    bSuccess = ReadCommandResponse(221);
    if (!bSuccess)
    {
      SetLastError(ERROR_BAD_COMMAND);
      TRACE(_T("An unexpected QUIT response was received\n"));
    }

    //Reset all the state variables
    m_bConnected = FALSE;
  }
  else
    TRACE(_T("Already disconnected from SMTP server, doing nothing\n"));
 
  //free up our socket
  m_SMTP.Close();
 
  return bSuccess;
}

BOOL CSMTPConnection::SendBodyPart(CSMTPBodyPart* pBodyPart, BOOL bRoot)
{
  //Assume success
  BOOL bSuccess = TRUE;

  if (!bRoot)
  {
    //发送信件头
    LPSTR pszHeader = NULL;
    int nHeaderSize = 0;
    if (!pBodyPart->GetHeader(pszHeader, nHeaderSize))
    {
		  TRACE(_T("Failed in call to send body parts header, GetLastError returns: %d\n"), GetLastError());
		  return FALSE;
    }
		if (!m_SMTP.Send(pszHeader, nHeaderSize))
		{
			bSuccess = FALSE;
		}
    //释放空间
    pBodyPart->FreeHeader(pszHeader);
  }
  
  //根信体内容
  LPSTR pszBody = NULL;
  int nBodySize = 0;
  if (!pBodyPart->GetBody(pszBody, nBodySize))
  {
		TRACE(_T("Failed in call to send body parts body, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
  }
	if (!m_SMTP.Send(pszBody, nBodySize))
	{
		TRACE(_T("Failed in call to send body parts body, GetLastError returns: %d\n"), GetLastError());
		bSuccess = FALSE;
	}
  //释放内存空间
  pBodyPart->FreeBody(pszBody);

  //发送附件
  int nChildBodyParts = pBodyPart->GetNumberOfChildBodyParts();
  for (int i=0; i<nChildBodyParts && bSuccess; i++)
  {
    CSMTPBodyPart* pChildBodyPart = pBodyPart->GetChildBodyPart(i);
    bSuccess = SendBodyPart(pChildBodyPart, FALSE);
  }

  //发送发送MINE结束部分
  BOOL bSendFooter = (pBodyPart->GetNumberOfChildBodyParts() != 0);
  if (bSendFooter)
  {
    LPSTR pszFooter = NULL;
    int nFooterSize = 0;
    if (!pBodyPart->GetFooter(pszFooter, nFooterSize))
    {
		  return FALSE;
    }
	  if (!m_SMTP.Send(pszFooter, nFooterSize))
	  {
		  bSuccess = FALSE;
	  }
    //释放内存
    pBodyPart->FreeFooter(pszFooter);
  }
  
  return bSuccess;
}

BOOL CSMTPConnection::SendMessage(CSMTPMessage& Message)
{
	USES_CONVERSION;

	ASSERT(m_bConnected); 

	//发送Email命令
	ASSERT(Message.m_From.m_sEmailAddress.GetLength());
	CString sBuf;
	sBuf.Format(_T("MAIL FROM:<%s>\r\n"), Message.m_From.m_sEmailAddress);
	LPCSTR pszMailFrom = T2A((LPTSTR) (LPCTSTR) sBuf);
	int nCmdLength = strlen(pszMailFrom);
	if (!m_SMTP.Send(pszMailFrom, nCmdLength))
	{
		TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
	}

	//检查服务器响应
	if (!ReadCommandResponse(250))
	{
		SetLastError(ERROR_BAD_COMMAND);
		TRACE(_T("An unexpected MAIL response was received\n"));
		return FALSE;
	} 

	//发送RCPT命令,每一个接受者都需要发送一次

	//必须有收信人
	ASSERT(Message.GetNumberOfRecipients(CSMTPMessage::TO) + 
         Message.GetNumberOfRecipients(CSMTPMessage::CC) + 
         Message.GetNumberOfRecipients(CSMTPMessage::BCC));

	//首先"TO"发送接收者
	for (int i=0; i<Message.GetNumberOfRecipients(CSMTPMessage::TO); i++)
	{
		CSMTPAddress* pRecipient = Message.GetRecipient(i, CSMTPMessage::TO);
		ASSERT(pRecipient);
		if (!SendRCPTForRecipient(*pRecipient))
		return FALSE;
	}

	//然后是 "CC" 接收者
	for (i=0; i<Message.GetNumberOfRecipients(CSMTPMessage::CC); i++)
	{
		CSMTPAddress* pRecipient = Message.GetRecipient(i, CSMTPMessage::CC);
		ASSERT(pRecipient);
		if (!SendRCPTForRecipient(*pRecipient))
			return FALSE;
	}

	//然后是"BCC" 接收者 
	for (i=0; i<Message.GetNumberOfRecipients(CSMTPMessage::BCC); i++)
	{
		CSMTPAddress* pRecipient = Message.GetRecipient(i, CSMTPMessage::BCC);
		ASSERT(pRecipient);
		if (!SendRCPTForRecipient(*pRecipient))
			return FALSE;
	}

	//发送DATA命令
	char* pszDataCommand = "DATA\r\n";
	nCmdLength = strlen(pszDataCommand);
	if (!m_SMTP.Send(pszDataCommand, nCmdLength))
	{
		TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
	}

	//检查DATA命令的响应
	if (!ReadCommandResponse(354))
	{
		SetLastError(ERROR_BAD_COMMAND);
		TRACE(_T("An unexpected DATA response was received\n"));
		return FALSE;
	} 

	//发送信息头
	CString sHeader = Message.GetHeader();
	char* pszHeader = T2A((LPTSTR) (LPCTSTR) sHeader);
	nCmdLength = strlen(pszHeader);
	if (!m_SMTP.Send(pszHeader, nCmdLength))
	{
		TRACE(_T("Failed in call to send the header, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
	}

	BOOL bHasChildParts = (Message.m_RootPart.GetNumberOfChildBodyParts() != 0);
	if (bHasChildParts || Message.m_bMime)
	{
		//发送信件内容
		if(!SendBodyPart(&Message.m_RootPart, TRUE))
		return FALSE;
	}
	else
	{
		//Send the Header for the body (Note in the case of non mime emails, it is just a simple line separaror
		char* pszBodyHeader = "\r\n";
		nCmdLength = strlen(pszBodyHeader);
		if (!m_SMTP.Send(pszBodyHeader, nCmdLength))
		{
			TRACE(_T("Failed in call to send the body header, GetLastError returns: %d\n"), GetLastError());
			return FALSE;
		}

		char* pszBody = T2A((LPTSTR) (LPCTSTR) Message.m_RootPart.GetText());
		ASSERT(pszBody);
		nCmdLength = strlen(pszBody);

		//发送信体
		if (!m_SMTP.Send(pszBody, nCmdLength))
		{
		TRACE(_T("Failed in call to send the header, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
		}
	}

	//发送邮件结束指示符
	char* pszEOM = "\r\n.\r\n";
	nCmdLength = strlen(pszEOM);
	if (!m_SMTP.Send(pszEOM, nCmdLength))
	{
		TRACE(_T("Failed in call to send end of message indicator, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
	}

	//检查结束符响应
	if (!ReadCommandResponse(250))
	{
		SetLastError(ERROR_BAD_COMMAND);
		TRACE(_T("An unexpected end of message response was received\n"));
		return FALSE;
	} 

	return TRUE;
}

BOOL CSMTPConnection::OnSendProgress(DWORD /*dwCurrentBytes*/, DWORD /*dwTotalBytes*/)
{
  //By default just return TRUE to allow the mail to continue to be sent
  return TRUE; 
}

BOOL CSMTPConnection::SendMessage(BYTE* pMessage, DWORD dwTotalBytes, CSMTPAddressArray& Recipients, const CSMTPAddress& From, DWORD dwSendBufferSize)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

	//paramater validity checking
  ASSERT(m_bConnected); //Must be connected to send a message

  //Send the MAIL command
	ASSERT(From.m_sEmailAddress.GetLength());
  CString sBuf;
  sBuf.Format(_T("MAIL FROM:<%s>\r\n"), From.m_sEmailAddress);
  LPCSTR pszMailFrom = T2A((LPTSTR) (LPCTSTR) sBuf);
  int nCmdLength = strlen(pszMailFrom);
  if (!m_SMTP.Send(pszMailFrom, nCmdLength))
  {
    TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
    return FALSE;
  }

  //check the response to the MAIL command
  if (!ReadCommandResponse(250))
  {
    SetLastError(ERROR_BAD_COMMAND);
    TRACE(_T("An unexpected MAIL response was received\n"));
    return FALSE;
  } 

  //Must be sending to someone
  int nRecipients = Recipients.GetSize();
  ASSERT(nRecipients);

  //Send the RCPT command, one for each recipient
  for (int i=0; i<nRecipients; i++)
  {
    CSMTPAddress& recipient = Recipients.ElementAt(i);
    if (!SendRCPTForRecipient(recipient))
      return FALSE;
  }

  //Send the DATA command
  char* pszDataCommand = "DATA\r\n";
  nCmdLength = strlen(pszDataCommand);
  if (!m_SMTP.Send(pszDataCommand, nCmdLength))
  {
    TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
    return FALSE;
  }

  //check the response to the DATA command
  if (!ReadCommandResponse(354))
  {
    SetLastError(ERROR_BAD_COMMAND);
    TRACE(_T("An unexpected DATA response was received\n"));
    return FALSE;
  } 

  //Read and send the data a chunk at a time
  BOOL bMore = TRUE;
  BOOL bSuccess = TRUE;
  DWORD dwBytesSent = 0;
  BYTE* pSendBuf = pMessage; 
  do
  {
    DWORD dwRead = min(dwSendBufferSize, dwTotalBytes-dwBytesSent);
    dwBytesSent += dwRead;

    //Call the progress virtual method
    if (OnSendProgress(dwBytesSent, dwTotalBytes))
    {
      if (!m_SMTP.Send((LPCSTR)pSendBuf, dwRead))
      {
        TRACE(_T("Failed in call to send the message, GetLastError returns: %d\n"), GetLastError());
        bSuccess = FALSE;
      }
    }
    else
    {
      //Abort the mail send (due to the progress virtual method returning FALSE
      bSuccess = FALSE;
    }

    //Prepare for the next time around
    pSendBuf += dwRead;
    bMore = (dwBytesSent < dwTotalBytes);
  }
  while (bMore && bSuccess);

  if (bSuccess)
  {
    //Send the end of message indicator
    char* pszEOM = "\r\n.\r\n";
	  nCmdLength = strlen(pszEOM);
    if (!m_SMTP.Send(pszEOM, nCmdLength))
    {
      TRACE(_T("Failed in call to send end of message indicator, GetLastError returns: %d\n"), GetLastError());
      return FALSE;
    }

    //check the response to the End of Message command
    if (!ReadCommandResponse(250))
    {
      SetLastError(ERROR_BAD_COMMAND);
      TRACE(_T("An unexpected end of message response was received\n"));
      return FALSE;
    } 

    return TRUE;
  }
  else
    return FALSE;
}


BOOL CSMTPConnection::SendMessage(const CString& sMessageOnFile, CSMTPAddressArray& Recipients, const CSMTPAddress& From, DWORD dwSendBufferSize)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

	//paramater validity checking
  ASSERT(m_bConnected); //Must be connected to send a message

  //Send the MAIL command
	ASSERT(From.m_sEmailAddress.GetLength());
  CString sBuf;
  sBuf.Format(_T("MAIL FROM:<%s>\r\n"), From.m_sEmailAddress);
  LPCSTR pszMailFrom = T2A((LPTSTR) (LPCTSTR) sBuf);
  int nCmdLength = strlen(pszMailFrom);
  if (!m_SMTP.Send(pszMailFrom, nCmdLength))
  {
    TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
    return FALSE;
  }

  //check the response to the MAIL command
  if (!ReadCommandResponse(250))
  {
    SetLastError(ERROR_BAD_COMMAND);
    TRACE(_T("An unexpected MAIL response was received\n"));
    return FALSE;
  } 

  //Must be sending to someone
  int nRecipients = Recipients.GetSize();
  ASSERT(nRecipients);

  //Send the RCPT command, one for each recipient
  for (int i=0; i<nRecipients; i++)
  {
    CSMTPAddress& recipient = Recipients.ElementAt(i);
    if (!SendRCPTForRecipient(recipient))
      return FALSE;
  }

  //Send the DATA command
  char* pszDataCommand = "DATA\r\n";
  nCmdLength = strlen(pszDataCommand);
  if (!m_SMTP.Send(pszDataCommand, nCmdLength))
  {
    TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
    return FALSE;
  }

  //check the response to the DATA command
  if (!ReadCommandResponse(354))
  {
    SetLastError(ERROR_BAD_COMMAND);
    TRACE(_T("An unexpected DATA response was received\n"));
    return FALSE;
  } 

  //Open up the file
  CFile mailFile;
  if (mailFile.Open(sMessageOnFile, CFile::modeRead | CFile::shareDenyWrite))
  {
    //Get the length of the file (used for the progress support)
    DWORD dwTotalBytes = mailFile.GetLength();

    //Allocate a buffer we will use in the sending
    char* pSendBuf = new char[dwSendBufferSize];

    //Read and send the data a chunk at a time
    BOOL bMore = TRUE;
    BOOL bSuccess = TRUE;
    int nBytesSent = 0; 
    do
    {
      try
      {
        //Read the chunk from file
        UINT nRead = mailFile.Read(pSendBuf, dwSendBufferSize);
        bMore = (nRead == dwSendBufferSize);

        //Send the chunk
        if (nRead)
        {
          nBytesSent += nRead;

          //Call the progress virtual method
          if (OnSendProgress(nBytesSent, dwTotalBytes))
          {
            if (!m_SMTP.Send(pSendBuf, nRead))
            {
              TRACE(_T("Failed in call to send the message, GetLastError returns: %d\n"), GetLastError());
              bSuccess = FALSE;
            }
          }
          else
          {
            //Abort the mail send (due to the progress virtual method returning FALSE
            bSuccess = FALSE;
          }
        }
      }
      catch(CFileException* pEx)
      {
        bSuccess = FALSE;
        TRACE(_T("An error occurred reading from the file to send %s\n"), sMessageOnFile);
        pEx->Delete();
      }
    }
    while (bMore && bSuccess);

    //Tidy up the heap memory we have used
    delete [] pSendBuf;

    if (bSuccess)
    {
      //Send the end of message indicator
      char* pszEOM = "\r\n.\r\n";
	    nCmdLength = strlen(pszEOM);
      if (!m_SMTP.Send(pszEOM, nCmdLength))
      {
        TRACE(_T("Failed in call to send end of message indicator, GetLastError returns: %d\n"), GetLastError());
        return FALSE;
      }

      //check the response to the End of Message command
      if (!ReadCommandResponse(250))
      {
        SetLastError(ERROR_BAD_COMMAND);
        TRACE(_T("An unexpected end of message response was received\n"));
        return FALSE;
      } 

    	return TRUE;
    }
    else
      return FALSE;
  }
  else
  {
    TRACE(_T("Could not open the file to send %s\n"), sMessageOnFile);
    return FALSE;
  }
}

BOOL CSMTPConnection::SendRCPTForRecipient(CSMTPAddress& recipient)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

	ASSERT(recipient.m_sEmailAddress.GetLength()); //must have an email address for this recipient

  CString sBuf;
  sBuf.Format(_T("RCPT TO:<%s>\r\n"), recipient.m_sEmailAddress);
  LPSTR pszRCPT = T2A((LPTSTR) (LPCTSTR) sBuf);

  int nCmdLength = strlen(pszRCPT);
  if (!m_SMTP.Send(pszRCPT, nCmdLength))
  {
    TRACE(_T("Failed in call to send RCPT command, GetLastError returns: %d\n"), GetLastError());
    return FALSE;
  }

  //check the response to the RCPT command
  if (!ReadCommandResponse(250))
  {
    SetLastError(ERROR_BAD_COMMAND);
    TRACE(_T("An unexpected RCPT response was received\n"));
    return FALSE;
  } 
  
  return TRUE;
}

BOOL CSMTPConnection::ReadCommandResponse(int nExpectedCode)
{
  LPSTR pszOverFlowBuffer = NULL;
  char sBuf[256];
  BOOL bSuccess = ReadResponse(sBuf, 256, "\r\n", nExpectedCode, &pszOverFlowBuffer);
  if (pszOverFlowBuffer)
    delete [] pszOverFlowBuffer;

  return bSuccess;
}

BOOL CSMTPConnection::ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, int nExpectedCode, LPSTR* ppszOverFlowBuffer, int nGrowBy)
{
	ASSERT(ppszOverFlowBuffer);          //验证是否有一个字符串指针
	ASSERT(*ppszOverFlowBuffer == NULL); //验证字符串是否委空

	//验证是否创建
	ASSERT(m_bConnected);

	//获取结束符的长度
	int nTerminatorLen = strlen(pszTerminator);

	//接收获得的数据
	LPSTR pszRecvBuffer = pszBuffer;
	int nBufSize = nInitialBufSize;

	//循环获得响应知道遇到结束符或者超时
	BOOL bFoundTerminator = FALSE;
	int nReceived = 0;
	DWORD dwStartTicks = ::GetTickCount();
	while (!bFoundTerminator)
	{
		//超时
		if ((::GetTickCount() - dwStartTicks) >	m_dwTimeout)
		{
			pszRecvBuffer[nReceived] = '\0';
			SetLastError(WSAETIMEDOUT);
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return FALSE;
		}

		//检查socket是否可读
		BOOL bReadible;
		if (!m_SMTP.IsReadable(bReadible))
		{
			pszRecvBuffer[nReceived] = '\0';
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return FALSE;
		}
		else if (!bReadible) //没有数据接受到
		{
			Sleep(250); //sleep
		}

		//从socket接收数据
		int nBufRemaining = nBufSize-nReceived-1; //Allows allow one space for the NULL terminator
		if (nBufRemaining<0)
			nBufRemaining = 0;
		int nData = m_SMTP.Receive(pszRecvBuffer+nReceived, nBufRemaining);

		//重新设置超时起始时间
		if (nData > 0)
		{
			dwStartTicks = ::GetTickCount();
			//增加获得的数据
			nReceived += nData;							   
		}

		//如果接收数据时错误产生
		if (nData < 1)
		{
			//以空字符结束字符串
			if (pszRecvBuffer)
			pszBuffer[nReceived] = '\0';
			m_sLastCommandResponse = pszRecvBuffer; //分离出最后一个响应
			return FALSE; 
		}
		else
		{
			//以空字符结束字符串
			if (pszRecvBuffer)
			  pszRecvBuffer[nReceived] = '\0';
			if (nBufRemaining-nData == 0) //检查是否还有空间
			{
				//申请新的缓冲区
				nBufSize += nGrowBy; //增加缓冲区
				LPSTR pszNewBuf = new char[nBufSize];

				//将原来的内容复制到新的缓冲区中，并用新的缓冲区接收数据
				if (pszRecvBuffer)
				  strcpy(pszNewBuf, pszRecvBuffer);
				pszRecvBuffer = pszNewBuf;

				//删除原来缓冲区
				if (*ppszOverFlowBuffer)
				  delete [] *ppszOverFlowBuffer;

				//将新的缓冲区内容存入溢出缓冲区
				*ppszOverFlowBuffer = pszNewBuf;        
			}
		}

		// 检查结束符是否被找到
		// 检查是否到了响应结束，否则提前停止循环读取数据，如果有可能会有多行响应
		bFoundTerminator = (strncmp( &pszRecvBuffer[ nReceived - nTerminatorLen ], pszTerminator, nTerminatorLen ) == 0);
	}

	//去掉结束符
	pszRecvBuffer[ nReceived - nTerminatorLen ] = '\0';

	//处理多行响应
	BOOL bSuccess = FALSE;
	do
	{
		// 检查响应是否是错误码
		char sCode[4];
		strncpy(sCode, pszRecvBuffer, 3 );
		sCode[3] = '\0';
		sscanf(sCode, "%d", &m_nLastCommandResponseCode );
		bSuccess = (m_nLastCommandResponseCode == nExpectedCode);

		// 分离上一次响应
		m_sLastCommandResponse = pszRecvBuffer;

		// 检查是否有多行响应
		pszRecvBuffer = strstr( pszRecvBuffer, pszTerminator );
		if (pszRecvBuffer)
			pszRecvBuffer += nTerminatorLen;	// 跳过结束符

	} while ( !bSuccess && pszRecvBuffer );

	if (!bSuccess)
		SetLastError(WSAEPROTONOSUPPORT);

	return bSuccess;
}

// 使用AUTH LOGIN命令协商登录，即需要对用户名进行编码
BOOL CSMTPConnection::AuthLogin(LPCTSTR pszUsername, LPCTSTR pszPassword)
{
	USES_CONVERSION;

	//发送AUTH LOGIN命令
	CString sBuf;
	sBuf.Format(_T("AUTH LOGIN\r\n"));
	LPCSTR pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
	int nCmdLength = strlen(pszData);
	if (!m_SMTP.Send(pszData, nCmdLength))
	{
	  TRACE(_T("An unexpected error occurred while sending the AUTH command\n"));
	  return FALSE;
	}

	//初始化
	CBase64Coder Coder;
	if (!ReadCommandResponse(334))
	{
		TRACE(_T("Server does not support AUTH LOGIN!\n"));
		return FALSE;
	}
	else
	{
		//发送base64编码后的用户名
		CString sLastCommandString = m_sLastCommandResponse;
		sLastCommandString = sLastCommandString.Right(sLastCommandString.GetLength() - 4);
		LPCSTR pszLastCommandString = T2A((LPTSTR) (LPCTSTR) sLastCommandString);
		Coder.Decode(pszLastCommandString);
		if (strcmp(Coder.DecodedMessage(), "Username:") == 0)
		{
			Coder.Encode(T2A((LPTSTR)pszUsername));
			sBuf.Format(_T("%s\r\n"), Coder.EncodedMessage());
			pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
			nCmdLength = strlen(pszData);
			if (!m_SMTP.Send(pszData, nCmdLength))
			{
			  TRACE(_T("An unexpected error occurred while sending the username\n"));
			  return FALSE;
			}
		}
		else
		{
			TRACE(_T("An unexpected request received when expecting username request"));
			return FALSE;
		}
	}

	//检查返回用户名返回码 
	if (!ReadCommandResponse(334))
  {
	  TRACE(_T("Server did not response correctly to AUTH LOGIN username field!\n"));
	  return FALSE;
  }
	else
	{
		//发送密码到服务器
		CString sLastCommandString = m_sLastCommandResponse;
		sLastCommandString = sLastCommandString.Right(sLastCommandString.GetLength() - 4);
		LPCSTR pszLastCommandString = T2A((LPTSTR) (LPCTSTR) sLastCommandString);
		Coder.Decode(pszLastCommandString);
		if (strcmp(Coder.DecodedMessage(), "Password:") == 0)
		{
			Coder.Encode(T2A((LPTSTR)pszPassword));
			sBuf.Format(_T("%s\r\n"), Coder.EncodedMessage());
			pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
			nCmdLength = strlen(pszData);
			if (!m_SMTP.Send(pszData, nCmdLength))
			{
			  TRACE(_T("An unexpected error occurred while sending the password\n"));
			  return FALSE;
			}

			//检查是否成功
			if (!ReadCommandResponse(235))
			{
				TRACE(_T("AUTH LOGIN authentication was unsuccessful\n"));
				return FALSE;
			}
		}
		else
		{
			TRACE(_T("An unexpected request received when expecting password request\n"));
			return FALSE;
		}
	}

	return TRUE;
}


// 该函数实现授权登录AUTH LOGIN PLAIN
BOOL CSMTPConnection::AuthLoginPlain(LPCTSTR pszUsername, LPCTSTR pszPassword)
{
	USES_CONVERSION;

	//发送AUTH LOGIN PLAIN 命令
	CString sBuf;
	sBuf.Format(_T("AUTH LOGIN PLAIN\r\n"));
	LPCSTR pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
	int nCmdLength = strlen(pszData);
	if (!m_SMTP.Send(pszData, nCmdLength))
	{
		return FALSE;
	}

	if (!ReadCommandResponse(334))
	{
		return FALSE;
	}
	else
	{
		//发送plain用户名
		CString sLastCommandString = m_sLastCommandResponse;
		sLastCommandString = sLastCommandString.Right(sLastCommandString.GetLength() - 4);
		LPCSTR pszLastCommandString = T2A((LPTSTR) (LPCTSTR) sLastCommandString);
		if (strcmp(pszLastCommandString, "Username:") == 0)
		{
			sBuf.Format(_T("%s\r\n"), pszUsername);
			pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
			nCmdLength = strlen(pszData);
			if (!m_SMTP.Send(pszData, nCmdLength))
			{
			  TRACE(_T("An unexpected error occurred while sending the username\n"));
			  return FALSE;
			}
		}
	}

	//检查命令Username命令的响应 
	if (!ReadCommandResponse(334))
	{
		TRACE(_T("Server did not response correctly to AUTH LOGIN PLAIN username field!\n"));
		return FALSE;
	}
	else
	{
		//发送密码命令
		CString LastCommandString = m_sLastCommandResponse;
		LastCommandString = LastCommandString.Right(LastCommandString.GetLength() - 4);
		if (lstrcmp(LastCommandString, _T("Password:")) == 0)
		{
			sBuf.Format(_T("%s\r\n"), pszPassword);
			pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
			nCmdLength = strlen(pszData);
			if (!m_SMTP.Send(pszData, nCmdLength))
			{
			  TRACE(_T("An unexpected error occurred while sending the password\n"));
			  return FALSE;
			}

			// 检查是否会话成功
			if (!ReadCommandResponse(235))
			{
				TRACE(_T("AUTH LOGIN PLAIN 验证方式成功\n"));
				return FALSE;
			}
		}
	}

	return TRUE;
}

#ifndef CSMTP_NORSA
BOOL CSMTPConnection::CramLogin(LPCTSTR pszUsername, LPCTSTR pszPassword)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

  //Send the AUTH CRAM-MD5 command
	CString sBuf;
	sBuf.Format(_T("AUTH CRAM-MD5\r\n"));
	LPCSTR pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
	int nCmdLength = strlen(pszData);
	if (!m_SMTP.Send(pszData, nCmdLength))
	{
	  TRACE(_T("An unexpected error occurred while sending the AUTH command\n"));
	  return FALSE;
	}

	// initialize base64 encoder / decoder
	CBase64Coder Coder;
	if (!ReadCommandResponse(334))
  {
	  TRACE(_T("Server does not support AUTH CRAM-MD5!\n"));
	  return FALSE;
  }
	else
	{
		CString sLastCommandString = m_sLastCommandResponse;
		sLastCommandString = sLastCommandString.Right(sLastCommandString.GetLength() - 4);
    LPCSTR pszLastCommandString = T2A((LPTSTR) (LPCTSTR) sLastCommandString);
		Coder.Decode(pszLastCommandString);

		// Get the base64 decoded challange 
		LPCSTR pszChallenge = Coder.DecodedMessage();

		// test data as per RFC 2195
    //		pszChallenge = "<1896.697170952@postoffice.reston.mci.net>";
    //		pszUsername = "tim";
    //		pszAsciiPassword = "tanstaaftanstaaf";
		// generate the MD5 digest from the challange and password
		unsigned char digest[16];    // message digest
    LPSTR pszAsciiPassword = T2A((LPTSTR)pszPassword);
		MD5Digest((unsigned char*) pszChallenge, strlen(pszChallenge), 
              (unsigned char*) pszAsciiPassword, strlen(pszAsciiPassword), digest);
		
		// make the CRAM-MD5 response
		CString sCramDigest;
		sCramDigest = pszUsername;
		sCramDigest += " ";
		for (int i=0; i<16; i++)
		{
		  CString csTemp;
			csTemp.Format(_T("%02x"), digest[i]);
			sCramDigest += csTemp;
		}
			
		// send base64 encoded username digest
    LPSTR pszCramDigest = T2A((LPTSTR) (LPCTSTR) sCramDigest);
		Coder.Encode(pszCramDigest);
		sBuf.Format(_T("%s\r\n"), Coder.EncodedMessage());
		pszData = T2A((LPTSTR) (LPCTSTR) sBuf);
		nCmdLength = strlen(pszData);
		if (!m_SMTP.Send(pszData, nCmdLength))
		{
		  TRACE(_T("An unexpected error occurred while sending the username\n"));
		  return FALSE;
		}
	}

	// check if authentication is successful
	if (!ReadCommandResponse(235))
  {
    TRACE(_T("AUTH CRAM-MD5 authentication was unsuccessful\n"));
		return FALSE;
  }

	return TRUE;
}
#endif

#ifndef CSMTP_NORSA
void CSMTPConnection::MD5Digest(unsigned char* text, int text_len, unsigned char* key, int key_len, unsigned char* digest)
{
  unsigned char tk[16];

  // if key is longer than 64 bytes reset it to key=MD5(key)
  if (key_len > 64) 
  {
    MD5_CTX tctx;
    MD5Init(&tctx);
    MD5Update(&tctx, key, key_len);
    MD5Final(tk, &tctx);

    key = tk;
    key_len = 16;
  }

  // the HMAC_MD5 transform looks like:
  //
  // MD5(K XOR opad, MD5(K XOR ipad, text))
  //
  // where K is an n byte key
  // ipad is the byte 0x36 repeated 64 times
  // opad is the byte 0x5c repeated 64 times
  // and text is the data being protected

  //start out by storing key in pads
  unsigned char k_ipad[65];    // inner padding - key XORd with ipad
  memset(k_ipad, 0, 64);
  unsigned char k_opad[65];    // outer padding - key XORd with opad
  memset(k_opad, 0, 64);
  memcpy(k_ipad, key, key_len);
  memcpy(k_opad, key, key_len);

  // XOR key with ipad and opad values
  for (int i=0; i<64; i++) 
  {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  //perform inner MD5
  MD5_CTX context;
  MD5Init(&context);                   // init context for 1st pass
  MD5Update(&context, k_ipad, 64);     // start with inner pad
  MD5Update(&context, text, text_len); // then text of datagram
  MD5Final(digest, &context);          // finish up 1st pass

  //perform outer MD5
  MD5Init(&context);                   // init context for 2nd pass
  MD5Update(&context, k_opad, 64);     // start with outer pad
  MD5Update(&context, digest, 16);     // then results of 1st hash
  MD5Final(digest, &context);          // finish up 2nd pass
}
#endif

BOOL CSMTPConnection::ConnectToInternet()
{
  if (_WinInetData.m_lpfnInternetGetConnectedState && _WinInetData.m_lpfnInternetAttemptConnect)
  {
    // Check to see if an internet connection exists already.
    // bInternet = TRUE  internet connection exists.
    // bInternet = FALSE internet connection does not exist
    DWORD dwFlags;
    BOOL bInternet = _WinInetData.m_lpfnInternetGetConnectedState(&dwFlags, 0);
    if (!bInternet)
    {
      // Attempt to establish internet connection, probably
      // using Dial-up connection.
      // static method CloseInternetConnection() called when
      // Mail dialog is destroyed.  Closes dial-up connection
      // if any.
      DWORD dwResult = _WinInetData.m_lpfnInternetAttemptConnect(0);
      if (dwResult != ERROR_SUCCESS)
      {
        SetLastError(dwResult);
        return FALSE;
      }
    }
    return TRUE;
  }
  else
  {
    //Wininet is not available. Do what would happen if the dll
    //was present but the function call failed
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);  
    return FALSE;
  }
}

BOOL CSMTPConnection::CloseInternetConnection()
{
  if (_WinInetData.m_lpfnInternetAutoDialHangup)
  {
    // Make sure any connection through a modem is 'closed'.
    return _WinInetData.m_lpfnInternetAutoDialHangup(0);
  }
  else
  {
    //Wininet is not available. Do what would happen if the dll
    //was present but the function call failed
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);  
    return FALSE;
  }
}

