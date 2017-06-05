#include "stdafx.h"

#include "HTMLParser.h"
#include "FilePars.h"
#include "InternetDownload.h"


const int g_nValidFiles = 17;
static char *g_aszValidFiles[g_nValidFiles] = 
{
	".htm",
	".html",
	".shtm",
	".htms",
	".shtml",
	".gif",
	".jpg",
	".jpeg",
	".wav",
	".au",
	".asp",
	".bmp",
	".pdf",
	".txt",
	".class",
	".mid",
	".midi",
};

BOOL CHTMLFileParser::IsTextPage(CString& strFileName)
{
	CString strExt = CInternetDownload::SplitFileName(strFileName,CInternetDownload::EXT);
	strExt.MakeLower();
	strExt = strExt.SpanExcluding("#");

	if(strExt == ".htm" || strExt == ".html" || strExt == ".shtml" ||
			strExt == ".shtm" || strExt == ".htms" || strExt == "" ||
			strExt == ".com" || strExt == ".asp")
		return(TRUE);

	return(FALSE);
}

BOOL CHTMLFileParser::WantFileType(CString& strFileName)
{
	CString strExt = CInternetDownload::SplitFileName(strFileName,CInternetDownload::EXT);
	strExt.MakeLower();
	strExt = strExt.SpanExcluding("#");

	if(strExt == ".com" || strExt.IsEmpty())
		return(TRUE);

	for(int i = 0; i < g_nValidFiles; i++)
	{
		if(strExt == g_aszValidFiles[i])
			return(TRUE);
	}

	return(FALSE);
}

void CHTMLFileParser::ResetArrays() 	
{ 
	m_arrStrMedia.RemoveAll(); 
	m_arrStrLinks.RemoveAll(); 
	m_arrbyIsOffsite.RemoveAll();
	m_Fixups.arrURL.RemoveAll();
	m_Fixups.arrIndex.RemoveAll();
	m_strTitle = "Untitled";
	m_bGetMedia = FALSE;
}

BOOL CHTMLFileParser::WantTag(const CString& strTag)
{
	if(strTag == "frame" || strTag == "a" || strTag == "area" ||
			strTag == "meta" || strTag == "img" || strTag == "body" ||
			strTag == "bgsound" || strTag == "title" || strTag == "embed" ||
			strTag == "applet")
		return(TRUE);
	else return(FALSE);
}

BOOL CHTMLFileParser::ProcessTag(const CString& strTag, int nIndex, 
										const HTML_ARGS& arrOptions) 
{
	CString strText;

	if(strTag == "frame" || strTag == "a" || strTag == "area")
	{
		for(int i = 0; i < arrOptions.GetSize(); i++)
		{
			strText = arrOptions[i].GetParm();

			if(strText == "src" || strText == "href")
			{
				strText = arrOptions[i].GetValue();

				strText = strText.SpanExcluding("?");
				if(!WantFileType(strText))
					break;

				CString strServer, strObject, strUser, strPassword;
				INTERNET_PORT nPort;
				DWORD dwServiceType;
				AfxParseURLEx(strText,dwServiceType,strServer,strObject,nPort,
										strUser,strPassword,
										ICU_NO_ENCODE);

				CString strTemp = strText;
				strTemp.MakeLower();
				BOOL bMail = (strTemp.Left(7) == "mailto:");

				if(bMail)
				{
					strText.MakeLower();
					m_arrStrMedia.Add(strText);
					return(TRUE);
				}

				CString strOldServer, strOldObject;
				AfxParseURLEx(m_strPageURL,dwServiceType,strOldServer,
										strOldObject,nPort,
										strUser,strPassword,
										ICU_NO_ENCODE);

				if(IsTextPage(strText))
				{
					if(!strServer.IsEmpty() && strOldServer != strServer)
						m_arrbyIsOffsite.Add(TRUE);
					else m_arrbyIsOffsite.Add(FALSE);
				}

				AfxParseURLEx(m_strPageURL,dwServiceType,strServer,strObject,nPort,
									strUser,strPassword,
									ICU_NO_ENCODE);

				strServer = "http://"+strServer+strObject;
				char szOut[1000];
				unsigned long ulLen = sizeof(szOut);
				::InternetCombineUrl(strServer,strText,szOut,&ulLen,
											ICU_NO_ENCODE);
				strText = szOut;

				BOOL bText = IsTextPage(strText);

				if(m_bInFixupMode)
				{
					if(bText || m_bGetMedia)
					{
						m_Fixups.arrURL.Add(strText);
						m_Fixups.arrIndex.Add(arrOptions[i].GetIndex());
						m_Fixups.arrTextPage.Add(bText);
					}
				}
				else
				{
					if(bText)
						m_arrStrLinks.Add(strText);
					else if(m_bGetMedia)
						m_arrStrMedia.Add(strText);
				}
				break;
			}
		}
	}
	else if(strTag == "meta" || strTag == "img" || strTag == "body" ||
						strTag == "bgsound" || strTag == "embed" || 
						strTag == "applet")
	{
		for(int i = 0; i < arrOptions.GetSize(); i++)
		{
			strText = arrOptions[i].GetParm();

			if(strText == "src" || strText == "background" || strText == "code")
			{
				strText = arrOptions[i].GetValue();
				strText = strText.SpanExcluding("?");

				if(!WantFileType(strText))
					break;

				CString strServer, strObject, strUser, strPassword;
				INTERNET_PORT nPort;
				DWORD dwServiceType;
				AfxParseURLEx(m_strPageURL,dwServiceType,strServer,strObject,nPort,
										strUser,strPassword,
										ICU_NO_ENCODE);

				strServer = "http://"+strServer+strObject;
				char szOut[1000];
				unsigned long ulLen = sizeof(szOut);
				::InternetCombineUrl(strServer,strText,szOut,&ulLen,
										ICU_NO_ENCODE);
				strText = szOut;

				if(m_bGetMedia && !IsTextPage(strText))
				{
					if(m_bInFixupMode)
					{
						m_Fixups.arrURL.Add(strText);
						m_Fixups.arrIndex.Add(arrOptions[i].GetIndex());
						m_Fixups.arrTextPage.Add(FALSE);
					}
					else m_arrStrMedia.Add(strText);
				}

				break;
			}
		}
	}
	else if(strTag == "title")
	{
		if(m_strTitle == "Untitled")
		{
			char ch = m_pRawBuffer[++nIndex];
			m_strTitle.Empty();

			while(ch && ch != '<')
			{
				m_strTitle += ch;
				nIndex++;
				ch = m_pRawBuffer[nIndex];
			}
		}
	}

	return(TRUE);
}
