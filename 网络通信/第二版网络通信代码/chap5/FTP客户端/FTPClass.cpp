/*/////////////////////////////////////////////////////////////////////
FTPclient.cpp (c) GDI 1999
V1.0.0 (10/4/99)
Phil Anderson. philip@gd-ind.com

Simple FTP client functionality. If you have any problems with it,
please tell me about them (or better still e-mail me the fixed
code). Please feel free to use this code however you wish, although
if you make changes please put your name in the source & comment what
you did.

Nothing awesome going on here at all (all sockets are used in
synchronous blocking mode), but it does the following
things WinInet doesn't seem to:
* Supports loads of different firewalls (I think, I don't
  have access to all types so they haven't all been fully
  tested yet)
* Allows you to execute any command on the FTP server
* Adds 10K to your app install rather than 1Mb #;-)

Functions return TRUE if everything went OK, FALSE if there was an,
error. A message describing the outcome (normally the one returned
from the server) will be in m_retmsg on return from the function.
There are a few error msgs in the app's string table that you'll
need to paste into your app, along with this file & FTPclient.h

If you created your app without checking the "Use Windows Sockets"
checkbox in AppWizard, you'll need to add the following bit of code
to you app's InitInstance()

if(!AfxSocketInit())
{
	AfxMessageBox("Could not initialize Windows Sockets!");
	return FALSE;
}

To use:

1/ Create an object of CFTPclient.

2/ Use LogOnToServer() to connect to the server. Any arguments
not used (e.g. if you're not using a firewall), pass an empty
string or zero for numeric args. You must pass a server port
number, use the FTP default of 21 if you don't know what it is.

3/ Use MoveFile() to upload/download a file, 1st arg is local file
path, 2nd arg is remote file path, 3rd arg is TRUE for a PASV
connection (required by some firewalls), FALSE otherwise, 4th arg
is TRUE to upload, FALSE to download file. MoveFile only works in
synchronous mode (ie the function will not return 'till the transfer
is finished). File transfers are always of type BINARY.

4/ You can use FTPcommand() to execute FTP commands (eg
FTPcommand("CWD /home/mydir") to change directory on the server),
note that this function will return FALSE unless the server response
is a 200 series code. This should work fine for most FTP commands, 
otherwise you can use WriteStr() and ReadStr() to send commands & 
interpret the response yourself. Use LogOffServer() to disconnect
when done.

/////////////////////////////////////////////////////////////////////*/


#include "stdafx.h"
#include "FTPSOCKET.h"
#include "FTPClass.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//构造函数，变量初始化
CFTPclient::CFTPclient()
{
	m_pCtrlsokfile=NULL;
	m_pCtrlTxarch=NULL;
	m_pCtrlRxarch=NULL;
	m_Ctrlsok=NULL;
}


CFTPclient::~CFTPclient()
{
	CloseControlChannel();
}


//////////////////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////////////////


