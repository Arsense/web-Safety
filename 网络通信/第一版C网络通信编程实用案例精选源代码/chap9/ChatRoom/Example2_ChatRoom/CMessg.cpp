
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
	m_tag=0;	//数据是0表示该数组为空，不需要放出声音,为1需要放出声音
	m_buffer[0]=0;
}

/////////////////////////////////////////////////////////////////////////////
// CMsg serialization

void CMessg::Serialize(CArchive& ar)
{
	int i=0;
	if (ar.IsStoring())
	{
		ar << m_strText;
		ar << m_tag;
		if(m_tag==1)
		{
			for(i=0;i<MAX_BUFFER_SIZE;i++)
				ar << m_buffer[i];
		}
	}
	else
	{
		ar >> m_strText;
		ar >> m_tag;
		if(m_tag==1)
		{
			for(i=0;i<MAX_BUFFER_SIZE;i++)
				ar >> m_buffer[i];
		}
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
