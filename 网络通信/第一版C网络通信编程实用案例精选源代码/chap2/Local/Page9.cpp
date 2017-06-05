// Page9.cpp : 实现文件
//

#include "stdafx.h"
#include "Chapter5.h"
#include "Page9.h"


// CPage9 对话框

IMPLEMENT_DYNAMIC(CPage9, CDialog)
CPage9::CPage9(CWnd* pParent /*=NULL*/)
	: CDialog(CPage9::IDD, pParent)
{
}

CPage9::~CPage9()
{
}

void CPage9::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage9, CDialog)
END_MESSAGE_MAP()


// CPage9 消息处理程序
