#ifndef _CMESSG_
#define _CMESSG_
#include "Sound.h"

class CMessg : public CObject
{
protected:
	DECLARE_DYNCREATE(CMessg)
public:
	CMessg();

// Attributes
public:
	CString m_strText;
	CHAR    m_tag;	//0表示不需要放出声音，1表示需要放出声音
	CHAR    m_buffer[MAX_BUFFER_SIZE];
// Operations
public:
	void Init();

// Implementation
public:
	virtual ~CMessg();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif

/////////////////////////////////////////////////////////////////////////////
