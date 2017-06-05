// ConnectSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "ConnectSocket.h"
#include "DataSocket.h"

extern CFTPServer theServer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectSocket

CConnectSocket::CConnectSocket()
{
	m_bLoggedon = FALSE;
	m_bPassiveMode = FALSE;
	m_strRemoteHost = "";
	m_nRemotePort = -1;
	m_dwRestartOffset = 0;
	m_pDataSocket = NULL;//数据连接的socket要记得初始化
}

CConnectSocket::~CConnectSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CConnectSocket, CSocket)
//{{AFX_MSG_MAP(CConnectSocket)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

#define BUFFERSIZE 4096
/////////////////////////////////////////////////////////////////////////////
// CConnectSocket member functions

void CConnectSocket::OnReceive(int nErrorCode) 
{
	TCHAR buff[BUFFERSIZE];
	int nRead = Receive(buff,BUFFERSIZE);
	
	switch(nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if(GetLastError() != WSAEWOULDBLOCK)
		{
			TCHAR szError[256];
			wsprintf(szError,"OnReceive error:%d",GetLastError());
			AfxMessageBox(szError);
		}
		break;
	default:
		if(nRead != SOCKET_ERROR && nRead != 0)
		{
			//((CConnectThread*)AfxGetThread())->IncReceivedBytes(nRead);
			buff[nRead] = 0;//再接收到的数据后面加上了' ',0是' '的ASCII码值。接受的命令（有的带参数）最后两个字符是\r(13)\n(10).
			m_RxBuffer += CString(buff);
			GetRxLine();
		}
		break;
	}
	CSocket::OnReceive(nErrorCode);
}

//解析整个命令行
void CConnectSocket::GetRxLine()
{
	CString strTemp;
	int nIndex;
	while(!m_RxBuffer.IsEmpty())
	{
		//寻找换行符
		nIndex = m_RxBuffer.Find("\r\n");
		if(nIndex != -1)
		{
			strTemp = m_RxBuffer.Left(nIndex);
			m_RxBuffer = m_RxBuffer.Mid(nIndex+2);
			if(!strTemp.IsEmpty())
			{
				m_strCommands.AddTail(strTemp);
				//解析并执行命令
				ParseCommand();
			}
		}
		else 
			break;
	}
}

//FireStatusMessage通过调用这个函数在TracePage页面显示记录信息的。
//从缓冲区中获得命令
BOOL CConnectSocket::GetRxCommand(CString &strCommand, CString &strArguments)
{
	if(!m_strCommands.IsEmpty())
	{
		CString strBuff = m_strCommands.RemoveHead();
		int nIndex = strBuff.Find(" ");
		if(nIndex != -1)
		{
			CString strPassword = strBuff;
			strPassword.MakeUpper();
			//是密码不可见
			if(strPassword.Left(5) == "PASS")
			{
				for(int i=5; i<strPassword.GetLength(); i++)
				{
					strPassword.SetAt(i,'*');
				}
				FireStatusMessage(strPassword,1);
			}
			else
			{
				FireStatusMessage(strBuff,1);
			}
			strCommand = strBuff.Left(nIndex);
			strArguments = strBuff.Mid(nIndex+1);
		}
		else
		{
			FireStatusMessage(strBuff,1);
			strCommand = strBuff;
		}
		
		if(strCommand != "")
		{
			strCommand.MakeUpper();
			
			if(strCommand.Right(4) == "ABOR")
			{
				strCommand = "ABOR";
			}
			TRACE2("COMMAND: %s, ARGS: %s\n", strCommand, strArguments);
			return TRUE;
		}
	}
	return FALSE;
}


