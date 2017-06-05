// Document.h : interface of the CMyDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(__Document_h__)
#define __Document_h__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "HTMLParser.h"
#include "FilePars.h"
#include "InternetDownload.h"
#include "MyTreeCtrl.h"
#include "ConfigureSetting.h"

typedef struct
{
   CStringArray arrLinks;
   CStringArray arrMedia;
   CByteArray arrOffsite;
   int nIndex;
} LINKS;



class CMyDoc : public CDocument
{
protected:
	CHTMLFileParser m_Parser;
	CInternetDownload m_Inet;
	MAP_FILES_ARRAY m_arrPagesDone;
	MAP_FILES_ARRAY m_arrMediaDone;
	LINKS m_aLinks[MAX_LEVELS];
	int m_nLevel;
	int m_nTotalTicks;
	int m_nTotalBytes;
	CString m_strDirectory;
	int m_nGottenPageCount;
	int m_nGottenFileCount;
	int m_nQueuedPageCount;
	BYTE m_byBuffer[MAX_INET_BUFFER];
	CConfigure m_Options;
	CFile m_fileLog;
	CFile m_fileContents;
	BOOL m_bProjectLoaded;
	CString m_strPageTitle;
	CString m_strStartPage;
	BOOL m_bAutoMode;

protected: // create from serialization only
	CMyDoc();
	DECLARE_DYNCREATE(CMyDoc)

protected:
	BOOL ShouldGetPage(CString& strPage, MAP_FILES*& pMapEntry);
	BOOL ShouldGetMedia(CString& strPage, MAP_FILES*& pMapEntry);
	void ClearCacheMaps();
	void SetPageCacheEntry(LPCTSTR lpszPage, LPCTSTR lpszFileName, int nLevel);
	void SetMediaCacheEntry(LPCTSTR lpszMedia, LPCTSTR lpszFileName);
	BOOL GetPage(CString& strPage, CString& strFileName, LINKS& linkEntry);
	BOOL GetMedia(CString& strMedia, CString& strFileName);
	void FixupLinks();
	static UINT DownloadThread(LPVOID lpvData);
	void ResetLink(int nLevel);
	BOOL ShouldQueuePage(CString& strNewPage, BOOL bOffsite);

// Attributes
public:
	void SetDirectory(CString &strDirectory) { m_strDirectory = strDirectory; }
	CString& GetDirectory() { return m_strDirectory; }
	BOOL ProjectLoaded() { return m_bProjectLoaded; }
	CConfigure *GetOptions() { return &m_Options; }
	void GetOptions(CConfigure& Options);
	void SetOptions(CConfigure& Options);
	CString GetStartPage() { return m_strStartPage; }
	void SetStartPage(CString& strStartPage) { m_strStartPage = strStartPage; }
	void SetAutoMode(BOOL bAutoMode) { m_bAutoMode = bAutoMode; }

// Operations
public:
	void RecursiveDownload(LPCTSTR lpszURL);
	void Reset(LPCTSTR lpszProjName="");

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL SaveModified();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__Document_h__)
