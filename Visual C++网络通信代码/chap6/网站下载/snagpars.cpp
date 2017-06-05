/*
	Snagpars.cpp : implementation of the CSnaggerHtmlParser class

	Implements a derived HTML parser that specifically handles the types
	of objects needed by SiteSnagger.

	Author: Steven E. Sipe
*/

#include "stdafx.h"

#include "parser.h"
#include "snagpars.h"
#include "inet.h"


// The list of valid filetypes that SiteSnagger cares about
const int g_nValidFiles = 18;
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
	".png",
};

// Returns TRUE if the current page is a text link reference
BOOL CSnaggerHtmlParser::IsTextPage(CString& strFileName)
{
	CString strExt = CInet::SplitFileName(strFileName,CInet::EXT);
	strExt.MakeLower();
	strExt = strExt.SpanExcluding("#");

	if(strExt == ".htm" || strExt == ".html" || strExt == ".shtml" ||
			strExt == ".shtm" || strExt == ".htms" || strExt == "" ||
			strExt == ".com" || strExt == ".asp" || strExt == ".")
		return(TRUE);

	return(FALSE);
}

// Returns TRUE if we want to get this type of file
BOOL CSnaggerHtmlParser::WantFileType(CString& strFileName)
{
	CString strExt = CInet::SplitFileName(strFileName,CInet::EXT);
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

// Clears all of the arrays used by the parser to collect links and
// so forth.
void CSnaggerHtmlParser::ResetArrays() 	
{ 
	m_arrStrMedia.RemoveAll(); 
	m_arrStrLinks.RemoveAll(); 
	m_arrbyIsOffsite.RemoveAll();
	m_Fixups.arrURL.RemoveAll();
	m_Fixups.arrIndex.RemoveAll();
	m_strTitle = "Untitled";
	m_bGetMedia = FALSE;
}

// Returns TRUE if we care about getting this particular HTML tag.  This
// method is called by the base HTML parser class during parsing of an
// HTML page
BOOL CSnaggerHtmlParser::WantTag(const CString& strTag)
{
	if(strTag == "frame" || strTag == "a" || strTag == "area" ||
			strTag == "meta" || strTag == "img" || strTag == "body" ||
			strTag == "bgsound" || strTag == "title" || strTag == "embed" ||
			strTag == "applet")
		return(TRUE);
	else return(FALSE);
}

// Called to process the specific tag the we've encountered during parsing.
// This method is called by the base HTML parser class (only if a previous
// call to WantTag() returned TRUE.
BOOL CSnaggerHtmlParser::ProcessTag(const CString& strTag, int nIndex, 
										const HTML_ARGS& arrOptions) 
{
	CString strText;

	// Frame, anchor or area tag?  -- these are all linked pages
	if(strTag == "frame" || strTag == "a" || strTag == "area")
	{
		for(int i = 0; i < arrOptions.GetSize(); i++)
		{
			strText = arrOptions[i].GetParm();

			// Get the src or href item -- this gives us the page
			if(strText == "src" || strText == "href")
			{
				strText = arrOptions[i].GetValue();

				// Only extract up to the parameters
				strText = strText.SpanExcluding("?");
				if(!WantFileType(strText))
					break;

				// Get rid of leading blankspace and check for
				// a relative page anchor...if we found one, then skip 
				// this entry
				strText.TrimLeft();
				if(strText.Left(1) == "#")
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

				// Is this a mailto link then save it and get out now
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

				// If the starting server is different that the new pages'
				// sever, then we must be offsite
				if(IsTextPage(strText))
				{
					if(!strServer.IsEmpty() && strOldServer != strServer)
						m_arrbyIsOffsite.Add(TRUE);
					else m_arrbyIsOffsite.Add(FALSE);
				}

				// Put the completely qualified name back together now
				AfxParseURLEx(m_strPageURL,dwServiceType,strServer,strObject,nPort,
									strUser,strPassword,
									ICU_NO_ENCODE);

				// Allow for pages that contain a relative URL (SS 1.2 FIX)
				CString strExt = CInet::SplitFileName(strObject,CInet::EXT);
				if(!strObject.IsEmpty() && strExt.IsEmpty() && strObject.Right(1) != "/")
					strObject += "/";

				strServer = "http://"+strServer+strObject;
				char szOut[1000];
				unsigned long ulLen = sizeof(szOut);
				::InternetCombineUrl(strServer,strText,szOut,&ulLen,
											ICU_NO_ENCODE);
				strText = szOut;

				// Save some fixup information
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
					// Save this link -- either links or multimedia
					if(bText)
						m_arrStrLinks.Add(strText);
					else if(m_bGetMedia)
						m_arrStrMedia.Add(strText);
				}

				// Found our matching option, so get out now
				break;
			}
		}
	}	// Handle the multimedia tags too
	else if(strTag == "meta" || strTag == "img" || strTag == "body" ||
						strTag == "bgsound" || strTag == "embed" || 
						strTag == "applet")
	{
		for(int i = 0; i < arrOptions.GetSize(); i++)
		{
			strText = arrOptions[i].GetParm();

			// Get the URL for the item
			if(strText == "src" || strText == "background" || strText == "code")
			{
				strText = arrOptions[i].GetValue();
				strText = strText.SpanExcluding("?");

				// Do we want this item?
				if(!WantFileType(strText))
					break;

				// Get rid of leading blankspace and check for
				// a relative page anchor...if we found one, then skip 
				// this entry
				strText.TrimLeft();
				if(strText.Left(1) == "#")
					break;

				// Put the complete URL back together
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

				// Save some fixup information
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

				// Found our matching option, so get out now
				break;
			}
		}
	}	// Get the page title (if we need to)
	else if(strTag == "title")
	{
		// Make sure we haven't gotten a title for this page yet...
		if(m_strTitle == "Untitled")
		{
			char ch = m_pRawBuffer[++nIndex];
			m_strTitle.Empty();

			// Grab the title up to the start of the </TITLE> tag
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
