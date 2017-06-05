#pragma once

#include "basepop3socket.h" 
#include "Pop3Manage.h"
#include <vector>
#include <strstream>
#include <string>

//主要早客户机处理各种情况

typedef enum {FIRST=0,USER,PASS,STAT,LIST,RETR,ENDRETR,DELE,GOON} STATE;
typedef struct 
{
	CString text;
	int MsgSize, retrSize;
} MESSAGEPROP;
#define GETNUMMSGS 5 //获得信件数量
#define GETSIZEMSGS 8 //获得信件大小
#define ENDRETR1 7 //接受信件


class CPop3Manage :
	public CBasePop3Socket
{
public:
	CPop3Manage(void);
	~CPop3Manage(void);

	void SetProp(CString UserName, CString PassWord);
	void DelAfterRead(BOOL bDelete);
	void CPop3Manage::ParseMessage();  //解析消息
	void CPop3Manage::GetLastMsg(CString &s);
	CString CPop3Manage::GetError();
	int CPop3Manage::GetNumMsg();
	int CPop3Manage::GetSizeMsg();
	int CPop3Manage::GetRetrMsgNum();
	CString CPop3Manage::GetMsgSubject(int i);
	void CPop3Manage::ReadLn(int index,CString src, CString &dst);
	CString CPop3Manage::GetMsgStuff(int i);
	CString CPop3Manage::GetMsgBody(int i);
	

	CString m_UserName;
	CString m_PassWord;
	BOOL m_bDelete;
	vector<MESSAGEPROP> m_Messsages;
	STATE m_state;
	int m_numMsg;
	int m_sizeMsg;
	int m_retrMsg;
	int m_delMsg;

	CString m_lastMsg;
	CString m_error;
	void OnReceive(int nErrorCode);
};

