//
// TERMSOCK.CPP -- Socket derived from CAsynCAsyncSocket
//

#include "stdafx.h"
#include "wsterm.h"
#include "doc.h"
#include "termsock.h"
#include "view.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTermSocket, CAsyncSocket)

//
void CTermSocket::OnConnect(int nErrorCode)
{
	CTermView *pView = CTermView::GetView();
	pView->OnConnect(nErrorCode);
}

void CTermSocket::OnSend(int nErrorCode)
{
	CTermView *pView = CTermView::GetView();
	pView->OnSend(nErrorCode);
}

void CTermSocket::OnReceive(int nErrorCode)
{
	CTermView *pView = CTermView::GetView();
	pView->OnReceive(nErrorCode);
}

void CTermSocket::OnClose(int nErrorCode)
{
	CTermView *pView = CTermView::GetView();
	pView->OnClose(nErrorCode);
}