void CConnectSocket::ParseCommand()
{
	static CFTPCommand commandList[] = 
	{
		{TOK_ABOR,	"ABOR", FALSE,	"Abort transfer: ABOR"}, 
		{TOK_BYE,	"BYE",  FALSE,	"Logout or break the connection: BYE"},
		{TOK_CDUP,	"CDUP", FALSE,	"Change to parent directory: CDUP"},
		{TOK_CWD,	"CWD",	TRUE,	"Change working directory: CWD [directory-name]"},
		{TOK_DELE,	"DELE", TRUE ,	"Delete file: DELE file-name"},
		{TOK_DIR,	"DIR",  FALSE,	"Get directory listing: DIR [path-name]"},
		{TOK_HELP,	"HELP",  FALSE, "Show help: HELP [command]"},
		{TOK_LIST,	"LIST", FALSE,	"Get directory listing: LIST [path-name]"}, 
		{TOK_MKD,	"MKD",	TRUE,	"Make directory: MKD path-name"},
		{TOK_NOOP,	"NOOP", FALSE,	"Do nothing: NOOP"},
		{TOK_PASS,	"PASS", TRUE,	"Supply a user password: PASS password"},
		{TOK_PASV,	"PASV", FALSE,	"Set server in passive mode: PASV"},
		{TOK_PORT,	"PORT", TRUE,	"Specify the client port number: PORT a0,a1,a2,a3,a4,a5"},
		{TOK_PWD,	"PWD",	FALSE,	"Get current directory: PWD"},
		{TOK_QUIT,	"QUIT",  FALSE,	"Logout or break the connection: QUIT"},
		{TOK_REST,	"REST", TRUE,	"Set restart transfer marker: REST marker"},
		{TOK_RETR,	"RETR", TRUE,	"Get file: RETR file-name"},
		{TOK_RMD,	"RMD",	TRUE,	"Remove directory: RMD path-name"},
		{TOK_RNFR,	"RNFR", TRUE,	"Specify old path name of file to be renamed: RNFR file-name"},
		{TOK_RNTO,	"RNTO", TRUE,	"Specify new path name of file to be renamed: RNTO file-name"},
		{TOK_SIZE,	"SIZE", TRUE,	"Get filesize: SIZE file-name"},
		{TOK_STOR,	"STOR", TRUE,	"Store file: STOR file-name"},
		{TOK_SYST,	"SYST", FALSE,	"Get operating system type: SYST"},
		{TOK_TYPE,	"TYPE", TRUE,	"Set filetype: TYPE [A | I]"},
		{TOK_USER,	"USER", TRUE,	"Supply a username: USER username"},
		{TOK_ERROR,	"",		FALSE,  ""},
	};
	
	CString strCommand,strArguments;
	if(!GetRxCommand(strCommand,strArguments))
	{
		return;
	}
	int nCommand;
	
	for(nCommand = TOK_ABOR; nCommand < TOK_ERROR; nCommand++)
	{
		if(strCommand == commandList[nCommand].m_pszName)
		{
			if(commandList[nCommand].m_bHasArguments && (strArguments.IsEmpty()))
			{
				SendResponse("501 Syntax error:Invalid number of parameters.");
				//AfxMessageBox(_T("501 Syntax error:Invalid number of parameters."));
				return;
			}
			break;
		}
	}
	
	if(nCommand == TOK_ERROR)
	{
		SendResponse("501 Syntax error:Command not understood.");
		//AfxMessageBox(_T("501 Syntax error:Command not understood."));
		return;
	}
	
	if((nCommand != TOK_USER && nCommand != TOK_PASS) && !m_bLoggedon)
	{
		SendResponse("530 Please login with USER and PASS.");
		//AfxMessageBox(_T("530 Please login with USER and PASS."));
		return;
	}
	
	switch(nCommand)
	{
		//指定用户名
	case TOK_USER:
		{
			strArguments.MakeLower();
			m_bLoggedon = FALSE;
			m_strUserName = strArguments;
			
			CString strPeerAddress;
			UINT nPeerPort;
			GetPeerName(strPeerAddress,nPeerPort);
			
			// 通知FTP服务器一个新的用户已经连接
			CConnectThread *pThread = (CConnectThread*)m_pThread;
			((CFTPServer *)pThread->m_pWndServer)->m_pEventSink->OnFTPUserConnected(m_pThread->m_nThreadID, m_strUserName, strPeerAddress);
			SendResponse("331 User name ok,need password.");
			//AfxMessageBox(_T("331 User name ok,need password."));//test!!!
		}
		break;
		
		//指定密码
	case TOK_PASS:
		{
			if(m_bLoggedon)
			{
				SendResponse("503 Login with USER first.");
				//AfxMessageBox(_T("503 Login with USER first."));//test
			}
			else
			{
				CUserAccount useraccount;
				if(theServer.m_UserManager.GetUserAccount(m_strUserName,useraccount))
				{
					if(!useraccount.m_strPassword.Compare(strArguments) || useraccount.m_strPassword.IsEmpty())
					{
						m_strCurrentDir = "/";
						m_bLoggedon = TRUE;
						SendResponse("230 User sucessfully logged in.");
						break;
					}
				}
				SendResponse("530 Not logged in,user or password incorrect!");
			}
			break;
			
			// 改变传输模式
	case TOK_TYPE:
		{
			SendResponse("200 Type set to %s", strArguments);
		}
		break;
		
		//打印当前路径
	case TOK_PWD:
		{
			SendResponse("257 \"%s\" is current directory.", m_strCurrentDir);
		}
		break;
		
		//重新启动传输
	case TOK_REST:
		{
			if(!IsNumeric(strArguments.GetBuffer(strArguments.GetLength())))
			{
				strArguments.ReleaseBuffer();
				SendResponse("501 Invalid numeric!");
			}
			else
			{
				strArguments.ReleaseBuffer();
				m_dwRestartOffset = atol(strArguments);
				SendResponse("350 Restarting at %d.", m_dwRestartOffset);
			}
		}
		break;
		
		//改变到被动模式
	case TOK_PASV:
		{ 
			//删除当前的数据连接
			DestroyDataConnection();
			//创建新的数据套接字连接
			m_pDataSocket = new CDataSocket(this,-1);
			
			if(!m_pDataSocket->Create())
			{
				DestroyDataConnection();
				SendResponse("421 Failed to create socket.");
				break;
			}
			//开始侦听
			m_pDataSocket->Listen();
			m_pDataSocket->AsyncSelect();
			
			CString strIP,strTmp;
			UINT nPort;
			//获取我们的IP地址
			GetSockName(strIP,nPort);//strIP是本机的IP地址，nPort是21
			//获取新的监听套接字的端口号
			m_pDataSocket->GetSockName(strTmp,nPort);//strTmp是0.0.0.0，nPort是机器自动分配的
			//原因在于这只是个监听socket还没有客户来连接，如果客户来连接了的话，就可以获得本机的IP地址了。
			//因此这里会有这么奇怪的两句代码。应该是这样的，有待验证，自己的理解嘿嘿！！！
			//替换"."
			strIP.Replace(".",",");
			//通知客户端要连接的IP地址和端口
			SendResponse("227 Entering Passive Mode(%s,%d,%d).",strIP,nPort/256,nPort%256);
			m_bPassiveMode = TRUE;
			break;
		}
		
		//列出当前路径
	case TOK_LIST:
	case TOK_DIR:
		{
			//如果不是被动模式，我们先需要PORT命令指定端口
			if(!m_bPassiveMode && (m_strRemoteHost == "" || m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				//AfxMessageBox(_T("503 Bad sequence of commands."));
			}
			else
			{
				//如果用户没有指定路径，用当前路径
				if(strArguments == "")
				{
					strArguments = m_strCurrentDir;
				}
				
				else
				{
					//检查参数是文件名还是路径
					CString strResult;
					int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_LIST,strResult);
					if(nResult == 0)
					{
						strArguments = strResult;
					}
				}
				
				CString strListing;
				int nResult = theServer.m_UserManager.GetDirectoryList(m_strUserName,strArguments,strListing);
				switch(nResult)
				{
				case 1:
					SendResponse("550 Permission denied.");
					break;
				case 2:
					SendResponse("550 Directory not found.");
					break;
				default:
					//创建套接字连接获得路径列表
					if(!CreateDataConnection(0,strListing))
					{
						DestroyDataConnection();
					}
					break;
				}
			}
		}
		break;
		
		// 改变上层目录
	case TOK_CDUP:
		strArguments = "..";
		
		//改变工作目录
	case TOK_CWD:
		{
			//改变到指定目录
			int nResult = theServer.m_UserManager.ChangeDirectory(m_strUserName,m_strCurrentDir,strArguments);
			
			switch(nResult)
			{
			case 0:
				SendResponse("250 CWD command successful.\"%s\" is current directory.",m_strCurrentDir);
				break;
			case 1:
				SendResponse("550 CWD command failed.\"%s\":Permission denied.",strArguments);
				break;
			default:
				SendResponse("550 CWD command failed.\"%s\":Directory not found.",strArguments);
				break;
			}
		}
		break;
		
    // 指定IP地址和端口
	case TOK_PORT:
		{
			CString strSub;
			int nCount=0;
			
			while (AfxExtractSubString(strSub, strArguments, nCount++, ','))
			{
				switch(nCount)
				{
				case 1:	
					m_strRemoteHost = strSub;
					m_strRemoteHost += ".";
					break;
				case 2:	
					m_strRemoteHost += strSub;
					m_strRemoteHost += ".";
					break;
				case 3:	
					m_strRemoteHost += strSub;
					m_strRemoteHost += ".";
					break;
				case 4:	
					m_strRemoteHost += strSub;
					break;
				case 5:	
					m_nRemotePort = 256*atoi(strSub);
					break;
				case 6:	
					m_nRemotePort += atoi(strSub);
					break;
				}
			}
			m_bPassiveMode = FALSE;
			SendResponse("200 Port command successful.");
			break;
		}
		
		//获得文件，下载文件
	case TOK_RETR:
		{
			//如果不是被动模式，我们先需要PORT命令指定端口
			if(!m_bPassiveMode && (m_strRemoteHost == "" || m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_DOWNLOAD,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 File not found.");
				break;
			default:
				//为文件传输创建一个套接字
				if(!CreateDataConnection(1,strResult))
				{
					DestroyDataConnection();
				}
				break;
			}
			break;
		}
		
		//客户端上传文件
	case TOK_STOR:
		{
			//如果不是被动模式，我们先需要PORT命令指定端口
			if(!m_bPassiveMode && (m_strRemoteHost == "" || m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_UPLOAD,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 Filename invalid.");
				break;
			default:
				//为文件传输创建一个套接字
				if(!CreateDataConnection(2,strResult))
				{
					DestroyDataConnection();
				}
				break;
			}
		}
		break;
		
		//删除文件
		//删除文件的命令不需要在发送PASV命令是由于这个不需要向客户发送太多数据，只需要发送一个响应就行了，不需要再开一个数据
		//连接的SOCKET.
	case TOK_DELE:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_DELETE,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 File not found.");
				break;
			default:
				//删除文件
				if(!DeleteFile(strResult))
				{
					SendResponse("450 Internal error deleting the file:\"%s\".",strArguments);
				}
				else 
				{
					SendResponse("250 File \"%s\" wsa deleted successfully.",strArguments);
				}
				break;
			}
		}
		break;
		
		//删除目录
		//多层的目录删除有问题，目录不空也不能删除，直接删除一个空的目录是没有问题的,但是里面有文件或目录是不能删除的。可能是协议的问题，也可能是路径有问题.
	case TOK_RMD:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.CheckDirectory(m_strUserName,strArguments,m_strCurrentDir,FTP_DELETE,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 Directory not found.");
				break;
			default:
				//删除目录
				if(!RemoveDirectory(strResult))
				{
					if(GetLastError() == ERROR_DIR_NOT_EMPTY)
					{
						SendResponse("550 Directory not empty.");
					}
					else
					{
						SendResponse("450 Internal error deleting the directory.");
					}
				}
				else
				{
					SendResponse("250 Directory deleted successfully.");
				}
				break;
			}
		}
		break;
		
		//创建目录
	case TOK_MKD:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.CheckDirectory(m_strUserName,strArguments,m_strCurrentDir,FTP_CREATE_DIR,strResult);
			//注意安全控制，如果我在客户端发送来的命令是(小白/小乐)可以正确创建目录
			//如果我发送来的是(小白\小乐)就会出现错误，这个需要在服务器端进行转换
			//而CheckDirectory的功能正是如此。

			switch(nResult)
			{
			case 0:
				SendResponse("550 Directory already exists.");
				break;
			case 1:
				SendResponse("550 Can't create directory.Permission denied.");
				break;
			default:
				//创建目录
				if(!MakeSureDirectoryPathExists(strResult))
				{
					SendResponse("450 Internal error creating the directory.");
				}
				else
				{
					SendResponse("250 Directory created successfully.");
				}
				break;
			}
		}
		break;
		
		//重命名文件名或者路径
	case TOK_RNFR:
		{
			//用户端重命名文件
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);

			if (nResult == 0)
			{
				m_strRenameFile = strResult;
				m_bRenameFile = TRUE;
				SendResponse("350 File exists, ready for destination name.");
				break;
			}
			else
			{
				// 用户端重命名目录
				nResult = theServer.m_UserManager.CheckDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
	
				switch(nResult)
				{
				case 0:
					m_strRenameFile = strResult;
					m_bRenameFile = FALSE;
					SendResponse("350 Directory exists, ready for destination name.");
					break;
				case 1:
					SendResponse("550 Permission denied.");
					break;
				default: 
					SendResponse("550 File/directory not found.");
					break;
				}
			}	
		}
		break;
		
		// 重命名文件名或目录
	case TOK_RNTO:
		{
			if (m_strRenameFile.IsEmpty())
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			if (m_bRenameFile)
			{
				CString strResult;
				// 检查目标文件名
				int nResult = theServer.m_UserManager.CheckFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);

				switch(nResult)
				{
				case 0:
					SendResponse("550 File already exists.");
					break;
				case 1:
					SendResponse("550 Permission denied.");
					break;
				default:
					// 重命名文件
					if (!MoveFile(m_strRenameFile, strResult))
					{
						SendResponse("450 Internal error renaming the file: \"%s\".", m_strRenameFile);
					}
					else
					{
						SendResponse("250 File \"%s\" renamed successfully.", m_strRenameFile);
					}
					break;
				}
			}
			else
			{
				CString strResult;
				// 检查目标路径名
				int nResult = theServer.m_UserManager.CheckDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
	
				switch(nResult)
				{
				case 0:
					SendResponse("550 Directory already exists.");
					break;
				case 1:
					SendResponse("550 Permission denied.");
					break;
				case 3:
					SendResponse("550 Directory invalid.");
					break;
				default:
					// 重命名路径
					if (!MoveFile(m_strRenameFile, strResult))
					{
						SendResponse("450 Internal error renaming the directory: \"%s\".", m_strRenameFile);
					}
					else
					{
						SendResponse("250 Directory \"%s\" renamed successfully.", m_strRenameFile);
					}
					break;
				}
			}		
		}
		break;
		
		//取消传输
	case TOK_ABOR:
		{
			if(m_pDataSocket)
			{
				if(m_pDataSocket->GetStatus() != XFERMODE_IDLE)//这个好像有点多余？？！！！
				{
					SendResponse("426 Data connection closed.");
				}
				//销毁数据连接
				m_pThread->PostThreadMessage(WM_THREADMSG,0,0);
			}
			SendResponse("226 ABOR command successful.");
			break;
		}
		
		//获得系统信息
	case TOK_SYST:
		{
			SendResponse("215 Windows FTPServer by SiXing Bai.");
			break;
		}
		
		//关闭连接
	case TOK_QUIT:
	case TOK_BYE:
		{
			//发送再见消息给客户机
			CConnectThread *pThread = (CConnectThread*)m_pThread;
			SendResponse("220 %s",((CFTPServer*)pThread->m_pWndServer)->GetGoodbyeMessage());
			SendResponse("220 GoodBye!");//test!!
			Close();
			//通知线程已经结束关闭连接
			m_pThread->PostThreadMessage(WM_THREADMSG,1,0);
			break;
		}
		
		//显示帮助信息
	case TOK_HELP:
		{
			//如果客户端不指定命令名显示所有命令
			if(strArguments == "")
			{
				CString strResponse = "214-The following commands are recognized:\r\n";
				//在命令列表中查找命令
				for(int i=TOK_ABOR; i<TOK_ERROR; i++)
				{
					strResponse += commandList[i].m_pszName;
					strResponse += "\r\n";
				}
				strResponse += "214 HELP command successful.";
				SendResponse(strResponse);
			}
			else
			{
				int nHelpCmd;
				//在命令列表中查找命令
				for(nHelpCmd = TOK_ABOR; nHelpCmd<TOK_ERROR; nHelpCmd++)
				{
					//判断是否找到命令
					if(strArguments.CompareNoCase(commandList[nHelpCmd].m_pszName)==0)
					{
						break;
					}
				}
				if(nHelpCmd != TOK_ERROR)
				{
					//显示命令帮助信息
					SendResponse("214 %s",commandList[nHelpCmd].m_pszDescription);
				}
				else
				{
					SendResponse("501 Unknown command %s",strArguments);
				}
			}
		}
		break;
		
		//空命令
	case TOK_NOOP:
		{
			SendResponse("220 ok");
		}
		break;
		
	default:
		SendResponse("502 Command not implemented - Try HELP.");
		break;
	}
	
}
}//这里少了一个括号，导致下面的函数在编译时报出局部函数定义非法的异常

