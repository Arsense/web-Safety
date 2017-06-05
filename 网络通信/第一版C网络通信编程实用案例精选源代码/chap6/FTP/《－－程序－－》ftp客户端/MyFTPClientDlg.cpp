// FTPSOCKETDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyFTPClient.h"
#include "MyFTPClientDlg.h"
#include "ListItem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

char szLocDirectory[MAX_PATH];
char szFtpDirectory[MAX_PATH];



char szAppName[256];
char szFtpName[256];
char szFtpUser[20];
char szFtpPassword[20];



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFTPClientDlg dialog

CMyFTPClientDlg::CMyFTPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyFTPClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyFTPClientDlg)
	m_PWD = _T("");
	m_Server = _T("");
	m_UID = _T("");
	m_Message = _T("");
	m_Port = 0;
	m_PASV = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyFTPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFTPClientDlg)
	DDX_Control(pDX, IDC_LIST_SERVERFILE, m_ctrlServerFile);
	DDX_Control(pDX, IDC_LIST_LOCALFILE, m_ctrlLocalFile);
	DDX_Control(pDX, IDC_COMBOBOXEX_SERVER, m_ctrlServer);
	DDX_Control(pDX, IDC_COMBOBOXEX_LOCAL, m_ctrlLocal);
	DDX_Text(pDX, IDC_EDIT_PWD, m_PWD);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_Server);
	DDX_Text(pDX, IDC_EDIT_UID, m_UID);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_Message);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Check(pDX, IDC_CHECK_PASV, m_PASV);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyFTPClientDlg, CDialog)
	//{{AFX_MSG_MAP(CMyFTPClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOCALFILE, OnDblclkListLocalfile)
	ON_CBN_SELCHANGE(IDC_COMBOBOXEX_LOCAL, OnSelchangeComboboxexLocal)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SERVERFILE, OnDblclkListServerfile)
//}}AFX_MSG_MAP

	ON_MESSAGE(WM_SETFILE,CMyFTPClientDlg::OnInsertLocFile)
	ON_MESSAGE(WM_SETREDRAWFLAG,OnSetLocRedrawFlag)
	ON_MESSAGE(WM_SETDIRECTORYDISPLAY,OnSetLocDirectoryDisplay)
	ON_MESSAGE(WM_REDISPLAYFILE,OnReDisplayLocFile)
	ON_MESSAGE(WM_SETDIR,OnSetLocDir)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFTPClientDlg message handlers

BOOL CMyFTPClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//创建ImageList
    HIMAGELIST hImageList;
	//定义文件信息变量
	SHFILEINFO shFi;
	//获取文件图标信息
	hImageList = (HIMAGELIST)SHGetFileInfo("C:\\",0,&shFi,sizeof(shFi),
		SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	//将文件图象列表存入m_ctImageList
	m_ctImageList.m_hImageList = hImageList;
	

	// TODO: Add extra initialization here
	//设定客户端文件显示列表框的栏标题
	SetFileColumns(&m_ctrlLocalFile);
	SetServerFileColumns(&m_ctrlServerFile);

	SetImageList(&m_ctrlLocalFile,&m_ctrlLocal);
	SetImageList(&m_ctrlServerFile,NULL);

	m_ctrlLocalFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);



	//m_ctrlLocalFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//获取本地驱动器信息 

	GetLocDir();

	int i=m_ctrlLocal.FindStringExact(-1,"C:\\");

	m_ctrlLocal.SetCurSel(i);
	::SetCurrentDirectory("C:\\");
	GetLocCurrentDirectory();
	BeginFindLocFile();	
	//初始化，为了调试，设定初试值
	char host[100];
	gethostname(host,100);
	hostent * ht=gethostbyname(host);
	char * temp=ht->h_addr_list[0];
	struct   in_addr in;  
	memcpy((void*)&in,temp,sizeof(in));
	m_Server=inet_ntoa(in);
	m_Port=21;
	m_UID="anonymouse";
	m_PWD="anonymouse";
	UpdateData(false);


	return TRUE;  // 
}

void CMyFTPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CMyFTPClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyFTPClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyFTPClientDlg::OnButtonConnect() 
{
	int ndx=0;	
	CString str;
	//将控件中的数据更新到对应的变量中
	UpdateData(TRUE);
	m_Message=m_Message+"连接服务器:"+m_Server+"\r\n";
	m_Message=m_Message+"登录用户名:"+m_UID+"\r\n";
	UpdateData(false);

	if(!ftp.LogOnToServer(m_Server,m_Port,m_UID,m_PWD,"","","","",1080,0)) 
	{
		m_Message=m_Message+m_UID+"登录失败。"+"\r\n";
		UpdateData(false);

		return;
	}
	else
	{
		ftp.List ();
		while(ftp.GetLine(ndx))
		{
			AddLine(ftp.m_strLine );
			str="/";
			strcpy(szFtpDirectory,str);
			
			SetServerDir(str);
		}
		ftp.FTPcommand(""); 
	}
}



void CMyFTPClientDlg::SetFileColumns(CListCtrl *pListCtrl)
{
   	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[]={
		_T("文件名"),LVCFMT_LEFT,
		_T("日期"),LVCFMT_CENTER,
		_T("字节数"),LVCFMT_CENTER
	};
	for(int i=0,j=120;i<sizeof(columns)/sizeof(columns[0]);i++,j-=10)
		pListCtrl->InsertColumn(i,columns[i].pszText,columns[i].uiFormat,j);
}


void CMyFTPClientDlg::SetServerFileColumns(CListCtrl *pListCtrl)
{
   	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[]={
		_T("文件名"),LVCFMT_LEFT,
		_T("日期"),LVCFMT_CENTER,
		_T("权限"),LVCFMT_CENTER
	};
	for(int i=0,j=120;i<sizeof(columns)/sizeof(columns[0]);i++,j-=10)
		pListCtrl->InsertColumn(i,columns[i].pszText,columns[i].uiFormat,j);
}


BOOL CMyFTPClientDlg::SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo)
{
	pListCtrl->SetImageList(&m_ctImageList,LVSIL_SMALL);
	if(!pCombo)
		return TRUE;
	pCombo->SetImageList(&m_ctImageList);
	return TRUE;
}

void CMyFTPClientDlg::GetLocDir()
{
	int iIcon,iIconSel;
	char lpBuffer[MAX_PATH];
	char* ptr;
	CString str;
	m_iDirCount=0;
	::GetLogicalDriveStrings(MAX_PATH,lpBuffer);
	ptr=lpBuffer;
	while(*ptr)
	{
		m_iDirCount++;
		str=ptr;
		GetFileIcon(str,&iIcon,&iIconSel);
		InsertLocDir(str,iIcon,iIconSel);
		ptr+=strlen(ptr)+1;
	}
}


int CMyFTPClientDlg::InsertLocDir(CString &str, int iIcon, int iIconSel)
{
   int nItem=m_ctrlLocal.GetCount();
	COMBOBOXEXITEM Combo;
	Combo.mask=CBEIF_IMAGE|CBEIF_TEXT|CBEIF_SELECTEDIMAGE;
	Combo.iImage=iIcon;
	Combo.iSelectedImage=iIconSel;
	Combo.pszText=(LPTSTR)(LPCTSTR)str;
	Combo.iItem=nItem;
	m_ctrlLocal.InsertItem(&Combo);
	return nItem;
}

