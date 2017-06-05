#ifndef _CMESSG_
#define _CMESSG_

class CMessg : public CObject
{
protected:
	DECLARE_DYNCREATE(CMessg)
public:
	CMessg();

// Attributes
public:
	//0表示发送本地的共享文件列表,所有的共享文件名放在m_strText中，各个文件名以'%'符号分割;
	//1表示请求对方传输相关文件，文件名放在m_strText中，各个文件名以'%'符号分割;
	//2表示传输文件给对方，文件内容放在m_strText中
	int		m_iCommand;			
//	long    m_lLength;
	CString m_strText;			

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