BOOL CConnectSocket::CreateDataConnection(int nTransferType, LPCTSTR lpszData)
{
	if(!m_bPassiveMode)
	{
		m_pDataSocket = new CDataSocket(this,nTransferType);
		if(m_pDataSocket->Create())
		{
			m_pDataSocket->AsyncSelect();
			m_pDataSocket->SetRestartOffset(m_dwRestartOffset);
			m_pDataSocket->SetData(lpszData);
			
			if(m_pDataSocket->Connect(m_strRemoteHost,m_nRemotePort))
			{
				if(GetLastError() != WSAEWOULDBLOCK)
				{
					SendResponse("425 Can't open data connection.");
					//AfxMessageBox(_T("425 Can't open data connection."));
					return FALSE;
				}
			}
			
			switch(nTransferType)
			{
			case 0:
				SendResponse("150 Opening ASCII mode data connection for directory list.");
				//AfxMessageBox(_T("150 Opening ASCII mode data connection for directory list."));
				break;
			case 1:
			case 2:
				SendResponse("150 Opening BINARY mode data connection for file transfer.");
				//AfxMessageBox(_T("150 Opening BINARY mode data connection for file transfer."));
				break;
			}
		}
		else
		{
			//SendResponse("421 Failed to create data connection socket.");
			AfxMessageBox(_T("421 Failed to create data connection socket."));
			return FALSE;
		}
	}
	else
	{
		m_pDataSocket->SetRestartOffset(m_dwRestartOffset);
		m_pDataSocket->SetData(lpszData);
		m_pDataSocket->SetTransferType(nTransferType,TRUE);
		//这3句执行完才会去执行m_pDataSocket的OnAccept()函数，所以在SetTransferType（）函数
		//中要判断在被动模式（服务器监听客户连接模式）下，是否客户来连接了，如果不判断的话，
		//OnAccept()将被跳过执行，即没有客户端来连接，这也是开始一直无法把数据发出去的原因!!
	}
	return TRUE;
}

