// ListItem.h: interface for the CListItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTITEM_H__34E03324_51B0_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_LISTITEM_H__34E03324_51B0_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CListItem : public CObject  
{
public:
	int GetImage();
	BOOL ParseLine(CString strLine);
	CListItem();
	virtual ~CListItem();
	BOOL	m_bDirectory;
	WORD	m_bSec;
	CString m_strSec;
	CString m_strDate;
	CString m_strName;
	int m_index;
};

#endif // !defined(AFX_LISTITEM_H__34E03324_51B0_11D5_B587_00E04C39B036__INCLUDED_)
