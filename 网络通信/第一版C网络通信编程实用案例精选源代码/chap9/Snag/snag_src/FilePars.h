#if !defined(__SnagPrs_h__)
#define __SnagPrs_h__

typedef struct
{
	CStringArray arrURL;
	CDWordArray arrIndex;
	CByteArray arrTextPage;
} FIXUPS;

class CHTMLFileParser : public CHtmlParser
{
protected:
	BOOL m_bInFixupMode;
	CString m_strPageURL;
	CString m_strTitle;
	CStringArray m_arrStrMedia;
	CStringArray m_arrStrLinks;
	CByteArray m_arrbyIsOffsite;
	FIXUPS m_Fixups;
	BOOL m_bGetMedia;

protected:
	BOOL IsTextPage(CString& strFileName);
	BOOL ProcessTag(const CString& strTag, int nIndex, const HTML_ARGS& arrOptions);
	BOOL WantTag(const CString& strTag);
	BOOL WantFileType(CString& strFileName);

public:
	void ResetArrays();
	void SetFixupMode(BOOL bFixupMode) { m_bInFixupMode = bFixupMode; }
	FIXUPS& GetFixups() { return m_Fixups; }
	void SetPageURL(LPCTSTR lpszURL) { m_strPageURL = lpszURL; }
	CString& GetTitle() { return m_strTitle; }
	CStringArray& GetLinks() { return m_arrStrLinks; }
	CStringArray& GetMedia() { return m_arrStrMedia; }
	CByteArray& GetOffsiteFlags() { return m_arrbyIsOffsite; }
	void SetGetMedia(BOOL bGetMedia) { m_bGetMedia = bGetMedia; }
};

#endif