void CMyFTPClientDlg::GetFileIcon(CString& fileName,int* iIcon,int* iIconSel)
{
	CString str=fileName;
	if(str.Right(1)!="\\")
		str+="\\";
	SHFILEINFO shFi;
	if(!SHGetFileInfo(str,0,&shFi,sizeof(shFi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME))
	{
		AfxMessageBox("111");
		return;
	}
	*iIcon=shFi.iIcon;
	DestroyIcon(shFi.hIcon);
	if(iIconSel)
	{
		if(!SHGetFileInfo(str,0,&shFi,sizeof(shFi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON|SHGFI_DISPLAYNAME))
		{
			AfxMessageBox("222");
			return;
		}
		*iIconSel=shFi.iIcon;
		DestroyIcon(shFi.hIcon);
	}
	return ;
}

void CMyFTPClientDlg::GetLocCurrentDirectory()
{
    CString str;
	m_ctrlLocal.GetWindowText(str);
	strcpy(szLocDirectory,str);
}

BOOL CMyFTPClientDlg::BeginFindLocFile()
{
	m_ctrlLocalFile.DeleteAllItems();

	FindLocFile();
		return TRUE;
}

void  CMyFTPClientDlg::FindLocFile()
{
	FILE_FTP_INFO* pInfo=new FILE_FTP_INFO;
	CFileFind fileFind;
	UINT i=0;
	CTime time;
	BOOL bContinue;
	::SetCurrentDirectory(szLocDirectory);
	::PostMessage(this->m_hWnd ,WM_SETDIRECTORYDISPLAY,0,0);
	bContinue=fileFind.FindFile("*.*");
	CString str;
	while(bContinue)
	{
		bContinue=fileFind.FindNextFile();
		if(fileFind.IsHidden())continue;
		str=fileFind.GetFileName();
		strcpy(pInfo->szFileName,str);
		fileFind.GetLastWriteTime(time);
		str=time.Format("%x");
		strcpy(pInfo->szFileDate,str);
		if(fileFind.IsDirectory())
		{
			strcpy(pInfo->szFileSize,"");
			pInfo->nType=DIRECTORYICON;
			::SendMessage(this->m_hWnd,WM_SETFILE,(WPARAM)pInfo,
				(LPARAM)LOCFILE);
		}
		else
		{
			int i=fileFind.GetLength();
			if(i>1024)
			{
				str.Format("%d",i/1024);
				str+="KB";
			}
			else
				str.Format("%d",i);
			strcpy(pInfo->szFileSize,str);
			pInfo->nType=FILEICON;
			::SendMessage(this->m_hWnd,WM_SETFILE,(WPARAM)pInfo,
				(LPARAM)LOCFILE);
		}
	}
	::SendMessage(this->m_hWnd,WM_SETREDRAWFLAG,0,0);
	delete pInfo;
	fileFind.Close();
	//return 0L;
}


LRESULT CMyFTPClientDlg::OnSetLocRedrawFlag(WPARAM wParam,LPARAM lParam)
{
	if(m_bHaveDotFlag)
	{
		CString str="..";
		int iIcon=DIRECTORYICON;
		m_ctrlLocalFile.InsertItem(0,str,iIcon);
		m_ctrlLocalFile.SetItemData(0,DIRECTORYICON);
		m_bHaveDotFlag=FALSE;
	}
	m_ctrlLocalFile.Invalidate();
	m_ctrlLocalFile.SetRedraw();
	return 0L;	
}
LRESULT CMyFTPClientDlg::OnSetLocDirectoryDisplay(WPARAM wParam,LPARAM lParam)
{
	CString str;
	str=szLocDirectory;
	m_ctrlLocal.SetWindowText(str);
	m_ctrlLocal.Invalidate();
	return 0L;
}
LRESULT CMyFTPClientDlg::OnReDisplayLocFile(WPARAM wParam,LPARAM lParam)
{
	CString str=(LPCTSTR)wParam;
	m_ctrlLocal.SetWindowText(str);
	m_ctrlLocal.Invalidate();
	GetLocCurrentDirectory();
	if(!BeginFindLocFile())
		AfxMessageBox("建立网络线程失败");
	return 0L;
}
LRESULT CMyFTPClientDlg::OnInsertLocFile(WPARAM wParam,LPARAM lParam)
{
	FILE_FTP_INFO* pInfo=(FILE_FTP_INFO*)wParam;
	CString str=pInfo->szFileName;
	if(str=="."||str=="..")
	{
		m_bHaveDotFlag=TRUE;
		return 0L;
	}
	OnInsertFile(&m_ctrlLocalFile,pInfo,lParam);
	return 0L;
}


LRESULT CMyFTPClientDlg::OnSetLocDir(WPARAM wParam,LPARAM lParam)
{
	CString str=(LPCTSTR)lParam;
	m_ctrlLocal.SetWindowText(str);
	m_ctrlLocalFile.SetRedraw(FALSE);
	m_ctrlLocalFile.DeleteAllItems();
	m_ctrlLocalFile.SetRedraw();
	return 0L;

}

  
void CMyFTPClientDlg::OnInsertFile(CListCtrl* pListCtrl,LPVOID pIn,LPARAM lParam)
{
	FILE_FTP_INFO*pInfo=(FILE_FTP_INFO*)pIn;
	LVITEM	lvIt;
	int iIcon,nItem;
	if(pInfo->nType==DIRECTORYICON)
		nItem=0;
	else
		nItem=pListCtrl->GetItemCount();
	CString fileName=pInfo->szFileName;;
	if((int)lParam==LOCFILE)
		GetFileIcon(fileName,&iIcon);
	else
		iIcon=pInfo->nType;
	lvIt.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	lvIt.iImage=iIcon;
	lvIt.lParam=pInfo->nType;
	lvIt.pszText=pInfo->szFileName;
	lvIt.iSubItem=0;
	lvIt.iItem=nItem;
	int iPos=pListCtrl->InsertItem(&lvIt);
	lvIt.mask=LVIF_TEXT;
	lvIt.iItem=iPos;
	lvIt.pszText=pInfo->szFileDate;
	lvIt.iSubItem=1;
	pListCtrl->SetItem(&lvIt);
	//m_ctrlLocalFile.SetItem(&lvIt);

	lvIt.pszText=pInfo->szFileSize;
	lvIt.iSubItem=2;
	pListCtrl->SetItem(&lvIt);
	//IsFree=TRUE;
}



void CMyFTPClientDlg::OnDblclkListLocalfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NMLVDISPINFO* pLocInfo=(NMLVDISPINFO*)pNMHDR;
	CString str,str1,filename,buf;
	int iIcon,iIconSel;
	str.Format("%d",pLocInfo->item);
	if(str=="-1")
		return;
	int nItem=atoi(str);
	DWORD dwFlag=m_ctrlLocalFile.GetItemData(nItem);
	if(dwFlag==FILEICON)
	{
		//OnLocFileOpen();
		str=szLocDirectory;
		m_ctrlLocalFile.GetItemText(nItem,0,szLocDirectory,MAX_PATH);
		str1=szLocDirectory;
		filename=str1;
		if (str.Right(1)!="\\")
			str1=str+"\\"+str1;
		else
			str1=str+str1;
		strcpy(szLocDirectory,str);
		MessageBox("你要上载文件："+str1);
		UpdateData(true);
		UpLoadFile(str1,filename,m_PASV,false);
		return;
	}
	::GetCurrentDirectory(MAX_PATH,szLocDirectory);
	str=szLocDirectory;
	m_ctrlLocalFile.GetItemText(nItem,0,szLocDirectory,MAX_PATH);
	buf=szLocDirectory;
	if(str.Right(1)!="\\")
		str+="\\";
	str+=szLocDirectory;

	GetFileIcon(buf,&iIcon,&iIconSel);
	int i=InsertLocDir(str,iIcon,iIconSel);
	m_ctrlLocal.SetCurSel(i);
	::SetCurrentDirectory(str);
	::GetCurrentDirectory(MAX_PATH,szLocDirectory);
	BeginFindLocFile();	
		
	*pResult = 0;
}



void CMyFTPClientDlg::OnSelchangeComboboxexLocal() 
{
	CString str;
    char lpBuffer[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,lpBuffer);
	int i=m_ctrlLocal.GetCurSel();
	m_ctrlLocal.GetLBText(i,(LPTSTR)(LPCTSTR)str);
	i=0;
	while(1)
	{
		if(!SetCurrentDirectory(str))
		{
			i++;
			if(i<2)
			{
				AfxMessageBox("请插入磁盘再试");
				continue;
			}
			str=lpBuffer;
			if((i=m_ctrlLocal.FindStringExact(-1,str))!=-1)
				m_ctrlLocal.SetCurSel(i);
			else
				m_ctrlLocal.SetWindowText(str);
		}
		else
		{
			::SetCurrentDirectory(str);
			GetLocCurrentDirectory();
			BeginFindLocFile();
		}
		break;
		
	}	
}





void CMyFTPClientDlg::SetServerDir (CString& str)
{
	m_ctrlServer.SetWindowText(str);
	m_ctrlServer.Invalidate();
}


void CMyFTPClientDlg::AddLine(CString strLine)
{
	CListItem* pItem = new CListItem();
	if(!pItem->ParseLine(strLine)){
		delete pItem;
		return;
	}

	AddItem(pItem);
}


BOOL CMyFTPClientDlg::AddItem(CListItem *pItem)
{
	//CListCtrl& list= GetListCtrl();
	// look for a match on the file name....
//	LVITEM lvfi;
	LV_ITEM lvi;
	lvi.lParam = (LPARAM)pItem;
	lvi.iImage = pItem->GetImage();
	lvi.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;

	int ndx = m_ctrlServerFile.GetItemCount ();
	lvi.pszText = pItem->m_strName.GetBuffer(0);
	pItem->m_strName.ReleaseBuffer();
	lvi.iItem = ndx;
	lvi.iSubItem = 0;

	ndx=m_ctrlServerFile.InsertItem( &lvi );
	lvi.mask =LVIF_TEXT;
	lvi.pszText = pItem->m_strDate.GetBuffer(0);
	pItem->m_strDate.ReleaseBuffer();
	lvi.iItem = ndx;
	lvi.iSubItem = 1;
	m_ctrlServerFile.SetItem( &lvi );
	lvi.pszText = pItem->m_strSec.GetBuffer(0);
	pItem->m_strSec.ReleaseBuffer();
	lvi.iItem = ndx;
	lvi.iSubItem = 2;
	m_ctrlServerFile.SetItem( &lvi );

	if (pItem->GetImage()==7)
		m_ctrlServerFile.SetItemData(ndx,DIRECTORYICON);
	else
		m_ctrlServerFile.SetItemData(ndx,FILEICON);

	return (pItem != NULL);
}


void CMyFTPClientDlg::OnDblclkListServerfile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLVDISPINFO* pLocInfo=(NMLVDISPINFO*)pNMHDR;
	CString str,tempstr,tempstr2,buf;
	int pos1=0;
	str.Format("%d",pLocInfo->item);
	if(str=="-1")
		return;
	int nItem=atoi(str);
	
	DWORD dwFlag=m_ctrlServerFile.GetItemData(nItem);
	tempstr2=m_ctrlServerFile.GetItemText(nItem,0);

	if(dwFlag==FILEICON)
	{
		str=szFtpDirectory;
		if (str.Right (1)!="/")
			str+="/"+tempstr2;
		else
			str+=tempstr2;
		DownLoadFile(str,tempstr2,m_PASV,true);
		return;
	}
	str=szFtpDirectory;
	tempstr=szFtpDirectory;
	
	if(str.Right (1)!="/")
		str+="/";

	str+=m_ctrlServerFile.GetItemText(nItem,0);
	tempstr2=m_ctrlServerFile.GetItemText(nItem,0);
	if(!tempstr2.CompareNoCase(".."))
	{
		if (tempstr=="/")
			return;
		else
		{
			m_ctrlServerFile.DeleteAllItems ();
			Back();
			pos1=tempstr.GetLength ()-1;
			while (pos1>=0)
			{	if (tempstr.Mid (pos1,1)!="/")
				{
					pos1--;
				}
				else
				{
					str=tempstr.Mid (0,pos1+1);
					break;
				}
			}
			strcpy(szFtpDirectory,str);
			SetServerDir(str);
		}
	}
	else
	{
		m_ctrlServerFile.DeleteAllItems ();
		CD(str);
		strcpy(szFtpDirectory,str);
		SetServerDir(str);

	}
	*pResult = 0;
}



