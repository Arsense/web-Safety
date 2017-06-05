
#include "stdafx.h"
#include "CMessg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsg

IMPLEMENT_DYNCREATE(CMessg, CObject)

/////////////////////////////////////////////////////////////////////////////
// CMsg construction/destruction

CMessg::CMessg()
{
	Init();
}

CMessg::~CMessg()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMsg Operations

void CMessg::Init()
{
	m_strText = _T("");
	m_x=-1;
	m_y=-1;
	m_turn=-1;
}

/////////////////////////////////////////////////////////////////////////////
// CMsg serialization

void CMessg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strText;
		ar << m_turn;
		ar << m_x;
		ar << m_y;
	}
	else
	{
		ar >> m_strText;
		ar >> m_turn;
		ar >> m_x;
		ar >> m_y;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMsg diagnostics

#ifdef _DEBUG
void CMessg::AssertValid() const
{
	CObject::AssertValid();
}

void CMessg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG
