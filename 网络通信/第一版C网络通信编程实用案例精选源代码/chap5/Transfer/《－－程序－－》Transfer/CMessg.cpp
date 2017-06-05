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
}

/////////////////////////////////////////////////////////////////////////////
// CMsg serialization

void CMessg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_iCommand;
		ar << m_strText;
	}
	else
	{
		ar >> m_iCommand;
		ar >> m_strText;
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