// 登录到服务器
BOOL CFTPclient::LogOnToServer(CString hostname,int hostport,CString username, CString password, CString acct, CString fwhost,CString fwusername, CString fwpassword,int fwport,int logontype) {
	int port,logonpoint=0;
	const int LO=-2, ER=-1;
	CString buf,temp;
	const int NUMLOGIN=9; // 支持9种不同的登录方式
	int logonseq[NUMLOGIN][100] = {
		// 下面的数组保存了针对不同防火墙的登录序列
		{0,LO,3, 1,LO,6, 2,LO,ER}, // 没有防火墙
		{3,6,3, 4,6,ER, 5,ER,9, 0,LO,12, 1,LO,15, 2,LO,ER}, // 主机名
		{3,6,3, 4,6,ER, 6,LO,9, 1,LO,12, 2,LO,ER}, // USER after logon
		{7,3,3, 0,LO,6, 1,LO,9, 2,LO,ER}, //proxy OPEN
		{3,6,3, 4,6,ER, 0,LO,9, 1,LO,12, 2,LO,ER}, // Transparent
		{6,LO,3, 1,LO,6, 2,LO,ER}, // USER with no logon
		{8,6,3, 4,6,ER, 0,LO,9, 1,LO,12, 2,LO,ER}, //USER fireID@remotehost
		{9,ER,3, 1,LO,6, 2,LO,ER}, //USER remoteID@remotehost fireID
		{10,LO,3, 11,LO,6, 2,LO,ER} // USER remoteID@fireID@remotehost
	};

	if(logontype<0||logontype>=NUMLOGIN) return FALSE; // illegal connect code
	if(!logontype) {
		temp=hostname;
		port=hostport;
	}
	else {
		temp=fwhost;
		port=fwport;
	}
	if(hostport!=21) hostname.Format(hostname+":%d",hostport); // 如果端口不是默认端口21，则设定端口
	if(!OpenControlChannel(temp,port)) return false;
	if(!FTPcommand("")) return FALSE; // 获得连接服务器初始化信息
	// 获得登录类型
	while(1) {
		switch(logonseq[logontype][logonpoint]) {
		case 0:
			temp="USER "+username;
			break;
		case 1:
			temp="PASS "+password;
			break;
		case 2:
			temp="ACCT "+acct;
			break;
		case 3:
			temp="USER "+fwusername;
			break;
		case 4:
			temp="PASS "+fwpassword;
			break;
		case 5:
			temp="SITE "+hostname;
			break;
		case 6:
			temp="USER "+username+"@"+hostname;
			break;
		case 7:
			temp="OPEN "+hostname;
			break;
		case 8:
			temp="USER "+fwusername+"@"+hostname;
			break;
		case 9:
			temp="USER "+username+"@"+hostname+" "+fwusername;
			break;
		case 10:
			temp="USER "+username+"@"+fwusername+"@"+hostname;
			break;
		case 11:
			temp="PASS "+password+"@"+fwpassword;
			break;
		}
		// 发出命令，获得响应
		if(!WriteStr(temp)) return FALSE;
		if(!ReadStr()) return FALSE;
		// 只有这些响应是合法的
		if(m_fc!=2&&m_fc!=3) return FALSE;
		logonpoint=logonseq[logontype][logonpoint+m_fc-1]; //get next command from array
		switch(logonpoint) {
		case ER: // 出现错误
			m_retmsg.LoadString(IDS_FTPMSG1);
			return FALSE;
		case LO: // L0表示成功登录
			return TRUE;
		}
	}
}


// 退出服务器
void CFTPclient::LogOffServer() {
	WriteStr("QUIT");
	CloseControlChannel();
}


// 发送命令到服务器
BOOL CFTPclient::FTPcommand(CString command) {
	if(command!=""&&!WriteStr(command)) return FALSE;
	if((!ReadStr())||(m_fc!=2)) return FALSE;
	return TRUE;
}


