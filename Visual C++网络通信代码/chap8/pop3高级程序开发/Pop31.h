// Pop31.h: interface for the CPop3 class.
//mailto: zmoraa@box43.pl
// feel free to e-mail me!
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_POP31_H__1957A2DC_027A_43AF_82B6_789B397147F3__INCLUDED_)
#define AFX_POP31_H__1957A2DC_027A_43AF_82B6_789B397147F3__INCLUDED_

#include "Gniazdo.h"
#include <vector>
#include <strstream>
#include <string>

using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//enum type describing actual pop3 state
typedef enum {FIRST=0,USER,PASS,STAT,LIST,RETR,ENDRETR,DELE,GOON} STATE;

#define S_GETNUMMSGS 5 //send when user can obtain number of messages
#define S_GETSIZEMSGS 6 //as above, but size of messages
#define S_ENDRETR 7 //send when done retrieving

//simple struct that keeps message data, size, and retrieving size
typedef struct 
{
	CString text;
	int msgSize, retrSize;
} MESSAGEPROP;

class CPop3  : public CBase4Pop3
{

public:
	void WriteMsg(int i,CString name); //保存邮件到文件
	//获得基本的邮件信息字段: From, To, Date, Subject, Body
	CString GetMsgStuff(int i); 
	//获得信件体
	CString GetMsgBody(int i);
	//是否收完信后从服务器删除邮件
	void DelAfterRead(BOOL del=FALSE);
	//获得信件标题
	CString GetMsgSubject(int i);
	//获得整封信的内容
	CString GetMsg(int i);
	//获得邮件的数量
	int GetRetrMsgNum();
	//获得所有邮件的大小
	int GetSizeMsg();
	//获得服务器上邮件的数量
	int GetNumMsg();
	//获得错误
	CString GetError();
	//退出服务器
	void Close();
	//设定用户名和密码
	void SetProp(CString u, CString p);
	//获得从服务器传来的最后一条信息
	void GetLastMsg(CString &);
	//接收时间
	void OnReceive(int err);
	CPop3();
	virtual ~CPop3();

private:
	//读入一行数据
	void ReadLn(int index,CString src, CString &dst);
	// 分开每条消息
	void ParseMsg();
	CString lastMsg, error;
	STATE state;
	CString user, pass;
	int numMsg, sizeMsg, retrMsg;
	vector<MESSAGEPROP> msgs; //vector for message stuff
	BOOL delAfterRead;
	int delMsg;
};

#endif // !defined(AFX_POP31_H__1957A2DC_027A_43AF_82B6_789B397147F3__INCLUDED_)
