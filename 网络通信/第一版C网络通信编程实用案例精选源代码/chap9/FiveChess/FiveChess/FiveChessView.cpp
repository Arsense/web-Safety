// FiveChessView.cpp : implementation of the CFiveChessView class
//

#include "stdafx.h"
#include "FiveChess.h"

#include "FiveChessDoc.h"
#include "FiveChessView.h"
#include "MainFrm.h"
#include "ClientDlg.h"
#include "ServerDlg.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOPDIS 10
#define LEFTDIS 100 

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView

IMPLEMENT_DYNCREATE(CFiveChessView, CView)

BEGIN_MESSAGE_MAP(CFiveChessView, CView)
	//{{AFX_MSG_MAP(CFiveChessView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_SETSERVER, OnSetserver)
	ON_COMMAND(ID_SETCLIENT, OnSetclient)
	ON_EN_UPDATE(2, INPUTMESSAGE)

	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView construction/destruction

CFiveChessView::CFiveChessView()
{
	// TODO: add construction code here
	m_bIsClient=FALSE;
	m_bIsInit=FALSE;
	m_bOver=FALSE;
	m_bWin=FALSE;
	m_turn= 1;//服务器先走,黑棋
}

CFiveChessView::~CFiveChessView()
{
}

BOOL CFiveChessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView drawing

void CFiveChessView::OnDraw(CDC* pDC)
{
	CFiveChessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CBrush back(RGB(125,125,20));
	pDC->SelectObject((CBrush*)&back);
	CRect rect;
	rect.left=LEFTDIS;rect.right=LEFTDIS+(LW-1)*30;
	rect.top=TOPDIS;rect.bottom=TOPDIS+(LW-1)*30;
	pDC->FillRect(&rect,&back);

	CPen pen(PS_SOLID,2,RGB(25,25,255));
	pDC->SelectObject((CPen*)&pen);
	int i,j;
	CPoint pos;
	for(i=0;i<LW;i++)//竖直的线
	{
		pos.x=LEFTDIS+30*i;
		pos.y=TOPDIS;
		pDC->MoveTo(pos);
		pos.y=TOPDIS+(LW-1)*30;
		pDC->LineTo(pos);
	}
	for(i=0;i<LW;i++)//水平的线
	{
		pos.x=LEFTDIS;
		pos.y=TOPDIS+30*i;
		pDC->MoveTo(pos);
		pos.x=LEFTDIS+(LW-1)*30;
		pDC->LineTo(pos);
	}
	CBrush whitebrush(RGB(255,255,255));
	CBrush blackbrush(RGB(0,0,0));
	for(i=0;i<LW;i++)
		for(j=0;j<LW;j++)
		{
			if(m_match.chessboard[i][j]==1)
			{
				pDC->SelectObject((CBrush*)&blackbrush);
				pDC->Ellipse(j*30+LEFTDIS-15,i*30+TOPDIS-15,j*30+LEFTDIS+15,i*30+TOPDIS+15);
			}
			else
				if(m_match.chessboard[i][j]==2)
				{
					pDC->SelectObject((CBrush*)&whitebrush);
					pDC->Ellipse(j*30+LEFTDIS-15,i*30+TOPDIS-15,j*30+LEFTDIS+15,i*30+TOPDIS+15);
				}
		}
		//星位的绘制
	CBrush starbrush(RGB(0,0,0));
	pDC->SelectObject((CBrush*)&starbrush);
	j=4;i=4;
	if(m_match.chessboard[i][j]==0)
	pDC->Ellipse(j*30+LEFTDIS-8,i*30+TOPDIS-8,j*30+LEFTDIS+8,i*30+TOPDIS+8);
	j=4;i=14;
	if(m_match.chessboard[i][j]==0)
	pDC->Ellipse(j*30+LEFTDIS-8,i*30+TOPDIS-8,j*30+LEFTDIS+8,i*30+TOPDIS+8);
	j=9;i=9;
	if(m_match.chessboard[i][j]==0)
	pDC->Ellipse(j*30+LEFTDIS-8,i*30+TOPDIS-8,j*30+LEFTDIS+8,i*30+TOPDIS+8);
	j=14;i=4;
	if(m_match.chessboard[i][j]==0)
	pDC->Ellipse(j*30+LEFTDIS-8,i*30+TOPDIS-8,j*30+LEFTDIS+8,i*30+TOPDIS+8);
	j=14;i=14;
	if(m_match.chessboard[i][j]==0)
	pDC->Ellipse(j*30+LEFTDIS-8,i*30+TOPDIS-8,j*30+LEFTDIS+8,i*30+TOPDIS+8);

	if(m_bOver==TRUE)
	{
		CBrush redbrush(RGB(255,0,0));
		pDC->SelectObject((CBrush*)&redbrush);
		for(int k=0;k<5;k++)
		{
			i=m_winpos[k][0];
			j=m_winpos[k][1];
			pDC->Ellipse(j*30+LEFTDIS-15,i*30+TOPDIS-15,j*30+LEFTDIS+15,i*30+TOPDIS+15);
		}
	}
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView printing

BOOL CFiveChessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFiveChessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFiveChessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView diagnostics

#ifdef _DEBUG
void CFiveChessView::AssertValid() const
{
	CView::AssertValid();
}

void CFiveChessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFiveChessDoc* CFiveChessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFiveChessDoc)));

	return (CFiveChessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFiveChessView message handlers
void CFiveChessView::OnSetserver() 
{
	// TODO: Add your command handler code here
	CServerDlg dlg;

	if(dlg.DoModal()==IDOK)
	{
		m_bIsClient=FALSE;
		m_bIsInit=TRUE;
		m_port=dlg.m_iPort;
		m_ListenSocket.Init(m_port,this);

	}
}

void CFiveChessView::OnSetclient() 
{
	// TODO: Add your command handler code here
	CClientDlg dlg;
	int ret=dlg.DoModal();
	if(ret==2000)
	{
		m_bIsClient=TRUE;
		m_bIsInit=TRUE;
		m_port=dlg.m_iPort;
		for(int i=0;i<4;i++)
			m_bIP[i]=dlg.m_bIP[i];
		m_ip.Format("%d.%d.%d.%d",dlg.m_bIP[0],dlg.m_bIP[1],dlg.m_bIP[2],dlg.m_bIP[3]);

		m_ClientSocket.Create();
		if(m_ClientSocket.Connect(LPCSTR(m_ip),m_port))
		{
			m_ClientSocket.Init(this);
			m_who=2;		//客户端用白子
			AfxMessageBox("成功的连接到了服务器,可以开始游戏了");
		}
		else
		{
			m_ClientSocket.Close();
			AfxMessageBox("client connection failed");
		}
	}

}


void CFiveChessView::ProcessPendingAccept()
{
	if(m_ListenSocket.Accept(m_ClientSocket)==FALSE)
	{
		AfxMessageBox("Server Listen Socket Error");
		return;
	}
	else
	{
		m_who=1;//服务器端用黑子
		m_ClientSocket.Init(this);
		m_outputedit.SetWindowText("有人进来了");
		AfxMessageBox("有人进来了");
	}
}
void CFiveChessView::GetLocalIP()
{
	static BOOL first=TRUE;
	if(first==TRUE)
	{
		CClientSocket::GetLocalHostName(m_sLocalName);
		CClientSocket::GetIpAddress(LPCSTR(m_sLocalName),m_bIP[0],m_bIP[1],m_bIP[2],m_bIP[3]);
		m_ip.Format("%d.%d.%d.%d",m_bIP[0],m_bIP[1],m_bIP[2],m_bIP[3]);
		first=FALSE;
	}
}
void CFiveChessView::INPUTMESSAGE()
{
	if(!m_bIsInit)
	{
		AfxMessageBox("No Connection ");
		return;
	}
	CString in;
	CMessg msg;
	m_inputedit.GetWindowText(in);
	if(in.GetLength()<1)
	{
		return;
	}
	if(in.GetAt(in.GetLength()-1)=='\n')
	{
		in.TrimRight(" ");
		m_inputedit.SetWindowText("");
		if(in.GetLength()>2)
		{
			m_sMsgList+=in;		
			m_outputedit.SetWindowText(m_sMsgList);
			m_iLineCurrentPos=m_outputedit.GetLineCount();
			m_outputedit.LineScroll(m_iLineCurrentPos);
			msg.m_strText=in;
			m_ClientSocket.SendMessage(&msg);
		}
	}
}

void CFiveChessView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	static bool oninitialupdatehaving=false;
	if(oninitialupdatehaving==false)
	{
		if(::AfxSocketInit()==FALSE)
		{
			AfxMessageBox("socket init error");
		}
		GetLocalIP();
		CRect rect;
		this->GetWindowRect(rect);
		m_outputedit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER|ES_READONLY,CRect(10,rect.bottom-200,rect.right-10,rect.bottom-140),this,1);
		m_inputedit.Create(ES_MULTILINE|WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER|ES_AUTOVSCROLL,CRect(10,rect.bottom-140,rect.right-10,rect.bottom-100),this,2);	
	}
}

