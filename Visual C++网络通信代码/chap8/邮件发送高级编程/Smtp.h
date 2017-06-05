/*
Module : SMTP.H
Purpose: Defines the interface for a MFC class encapsulation of the SMTP protocol
Created: PJN / 22-05-1998

Copyright (c) 1998 - 2002 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////// Defines ///////////////////////////////////////

#ifndef __SMTP_H__
#define __SMTP_H__

#ifndef __AFXTEMPL_H__
#pragma message("To avoid this message, put afxtempl.h in your PCH")
#include <afxtempl.h>
#endif

#ifndef _WINSOCKAPI_
#pragma message("To avoid this message, put afxsock.h or winsock.h in your PCH")
#include <winsock.h>
#endif

#ifndef __AFXPRIV_H__
#pragma message("To avoid this message, put afxpriv.h in your PCH")
#include <afxpriv.h>
#endif

#include "Base64Coder.h"

 

/////////////////////////////// Classes ///////////////////////////////////////

//Simple Socket wrapper class
class CSMTPSocket
{
public:
//Constructors / Destructors
  CSMTPSocket();
  ~CSMTPSocket();

//methods
  BOOL  Create();
  BOOL  Connect(LPCTSTR pszHostAddress, int nPort, LPCTSTR pszLocalBoundAddress);
  BOOL  Send(LPCSTR pszBuf, int nBuf);
  void  Close();
  int   Receive(LPSTR pszBuf, int nBuf);
  BOOL  IsReadable(BOOL& bReadible);

protected:
  BOOL   Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
  SOCKET m_hSocket;
};

                     
//SMTP地址类，用于对EMAIL地址进行各种处理
class CSMTPAddress
{
public: 
  CSMTPAddress();
  CSMTPAddress(const CSMTPAddress& address);
	CSMTPAddress(const CString& sAddress);
	CSMTPAddress(const CString& sFriendly, const CString& sAddress);
	CSMTPAddress& operator=(const CSMTPAddress& r);

//方法
  CString GetRegularFormat() const;

//数据成员
	CString m_sFriendlyName; //保存名称
	CString m_sEmailAddress; //保存EMAIL地址
};


//该类处理信件内容
class CSMTPBodyPart
{
public:
  CSMTPBodyPart();
  CSMTPBodyPart(const CSMTPBodyPart& bodyPart);
  CSMTPBodyPart& operator=(const CSMTPBodyPart& bodyPart);
  virtual ~CSMTPBodyPart();

//Accessors / Mutators
  BOOL    SetFilename(const CString& sFilename);
  CString GetFilename() const { return m_sFilename; }; 

  void    SetText(const CString& sText);
  CString GetText() const { return m_sText; };

	void    SetTitle(const CString& sTitle) { m_sTitle = sTitle; };
	CString GetTitle() const { return m_sTitle; };

	void    SetContentType(const CString& sContentType) { m_sContentType = sContentType; };
	CString GetContentType() const { return m_sContentType; };

	void    SetCharset(const CString& sCharset) { m_sCharset = sCharset; };
	CString GetCharset() const { return m_sCharset; };

  void    SetContentBase(const CString& sContentBase) { m_sContentBase = sContentBase; };
  CString GetContentBase() const { return m_sContentBase; };

  void    SetContentID(const CString& sContentID);
  CString GetContentID() const;

  void    SetContentLocation(const CString& sContentLocation);
  CString GetContentLocation() const;

  CString GetBoundary() const { return m_sBoundary; };

//Misc methods
  BOOL GetHeader(LPSTR& pszHeader, int& nHeaderSize);
  BOOL GetBody(LPSTR& pszBody, int& nBodySize);
  BOOL GetFooter(LPSTR& pszFooter, int& nFooterSize);
  void FreeHeader(LPSTR& pszHeader);
  void FreeBody(LPSTR& pszBody);
  void FreeFooter(LPSTR& pszFooter);
  CSMTPBodyPart* FindFirstBodyPart(const CString sContentType);
  void SetQuotedPrintable(BOOL bValue) { m_bQuotedPrintable = bValue; };
  BOOL GetQuotedPrintable() const { return m_bQuotedPrintable; };

//子信体函数
	int            GetNumberOfChildBodyParts() const;
	int            AddChildBodyPart(CSMTPBodyPart& bodyPart);
	void           RemoveChildBodyPart(int nIndex);
	CSMTPBodyPart* GetChildBodyPart(int nIndex);
  CSMTPBodyPart* GetParentBodyPart();

//静态函数
  static CString QuotedPrintableEncode(const CString& sText);
  static char HexDigit(int nDigit);

protected:
//成员函数
  CString      m_sFilename;                                 //附件名称
  CString      m_sTitle;                                    //呵呵，信件名称What is it to be know as when emailed
  CString      m_sContentType;                              //信件内容类型
  CString      m_sCharset;                                  //新建内容的字符集
  CString      m_sContentBase;                              //绝对URL路径
  CString      m_sContentID;                                //The uniqiue ID for this body part (allows other body parts to refer to us via a CID URL)
  CString      m_sContentLocation;                          //The relative URL for this body part (allows other body parts to refer to us via a relative URL)
  CString      m_sText;                                     //If using strings rather than file, then this is it!
  CBase64Coder m_Coder;	                                    //Base64 encoder / decoder instance for this body part
  CArray<CSMTPBodyPart*, CSMTPBodyPart*&> m_ChildBodyParts; //Child body parts for this body part
  CSMTPBodyPart* m_pParentBodyPart;                         //The parent body part for this body part
  CString      m_sBoundary;                                 //String which is used as the body separator for all child mime parts
  BOOL         m_bQuotedPrintable;                          //Should the body text by quoted printable encoded

//方法
  void FixSingleDot(CString& sBody);
  CString Replace(const CString& sText, const CString& sToBeReplaced, const CString& sReplaceWith);

  friend class CSMTPMessage;
  friend class CSMTPConnection;
};


////////////////// typedefs 
typedef CArray<CSMTPAddress, CSMTPAddress&> CSMTPAddressArray;


////////////////// Forward declaration
class CSMTPConnection;


//该类用来保存服务器返回的消息
class CSMTPMessage
{
public:
//Enums
	enum RECIPIENT_TYPE { TO, CC, BCC };

//Constructors / Destructors
  CSMTPMessage();
  virtual ~CSMTPMessage();

//接收相关函数
	int           GetNumberOfRecipients(RECIPIENT_TYPE RecipientType = TO) const;
	int           AddRecipient(CSMTPAddress& recipient, RECIPIENT_TYPE RecipientType = TO);
	void          RemoveRecipient(int nIndex, RECIPIENT_TYPE RecipientType = TO);
	CSMTPAddress* GetRecipient(int nIndex, RECIPIENT_TYPE RecipientType = TO);
  BOOL          AddMultipleRecipients(const CString& sRecipients, RECIPIENT_TYPE RecipientType);
  static CSMTPAddressArray* ParseMultipleRecipients(const CString& sRecipients);

//信体处理函数
  int            GetNumberOfBodyParts() const;
	int            AddBodyPart(CSMTPBodyPart& bodyPart);
	void           RemoveBodyPart(int nIndex);
	CSMTPBodyPart* GetBodyPart(int nIndex);
  int            AddMultipleAttachments(const CString& sAttachments);

//其他方法
  virtual CString GetHeader();
  void            AddTextBody(const CString& sBody);
  CString         GetTextBody();
  void            AddHTMLBody(const CString& sBody, const CString& sContentBase);
  CString         GetHTMLBody();
  void            AddCustomHeader(const CString& sHeader);
  CString         GetCustomHeader(int nIndex);
  int             GetNumberOfCustomHeaders() const;
  void            RemoveCustomHeader(int nIndex);
  void            SetCharset(const CString& sCharset);
  CString         GetCharset() const;
  void            SetMime(BOOL bMime);
  BOOL            GetMime() const { return m_bMime; };
  BOOL            SaveToDisk(const CString& sFilename);
                          
//数据成员
	CSMTPAddress  m_From;
	CString       m_sSubject;
  CString       m_sXMailer;
	CSMTPAddress  m_ReplyTo;
  CSMTPBodyPart m_RootPart;


protected:
  BOOL WriteToDisk(CFile& file, CSMTPBodyPart* pBodyPart, BOOL bRoot);
  CString HeaderEncode(const CString& sText) const;
  CString ConvertHTMLToPlainText(const CString& sHtml);

	CArray<CSMTPAddress*, CSMTPAddress*&> m_ToRecipients;
	CArray<CSMTPAddress*, CSMTPAddress*&> m_CCRecipients;
	CArray<CSMTPAddress*, CSMTPAddress*&> m_BCCRecipients;
  CStringArray                          m_CustomHeaders;
  BOOL                                  m_bMime;

  friend class CSMTPConnection;
};



//SMTP的主类，用于进行SMTP的连接
class CSMTPConnection
{
public:

//typedefs
enum LoginMethod
{
  NoLoginMethod=0,
  CramMD5Method=1,
  AuthLoginMethod=2,
  LoginPlainMethod=3
};

  CSMTPConnection();
  virtual ~CSMTPConnection();

//方法
  BOOL    Connect(LPCTSTR pszHostName, LoginMethod lm=NoLoginMethod, LPCTSTR pszUsername=NULL, LPCTSTR pszPassword=NULL, int nPort=25, LPCTSTR pszLocalBoundAddress=NULL);
  BOOL    Disconnect();
  CString GetLastCommandResponse() const { return m_sLastCommandResponse; };
  int     GetLastCommandResponseCode() const { return m_nLastCommandResponseCode; };
  DWORD   GetTimeout() const { return m_dwTimeout; };
  void    SetTimeout(DWORD dwTimeout) { m_dwTimeout = dwTimeout; };
	BOOL    SendMessage(CSMTPMessage& Message);
  BOOL    SendMessage(const CString& sMessageOnFile, CSMTPAddressArray& Recipients, const CSMTPAddress& From, DWORD dwSendBufferSize = 4096);
  BOOL    SendMessage(BYTE* pMessage, DWORD dwMessageSize, CSMTPAddressArray& Recipients, const CSMTPAddress& From, DWORD dwSendBufferSize = 4096);
  void    SetHeloHostname(const CString& sHostname) { m_sHeloHostname = sHostname; };
  CString GetHeloHostName() const { return m_sHeloHostname; };

//静态方法
  static BOOL ConnectToInternet();
  static BOOL CloseInternetConnection();

//虚拟方法
  virtual BOOL OnSendProgress(DWORD dwCurrentBytes, DWORD dwTotalBytes);

protected:
#ifndef CSMTP_NORSA
  void MD5Digest(unsigned char*text, int text_len, unsigned char*key, int key_len, unsigned char* digest);
#endif
  BOOL ConnectESMTP(LPCTSTR pszLocalName, LPCTSTR pszUsername, LPCTSTR pszPassword, LoginMethod lm);
  BOOL ConnectSMTP(LPCTSTR pszLocalName);
#ifndef CSMTP_NORSA
	BOOL CramLogin(LPCTSTR pszUsername, LPCTSTR pszPassword);
#endif
	BOOL  AuthLogin(LPCTSTR pszUsername, LPCTSTR pszPassword);
	BOOL  AuthLoginPlain(LPCTSTR pszUsername, LPCTSTR pszPassword);
	BOOL  SendRCPTForRecipient(CSMTPAddress& recipient);
  BOOL  SendBodyPart(CSMTPBodyPart* pBodyPart, BOOL bRoot);
	virtual BOOL ReadCommandResponse(int nExpectedCode);
	virtual BOOL ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, 
                            int nExpectedCode, LPSTR* ppszOverFlowBuffer, int nGrowBy=4096);

  CSMTPSocket m_SMTP;
  BOOL        m_bConnected;
  CString     m_sLastCommandResponse;
  CString     m_sHeloHostname;
	DWORD       m_dwTimeout;
  int         m_nLastCommandResponseCode;
};

#endif //__SMTP_H__

