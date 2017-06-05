// ListItem.cpp: implementation of the CListItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListItem::CListItem()
{
}

CListItem::~CListItem()
{

}

BOOL CListItem::ParseLine(CString strLine)
{
	if(strLine.IsEmpty())
		return FALSE;
	char ch = strLine.GetAt(0);
	if(ch == 'd' || ch == 'D'){
		m_bDirectory = TRUE;
	}
	else
		if(ch == '-')
			m_bDirectory = FALSE;
		else{
			if(strLine.Find("<DIR>") != -1 || strLine.Find("<dir>") != -1)
				m_bDirectory = TRUE;
			else
				m_bDirectory = FALSE;
			if(strLine.GetLength() < 40)
				return FALSE;
			m_strName = strLine.Mid(39);
			m_strDate = strLine.Left(18);
			m_strName.TrimLeft();
			m_strName.TrimRight();
			return TRUE;
		}
	m_bSec = 0;
	for(int i = 0; i < 9; i++){
		ch = strLine.GetAt(i);
		if(ch == '-')
			m_bSec |= 0x01;
		m_bSec <<= 1;
	}
	m_strSec = strLine.Mid(1, 9);
	int ndx = strLine.Find(':');
	if(ndx == -1){
		if(strLine.GetLength() > 56)
			ndx = 51;
		else
			return FALSE;
	}
	m_index = ndx;
	m_strName = strLine.Mid(ndx + 3);
	m_strName.TrimLeft();
	m_strName.TrimRight();
	m_strDate = strLine.Mid(ndx - 9, 12);
	return TRUE;
}

int CListItem::GetImage()
{ 
	if(m_bDirectory)
		return 7;
	else
		return 3;
}