void CConnectSocket::DestroyDataConnection()
{
	if (!m_pDataSocket)
		return;
	
	delete m_pDataSocket;
	
	// reset transfer status
	m_pDataSocket = NULL;
	m_strRemoteHost = "";
	m_nRemotePort = -1;
	m_dwRestartOffset = 0;
	m_bPassiveMode = FALSE;
}


BOOL CConnectSocket::SendResponse(LPCTSTR pstrFormat, ...)
{
	CString str;
	
	// 格式化参数
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);
	
	//判断连接是否在活动状态
	/*if (HasConnectionDropped())
	{
	FireStatusMessage("Could not send reply, disconnected.", 2);	
	
	  Close();
	  // 通知线程已经停止，关闭连接
	  m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
	  return FALSE;
}*/
	
	int nBytes = CSocket::Send(str + "\r\n", str.GetLength()+2);
	if (nBytes == SOCKET_ERROR)
	{
		Close();
		FireStatusMessage("Could not send reply, disconnected.", 2);	
		
		//通知线程连接已经关闭
		m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
		
		return FALSE;
	}
	
	FireStatusMessage(str, 2);
	
   	//((CConnectThread *)AfxGetThread())->IncSentBytes(nBytes);
	return TRUE;
}

//Fire status message.	
//通过在SendResponse函数中调用
//FireStatusMessage(str, 2);
//来向文件中写入日志和在日志窗口中显示服务器和用户交互过程
void CConnectSocket::FireStatusMessage(LPCTSTR lpszStatus, int nType)
{
	CConnectThread *pThread = (CConnectThread *)m_pThread;
	((CFTPServer *)pThread->m_pWndServer)->AddTraceLine(nType, "[%d] %s", m_pThread->m_nThreadID, lpszStatus);
}

/********************************************************************/
/*																	*/
/* Function name : OnClose											*/		
/* Description   : Send WM_QUIT message to the thread containing	*/
/*				   the socket to shutdown once the connection is	*/
/*                 closed.											*/
/*																	*/
/********************************************************************/
void CConnectSocket::OnClose(int nErrorCode) 
{
	Close();
	// destroy connection
	m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
	TRACE("CConnectSocket() OnClose()\n");

	CSocket::OnClose(nErrorCode);
}