void CFiveChessView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bOver==TRUE)
	{
		if(AfxMessageBox("你想重新开始游戏吗?",MB_YESNO)==IDYES)
		{
			//重新开始游戏
			m_bIsClient=FALSE;
			m_bIsInit=FALSE;
			m_turn=1;
			m_match.Clear();
			m_bWin=FALSE;
			Invalidate(FALSE);
			return;
		}
	}
	if(m_turn==m_who)
	{
		m_POS[0]=(point.y-TOPDIS)/30;	//x	,绘图坐标中的y
		if((point.y-TOPDIS)%30>15)
			m_POS[0]++;
		m_POS[1]=(point.x-LEFTDIS)/30;	//y ,绘图坐标中的x
		if((point.x-LEFTDIS)%30>15)
			m_POS[1]++;
		if(m_POS[0]<0||m_POS[0]>=LW||m_POS[1]<0||m_POS[1]>=LW)
		{
			return;
		}
		if(m_match.CanDown(m_POS[0],m_POS[1],m_who)==TRUE)
		{
			Invalidate(FALSE);
			m_turn=m_who%2+1;
			CMessg msg;
			msg.m_turn=m_turn;
			msg.m_x=m_POS[0];
			msg.m_y=m_POS[1];
			m_ClientSocket.SendMessage(&msg);
			if(m_match.IsWin(m_who,m_winpos))
			{
				m_bWin=TRUE;
				m_bOver=TRUE;
				Invalidate(FALSE);
				AfxMessageBox("你赢了耶");	
				m_sMsgList+="你赢了耶";
				m_outputedit.SetWindowText(m_sMsgList);
			}
		}
	}
	else
	{
		m_sMsgList+="不该你走棋";
		m_outputedit.SetWindowText(m_sMsgList);
	}
	CView::OnLButtonDown(nFlags, point);
}

