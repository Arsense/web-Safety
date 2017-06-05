// FTPclient.h: interface for the CFTPclient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPCLIENT_H__05003AE0_E234_11D2_970B_00A024EBF6AB__INCLUDED_)
#define AFX_FTPCLIENT_H__05003AE0_E234_11D2_970B_00A024EBF6AB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



class CFTPclient
{
public:
	CString m_retmsg;
	CByteArray m_buf;
	CString m_strLine;


	BOOL MoveFile(CString remotefile, CString localfile,BOOL pasv,BOOL get);
	void LogOffServer();
	BOOL LogOnToServer(CString hostname,int hostport,CString username, CString password, CString acct, CString fwhost,CString fwusername, CString fwpassword,int fwport,int logontype);
	CFTPclient();
	~CFTPclient();
	BOOL FTPcommand(CString command);
	BOOL ReadStr();
	BOOL WriteStr(CString outputstring);
	BOOL List();
	void ProcessList();
	BOOL GetLine(int& ndx);




private:
	CArchive* m_pCtrlRxarch;
	CArchive* m_pCtrlTxarch;
	CSocketFile* m_pCtrlsokfile;
	CSocket* m_Ctrlsok;
	int m_fc;
	BOOL ReadStr2();
	BOOL OpenControlChannel(CString serverhost,int serverport);
	void CloseControlChannel();

protected:

};


#endif // !defined(AFX_FTPCLIENT_H__05003AE0_E234_11D2_970B_00A024EBF6AB__INCLUDED_)