// 上载或者下载文件
BOOL CFTPclient::MoveFile(CString remotefile, CString localfile,BOOL pasv,BOOL get) {
	CString lhost,temp,rhost;
	UINT localsock,serversock,i,j;
	CFile datafile;
	CSocket sockSrvr;
	CAsyncSocket datachannel;
	int num,numread,numsent;
	const int BUFSIZE=4096;
	char cbuf[BUFSIZE];
	DWORD lpArgument=0;
	
	// 打开本地文件
	if(!datafile.Open(localfile,(get?CFile::modeWrite|CFile::modeCreate:CFile::modeRead))) {
		m_retmsg.LoadString(IDS_FTPMSG4);
		return FALSE;
	}
	if(!FTPcommand("TYPE I")) return FALSE; // 请求二进制传输
	if(pasv) { // 建立被动传输方式
		if(!FTPcommand("PASV")) return FALSE;
		// 分析出服务器传回的临时IP地址以及端口号
		if((i=m_retmsg.Find("("))==-1||(j=m_retmsg.Find(")"))==-1) return FALSE;
		temp=m_retmsg.Mid(i+1,(j-i)-1);
		i=temp.ReverseFind(',');
		serversock=atol(temp.Right(temp.GetLength()-(i+1))); //get ls byte of server socket
		temp=temp.Left(i);
		i=temp.ReverseFind(',');
		serversock+=256*atol(temp.Right(temp.GetLength()-(i+1))); // add ms byte to server socket
		rhost=temp.Left(i);
		while(1) { // 将逗号转化成点
			if((i=rhost.Find(","))==-1) break;
			rhost.SetAt(i,'.');
		}
	}
	else { // 设置主动的传输模式
		m_retmsg.LoadString(IDS_FTPMSG6);
		//获取本地的ip地址，发送到服务器
		if(!m_Ctrlsok->GetSockName(lhost,localsock)) return FALSE;;
		while(1) { // 将IP地址中的点转化成逗号
			if((i=lhost.Find("."))==-1) break;
			lhost.SetAt(i,',');
		}
		// 创建本地侦听进程
		if((!sockSrvr.Create(0,SOCK_STREAM,NULL))||(!sockSrvr.Listen())) return FALSE;
		if(!sockSrvr.GetSockName(temp,localsock)) return FALSE;// get the port that MFC chose
		// 将端口转化成2字节，然后加入到本地IP地址中
		lhost.Format(lhost+",%d,%d",localsock/256,localsock%256);
		if(!FTPcommand("PORT "+lhost)) return FALSE;// 发送端口到服务器
	}
	// 发送 RETR/STOR 命令到服务器
	if(!WriteStr((get?"RETR ":"STOR ")+remotefile)) return FALSE;
	if(pasv) {// 如果是PASV模式，则创建socket并初始化外部数据连接，即数据传输通道
		if(!datachannel.Create()) {
			m_retmsg.LoadString(IDS_FTPMSG6);
			return FALSE;
		}
		datachannel.Connect(rhost,serversock); // 试图异步连接服务器
	}
	if(!ReadStr()||m_fc!=1) return FALSE; // 获得服务器响应
	if(!pasv&&!sockSrvr.Accept(datachannel)) return FALSE; // 接收从服务器来的内部绑定数据
	// 连接成功，然后进行阻塞式数据传输
	if((!datachannel.AsyncSelect(0))||(!datachannel.IOCtl(FIONBIO,&lpArgument))) {
		m_retmsg.LoadString(IDS_FTPMSG6);
		return FALSE;
	}
	while(1) { // 开始传输数据
		TRY {
			if(get) {
				if(!(num=datachannel.Receive(cbuf,BUFSIZE,0))||num==SOCKET_ERROR) break; // (EOF||network error)
				else datafile.Write(cbuf,num);
			}
			else {
				if(!(numread=datafile.Read(cbuf,BUFSIZE))) break; //EOF
				if((numsent=datachannel.Send(cbuf,numread,0))==SOCKET_ERROR) break;
				// 如果发送出去的字节少于从文件读取的字节，则调整发送指针，以使得数据发送正确
				if(numread!=numsent) datafile.Seek(numsent-numread,CFile::current);
			}
		}
		CATCH (CException,e) {
			m_retmsg.LoadString(IDS_FTPMSG5);
			return FALSE;
		}
		END_CATCH
	}
	datachannel.Close();
	datafile.Close();
	if(!FTPcommand("")) return FALSE; // 检查从服务器发送的传输结果信息
	return TRUE; // 传输成功
}


// 通过控制通道向服务器发送命令
BOOL CFTPclient::WriteStr(CString outputstring) {
	m_retmsg.LoadString(IDS_FTPMSG6); // pre-load "network error" msg (in case there is one) #-)
	TRY {
		m_pCtrlTxarch->WriteString(outputstring+"\r\n");
		m_pCtrlTxarch->Flush();
	}
	CATCH(CException,e) {
		return FALSE;
	}
	END_CATCH
	return TRUE;
}


// 获得服务器的响应
BOOL CFTPclient::ReadStr() {
	int retcode;

	if(!ReadStr2()) return FALSE;

	if(m_retmsg.GetLength()<4||m_retmsg.GetAt(3)!='-') return TRUE;
	retcode=atol(m_retmsg);
	while(1) { //处理多行服务器响应

		if(m_retmsg.GetLength()>3&&(m_retmsg.GetAt(3)==' '&&atol(m_retmsg)==retcode)) return TRUE;
		if(!ReadStr2()) return FALSE;
	}
}



//////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////


// 从服务器控制通道获取一行响应
BOOL CFTPclient::ReadStr2() {
	TRY {
		if(!m_pCtrlRxarch->ReadString(m_retmsg)) {
			m_retmsg.LoadString(IDS_FTPMSG6);
			return FALSE;
		}
	}
	CATCH(CException,e) {
		m_retmsg.LoadString(IDS_FTPMSG6);
		return FALSE;
	}
	END_CATCH
	if(m_retmsg.GetLength()>0) m_fc=m_retmsg.GetAt(0)-48; // get 1st digit of the return code (indicates primary result)
	return TRUE;
}


