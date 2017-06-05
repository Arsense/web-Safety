
// Pop3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Pop3.h"
#include "Pop3Dlg.h"
#include "afxdialogex.h"
#include "ChooseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPop3Dlg 对话框




CPop3Dlg::CPop3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPop3Dlg::IDD, pParent)
	, m_Password(_T("ml1075256556"))
	, m_UserName(_T("17802926324"))
	, m_ServerAddress(_T("pop.163.com"))
	, m_bDelete(FALSE)
	, m_InFormation(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPop3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PASS, m_Password);
	DDX_Text(pDX, IDC_USER, m_UserName);
	DDX_Text(pDX, IDC_SERVER, m_ServerAddress);
	DDX_Check(pDX, IDC_DELETE, m_bDelete);
	DDX_Text(pDX, IDC_INFO, m_InFormation);
	DDX_Control(pDX, IDC_INFO, m_RichEditCtrlInformation);
}

BEGIN_MESSAGE_MAP(CPop3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CPop3Dlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_VIEW, &CPop3Dlg::OnBnClickedView)
END_MESSAGE_MAP()


// CPop3Dlg 消息处理程序

BOOL CPop3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CHARFORMAT cf;                      //字符格式类的结构在richedit中使用
	cf.cbSize=sizeof(cf);
	cf.dwMask=CFM_COLOR | CFM_FACE;    //可以设置字体和颜色
	cf.dwEffects=0;
	cf.crTextColor=RGB(255,0,0);
	WCHAR* v1 = L"Verdana";
	memcpy(cf.szFaceName,v1,sizeof(v1));
	CRichEditCtrl* re=(CRichEditCtrl*)GetDlgItem(IDC_INFO);
	re->SetDefaultCharFormat(cf);

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPop3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPop3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPop3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPop3Dlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码

	Pop3.SetMsgWnd(this); //设定串口以便于接收信息
	UpdateData(TRUE);
	m_InFormation = "";
	Pop3.SetProp(m_UserName,m_Password); //设定用户名和密码
	Pop3.DelAfterRead(m_bDelete);
	Pop3.Create();
	
	Pop3.Connect(((LPCTSTR)m_ServerAddress),110); //连接服务器
	UpdateData(FALSE);
}

void CPop3Dlg::Dispatch(LONG param)
{
	CString cStr;
	switch(param)
	{
	case CONNECT:
		m_InFormation = "Connect to" + m_ServerAddress;
		m_InFormation += "\r\n";
		GetDlgItem(IDC_CONN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DISC)->EnableWindow(TRUE);
		GetDlgItem(IDC_VIEW)->EnableWindow(TRUE);
		break;
	case RECEIVE:
		Pop3.GetLastMsg(cStr);
		m_InFormation+=cStr;
		break; 
	case CLOSE: //关闭连接
		m_InFormation+=Pop3.GetError();
		m_InFormation+="Connection closed\r\n";
		GetDlgItem(IDC_CONN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DISC)->EnableWindow(FALSE);
		break;
	case GETNUMMSGS: //邮件的数量
		{
			cStr.Format("There are %d messages\r\n",Pop3.GetNumMsg());
			m_InFormation+=cStr;
			break;
		}
	case GETSIZEMSGS:               //邮件的大小
		{
			cStr.Format("Size is: %d\r\n",Pop3.GetSizeMsg());
			m_InFormation+=cStr;
			break;
		}
	case ENDRETR: //we have received all of messages
		{
			cStr.Format("Received %d messages\r\n",Pop3.GetRetrMsgNum());
			m_InFormation+=cStr;
			if(Pop3.GetRetrMsgNum()>0)
			{
				GetDlgItem(IDC_VIEW)->EnableWindow(TRUE);
			}
			break;
		}
	}
	//SetDlgItemTextA(IDC_INFO,m_InFormation);

	UpdateData(FALSE);
	
}


void CPop3Dlg::OnBnClickedView()
{
	// TODO: 在此添加控件通知处理程序代码
	CChooseDlg dlg;
	dlg.DoModal();
}
