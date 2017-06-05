
/*

	<tag[blank+]|

		Repeating groups of:

		[blank*]opt[blank*]=|
							 [blank*]["]value["][blank*]|

	>

*/

#if !defined(__Parser_h__)
#define __Parser_h__

class COptions
{
private:
	CString m_strParm;
	CString m_strValue;
	int m_nIndex;

public:
	void SetParm(LPCTSTR lpszTag) { m_strParm = lpszTag; }
	void SetValue(LPCTSTR lpszValue) { m_strValue = lpszValue; }
	void SetIndex(int nIndex) { m_nIndex = nIndex; }
	LPCTSTR GetParm() { return m_strParm; }
	LPCTSTR GetValue() { return m_strValue; }
	int GetIndex() { return m_nIndex; }
};

typedef CArray<COptions,COptions&> HTML_ARGS;

class CHtmlParser 
{
protected:
	CString m_strTag;
	HTML_ARGS m_arrOptions;
	char *m_pRawBuffer;

protected:
	BOOL FindToken(const char *pTokens, const char *pBuffer, int& nIndex, 
                                    int nMaxLen);
	CString GetString(const char *pTerminals, const char *pBuffer, int &nIndex, 
                                    int nMaxLen);
	CString SaveParm(const CString& strParm, BOOL bLowercase);
	void GetOptions(const char *pBuffer, int &nIndex, int nMaxLen);
	BOOL GetTag(const char *pBuffer, int& nIndex, int nMaxLen);

public:
	CHtmlParser();
	virtual ~CHtmlParser();

public:
	void ParseText(char *pBuffer, int nMaxLen);
	virtual BOOL ProcessTag(const CString& strTag, int nIndex, const HTML_ARGS& arrOptions);
	virtual BOOL WantTag(const CString& strTag);
};

#endif
