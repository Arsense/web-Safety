
/*
	Parser.cpp : CHtmlParser implemention.

	Implements the base HTML parser class.

	Author: Steven E. Sipe
*/

#include "stdafx.h"

#include <stdlib.h>

#include "parser.h"


// Constructor
CHtmlParser::CHtmlParser()
{
}

// Destructor
CHtmlParser::~CHtmlParser()
{
}

// Finds the specified token(s) in the supplied input buffer.  Sets nIndex to the 
// location of the token
BOOL CHtmlParser::FindToken(const char *pTokens, const char *pBuffer, int& nIndex, 
                                             int nMaxLen)
{
	int nTokenCount = strlen(pTokens);
	BOOL bQuote = FALSE;

	// While we haven't run out of buffer space...
	while(nIndex < nMaxLen)
	{
		// Does this match any of the tokens?
		for(int i = 0; i < nTokenCount; i++)
		{
			// Yes, return now
			if(pBuffer[nIndex] == pTokens[i])
				return(TRUE);
		}

		// Try the next character
		nIndex++;
	}

	return(FALSE);
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

// Returns a string delimited by the specifed terminating characters.  Sets nIndex
// to point to the end of the string.
CString CHtmlParser::GetString(const char *pTerminals, const char *pBuffer, 
                                       int &nIndex, int nMaxLen)
{
	int nOldIndex = nIndex;
	CString strText;

	// Skip leading spaces
	while(nIndex < nMaxLen && pBuffer[nIndex] == ' ')
		nIndex++;

	// Look for the terminators
	if(FindToken(pTerminals,pBuffer,nIndex,nMaxLen))
	{
		// Found one, save the location and the new string
		nIndex++;
		strText = pBuffer+nOldIndex;
		strText = strText.Left(nIndex-nOldIndex-1);

	}

	return(strText);
}

// Gets a parameter name
CString CHtmlParser::SaveParm(const CString& strParm, BOOL bLowercase)
{
	int nLen = strParm.GetLength();
	CString strNewParm = strParm;

	// Parameter are delimited by a space or a quote
	for(int i = 0; i < nLen; i++)
	{
		if(strNewParm[i] == '"')
			strNewParm.SetAt(i,' ');
	}

	// Remove leading or trailing spaces
	strNewParm.TrimLeft();
	strNewParm.TrimRight();

	// Make the parameter's name lower case --- NOT the value
	if(bLowercase)
		strNewParm.MakeLower();

	// Return the new name
	return(strNewParm);
}

// Gets options supplied with a parameter
void CHtmlParser::GetOptions(const char *pBuffer, int &nIndex, int nMaxLen)
{
	CString strParm;
	int nStart;

	// Iterate through the buffer until we're finished
	while(nIndex < nMaxLen)
	{
		nStart = nIndex;

		// Find the first assignment statement
		strParm = GetString("=",pBuffer,nIndex,nMaxLen);

		// Do we have a parameter name?
		if(!strParm.IsEmpty())
		{
			COptions opt;
			int nStart = nIndex;

			opt.SetParm(SaveParm(strParm,TRUE));

			// Skip leading spaces
			while(nIndex < nMaxLen && pBuffer[nIndex] == ' ')
				nIndex++;

			// Look for the end of the parameter or for the end
			// of the HTML tag
//			if(FindToken("\" >\r\n",pBuffer,nIndex,nMaxLen))
			if(FindToken("\" >",pBuffer,nIndex,nMaxLen))
			{
				// Handle quoted arguments
				if(pBuffer[nIndex] == '"')
					FindToken("\"",pBuffer,++nIndex,nMaxLen);

				// Save the option's location
				strParm = pBuffer+nStart;
				strParm = strParm.Left(nIndex-nStart);
				opt.SetValue(SaveParm(strParm,FALSE));
				opt.SetIndex(nStart);
			}

			// Add the option to the collection
			m_arrOptions.Add(opt);

		}
		else break;
	}
}

// Parses an HTML tag into parameters and values.
BOOL CHtmlParser::GetTag(const char *pBuffer, int& nIndex, int nMaxLen)
{
	m_strTag.Empty();
	m_arrOptions.RemoveAll();

	// Find the start of the tag
	if(FindToken("<",pBuffer,nIndex,nMaxLen))
	{
		int nStart = nIndex+1;
		BOOL bFound = FALSE;

		// Point to the next character
		nIndex++;

		// Not a valid tag, or a comment...continue looking
		if(pBuffer[nIndex] == ' ' || pBuffer[nIndex] == '>' || 
                     pBuffer[nIndex] == '!')
			return(FALSE);

		// Find the end of the tag
		if(FindToken(">",pBuffer,nIndex,nMaxLen))
		{
	         // Get the tag
			m_strTag = GetString(" >\r\n",pBuffer,nStart,nMaxLen);
			m_strTag.MakeLower();

			// Do we care about this tag???
			if(WantTag(m_strTag))
			{
				// Move back to the terminator so we can look for options
				nStart--;

				// Get the tags options
				GetOptions(pBuffer,nStart,nIndex+1);
			}

				return(TRUE);
		}
	}

	return(FALSE);
}

// Default implementation -- gets every tag.  OVERRIDE THIS
BOOL CHtmlParser::WantTag(const CString& strTag)
{
   return(TRUE);
}

// Default implementation -- does nothing with tags.  OVERRIDE THIS


// Parses the specified buffer of data for HTML tags
void CHtmlParser::ParseText(char *pBuffer, int nMaxLen)
{
	int nIndex = 0;

	// Initialize some work flags
	m_strTag.Empty();
	m_arrOptions.RemoveAll();
	m_pRawBuffer = pBuffer;

	// Iterate through the entire buffer
	while(nIndex < nMaxLen)
	{
		// Look for tags until no more are found
		if(GetTag(pBuffer,nIndex,nMaxLen))
		{
			// Should we continue processing?
			if(!ProcessTag(m_strTag,nIndex,m_arrOptions))
				break;
		}
	}
}