void CMyFTPClientDlg::CD(CString dir)
{
	int ndx=0;
	dir.TrimLeft();
	dir.TrimRight();
	CString cmd = "CWD " + dir;// + "\r\n";


	ftp.FTPcommand(cmd);
	ftp.FTPcommand("PWD");
	ftp.List();
		while(ftp.GetLine(ndx))
		{
			UpdateData(false);
			AddLine(ftp.m_strLine );
		}
		ftp.FTPcommand(""); 
}

void CMyFTPClientDlg::Back()
{
int ndx=0;
	ftp.FTPcommand("CDUP");
	ftp.List();
		while(ftp.GetLine(ndx))
		{
			UpdateData(false);
			AddLine(ftp.m_strLine );
		}
		ftp.FTPcommand(""); 
}
void CMyFTPClientDlg::UpLoadFile(CString LocalFilePath,CString LocalFileName,bool pasv,bool get)
{
	CString str;
	str=szFtpDirectory;
	if (str.Right(1)!="/")
		str+="/"+LocalFileName;
	else
		str+=LocalFileName;
	ftp.MoveFile (str,LocalFilePath,pasv,get);
}

void CMyFTPClientDlg::DownLoadFile(CString ServerFilePath,CString ServerFileName,bool pasv,bool get)
{
	CString str;
	str=szLocDirectory;
	if (str.Right(1)!="\\")
		str+="\\"+ServerFileName;
	else
		str+=ServerFileName;
	ftp.MoveFile (ServerFilePath,str,pasv,get);
}