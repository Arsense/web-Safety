// HTTPClientView.cpp : implementation of the CHTTPClientView class
// Download by http://www.codefans.net

#include "stdafx.h"
#include "HTTPClient.h"

#include "HTTPClientDoc.h"
#include "CntrItem.h"
#include "HTTPClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientView

IMPLEMENT_DYNCREATE(CHTTPClientView, CRichEditView)

BEGIN_MESSAGE_MAP(CHTTPClientView, CRichEditView)
	//{{AFX_MSG_MAP(CHTTPClientView)
	ON_WM_DESTROY()
	ON_COMMAND(IDC_GET, OnGet)
	ON_COMMAND(IDC_HEAD, OnHead)
	ON_COMMAND(IDC_POST, OnPost)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientView construction/destruction

CHTTPClientView::CHTTPClientView()
{
	m_pSocket = NULL;
	nPort = 80;
	m_bConnected = FALSE;
	m_strSvr = CString("127.0.0.1");
	m_pReq = NULL;
	m_status = 0;
	m_bSend = FALSE;
}

CHTTPClientView::~CHTTPClientView()
{
	if(m_pSocket != NULL){
		delete m_pSocket;
		m_pSocket = NULL;
	}
	if(m_pReq != NULL){
		delete m_pReq;
		m_pReq = NULL;
	}
}

BOOL CHTTPClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CRichEditView::PreCreateWindow(cs);
}

void CHTTPClientView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
}

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientView printing

BOOL CHTTPClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CHTTPClientView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CRichEditView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// CHTTPClientView diagnostics

#ifdef _DEBUG
void CHTTPClientView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CHTTPClientView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CHTTPClientDoc* CHTTPClientView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHTTPClientDoc)));
	return (CHTTPClientDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHTTPClientView message handlers

void CHTTPClientView::TextOut(LPCTSTR lpszMessage, COLORREF clr)
{
	cfm.cbSize=sizeof(cfm);
	cfm.crTextColor=clr;
	cfm.dwMask=CFM_COLOR;
	CString strTemp = lpszMessage;
	
	int len = GetWindowTextLength();
	GetRichEditCtrl().SetSel(len,len);
	
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
	GetRichEditCtrl().ReplaceSel(strTemp);
}

void CHTTPClientView::OnGet() 
{
	if(m_pReq != NULL)
		m_pReq->m_cbOut = 0;
	Get("");
}

CString CHTTPClientView::GetLine(const CByteArray &bytes, int nBytes, int &ndx)
{
	CString ret;
	BOOL bLine = FALSE;
	while ( bLine == FALSE && ndx < nBytes )
	{
		char ch = (char)(bytes.GetAt( ndx ));
		switch( ch )
		{
		case '\r': // ignore
			break;
		case '\n': // end-of-line
			ret += '\n';
			bLine = TRUE;
			break;
		default:   // other....
			ret += ch;
			break;
		}
		++ndx;
	}
	return ret;
}

void CHTTPClientView::Connect()
{
	if(	m_pSocket == NULL){
		m_pSocket = new CClientSocket(this);
		m_pSocket->Create(0, SOCK_STREAM, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE);
		m_pSocket->Connect(m_strSvr, nPort);
		return;
	}
	else{
		delete m_pSocket;
		m_pSocket = new CClientSocket(this);
		m_pSocket->Create(0, SOCK_STREAM, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE);
		m_pSocket->Connect(m_strSvr, nPort);
	}
}

void CHTTPClientView::Receive()
{
	m_pReq->m_buf.SetSize(1024);
	m_pSocket->Receive(m_pReq->m_buf.GetData(), m_pReq->m_buf.GetSize());
	CString str;
	int ndx = 0;
	m_status = 0;
	m_location.Empty();
	TextOut("\n从服务器接受:\n", RGB(10,10,200));
	while(1){
		TextOut(str, RGB(0,0,0));	
		str = GetLine(m_pReq->m_buf, m_pReq->m_buf.GetSize(), ndx);
		ProcessLine(str);
		if(str.IsEmpty())
			break;
	}
	m_pReq->m_buf.RemoveAll();
	m_bConnected = FALSE;
	if(!m_location.IsEmpty()){
		m_pReq->m_buf.RemoveAll();
		m_pReq->m_buf.SetSize(1024);
		m_pReq->m_cbOut = 0;
		Get(m_location);
	}
}

void CHTTPClientView::ProcessLine(CString str)
{
	CString cmd;
	CString tmp;
	CString strVal;
	int value,ndx;
	if(m_status == 0){
		ndx = str.Find( ' ' );
		m_status++;
		if ( ndx != -1 ){
			cmd = str.Left( ndx );
			tmp = str.Mid( ndx + 1);
			ndx = tmp.Find( ' ' );
			if(ndx != -1){
				strVal = tmp.Left(ndx);
				value = atoi(strVal.GetBuffer(0));
				strVal.ReleaseBuffer();
			}
		}
		if(value == 302)
			m_status = 10;
	}
	if(m_status == 10){
		ndx = str.Find( ':' );
		if(ndx != -1){
			cmd = str.Left(ndx);
			tmp = str.Mid(ndx + 1);
		}
		if(!cmd.CompareNoCase("Location"))
			m_location = tmp;
	}
}

void CHTTPClientView::Get(CString location)
{
	if(m_pReq == NULL)
		m_pReq = new CRequest;
	int ndx = 0;
	CString strURL;
	m_pReq->m_buf.RemoveAll();
	m_pReq->m_cbOut = 0;
	if(!location.IsEmpty() && location.GetLength() > 7){
		location = location.Mid(8);
		ndx = location.Find( '/');
		if(ndx != -1)
			location = location.Mid(ndx + 1);
		location = location.Left(location.GetLength() - 1);
	}
	m_pReq->StuffHead("GET", location);
	m_bSend = TRUE;
	if(!m_bConnected){
		Connect();
	}
}

void CHTTPClientView::SendData()
{
	int nBytes = m_pSocket->Send(m_pReq->m_buf.GetData(), m_pReq->m_buf.GetSize());
	CString str;
	int ndx = 0;
	TextOut("向服务器发送：\n", RGB(10,10,200));
	while(1){
		TextOut(str, RGB(0,0,0));	
		str = GetLine(m_pReq->m_buf, m_pReq->m_buf.GetSize(), ndx);
		if(str.IsEmpty())
			break;
	}
	m_pReq->m_buf.RemoveAll();
}

void CHTTPClientView::Head(CString location)
{
	if(m_pReq == NULL)
		m_pReq = new CRequest;
	int ndx = 0;
	CString strURL;
	m_pReq->m_buf.RemoveAll();
	m_pReq->m_cbOut = 0;
	if(!location.IsEmpty() && location.GetLength() > 7){
		location = location.Mid(8);
		ndx = location.Find( '/');
		if(ndx != -1)
			location = location.Mid(ndx + 1);
		location = location.Left(location.GetLength() - 1);
	}
	m_pReq->StuffHead("HEAD", location);
	m_bSend = TRUE;
	if(!m_bConnected){
		Connect();
	}
}

void CHTTPClientView::OnHead() 
{
	Head("http://para/index.html");	
}

void CHTTPClientView::Post(CString filename)
{
	if(m_pReq == NULL)
		m_pReq = new CRequest;
	int ndx = 0;
	m_pReq->m_buf.RemoveAll();
	m_pReq->m_cbOut = 0;
	m_pReq->StuffHead("POST", filename);
	m_bSend = TRUE;
	if(!m_bConnected){
		Connect();
	}
}

void CHTTPClientView::OnPost() 
{
	Post("post.htm");	
}