// 打开控制通道
BOOL CFTPclient::OpenControlChannel(CString serverhost,int serverport) {
	m_retmsg.LoadString(IDS_FTPMSG2);
	if(!(m_Ctrlsok=new CSocket)) return FALSE;
	if(!(m_Ctrlsok->Create())) return FALSE;
	m_retmsg.LoadString(IDS_FTPMSG3);
	if(!(m_Ctrlsok->Connect(serverhost,serverport))) return FALSE;
	m_retmsg.LoadString(IDS_FTPMSG2);
	if(!(m_pCtrlsokfile=new CSocketFile(m_Ctrlsok))) return FALSE;
	if(!(m_pCtrlRxarch=new CArchive(m_pCtrlsokfile,CArchive::load))) return FALSE;
	if(!(m_pCtrlTxarch=new CArchive(m_pCtrlsokfile,CArchive::store))) return FALSE;
	return TRUE;
}


// 关闭控制通道
void CFTPclient::CloseControlChannel() {
	if(m_pCtrlTxarch) delete m_pCtrlTxarch;
	m_pCtrlTxarch=NULL;
	if(m_pCtrlRxarch) delete m_pCtrlRxarch;
	m_pCtrlRxarch=NULL;
	if(m_pCtrlsokfile) delete m_pCtrlsokfile;
	m_pCtrlsokfile=NULL;
	if(m_Ctrlsok) delete m_Ctrlsok;
	m_Ctrlsok=NULL;
	return;
}


//列出文件列表
BOOL CFTPclient::List()
{
	CString lhost,temp,rhost;
	UINT localsock,i;
	CFile datafile;
	CSocket sockSrvr;
	CAsyncSocket datachannel;
	int num, sum;
	const int BUFSIZE = 4096;
	DWORD lpArgument=0;
	
	m_buf.RemoveAll();
	m_buf.SetSize(BUFSIZE);
	if(!FTPcommand("TYPE I")) 
		return FALSE; // 请求二进制模式

	m_retmsg.LoadString(IDS_FTPMSG6);
	// 获取本地IP地址
	if(!m_Ctrlsok->GetSockName(lhost,localsock)) 
		return FALSE;;
	while(1) {
		// 将点转化成逗号
		if((i=lhost.Find("."))==-1) break;
		lhost.SetAt(i,',');
	}
	if((!sockSrvr.Create(0, SOCK_STREAM, NULL))
			|| (!sockSrvr.Listen()))
		return FALSE;
	if(!sockSrvr.GetSockName(temp,localsock))
		return FALSE;
	lhost.Format(lhost+",%d,%d", localsock / 256, localsock % 256);
	if(!FTPcommand("PORT "+lhost)) 
		return FALSE;

	if(!WriteStr("LIST") )
		return FALSE;
	if(!ReadStr())
		return FALSE; 
	if(!sockSrvr.Accept(datachannel)) 
		return FALSE; 
	if((!datachannel.AsyncSelect(0)) || 
		(!datachannel.IOCtl(FIONBIO,&lpArgument))) {
		m_retmsg.LoadString(IDS_FTPMSG6);
		return FALSE;
	}
	sum = 0;
	while(1) { // 获得数据
		TRY {
			if(!(num = datachannel.Receive(m_buf.GetData() + sum, BUFSIZE, 0)) 
				|| num == SOCKET_ERROR)
				break;
			TRACE("Received :%d\n", num);
			Sleep(0);
			sum += num;
			m_buf.SetSize(sum + BUFSIZE);
		}
		CATCH (CException,e) {
			m_retmsg.LoadString(IDS_FTPMSG5);
			return FALSE;
		}
		END_CATCH
	}
	datachannel.Close();
}


void CFTPclient::ProcessList()
{
}

BOOL CFTPclient::GetLine(int& ndx)
{
	m_strLine.Empty();
	int nBytes = m_buf.GetSize();
	BOOL bLine = FALSE;
	while ( bLine == FALSE && ndx < nBytes )
	{
		char ch = (char)(m_buf.GetAt( ndx ));
		switch( ch )
		{
		case '\n': // 行尾
			bLine = TRUE;
			break;
		default:   // 其他情况
			m_strLine += ch;
			break;
		}
		++ndx;
	}
	m_strLine = m_strLine.Left(m_strLine.GetLength() - 1);
	return bLine;
}

